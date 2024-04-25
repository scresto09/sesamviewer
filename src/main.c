/* main.c
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

gint main (gint argc, gchar *argv[])
{
    gint i;
    GError *error = NULL;

#ifdef ENABLE_NLS
    bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
    bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
    textdomain (GETTEXT_PACKAGE);
    gtk_disable_setlocale();
#endif
    process_init(&argc, &argv);
#ifdef ENABLE_NLS
    gtk_set_locale();
#endif
    g_set_application_name(PACKAGE);

    /* lecture de l'eventuelle configuration */
    read_config(NULL);

    /* fenêtre principale */
    main_window (&error);
    if (!error) {
      for (i = 1; i < argc; i++) {
          if (i == 1) {
              ouvrir_un_fichier(argv[i], NULL);
          } else {
              open_new_window(argv[i], &error);
          }
          if (error) {
              error_window_main(NULL, GTK_MESSAGE_ERROR, error);
              g_error_free(error);
              error = NULL;
          }
      }

      gtk_main(); 
    }

    /* sauvegarde de la configuration */
    if (!error) write_config(&error);
    if (error) {
        error_window_main(NULL, GTK_MESSAGE_ERROR, error);
        g_error_free(error);
    }

    return EXIT_SUCCESS;
}
