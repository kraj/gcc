/* Map one directory name to another in __FILE__, __BASE_FILE__
   and __builtin_FILE ().
   Copyright (C) 2001-2016 Free Software Foundation, Inc.

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3, or (at your option) any
later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.

 In other words, you are welcome to use, share and improve this program.
 You are forbidden to forbid anyone else to use, share and improve
 what you give them.   Help stamp out software-hoarding!  */

#include "config.h"
#include "system.h"
#include "file-map.h"

/* Structure recording the mapping from source file and directory
   names at compile time to __FILE__ */
typedef struct file_prefix_map
{
  const char *old_prefix;
  const char *new_prefix;
  size_t old_len;
  size_t new_len;
  struct file_prefix_map *next;
} file_prefix_map;

/* Linked list of such structures.  */
static file_prefix_map *file_prefix_maps;

/* Record prefix mapping of __FILE__.  ARG is the argument to
   -ffile-prefix-map and must be of the form OLD=NEW.  */
int
add_file_prefix_map (const char *arg)
{
  file_prefix_map *map;
  const char *p;

  p = strchr (arg, '=');
  if (!p)
  {
      fprintf(stderr, "invalid argument %qs to -ffile-prefix-map", arg);
      return -1;
  }
  map = XNEW (file_prefix_map);
  map->old_prefix = xstrndup (arg, p - arg);
  map->old_len = p - arg;
  p++;
  map->new_prefix = xstrdup (p);
  map->new_len = strlen (p);
  map->next = file_prefix_maps;
  file_prefix_maps = map;

  return 0;
}

/* Perform user-specified mapping of __FILE__ prefixes.  Return
   the new name corresponding to filename.  */

const char *
remap_file_filename (const char *filename)
{
  file_prefix_map *map;
  char *s;
  const char *name;
  size_t name_len;

  for (map = file_prefix_maps; map; map = map->next)
    if (filename_ncmp (filename, map->old_prefix, map->old_len) == 0)
      break;
  if (!map)
    return filename;
  name = filename + map->old_len;
  name_len = strlen (name) + 1;
  s = (char *) alloca (name_len + map->new_len);
  memcpy (s, map->new_prefix, map->new_len);
  memcpy (s + map->new_len, name, name_len);

  return xstrdup (s);
}


