/* config_file.c
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

GKeyFile *config = NULL;

/* initialisation des valeurs par défaut */
void init_default_config(GError **error)
{
    if (! g_key_file_has_key(config, "global", "replacechars", error))
        g_key_file_set_boolean(config, "global", "replacechars", TRUE);
    if (! g_key_file_has_key(config, "global", "replacewith", error))
        g_key_file_set_string(config, "global", "replacewith", "_");
    if (! g_key_file_has_key(config, "global", "savepos", error))
        g_key_file_set_boolean(config, "global", "savepos", TRUE);
    if (! g_key_file_has_key(config, "global", "showtree", error))
        g_key_file_set_boolean(config, "global", "showtree", TRUE);
    if (! g_key_file_has_key(config, "global", "showdetails", error))
        g_key_file_set_boolean(config, "global", "showdetails", TRUE);
}

/* lecture de la configuration, on regarde d'abord dans la répertoire data de 
 * l'utilisateur puis dans le répertoire data global */
void read_config(GError **error)
{
    gchar *filename = NULL;
    gchar *shortname = get_config_filename();
    if (config)
        g_key_file_free(config);
    config = g_key_file_new();
    g_key_file_load_from_data_dirs(config, shortname, &filename, G_KEY_FILE_NONE, error);
    g_free(shortname);

    init_default_config(error);
}

/* retourne la valeur d'une clef boolean de la configuration */
gboolean get_boolean_config(gchar *key)
{
    GError *error = NULL;
    gboolean ret = g_key_file_get_boolean(config, "global", key, &error);
    if (error) {
        ret = TRUE;
        g_error_free(error);
    }
    return ret;
}

/* definir la valeur d'une clef boolean de la configuration */
void set_boolean_config(gchar *key, gboolean value)
{
    g_key_file_set_boolean(config, "global", key, value);
}

gint get_int_config(gchar *key)
{
    GError *error = NULL;
    gint ret = g_key_file_get_integer(config, "global", key, &error);
    if (error) {
        g_error_free(error);
        ret = -1;
    }
    return ret;
}

/* definir la valeur d'une clef numerique de la configuration */
void set_int_config(gchar *key, gint value)
{
    g_key_file_set_integer(config, "global", key, value);
}


gchar *get_char_config(gchar *key, gchar *value)
{
    gchar *ret = g_key_file_get_string(config, "global", key, NULL);
    *value = 0;
    if (ret) {
        *value = *ret;
        g_free(ret);
    }
    return value;
}

gchar *get_string_config(gchar *key)
{
    gchar *ret = g_key_file_get_string(config, "global", key, NULL);
    return ret;
}
/* definir la valeur d'une clef de type char de la configuration */
void set_string_config(gchar *key, gchar *value)
{
    g_key_file_set_string(config, "global", key, value);
}

/* ecriture de la configuration */
void write_config(GError **error)
{
    gchar *shortname = get_config_filename();
    gchar *file = g_build_filename(g_get_user_data_dir(), shortname, NULL);
    gchar *data;
    g_free(shortname);

    data = g_key_file_to_data(config, NULL, NULL);
    g_file_set_contents(file, data, -1, NULL);
    g_free(data);
    g_free(file);
}

/* lecture et restoration de la position d'une fenetre si les données
 * semblent valides */
gboolean restore_window_position(GtkWindow *window)
{
    const gchar *name = gtk_widget_get_name(GTK_WIDGET(window));
    gint x = -1, y = -1, width = -1, height = -1;
    x = g_key_file_get_integer(config, name, "x", NULL);
    y = g_key_file_get_integer(config, name, "y", NULL);
    height = g_key_file_get_integer(config, name, "height", NULL);
    width = g_key_file_get_integer(config, name, "width", NULL);
    if (height <= 0 || width <= 0)
        return FALSE;

    gtk_window_resize(window, width, height);
    gtk_window_move(window, x, y);
    return TRUE;
}

void save_window_position(GtkWindow *window)
{
    const gchar *name = gtk_widget_get_name(GTK_WIDGET(window));
    gint x = 0, y = 0, width = 0, height = 0;
    /* mise a jour position et taille de la fenetre */
    gtk_window_get_position(window, &x, &y);
    gtk_window_get_size(window, &width, &height);

    g_key_file_set_integer(config, name, "x", x);
    g_key_file_set_integer(config, name, "y", y);
    g_key_file_set_integer(config, name, "width", width);
    g_key_file_set_integer(config, name, "height", height);
}
