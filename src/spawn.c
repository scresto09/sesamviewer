/* spawn.c
 * vim:et:ts=8:sts=4:sw=4
 *
 * Copyright (C) 2007 Sylvain Cresto <scresto@gmail.com>
 *
 * This file is part of sesam viewer.
 *
 * sesam viewer is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * sesam viewer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with sesam viewer; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "sesam.h"

gchar *_mainexe = NULL;

void process_init(int *argc, char ***argv)
{
    g_assert (*argc>0);
    _mainexe = *argv[0];
    gtk_init (argc, argv);
}

GPid open_new_window(gchar *file, GError **error)
{
    GPid pid = 0;
    gchar **argv = g_malloc0(3 * sizeof(char *));
    argv[0] = _mainexe;
    argv[1] = file;

    g_spawn_async(NULL, argv, NULL, 0, NULL, NULL, &pid, error);

    g_free(argv);

    return pid;
}
