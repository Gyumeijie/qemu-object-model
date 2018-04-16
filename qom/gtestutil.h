/* GLib testing utilities
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __G_TEST_UTILS_H__
#define __G_TEST_UTILS_H__

#include "gtypes.h"

G_BEGIN_DECLS

#define G_STRFUNC __func__
#define G_GINT64_MODIFIER "ll"

/* assertion API */
#define g_assert_cmpint(n1, cmp, n2)    G_STMT_START { \
                                             gint64 __n1 = (n1), __n2 = (n2); \
                                             if (__n1 cmp __n2) ; else \
                                               g_assertion_message_cmpnum (__FILE__, __LINE__, G_STRFUNC, \
                                                 #n1 " " #cmp " " #n2, (long double) __n1, #cmp, (long double) __n2, 'i'); \
                                        } G_STMT_END

#ifdef G_DISABLE_ASSERT
#define g_assert_not_reached()          G_STMT_START { (void) 0; } G_STMT_END
#define g_assert(expr)                  G_STMT_START { (void) 0; } G_STMT_END
#else /* !G_DISABLE_ASSERT */
#define g_assert_not_reached()          G_STMT_START { g_assertion_message_expr (__FILE__, __LINE__, G_STRFUNC, NULL);\
                                        } G_STMT_END

#define g_assert(expr)                  G_STMT_START { \
                                             if (expr) ; else \
                                               g_assertion_message_expr (__FILE__, __LINE__, G_STRFUNC, #expr);\
                                        } G_STMT_END
#endif /* !G_DISABLE_ASSERT */

int     g_strcmp0                       (const char     *str1,
                                         const char     *str2);

void    g_assertion_message             (const char     *file,
                                         int             line,
                                         const char     *func,
                                         const char     *message);

void    g_assertion_message_expr        (const char     *file,
                                         int             line,
                                         const char     *func,
                                         const char     *expr);

void    g_assertion_message_cmpnum      (const char     *file,
                                         int             line,
                                         const char     *func,
                                         const char     *expr,
                                         long double     arg1,
                                         const char     *cmp,
                                         long double     arg2,
                                         char            numtype);

G_END_DECLS

#endif /* __G_TEST_UTILS_H__ */
