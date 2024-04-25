/* b2.h
 * vim:et:ts=8:sts=4:sw=4
 */

#ifndef B2_H_
#define B2_H_ 1

extern _fichier *init_b2(GError **error);
extern gboolean open_b2(gchar *filename, GtkTreeView *treeview, GtkTextView *textview, GError **error);

#endif
