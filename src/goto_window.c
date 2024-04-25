/* goto_window.c
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

/* création et affichage de la fenêtre "aller à" de sesam viewer */
gboolean goto_window_main(GtkWindow *parent, gint *x, gint *y, gint maxy, GError **error)
{
    GtkDialog *dialog;
    GladeXML *gladexmlGoto = load_widget("goto", error);
    GtkSpinButton *ligne = NULL, *colonne = NULL;

    gint rep;
    gboolean retour = FALSE;

    if (!gladexmlGoto || (error && *error)) {
        *x = -1;
        *y = -1;
        return retour;
    }
    dialog = GTK_DIALOG(glade_xml_get_widget(gladexmlGoto, "gotodialog"));
    ligne = GTK_SPIN_BUTTON(glade_xml_get_widget(gladexmlGoto, "spinLigne"));
    colonne = GTK_SPIN_BUTTON(glade_xml_get_widget(gladexmlGoto, "spinColonne"));

    gtk_spin_button_set_range(ligne, 1, maxy);

    if (y>0) gtk_spin_button_set_value(ligne, (gdouble) *y);
    if (x>0) gtk_spin_button_set_value(colonne, (gdouble) *x);
    rep = gtk_dialog_run(dialog);
    if (rep == GTK_RESPONSE_OK) {
  	gdouble doubleY = gtk_spin_button_get_value(ligne);
	gdouble doubleX = gtk_spin_button_get_value(colonne);
        *y = (gint) doubleY;
        *x = (gint) doubleX;
        retour = TRUE;
    }

    gtk_widget_destroy(GTK_WIDGET(dialog));

    return retour;
}
