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

GdkPixbuf *iconEntity = NULL;
GladeXML *gladexmlMain = NULL;
const gchar *_original_title_window = NULL;

void init_store_navigFile(GtkTreeView *treeview)
{
    GtkTreeStore *model = gtk_tree_store_new(4, G_TYPE_HASH_TABLE, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING);
    GtkTreeSelection *selection = gtk_tree_view_get_selection(treeview);
    GtkCellRenderer *cellrender;
    GtkTreeViewColumn *cell;

    gtk_tree_selection_set_mode(selection, GTK_SELECTION_BROWSE);

    /* colonne icone */
    cellrender = gtk_cell_renderer_pixbuf_new();
    g_object_set (cellrender, "yalign", 0.5, NULL);
    cell = gtk_tree_view_column_new_with_attributes("", cellrender, "pixbuf", 1, NULL);
    gtk_tree_view_column_set_resizable(cell, TRUE);
    gtk_tree_view_append_column(treeview, cell);

    /* colonne segment */
    cellrender = gtk_cell_renderer_text_new();
    g_object_set (cellrender, "yalign", 0.5, "wrap-mode", PANGO_WRAP_WORD, "wrap-width", 150, NULL);
    cell = gtk_tree_view_column_new_with_attributes(_("Segment"), cellrender, "text", 2, NULL);
    gtk_tree_view_column_set_sort_column_id (cell, 2);
    gtk_tree_view_column_set_resizable(cell, TRUE);
    gtk_tree_view_append_column(treeview, cell);

    /* colonne description */
    cellrender = gtk_cell_renderer_text_new();
    g_object_set (cellrender, "yalign", 0.0, "wrap-mode", PANGO_WRAP_WORD, "wrap-width", 200, NULL);
    cell = gtk_tree_view_column_new_with_attributes(_("Description"), cellrender, "text", 3, NULL);
    gtk_tree_view_column_set_sort_column_id (cell, 3);
    gtk_tree_view_column_set_resizable(cell, TRUE);
    gtk_tree_view_append_column(treeview, cell);

    gtk_tree_view_set_model(treeview, GTK_TREE_MODEL(model));
}

