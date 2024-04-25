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

/* charger un fichier de description d'interface glade */
GladeXML *load_widget(gchar *onlyname, GError **error)
{
    gchar *shortname = g_strdup_printf("%s.glade", onlyname); 
    gchar *fullname = g_build_filename(PACKAGE_DATA_DIR, PACKAGE, shortname, NULL);
    GladeXML *data;

    if (!(data = glade_xml_new(fullname, NULL, NULL)))
        g_set_error(error, G_FILE_ERROR, g_file_error_from_errno(errno), \
            _("Erreur lors de la lecture du fichier glade '%s'"), fullname);

    g_free(fullname);
    g_free(shortname);

    return data;
}

/* charger un fichier de description de format */
xmlDocPtr load_xml(gchar *onlyname, GError **error)
{
    gchar *shortname = g_strdup_printf("%s.xml", onlyname); 
    gchar *fullname = g_build_filename(PACKAGE_DATA_DIR, PACKAGE, shortname, NULL);
    xmlDocPtr data;

    if (!(data = xmlParseFile(fullname))) {
        xmlErrorPtr xml_error = xmlGetLastError ();
        g_set_error(error, G_FILE_ERROR, g_file_error_from_errno(errno),
            _("Erreur lors de la lecture du fichier xml \"%s\":\n%s"), fullname,
            xml_error ? xml_error->message : "-");
        xmlResetLastError();
    }
    g_free(fullname);
    g_free(shortname);

    return data;

}

xmlNodePtr search_entity_doc(xmlDocPtr doc, gchar *name) {
    xmlNodePtr cur;

    for (cur = xmlDocGetRootElement(doc); cur; cur = cur->next) {
        if (cur->name && *cur->name && ((!strcmp((gchar *)cur->name, name))) && cur->children)
            return cur->children;
    }

    return NULL;
}

gint int_xmlGetProp(xmlNodePtr cur, const xmlChar *name)
{
    gchar *tmp = (gchar *)xmlGetProp(cur, name);
    gint retour = tmp ? atoi(tmp) : 0;
    if (tmp) g_free(tmp);
    return retour;
}

gboolean bool_xmlGetProp(xmlNodePtr cur, const xmlChar *name)
{
    gchar *tmp = (gchar *)xmlGetProp(cur, name);
    gboolean retour = tmp && (*tmp == '1' || *tmp == 'O') ? TRUE : FALSE;
    if (tmp) g_free(tmp);
    return retour;
}

/* retourne le details associé à une valeur */
gchar *get_details_from_segment(_segment *segment, gchar *value) {
    if (!segment->descriptions)
        return NULL;

    return (gchar *) g_hash_table_lookup(segment->descriptions, value);
}

/* convertir un bloc xml segment */
_segment *segment_from_xml(xmlNodePtr xml)
{
    xmlChar *tmp;
    xmlNodePtr child;
    _segment *segment = NULL;
    gchar *key, *value;

    if (!xml)
        return NULL;
    segment = g_malloc0(sizeof(_segment));

    segment->name = (gchar *)xmlGetProp(xml, (const xmlChar *)"name");
    segment->valeur = (gchar *)xmlGetProp(xml, (const xmlChar *)"valeur");
    segment->saveas = (gchar *)xmlGetProp(xml, (const xmlChar *)"saveas");
    segment->ordre = int_xmlGetProp(xml, (const xmlChar *)"order");
    segment->pkey = bool_xmlGetProp(xml, (const xmlChar *)"pkey");
    segment->debut = int_xmlGetProp(xml, (const xmlChar *)"start");
    segment->fin = int_xmlGetProp(xml, (const xmlChar *)"end");
    segment->longueur = segment->fin - segment->debut + 1;
    tmp = xmlGetProp(xml, (const xmlChar *)"type");
    *segment->type = tmp ? *tmp : 'A';
    if (tmp) xmlFree(tmp);
    tmp = xmlGetProp(xml, (const xmlChar *)"presence");
    *segment->obligatoire = tmp ? *tmp : 'F';
    if (tmp) xmlFree(tmp);

    /* extraction des champs valeur */
    segment->descriptions = g_hash_table_new(g_str_hash, g_str_equal);
    for (child = xml->children; child; child = child->next) {
        if (!child->name || strcmp((gchar *)child->name, "valeur"))
            continue;
        key = (gchar *)xmlGetProp(child, (const xmlChar *)"valeur");
        if (!key || !*key)
            continue;
        value = (gchar *)xmlNodeGetContent(child);

        g_hash_table_insert(segment->descriptions, key, value);
    }

    return segment;
}

