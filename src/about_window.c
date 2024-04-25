/* about_window.c
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

GdkPixbuf *logoEntity = NULL;


gchar license[] =
    "sesamviewer est un logiciel libre; vous pouvez le redistribuer et/ou\n"
    "le modifier sous les conditions définies par la licence GNU General\n"
    "Public License publiée par la Free Software Foundation; sous sa version 2\n"
    "ou (si vous le préferez) sous une version plus récente.\n"
    "\n"
    "sesamviewer est distribué dans l'espoir qu'il vous sera utile, mais\n"
    "SANS AUCUNE GARANTIE. Consultez la licence GNU General Public License\n"
    "pour plus de détails.\n"
    "\n"
    "Vous devriez avoir reçu une copie de la GNU General Public License\n"
    "avec ce programme; regardez le fichier COPYING. Si vous ne le trouvez\n"
    "pas vous pouvez la réclamer en écrivant à l'adresse suivante:\n"
    "Free Software Foundation, Inc., 59 Temple Place - Suite 330, Boston,\n"
    "MA 02111-1307, USA.\n";

void about_window_main(GtkWindow *parent)
{
    gchar *authors[] = { "Sylvain Cresto", NULL };
    gchar *logofile;

    if (!logoEntity) {
        logofile = g_build_filename(PACKAGE_DATA_DIR, PACKAGE, LOGO_FILE, NULL);
        logoEntity = gdk_pixbuf_new_from_file(logofile, NULL);
        g_free(logofile);
    }

    gtk_show_about_dialog(parent, 
        "name", PACKAGE,
        "authors", authors,
        "comments", _("Un visualisateur GPL pour les fichiers sesam-vitale (B2 ...)"),
        "copyright", _("Copyright (c) Sylvain Cresto 2007"),
        "license", license,
        "logo", logoEntity,
        "version", VERSION,
        "translator-credits", _("translator-credits"),
        "website", "http://scresto.free.fr/sesamviewer",
        NULL);
}