void init_store_detailsFile(GtkTreeView *treeview)
{
    GtkTreeStore *model = gtk_tree_store_new(8, G_TYPE_INT, G_TYPE_STRING, 
        G_TYPE_STRING, G_TYPE_INT, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING, G_TYPE_STRING);
    
    GtkCellRenderer *cellrender;
    GtkTreeViewColumn *cell;

    /* colonne ordre */
    cellrender = gtk_cell_renderer_text_new();
    g_object_set (cellrender, "yalign", 0.0, "xalign", 0.0, NULL);
    cell = gtk_tree_view_column_new_with_attributes(_("Pos."), cellrender, "text", COL_ORDRE, NULL);
    gtk_tree_view_column_set_sort_column_id (cell, COL_ORDRE);
    gtk_tree_view_column_set_resizable(cell, TRUE);
    gtk_tree_view_append_column(treeview, cell);

    /* colonne segment */
    cellrender = gtk_cell_renderer_text_new();
    g_object_set (cellrender, "yalign", 0.0, "wrap-mode", PANGO_WRAP_WORD, "wrap-width", 200, NULL);
    cell = gtk_tree_view_column_new_with_attributes(_("Information"), cellrender, "text", COL_SEGMENT, NULL);
    gtk_tree_view_column_set_sort_column_id (cell, COL_SEGMENT);
    gtk_tree_view_column_set_resizable(cell, TRUE);
    gtk_tree_view_append_column(treeview, cell);

    /* colonne début */
    cellrender = gtk_cell_renderer_text_new();
    g_object_set (cellrender, "yalign", 0.0, NULL);
    cell = gtk_tree_view_column_new_with_attributes(_("Positions"), cellrender, "text", COL_POSITIONS, NULL);
    gtk_tree_view_column_set_sort_column_id (cell, COL_POSITIONS);
    gtk_tree_view_column_set_resizable(cell, TRUE);
    gtk_tree_view_append_column(treeview, cell);

    /* colonne fin */
    cellrender = gtk_cell_renderer_text_new();
    g_object_set (cellrender, "yalign", 0.0, NULL);
    cell = gtk_tree_view_column_new_with_attributes(_("Longueur"), cellrender, "text", COL_LONGUEUR, NULL);
    gtk_tree_view_column_set_sort_column_id (cell, COL_LONGUEUR);
    gtk_tree_view_column_set_resizable(cell, TRUE);
    gtk_tree_view_append_column(treeview, cell);

    /* colonne type */
    cellrender = gtk_cell_renderer_text_new();
    g_object_set (cellrender, "yalign", 0.0, NULL);
    cell = gtk_tree_view_column_new_with_attributes(_("Type"), cellrender, "text", COL_TYPE, NULL);
    gtk_tree_view_column_set_sort_column_id (cell, COL_TYPE);
    gtk_tree_view_column_set_resizable(cell, TRUE);
    gtk_tree_view_append_column(treeview, cell);

    /* colonne obligatoire */
    cellrender = gtk_cell_renderer_text_new();
    g_object_set (cellrender, "yalign", 0.0, NULL);
    cell = gtk_tree_view_column_new_with_attributes(_("Présence"), cellrender, "text", COL_PRESENCE, NULL);
    gtk_tree_view_column_set_sort_column_id (cell, COL_PRESENCE);
    gtk_tree_view_column_set_resizable(cell, TRUE);
    gtk_tree_view_append_column(treeview, cell);
    gtk_tree_view_set_model(treeview, GTK_TREE_MODEL(model));

    /* colonne valeur */
    cellrender = gtk_cell_renderer_text_new();
    g_object_set (cellrender, "yalign", 0.0, "wrap-mode", PANGO_WRAP_WORD, "wrap-width", 150, NULL);
    cell = gtk_tree_view_column_new_with_attributes(_("Valeur"), cellrender, "text", COL_VALEUR, NULL);
    gtk_tree_view_column_set_sort_column_id (cell, COL_VALEUR);
    gtk_tree_view_column_set_resizable(cell, TRUE);
    gtk_tree_view_append_column(treeview, cell);
    gtk_tree_view_set_model(treeview, GTK_TREE_MODEL(model));

    /* colonne détails */
    cellrender = gtk_cell_renderer_text_new();
    g_object_set (cellrender, "yalign", 0.0, "wrap-mode", PANGO_WRAP_WORD, "wrap-width", 200, NULL);
    cell = gtk_tree_view_column_new_with_attributes(_("Détails"), cellrender, "text", COL_DETAILS, NULL);
    gtk_tree_view_column_set_sort_column_id (cell, COL_DETAILS);
    gtk_tree_view_column_set_resizable(cell, TRUE);
    gtk_tree_view_append_column(treeview, cell);
    gtk_tree_view_set_model(treeview, GTK_TREE_MODEL(model));
}

/* restauration du titre original de la fenêtre */
const gchar *restore_original_title()
{
    return _original_title_window;
}

/* en fonction de l'état du bouton "details" on affiche ou pas le panneau "affichage détaillé" */
void apply_toggle_details()
{
    GtkToggleToolButton *togbtn = GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(gladexmlMain, "detailsbutton"));
    GtkCheckMenuItem *detailsmenu = GTK_CHECK_MENU_ITEM(glade_xml_get_widget(gladexmlMain, "detailsmenu"));
    GtkWidget *jmpbtn = glade_xml_get_widget(gladexmlMain, "jumpbutton");
    GtkWidget *jmpmenu = glade_xml_get_widget(gladexmlMain, "jumpmenu");
    gboolean etat = gtk_toggle_tool_button_get_active(togbtn);
    GtkWidget *panel = glade_xml_get_widget(gladexmlMain, "boxpanel");
    if (etat) {
        gtk_widget_show(panel);
    } else {
        gtk_widget_hide(panel);
    }

    gtk_check_menu_item_set_active(detailsmenu, etat);
    gtk_widget_set_sensitive(jmpmenu, etat);
    gtk_widget_set_sensitive(jmpbtn, etat);

}


