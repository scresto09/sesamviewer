/* properties_window.c
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

void init_from_config(GladeXML *gladexml)
{
    GtkToggleButton *replacechars = GTK_TOGGLE_BUTTON(glade_xml_get_widget(gladexml, "checkreplacechars"));
    GtkToggleButton *savepos = GTK_TOGGLE_BUTTON(glade_xml_get_widget(gladexml, "checksavepos"));
    GtkToggleButton *showtree = GTK_TOGGLE_BUTTON(glade_xml_get_widget(gladexml, "checkshowtree"));
    GtkToggleButton *showdetails = GTK_TOGGLE_BUTTON(glade_xml_get_widget(gladexml, "checkshowdetails"));
    GtkEntry *replacewith = GTK_ENTRY(glade_xml_get_widget(gladexml, "entryreplacewith"));
    gchar entry[2];

    gtk_toggle_button_set_active(replacechars, get_boolean_config("replacechars"));
    gtk_toggle_button_set_active(savepos, get_boolean_config("savepos"));
    gtk_toggle_button_set_active(showtree, get_boolean_config("showtree"));
    gtk_toggle_button_set_active(showdetails, get_boolean_config("showdetails"));

    get_char_config("replacewith", entry);
    entry[1] = 0;
    gtk_entry_set_text(replacewith, entry);
}

void init_from_properties(GladeXML *gladexml)
{
    GtkToggleButton *replacechars = GTK_TOGGLE_BUTTON(glade_xml_get_widget(gladexml, "checkreplacechars"));
    GtkToggleButton *savepos = GTK_TOGGLE_BUTTON(glade_xml_get_widget(gladexml, "checksavepos"));
    GtkToggleButton *showtree = GTK_TOGGLE_BUTTON(glade_xml_get_widget(gladexml, "checkshowtree"));
    GtkToggleButton *showdetails = GTK_TOGGLE_BUTTON(glade_xml_get_widget(gladexml, "checkshowdetails"));
    GtkEntry *replacewith = GTK_ENTRY(glade_xml_get_widget(gladexml, "entryreplacewith"));

    set_boolean_config("replacechars", gtk_toggle_button_get_active(replacechars));
    set_boolean_config("savepos", gtk_toggle_button_get_active(savepos));
    set_boolean_config("showtree", gtk_toggle_button_get_active(showtree));
    set_boolean_config("showdetails", gtk_toggle_button_get_active(showdetails));

    set_string_config("replacewith", (gchar *)gtk_entry_get_text(replacewith));
}
/* création et affichage de la fenêtre des propriétés de sesam viewer */
void properties_window_main(GtkWindow *parent, GError **error)
{
    GtkDialog *dialog;
    GladeXML *gladexmlProperties = load_widget("properties", error);
    gint rep;

    if (!gladexmlProperties || (error && *error))
        return;
    dialog = GTK_DIALOG(glade_xml_get_widget(gladexmlProperties, "propertiesdialog"));

    init_from_config(gladexmlProperties);

    rep = gtk_dialog_run(dialog);

    if (rep == GTK_RESPONSE_OK) {
        init_from_properties(gladexmlProperties);
        write_config(NULL);
    }

    gtk_widget_destroy(GTK_WIDGET(dialog));

}
