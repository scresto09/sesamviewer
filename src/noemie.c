/* noemie.c
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

_fichier *ficNOEMIE = NULL;

_fichier *init_noemie(GError **error)
{
    xmlDocPtr noemiexml = load_xml("noemie-ps", error);
    xmlNodePtr cur = noemiexml ? search_entity_doc(noemiexml, "file") : NULL;

    if (cur) {
        ficNOEMIE = file_from_xml(cur, "NOEMIE");
    } else if (NO_ERROR(error)) {
        g_set_error(error, G_SESAM_VIEWER, _ERR_ERROR_INVALID_DATA, 
            _("Le fichier noemie-ps.xml est invalide, vous devriez reinstaller sesamviewer !"));
    }

    return ficNOEMIE;
}

gchar *assemble_noemie(gchar *filename, GError **error) {
    FILE *fic;
    gchar buffer[129];
    gchar *content = NULL, *d;
    gint taille = 0;
    gint position = 0;
    gint len;
    gboolean lastsegment = FALSE;
    gboolean clean_data = get_boolean_config("replacechars");
    gchar replace_char[1];

    get_char_config("replacewith", replace_char);

    if (!(fic = g_fopen(filename, "rb"))) {
        g_set_error(error, G_FILE_ERROR, g_file_error_from_errno(errno), \
            _("Erreur lors de l'ouverture du fichier \"%s\":\n%s."), filename, \
            g_strerror(errno));
        return NULL;
    }
    buffer[sizeof(buffer)-1]=0;

    g_assert(NOEMIE_BUFFER > 128);
    while (fread(buffer, sizeof(buffer)-1, 1, fic)) {
        if (clean_data)
            replace_no_alnum(buffer, *replace_char);
        if (strlen(buffer) + position > taille) {
            taille += NOEMIE_BUFFER;
            content = g_realloc(content, taille + 1);
            if (!content) {
                g_set_error(error, _ERR_NO_MEMORY, errno, \
                    _("Plus de mémoire disponible (%d octets demandés) !"), taille);
                return NULL;
            }
            content[taille-NOEMIE_BUFFER] = 0;
        }
        if (position && FALSE == lastsegment) {
            d = buffer+1;
            len = sizeof(buffer)-2;
            if (*buffer == '1') {
                lastsegment = TRUE;
                /* on enleve les eventuels espaces présent en
                 * fin de la ligne commençant par 1 */
                while (len && buffer[len] == ' ')
                    len--;
            }
        } else {
            d = buffer;
            len = sizeof(buffer)-1;
        }
        if (len>0) {
            strncpy(content+position, d, len);
            position+=len;
        }
    }
    if (content) *(content+position)=0;
    if (ferror(fic) || ((0!=fclose(fic)))) {
        g_set_error(error, G_FILE_ERROR, g_file_error_from_errno(errno), \
            _("Erreur lors de la lecture du fichier \"%s\":\n%s."), filename, \
            g_strerror(errno));
        if (content)
            g_free(error);
        return NULL;
    }

    return content;
}

gboolean parse_noemie(_fichier *file, gchar *filename, gchar *content, GtkTreeStore *treestore, GtkTextBuffer *widbuffer, GError **error) {
    gchar *b = content;
    gint line_length, warning_position;
    GtkTreeIter iter, iterparent;
    gboolean use_depth = get_boolean_config("showtree");
    gboolean warning = FALSE;
    gboolean parsed = FALSE;
    gint dernierrang = 0, depth = 0, utilrang;  /* niveau d'indentation */
    GHashTable *data;

    gtk_tree_store_clear(treestore);

    while (b && *b) {
        _line *line;
        line = get_line(file, b);
        if (!line) {
            warning = TRUE;
            break;
        }

        utilrang = line->rang;
        if (utilrang>1)
            utilrang = 1 + GPOINTER_TO_INT(get_variable_value(file, "N", "rang"));

        if (use_depth && dernierrang != utilrang) {
            if (parsed) {
                if (utilrang > dernierrang) {
                    iterparent = iter;
                    depth = 1;
                } else if (utilrang < dernierrang) {
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
            dernierrang = utilrang;
        }

        line_length = get_line_length(line);
        data = alloc_data(line, sizeof(_line), b, line_length);
        b += line_length;

        gtk_tree_store_append(treestore, &iter, depth ? &iterparent : NULL);

        gtk_tree_store_set(treestore, &iter, 0, data, 1, iconEntity, 2, line->name, 3, line->description, -1);

        parsed = TRUE;
    }

    warning_position = (b - content);
    if (!parsed || (warning && 0==warning_position)) {
        g_set_error(error, G_SESAM_VIEWER, _ERR_ERROR_INVALID_DATA, 
            _("Le fichier \"%s\" n'est pas un fichier NOEMIE valide."), filename);
    } else if (warning) {
        g_set_error(error, G_SESAM_VIEWER, _ERR_WARNING_INVALID_DATA, 
            _("Ce fichier NOEMIE contient des données invalides ou non reconnues "
              "par sesam-viewer à la position %d."), warning_position);
    }

    return parsed;
}

gboolean open_noemie(gchar *filename, GtkTreeView *treeview, GtkTextView *textview, GError **error)
{
    GtkTreeStore *treestore= GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    GtkTextBuffer *widbuffer = gtk_text_view_get_buffer(textview);
    gchar *content = NULL;
    gboolean result;

    if (!ficNOEMIE)
        ficNOEMIE = init_noemie(error);

    if (!ficNOEMIE || !error)
        return FALSE;


    if (!(content = assemble_noemie(filename, error)))
        return FALSE;

    result = parse_noemie(ficNOEMIE, filename, content, treestore, widbuffer, error);
    g_free(content);
    return result; //parse_b2(ficNOEMIE, filename, treestore, error);
}
