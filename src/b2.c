/* b2.c
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

_fichier *ficB2 = NULL;

_fichier *init_b2(GError **error)
{
    xmlDocPtr b2xml = load_xml("b2", error);
    xmlNodePtr cur = b2xml ? search_entity_doc(b2xml, "file") : NULL;

    if (cur) {
        ficB2 = file_from_xml(cur, "B2");
    } else if (NO_ERROR(error)) {
        g_set_error(error, G_SESAM_VIEWER, _ERR_ERROR_INVALID_DATA, 
            _("Le fichier b2.xml est invalide, vous devriez reinstaller sesamviewer !"));
    }

    return ficB2;
}

gboolean parse_b2(_fichier *file, gchar *filename, GtkTreeStore *treestore, GtkTextBuffer *widbuffer, GError **error) {
    FILE *fic;
    GtkTreeIter iter, iterparent;
    gchar buffer[129];
    gboolean warning = FALSE;
    guint position = 0, warning_position = 0;
    gboolean parsed = FALSE;
    gboolean use_depth = get_boolean_config("showtree");
    gboolean clean_data = get_boolean_config("replacechars");
    gchar replace_char[1];
    gint dernierrang = 0, depth = 0;  /* niveau d'indentation */
    GHashTable *data;

    get_char_config("replacewith", replace_char);

    buffer[128]=0;
    gtk_tree_store_clear(treestore);

    if (!(fic = g_fopen(filename, "rb"))) {
        g_set_error(error, G_FILE_ERROR, g_file_error_from_errno(errno), \
            _("Erreur lors de l'ouverture du fichier \"%s\":\n%s."), filename, \
            g_strerror(errno));
        return FALSE;
    }
    while (fread(buffer, 128, 1, fic)) {
        _line *line;

        if (clean_data)
            replace_no_alnum(buffer, *replace_char);
        append_text(widbuffer, buffer);
        append_text(widbuffer, "\n"); /* en cours ici */

//        gtk_text_buffer_insert_with_tags("bla", -1);

        line = get_line(file, buffer);

        if (!line) {
            if (!warning)
                warning_position = position;
            warning = TRUE;
            continue;
        }

        if (use_depth && dernierrang != line->rang) {
            if (parsed) {
                if (line->rang > dernierrang) {
                    iterparent = iter;
                    depth = 1;
                } else if (line->rang < dernierrang) {
                    GtkTreePath *path = gtk_tree_model_get_path(GTK_TREE_MODEL(treestore), &iterparent);
                    if (gtk_tree_path_get_depth(path) > 1) {
                        /* on remonte au parent */
                        if (! gtk_tree_path_up(path) ||
                                ! gtk_tree_model_get_iter(GTK_TREE_MODEL(treestore), &iterparent, path))
                            /* si pas de parent on remonte à l'élément racine, ne devrait pas arriver */
                            depth = 0;
                    } else {
                        depth = 0;
                    }
                    if (path) gtk_tree_path_free(path);
                }
            }
            dernierrang = line->rang;
        }

        data = alloc_data(line, sizeof(_line), buffer, sizeof(buffer));
        gtk_tree_store_append(treestore, &iter, depth ? &iterparent : NULL);

        gtk_tree_store_set(treestore, &iter, 0, data, 1, iconEntity, 2, line->name, 3, line->description, -1);

        parsed = TRUE;
        position+=128;
    }
if (1) {

  GtkTextIter iter, iter2;
    gtk_text_buffer_get_bounds(widbuffer, &iter, &iter2);
    GtkTextTag *tag = gtk_text_buffer_create_tag(widbuffer, "monospace", "family", "monospace", NULL);
    gtk_text_buffer_apply_tag_by_name(widbuffer, "monospace", &iter, &iter2);
}
    if (ferror(fic) || ((0!=fclose(fic)))) {
        g_set_error(error, G_FILE_ERROR, g_file_error_from_errno(errno), \
            _("Erreur lors de la lecture du fichier \"%s\":\n%s."), filename, \
            g_strerror(errno));
        return FALSE;
    }

    if (!parsed || (warning && 0==warning_position)) {
        g_set_error(error, G_SESAM_VIEWER, _ERR_ERROR_INVALID_DATA, 
            _("Le fichier \"%s\" n'est pas un fichier B2 valide."), filename);
    } else if (warning) {
        g_set_error(error, G_SESAM_VIEWER, _ERR_WARNING_INVALID_DATA, 
            _("Ce fichier B2 contient des données invalides ou non reconnues "
              "par sesam-viewer à la position %d."), warning_position);
    }

    return parsed;
}

gboolean open_b2(gchar *filename, GtkTreeView *treeview, GtkTextView *textview, GError **error)
{
    GtkTreeStore *treestore= GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    GtkTextBuffer *widbuffer = gtk_text_view_get_buffer(textview);
    if (!ficB2)
        ficB2 = init_b2(error);

    if (!ficB2 || !error)
        return FALSE;

    return parse_b2(ficB2, filename, treestore, widbuffer, error);
}
