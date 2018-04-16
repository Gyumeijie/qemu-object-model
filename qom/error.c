/*
 * QEMU Error Objects
 *
 *
 *  Anthony Liguori   <aliguori@us.ibm.com>
 *  Markus Armbruster <armbru@redhat.com>,
 *
 * This work is licensed under the terms of the GNU LGPL, version 2.  See
 * the COPYING.LIB file in the top-level directory.
 * File location: util/error.c
 */


#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include "glib.h"
#include "error.h"

/*
 * Special error destination to abort on error.
 * See error_setg() and error_propagate() for details.
 */
extern Error *error_abort;

/*
 * Special error destination to exit(1) on error.
 * See error_setg() and error_propagate() for details.
 */
extern Error *error_fatal;

extern int errno;

static void error_handle_fatal(Error **errp, Error *err)
{
    if (errp == &error_abort) {
        fprintf(stderr, "Unexpected error in %s() at %s:%d:\n",
                err->func, err->src, err->line);
        error_report_err(err);
        abort();
    }
    if (errp == &error_fatal) {
        error_report_err(err);
        exit(1);
    }
}

static void error_setv(Error **errp,
                       const char *src, int line, const char *func,
                       ErrorClass err_class, const char *fmt, va_list ap)
{
    Error *err;
    int saved_errno = errno;

    if (errp == NULL) {
        return;
    }
    g_assert(*errp == NULL);

    err = g_malloc0(sizeof(*err));
    err->msg = g_strdup_vprintf(fmt, ap);

    err->err_class = err_class;
    err->src = src;
    err->line = line;
    err->func = func;

    error_handle_fatal(errp, err);
    *errp = err;

    errno = saved_errno;
}

void error_set_internal(Error **errp,
                        const char *src, int line, const char *func,
                        ErrorClass err_class, const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    error_setv(errp, src, line, func, err_class, fmt, ap);
    va_end(ap);
}

void error_setg_internal(Error **errp,
                         const char *src, int line, const char *func,
                         const char *fmt, ...)
{
    va_list ap;

    va_start(ap, fmt);
    error_setv(errp, src, line, func, ERROR_CLASS_GENERIC_ERROR, fmt, ap);
    va_end(ap);
}

void error_setg_errno_internal(Error **errp,
                               const char *src, int line, const char *func,
                               int os_errno, const char *fmt, ...)
{
    va_list ap;
    int saved_errno = errno;

    if (errp == NULL) {
        return;
    }

    va_start(ap, fmt);
    error_setv(errp, src, line, func, ERROR_CLASS_GENERIC_ERROR, fmt, ap);
    va_end(ap);

    errno = saved_errno;
}

void error_report_err(Error *err)
{
    fprintf(stderr, "%s\n", err->msg);

    error_free(err);
}

void error_free(Error *err)
{
    if (err) {
        g_free(err->msg);
        g_free(err);
    }
}

void error_propagate(Error **dst_errp, Error *local_err)
{
    if (!local_err) {
        return;
    }
    error_handle_fatal(dst_errp, local_err);
    if (dst_errp && !*dst_errp) {
        *dst_errp = local_err;
    } else {
        error_free(local_err);
    }
}