GSList *lines_from_xml(xmlNodePtr xml)
{
    xmlNodePtr cur;
    xmlNodePtr child;
    GSList *lines = NULL;
    _line *nline;

    for (cur = xml; cur; cur = cur->next) {
        if (!cur->name || strcmp((gchar *)cur->name, "ligne"))
            continue;

        nline = g_malloc0(sizeof(_line));
        nline->name = (gchar *) xmlGetProp(cur, (const xmlChar *)"name");
        nline->description = (gchar *) xmlGetProp(cur, (const xmlChar *)"description");
        nline->rang = int_xmlGetProp(cur, (const xmlChar *)"rang");

        for (child = cur->children; child; child = child->next) {
            if (!child->name)
                continue;
            if (!strcmp((gchar *)child->name, "segment")) {
                _segment *segment = segment_from_xml(child);
                if (segment) {
                    if (*segment->type != 'V')
                        nline->segmentlist = g_slist_append(nline->segmentlist, segment);
                    if (segment->pkey)
                        nline->pkeylist = g_slist_append(nline->pkeylist, segment);
                    if (segment->saveas && *segment->saveas) {
                        nline->saveaslist = g_slist_append(nline->saveaslist, segment);
                    }
                }
            }
        }

        lines = g_slist_append(lines, nline);
    }

    return lines;
}

_fichier *file_from_xml(xmlNodePtr xml, gchar *name)
{
    _fichier *file = g_malloc0(sizeof (_fichier));

    file->name = g_strdup(name ? name : "");
    file->linelist = lines_from_xml(xml);
    file->variables_str = g_hash_table_new(g_str_hash, g_str_equal);
    file->variables_int = g_hash_table_new(g_int_hash, g_int_equal);

    return file;
}

/* nom - debut - fin - type - valeur - description */
GArray *get_array_from_line(_line *line, gchar *data) {
    GArray *datas = NULL;

    GSList *segments;
    for (segments = line->segmentlist; segments; segments = segments->next) {
        
    }

    return datas;
}

gchar *format_positions(_segment *segment) {
    static gchar positions[1024];
    if (segment->debut != segment->fin) {
        g_snprintf(positions, sizeof(positions)-1, "%d-%d", segment->debut, segment->fin);
    } else {
        g_snprintf(positions, sizeof(positions)-1, "%d", segment->debut);
    }
    return positions;
}

gchar *get_data_from_segment(_segment *segment, gchar *data) {
    static gchar retour[1024];
    *retour = 0;

    if (!data || strlen(data) < segment->debut || segment->longueur > sizeof(retour) - 1)
        return retour;

    strncpy(retour, data+segment->debut-1, segment->longueur);
    retour[segment->longueur] = 0;

    return retour;
}

gpointer get_variable_value(_fichier *file, gchar *type, gchar *name)
{
    return type && *type == 'N' ? 
                g_hash_table_lookup(file->variables_int, name) :
                g_hash_table_lookup(file->variables_str, name);
}

_line *get_line(_fichier *file, gchar *data)
{
    GSList *lines = NULL, *pkeys = NULL, *saveas = NULL;
    _line *line, *line_match = NULL;
    _segment *segment;
    gboolean match;
    gint nbr_match;
    gint max_match = 0;
    gchar *currentdata;

    /* on recherche dans toutes les lignes définies */
    for (lines = file->linelist; lines; lines = lines->next) {
        line = (_line *) lines->data;

        match = TRUE;
        nbr_match = 0;
        /* comparaison avec toutes les pkey de la ligne */
        for (pkeys = line->pkeylist; pkeys; pkeys = pkeys->next) {
            segment = (_segment *) pkeys->data;
            if (!segment->valeur) {
                g_warning("pkey sans valeur [%s]\n", segment->name);
                continue;
            }

            if (segment->type && *segment->type == 'V') {
                gchar *comparewith = g_hash_table_lookup(file->variables_str, segment->name);
                if (!comparewith || strcmp(segment->valeur, comparewith)) {
                    match = FALSE;
                    break;
                }
            } else {
                currentdata = get_data_from_segment(segment, data);
                if (strcmp(currentdata, segment->valeur)) {
                    match = FALSE;
                    break;
                }
            }
            nbr_match++;
        }

        /* on ne conserve que la ligne qui a le plus de correspondance
         * avec le motif recherché */
        if (match && nbr_match > max_match) {
            max_match = nbr_match;
            line_match = line;
        }
    }

    if (line_match) {
        for (saveas = line_match->saveaslist; saveas; saveas = saveas->next) {
            segment = (_segment *) saveas->data;
            if (segment->saveas && *segment->saveas) {
                currentdata = get_data_from_segment(segment, data);
                if (*segment->type == 'N') {
                    g_hash_table_replace(file->variables_int, segment->saveas, GINT_TO_POINTER(atoi(currentdata)));
                } else {
                    g_hash_table_replace(file->variables_str, segment->saveas, g_strdup(currentdata));
                }
            }
        }
    }
    return line_match;
}