/* création et affichage de la fenêtre principal de sesam viewer */
void main_window(GError **error)
{
    GtkWidget *window;
    GtkToggleToolButton *togbtn = NULL;

    if (!gladexmlMain)
        gladexmlMain = load_widget("main", error);
    if (!gladexmlMain || (error && *error))
        return;
    window = glade_xml_get_widget(gladexmlMain, "mainwindow");
    if (!_original_title_window)
        _original_title_window = g_strdup(gtk_window_get_title(GTK_WINDOW(window)));

    iconEntity = gtk_widget_render_icon(window, GTK_STOCK_FILE, GTK_ICON_SIZE_MENU, NULL);

    init_store_navigFile(GTK_TREE_VIEW(glade_xml_get_widget(gladexmlMain, "navigFile")));
    init_store_detailsFile(GTK_TREE_VIEW(glade_xml_get_widget(gladexmlMain, "navigDetails")));

    if (get_boolean_config("savepos"))
        restore_window_position(GTK_WINDOW(window));
   
    togbtn = GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(gladexmlMain, "detailsbutton"));
    if (get_boolean_config("showdetails")) {
        gtk_toggle_tool_button_set_active(togbtn, TRUE);
    } else {
        apply_toggle_details();
    }
 
    gtk_widget_show(window);

    glade_xml_signal_autoconnect(gladexmlMain);

}

/* mise à jour de la barre de status avec la position du curseur */
void update_status(GtkTextView *text, gint x, gint y)
{
    GtkLabel *status = GTK_LABEL(glade_xml_get_widget(gladexmlMain, "status"));
    gchar statustext[64];

    g_snprintf(statustext, sizeof(statustext)-1, "Ligne: %d Colonne: %d", x, y);
    gtk_label_set_text(status, statustext);
}


/* fermeture application (ALT+F4 ..) */
G_MODULE_EXPORT void on_mainwindow_delete_event(GtkWidget *widget, GdkEvent *event, gpointer data)
{
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(widget)));
    if (get_boolean_config("savepos"))
        save_window_position(parent);
    gtk_main_quit();
}

/* menu fichier / quitter */
G_MODULE_EXPORT void on_menu_close_event(GtkMenuItem *item, gpointer data)
{
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(item)));
    if (get_boolean_config("savepos"))
        save_window_position(parent);
    gtk_main_quit();
}

/* menu fichier / ouvrir */
G_MODULE_EXPORT void on_menu_open_event(GtkMenuItem *item, gpointer data)
{
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(item)));

    selectionner_un_fichier(parent, FALSE);
}

/* menu fichier / ouvrir dans une nouelle fenetre */
G_MODULE_EXPORT void on_menu_open_new_window_event(GtkMenuItem *item, gpointer data)
{
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(item)));

    selectionner_un_fichier(parent, TRUE);
}

/* menu edition / preferences */
G_MODULE_EXPORT void on_menu_preferences_event(GtkMenuItem *item, gpointer data)
{
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(item)));

    properties_window_main(parent, NULL);
}

/* menu affichage / aller à */
G_MODULE_EXPORT void on_menu_goto_event(GtkMenuItem *item, gpointer data)
{
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(item)));
    gint x, y;
    gint maxy = -1;
    GtkTextView *textview = GTK_TEXT_VIEW(glade_xml_get_widget(gladexmlMain, "bufferFile"));
    get_max_cursor(textview, &maxy);
    get_cursor(textview, &x, &y);

    if (goto_window_main(parent, &x, &y, maxy, NULL)) {
        move_cursor(textview, x, y);
        update_status(textview, x, y);
    }
}

