/* GLIB - Library of useful routines for C programming
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

/*
 * Modified by the GLib Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GLib Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GLib at ftp://ftp.gtk.org/pub/gtk/.
 */

#ifndef __G_STRFUNCS_H__
#define __G_STRFUNCS_H__

#include <stdarg.h>
#include "gtypes.h"

G_BEGIN_DECLS

gchar* g_strdup	(const gchar *str);

gpointer g_memdup (gconstpointer mem, guint byte_size);

gchar* g_strdup_vprintf (const gchar *format, va_list args1);
gchar* g_strdup_printf (const gchar *format, ...);

G_END_DECLS

#if !defined (G_VA_COPY)
#  if defined (__GNUC__) && defined (__PPC__) && (defined (_CALL_SYSV) || defined (_WIN32))
#    define G_VA_COPY(ap1, ap2)	  (*(ap1) = *(ap2))
#  elif defined (G_VA_COPY_AS_ARRAY)
#    define G_VA_COPY(ap1, ap2)	  memmove ((ap1), (ap2), sizeof (va_list))
#  else /* va_list is a pointer */
#    define G_VA_COPY(ap1, ap2)	  ((ap1) = (ap2))
#  endif /* va_list is a pointer */
#endif /* !G_VA_COPY */

#endif /* __G_STRFUNCS_H__ */
