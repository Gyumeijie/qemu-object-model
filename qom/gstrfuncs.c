#include <string.h>
#include <stdio.h>
#include "gmem.h"
#include "gstrfuncs.h"

/**
 * g_strdup:
 * @str: (nullable): the string to duplicate
 *
 * Duplicates a string. If @str is %NULL it returns %NULL.
 * The returned string should be freed with g_free()
 * when no longer needed.
 *
 * Returns: a newly-allocated copy of @str
 */
gchar*
g_strdup (const gchar *str)
{
  gchar *new_str;
  gsize length;

  if (str)
    {
      length = strlen (str) + 1;
      new_str = g_new (char, length);
      memcpy (new_str, str, length);
    }
  else
    new_str = NULL;

  return new_str;
}

/**
 * g_memdup:
 * @mem: the memory to copy.
 * @byte_size: the number of bytes to copy.
 *
 * Allocates @byte_size bytes of memory, and copies @byte_size bytes into it
 * from @mem. If @mem is %NULL it returns %NULL.
 *
 * Returns: a pointer to the newly-allocated copy of the memory, or %NULL if @mem
 *  is %NULL.
 */
gpointer
g_memdup (gconstpointer mem, guint byte_size)
{
  gpointer new_mem;

  if (mem && byte_size != 0) {
      new_mem = g_malloc (byte_size);
      memcpy (new_mem, mem, byte_size);
  } else {
      new_mem = NULL;
  }
  return new_mem;
}

gchar*
g_strdup_vprintf (const gchar *format, va_list args1)
{
  gchar *buffer;
  va_list args2;

  G_VA_COPY (args2, args1);


  #define MAX_BUFSIZ 256
  // buffer = g_new(gchar, g_printf_string_upper_bound (format, args1));
  // TODO Not use g_printf_string_upper_bound in gmessage.c, instead of
  // a constant to define the buffer size.
  buffer = g_new(gchar, MAX_BUFSIZ);
  buffer[MAX_BUFSIZ-1] = '\0';
  vsprintf (buffer, format, args2);
  va_end (args2);

  return buffer;
}

gchar*
g_strdup_printf (const gchar *format, ...)
{
  gchar *buffer;
  va_list args;

  va_start (args, format);
  buffer = g_strdup_vprintf (format, args);
  va_end (args);

  return buffer;
}