/* menu affichage / affichage detaillé */
G_MODULE_EXPORT void on_menu_detail_event(GtkMenuItem *item, gpointer data)
{
    gboolean etat = gtk_check_menu_item_get_active(GTK_CHECK_MENU_ITEM(item));
    GtkToggleToolButton *togbtn = GTK_TOGGLE_TOOL_BUTTON(glade_xml_get_widget(gladexmlMain, "detailsbutton"));
    gtk_toggle_tool_button_set_active(togbtn, etat);
}

G_MODULE_EXPORT void on_toolbutton_detail_event(GtkToolButton *item, gpointer data)
{
    apply_toggle_details();
}

G_MODULE_EXPORT void on_toolbutton_goto_event(GtkToolButton *item, gpointer data)
{
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(item)));
    GtkTextView *textview = GTK_TEXT_VIEW(glade_xml_get_widget(gladexmlMain, "bufferFile"));
    gint x = -1, y = -1;
    gint maxy = -1;
    get_max_cursor(textview, &maxy);
    get_cursor(textview, &x, &y);

    if (goto_window_main(parent, &x, &y, maxy, NULL)) {
        move_cursor(textview, x, y);
        update_status(textview, x, y);
    }
}

G_MODULE_EXPORT void on_toolbutton_preferences_event(GtkToolButton *item, gpointer data)
{
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(item)));

    properties_window_main(parent, NULL);
}

/* menu Aide / a propos de */
G_MODULE_EXPORT void on_menu_about_event(GtkMenuItem *item, gpointer data)
{
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(item)));

    about_window_main(parent);
}


/* barre d'outils / ouvrir */
G_MODULE_EXPORT void on_toolbutton_open_event(GtkToolButton *item, gpointer data)
{
    GtkWindow *parent = GTK_WINDOW(gtk_widget_get_toplevel(GTK_WIDGET(item)));

    selectionner_un_fichier(parent, FALSE);
}

/* déplacement du curseur dans la zone d'affichage détaillé des fichiers */
G_MODULE_EXPORT gboolean on_bufferFile_release_event(GtkTextView *text, GdkEvent release)
{
    gint posX = -1, posY = -1;

    get_cursor(text, &posX, &posY);
    update_status(text, posX, posY);

    return FALSE;
}

/* clique sur la treeview de navigation de gauche */
G_MODULE_EXPORT void on_navigFile_cursor_changed(GtkTreeView *liste, gpointer data)  
{
    GtkTreeIter iter;
    GHashTable *linedata;
    GtkTreeModel *model = NULL;
    GtkTreeSelection *selection = gtk_tree_view_get_selection(liste);
    GtkTreeView *viewdetails;
    if (!gtk_tree_selection_get_selected(selection, &model, &iter))
        return;
    viewdetails = GTK_TREE_VIEW(glade_xml_get_widget(gladexmlMain, "navigDetails"));
    g_assert(viewdetails);

    gtk_tree_model_get(model, &iter, 0, &linedata, -1);
    populate_details(linedata, viewdetails, NULL);
}

/* effacer les données affichées dans la fenêtre principale */
void clear_main_window()
{
    GtkTreeView *viewdetails = GTK_TREE_VIEW(glade_xml_get_widget(gladexmlMain, "navigDetails"));
    GtkTreeView *viewnavig = GTK_TREE_VIEW(glade_xml_get_widget(gladexmlMain, "navigFile"));
    GtkTextView *text = GTK_TEXT_VIEW(glade_xml_get_widget(gladexmlMain, "bufferFile"));
    GtkTreeStore *treedetails = GTK_TREE_STORE(gtk_tree_view_get_model(viewdetails));
    GtkTreeStore *treenavig = GTK_TREE_STORE(gtk_tree_view_get_model(viewnavig));
    GtkTextBuffer *buffer = gtk_text_view_get_buffer(text);
    GtkTextIter bufdebut, buffin;

    gtk_tree_store_clear(treedetails);
    gtk_tree_store_clear(treenavig);

    gtk_text_buffer_get_bounds(buffer, &bufdebut, &buffin);
    gtk_text_buffer_delete (buffer, &bufdebut, &buffin);
}
