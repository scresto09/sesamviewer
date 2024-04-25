/* noemie.h
 * vim:et:ts=8:sts=4:sw=4
 */

#ifndef NOEMIE_H_
#define NOEMIE_H_ 1

#define NOEMIE_BUFFER 8192
extern _fichier *init_noemie(GError **error);
extern gboolean open_noemie(gchar *filename, GtkTreeView *treeview, GtkTextView *textview, GError **error);

#endif