GHashTable *alloc_data(gpointer data, gint data_len, gpointer buffer, gint buffer_len)
{
    GHashTable *dataptr = g_hash_table_new(g_str_hash, g_str_equal);
    g_hash_table_insert(dataptr, "data", g_memdup(data, data_len));
    g_hash_table_insert(dataptr, "buffer", g_strndup(buffer, buffer_len));

    return dataptr;
}

gchar *get_config_filename()
{
    return g_strdup_printf("%s.conf", PACKAGE);
}

/* remplacer tous les caracteres non affichables par 'with' */
gchar *replace_no_alnum(gchar *buffer, gint with)
{
    gchar *c;
    for (c = buffer; c && *c; c++) {
        if (*c<32 || *c>126) *c=with;
    }

    return buffer;
}

open_callback_t detect_file_type(gchar *filename, GError **error)
{
    FILE *fic;
    gchar header[512], *c;
    open_callback_t type = NULL;
    gint nbr = 0;

    if (!(fic = g_fopen(filename, "rb"))) {
        g_set_error(error, G_FILE_ERROR, g_file_error_from_errno(errno), \
            _("Erreur lors de l'ouverture du fichier \"%s\":\n%s."), filename, \
            g_strerror(errno));
        return NULL;
    }
    *header = 0;
    fgets(header, sizeof(header)-1, fic);
    header[sizeof(header)-1]=0;
    if (ferror(fic) || ((0!=fclose(fic)))) {
        g_set_error(error, G_FILE_ERROR, g_file_error_from_errno(errno), \
            _("Erreur lors de la lecture du fichier \"%s\":\n%s."), filename, \
            g_strerror(errno));
        return NULL;
    }

    if (!strncmp(header, FILE_NOEMIE, sizeof(FILE_NOEMIE)-1)) {
        for (c = header; *c; c++) {
            if (*c=='@' && ++nbr>2)
                    break;
        }
        if (nbr>2)
            type = open_noemie;
    }
    if (!type && !strncmp(header, FILE_B2, sizeof(FILE_B2)-1))
        type = open_b2;

    if (!type)
        g_set_error(error, G_SESAM_VIEWER, _ERR_ERROR_INVALID_DATA,
            _("Le fichier \"%s\" n'est pas supporté ou endommagé."), filename);

    return type;
}

gint get_line_length(_line *line) {
    _segment *segment;
    GSList *list;
    if (!line->segmentlist)
        return 0;

    if (!(list = g_slist_last(line->segmentlist)))
        return 0;

    segment = (_segment *) list->data;

    return segment->fin;
}

gboolean append_text(GtkTextBuffer *buffer, gchar *txt)
{
    GtkTextIter iter;

    gtk_text_buffer_get_end_iter(buffer, &iter);
    gtk_text_buffer_insert(buffer, &iter, txt, -1);

    return TRUE;
}

void get_cursor(GtkTextView *textview, gint *x, gint *y)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
    GtkTextMark *mark = gtk_text_buffer_get_insert(buffer);
    GtkTextIter iter;
    if (!mark || !buffer) {
      *x = *y = -1;
      return;
    }

    gtk_text_buffer_get_iter_at_mark(buffer, &iter, mark);
    *y = gtk_text_iter_get_line(&iter) + 1;
    *x = gtk_text_iter_get_line_offset(&iter) + 1;
}

void get_max_cursor(GtkTextView *textview, gint *maxy)
{
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
    *maxy = gtk_text_buffer_get_line_count(buffer);
}


void move_cursor(GtkTextView *textview, gint x, gint y)
{
    GtkTextIter iter, iterend;
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(textview);
    
    gtk_text_buffer_get_iter_at_line(buffer, &iter, y - 1);

    gtk_text_iter_forward_chars(&iter, x - 1);
    iterend = iter;
    gtk_text_iter_forward_char(&iterend);

    gtk_text_buffer_select_range(buffer, &iter, &iterend);

    gtk_widget_grab_focus(GTK_WIDGET(textview));
}
