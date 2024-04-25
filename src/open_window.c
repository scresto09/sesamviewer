/* open_window.c
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

void ouvrir_un_fichier(gchar *fichier, open_callback_t open_callback)
{
    GtkTreeView *treeview = GTK_TREE_VIEW(glade_xml_get_widget(gladexmlMain, "navigFile"));
    GtkTextView *buffer = GTK_TEXT_VIEW(glade_xml_get_widget(gladexmlMain, "bufferFile"));
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(treeview)));
    GError *error = NULL;
    gboolean loaded;

    clear_main_window();
    if (! open_callback)
        open_callback = detect_file_type(fichier, &error);

    if (! error)
        loaded = (*(open_callback))(fichier, treeview, buffer, &error);

/* TODO: NE FONCTIONNE PAS .. ENCORE ..
if (0) {
PangoFontDescription *font_desc;
    font_desc = pango_font_description_from_string ("Monospace 10");
    gtk_widget_modify_font(GTK_WIDGET(buffer), font_desc);
    pango_font_description_free(font_desc);
} */
    if (error) {
        /* error est un message d'erreur qui informe de l'impossibilité
         * de l'opération mais peu aussi être un simple message d'avertissement
         * (par exemple si le fichier semble endommagé)
         */
        error_window_main(parent, GTK_MESSAGE_WARNING, error);
        g_error_free(error);
    }

    if (loaded) {
        gtk_tree_view_expand_all(treeview);
        gtk_widget_grab_focus(GTK_WIDGET(treeview));
        gtk_tree_view_columns_autosize(treeview);
    }
    gtk_window_set_title(parent, loaded ? fichier : restore_original_title());
}

void load_chooser_dialog_pref(GtkFileChooser *chooser)
{
    gchar *directory = get_string_config("file_chooser_directory");
    gint filter = get_int_config("file_chooser_filter");

    if (directory) {
        /* retour au dernier répertoire utilisé */
        gtk_file_chooser_set_current_folder_uri(chooser, directory);
        g_free(directory);
    }
    if (filter > -1) {
        /* on sélectionne le dernier filtre utilisé */
        GSList *filters = gtk_file_chooser_list_filters(chooser);
        GtkFileFilter *unfiltre = GTK_FILE_FILTER(g_slist_nth_data(filters, filter));
        gtk_file_chooser_set_filter(chooser, unfiltre);
        g_slist_free(filters);
    }
}

gpointer save_chooser_dialog_pref(GtkFileChooser *chooser)
{
    gchar *directory = gtk_file_chooser_get_current_folder_uri(chooser);
    gint iter;
    GSList *filters = gtk_file_chooser_list_filters(chooser);
    GtkFileFilter *unfilter;
    GtkFileFilter *current = gtk_file_chooser_get_filter(chooser);
    gpointer filter_callback = NULL;

    set_string_config("file_chooser_directory", directory);
    for (iter = 0; ((unfilter = GTK_FILE_FILTER(g_slist_nth_data(filters, iter)))); iter++) {
        if (unfilter == current) {
            filter_callback = g_object_get_data(G_OBJECT(unfilter), "callback");

            set_int_config("file_chooser_filter", iter);
            break;
        }
    }

    g_slist_free(filters);

    return filter_callback;
}

void selectionner_un_fichier(GtkWindow *parent, gboolean new_window)
{
    gchar *filename = NULL;
    GtkFileFilter *filter_all, *filter_b2, *filter_noemie;
    open_callback_t open_callback = NULL;
    GtkWidget *opendlg = gtk_file_chooser_dialog_new(_("Ouvrir un fichier"),
                        parent, 
                        GTK_FILE_CHOOSER_ACTION_OPEN,
			GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
			GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
			NULL);
    gtk_dialog_set_default_response (GTK_DIALOG (opendlg), GTK_RESPONSE_ACCEPT);

    /* filtres pour la selection de fichiers */
    filter_b2 = gtk_file_filter_new ();
    g_object_set_data(G_OBJECT(filter_b2), "callback", open_b2);
    gtk_file_filter_set_name (filter_b2, _("Fichiers B2 (*.b2 *.fse *.dre)"));
    gtk_file_filter_add_pattern(filter_b2, "*.b2");
    gtk_file_filter_add_pattern(filter_b2, "*.fse");
    gtk_file_filter_add_pattern(filter_b2, "*.dre");
    gtk_file_filter_add_pattern(filter_b2, "*.fse");
    gtk_file_filter_add_pattern(filter_b2, "*.DRE");
    gtk_file_filter_add_pattern(filter_b2, "*.FSE");
    gtk_file_filter_add_pattern(filter_b2, "*.B2");
    filter_noemie = gtk_file_filter_new ();
    gtk_file_filter_set_name (filter_noemie, _("Fichiers NOEMIE (*.noe *.noemie)"));
    g_object_set_data(G_OBJECT(filter_noemie), "callback", open_noemie);
    gtk_file_filter_add_pattern(filter_noemie, "*.noe");
    gtk_file_filter_add_pattern(filter_noemie, "*.noemie");
    gtk_file_filter_add_pattern(filter_noemie, "*.NOE");
    gtk_file_filter_add_pattern(filter_noemie, "*.NOEMIE");
    filter_all = gtk_file_filter_new ();
    gtk_file_filter_set_name (filter_all, _("Tous les fichiers (*.*)"));
    gtk_file_filter_add_pattern(filter_all, "*");

    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (opendlg), filter_b2);
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (opendlg), filter_noemie);
    gtk_file_chooser_add_filter (GTK_FILE_CHOOSER (opendlg), filter_all);

    load_chooser_dialog_pref(GTK_FILE_CHOOSER(opendlg));
    if (gtk_dialog_run (GTK_DIALOG (opendlg)) == GTK_RESPONSE_ACCEPT) {
        filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (opendlg));
        open_callback = save_chooser_dialog_pref(GTK_FILE_CHOOSER(opendlg));
    }

    gtk_widget_destroy(opendlg);
    if (filename) {
        if (new_window) {
            GError *error = NULL;
            open_new_window(filename, &error);
            if (error) {
                error_window_main(parent, GTK_MESSAGE_WARNING, error);
                g_error_free(error);
            }
        } else {
            ouvrir_un_fichier(filename, open_callback);
        }
        g_free (filename);
    }
}
