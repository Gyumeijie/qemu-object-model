/*
 * QEMU Error Objects
 *
 *
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *  Markus Armbruster <armbru@redhat.com>
 *
 * This work is licensed under the terms of the GNU LGPL, version 2.  See
 * the COPYING.LIB file in the top-level directory.
 */

/*
 * Error reporting system loosely patterned after Glib's GError.
 *
 * Create an error:
 *     error_setg(&err, "situation normal, all fouled up");
 *
 * Create an error and add additional explanation:
 *     error_setg(&err, "invalid quark");
 *     error_append_hint(&err, "Valid quarks are up, down, strange, "
 *                       "charm, top, bottom.\n");
 *
 * Do *not* contract this to
 *     error_setg(&err, "invalid quark\n"
 *                "Valid quarks are up, down, strange, charm, top, bottom.");
 *
 * Report an error to the current monitor if we have one, else stderr:
 *     error_report_err(err);
 * This frees the error object.
 *
 * Likewise, but with additional text prepended:
 *     error_reportf_err(err, "Could not frobnicate '%s': ", name);
 *
 * Report an error somewhere else:
 *     const char *msg = error_get_pretty(err);
 *     do with msg what needs to be done...
 *     error_free(err);
 * Note that this loses hints added with error_append_hint().
 *
 * Handle an error without reporting it (just for completeness):
 *     error_free(err);
 *
 * Assert that an expected error occurred, but clean it up without
 * reporting it (primarily useful in testsuites):
 *     error_free_or_abort(&err);
 *
 * Pass an existing error to the caller:
 *     error_propagate(errp, err);
 * where Error **errp is a parameter, by convention the last one.
 *
 * Pass an existing error to the caller with the message modified:
 *     error_propagate(errp, err);
 *     error_prepend(errp, "Could not frobnicate '%s': ", name);
 *
 * Create a new error and pass it to the caller:
 *     error_setg(errp, "situation normal, all fouled up");
 *
 * Call a function and receive an error from it:
 *     Error *err = NULL;
 *     foo(arg, &err);
 *     if (err) {
 *         handle the error...
 *     }
 *
 * Call a function ignoring errors:
 *     foo(arg, NULL);
 *
 * Call a function aborting on errors:
 *     foo(arg, &error_abort);
 *
 * Call a function treating errors as fatal:
 *     foo(arg, &error_fatal);
 *
 * Receive an error and pass it on to the caller:
 *     Error *err = NULL;
 *     foo(arg, &err);
 *     if (err) {
 *         handle the error...
 *         error_propagate(errp, err);
 *     }
 * where Error **errp is a parameter, by convention the last one.
 *
 * Do *not* "optimize" this to
 *     foo(arg, errp);
 *     if (*errp) { // WRONG!
 *         handle the error...
 *     }
 * because errp may be NULL!
 *
 * But when all you do with the error is pass it on, please use
 *     foo(arg, errp);
 * for readability.
 *
 * Receive and accumulate multiple errors (first one wins):
 *     Error *err = NULL, *local_err = NULL;
 *     foo(arg, &err);
 *     bar(arg, &local_err);
 *     error_propagate(&err, local_err);
 *     if (err) {
 *         handle the error...
 *     }
 *
 * Do *not* "optimize" this to
 *     foo(arg, &err);
 *     bar(arg, &err); // WRONG!
 *     if (err) {
 *         handle the error...
 *     }
 * because this may pass a non-null err to bar().
 */

#ifndef ERROR_H
#define ERROR_H

G_BEGIN_DECLS 
/*
 * Overall category of an error.
 * Based on the qapi type QapiErrorClass, but reproduced here for nicer
 * enum names.
 */
typedef enum ErrorClass {
    ERROR_CLASS_GENERIC_ERROR,
    ERROR_CLASS_COMMAND_NOT_FOUND,
    ERROR_CLASS_DEVICE_NOT_ACTIVE,
    ERROR_CLASS_DEVICE_NOT_FOUND,
    ERROR_CLASS_KVM_MISSING_CAP,
} ErrorClass;

typedef struct _Error Error;
struct _Error
{
    char *msg;
    ErrorClass err_class;
    const char *src, *func;
    int line;
};


/*
 * Create a new error object and assign it to *@errp.
 * If @errp is NULL, the error is ignored.  Don't bother creating one
 * then.
 * If @errp is &error_abort, print a suitable message and abort().
 * If @errp is &error_fatal, print a suitable message and exit(1).
 * If @errp is anything else, *@errp must be NULL.
 * The new error's class is ERROR_CLASS_GENERIC_ERROR, and its
 * human-readable error message is made from printf-style @fmt, ...
 * The resulting message should be a single phrase, with no newline or
 * trailing punctuation.
 * Please don't error_setg(&error_fatal, ...), use error_report() and
 * exit(), because that's more obvious.
 * Likewise, don't error_setg(&error_abort, ...), use assert().
 */
#define error_setg(errp, fmt, ...)                              \
    error_setg_internal((errp), __FILE__, __LINE__, __func__,   \
                        (fmt), ## __VA_ARGS__)
void error_setg_internal(Error **errp,
                         const char *src, int line, const char *func,
                         const char *fmt, ...);
/*
 * Propagate error object (if any) from @local_err to @dst_errp.
 * If @local_err is NULL, do nothing (because there's nothing to
 * propagate).
 * Else, if @dst_errp is NULL, errors are being ignored.  Free the
 * error object.
 * Else, if @dst_errp is &error_abort, print a suitable message and
 * abort().
 * Else, if @dst_errp is &error_fatal, print a suitable message and
 * exit(1).
 * Else, if @dst_errp already contains an error, ignore this one: free
 * the error object.
 * Else, move the error object from @local_err to *@dst_errp.
 * On return, @local_err is invalid.
 * Please don't error_propagate(&error_fatal, ...), use
 * error_report_err() and exit(), because that's more obvious.
 */
void error_propagate(Error **dst_errp, Error *local_err);


void error_report_err(Error *err);

/*
 * Free @err.
 * @err may be NULL.
 */
void error_free(Error *err);

/*
 * Just like error_setg(), except you get to specify the error class.
 * Note: use of error classes other than ERROR_CLASS_GENERIC_ERROR is
 * strongly discouraged.
 */
#define error_set(errp, err_class, fmt, ...)                    \
    error_set_internal((errp), __FILE__, __LINE__, __func__,    \
                       (err_class), (fmt), ## __VA_ARGS__)
void error_set_internal(Error **errp,
                        const char *src, int line, const char *func,
                        ErrorClass err_class, const char *fmt, ...);

G_END_DECLS

#endif
