#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "gtestutil.h"

/**
 * g_strcmp0:
 * @str1: (nullable): a C string or %NULL
 * @str2: (nullable): another C string or %NULL
 *
 * Compares @str1 and @str2 like strcmp(). Handles %NULL
 * gracefully by sorting it before non-%NULL strings.
 * Comparing two %NULL pointers returns 0.
 *
 * Returns: an integer less than, equal to, or greater than zero,
 * if @str1 is <, == or > than @str2.
 *
 * Since: 2.16
 */
int
g_strcmp0 (const char     *str1,
           const char     *str2)
{
  if (!str1)
    return -(str1 != str2);
  if (!str2)
    return str1 != str2;

  return strcmp(str1, str2);
}


void
g_assertion_message (const char     *file,
                     int             line,
                     const char     *func,
                     const char     *message)
{

  if (!message)
    message = "code should not be reached";

  char str[256];
  sprintf(str, "ERROR: in %s %d %s\n%s", file, line, func, message);
  fprintf(stderr, "%s\n", str);

  exit(1);
}

/**
 * g_assertion_message_expr: (skip)
 * @domain: (nullable):
 * @file:
 * @line:
 * @func:
 * @expr: (nullable):
 */
void
g_assertion_message_expr (const char     *file,
                          int             line,
                          const char     *func,
                          const char     *expr)
{
  char str[256];

  if (!expr)
    sprintf(str, "code should not be reached");
  else
    sprintf(str, "assertion failed: ( %s )", expr);

  g_assertion_message (file, line, func, str);
}

void
g_assertion_message_cmpnum (const char     *file,
                            int             line,
                            const char     *func,
                            const char     *expr,
                            long double     arg1,
                            const char     *cmp,
                            long double     arg2,
                            char            numtype)
{
  char str[256];
  
  switch (numtype)
    {
    case 'i':
         sprintf(str, 
                 "assertion failed (%s): (%" \
                 G_GINT64_MODIFIER "i %s %" G_GINT64_MODIFIER "i)", 
                 expr, (gint64)arg1, cmp, (gint64) arg2); 
        break;
    case 'x':
        sprintf(str, 
                "assertion failed (%s): (0x%08" \
                G_GINT64_MODIFIER "x %s 0x%08" G_GINT64_MODIFIER "x)", 
                expr, (guint64)arg1, cmp, (guint64)arg2); 
        break;
    case 'f':
        sprintf(str,
                "assertion failed (%s): (%.9g %s %.9g)",
                expr, (double)arg1, cmp, (double)arg2); 
        break;
      /* ideally use: floats=%.7g double=%.17g */
    }

  g_assertion_message (file, line, func, str);

}

