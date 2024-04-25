/* main_window.c
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

void populate_details(GHashTable *data, GtkTreeView *treeview, GError **error)
{
    GtkTreeStore *treestore= GTK_TREE_STORE(gtk_tree_view_get_model(GTK_TREE_VIEW(treeview)));
    GtkTreeIter iter;
    _line *line;
    _segment *segment;
    gchar *buffer, *entrydata;
    GSList *segmentlist;

    if (! GTK_IS_TREE_VIEW(treeview))
        return;

    gtk_tree_store_clear(treestore);

    line = (_line *) g_hash_table_lookup(data, "data");
    buffer = (gchar *) g_hash_table_lookup(data, "buffer");
    g_assert(line);
    for (segmentlist= line->segmentlist; segmentlist; segmentlist = segmentlist->next) {
        segment = (_segment *) segmentlist->data;

        gtk_tree_store_append(treestore, &iter, NULL); 
        gtk_tree_store_set(treestore, &iter, COL_ORDRE, segment->ordre, -1);
        gtk_tree_store_set(treestore, &iter, COL_SEGMENT, segment->name, -1);
        gtk_tree_store_set(treestore, &iter, COL_POSITIONS, format_positions(segment), -1);
        gtk_tree_store_set(treestore, &iter, COL_LONGUEUR, segment->longueur, -1);
        gtk_tree_store_set(treestore, &iter, COL_TYPE, segment->type, -1);
        gtk_tree_store_set(treestore, &iter, COL_PRESENCE, segment->obligatoire, -1);
        entrydata = get_data_from_segment(segment, buffer);
        gtk_tree_store_set(treestore, &iter, COL_VALEUR, entrydata, -1);
        gtk_tree_store_set(treestore, &iter, COL_DETAILS, get_details_from_segment(segment, entrydata), -1);
    }
}
