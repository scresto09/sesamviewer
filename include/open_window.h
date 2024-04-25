/* open_window.h
 * vim:et:ts=8:sts=4:sw=4
 */

#ifndef OPEN_WINDOW_H_
#define OPEN_WINDOW_H_ 1

extern void ouvrir_un_fichier(gchar *fichier, open_callback_t open_callback);
extern void selectionner_un_fichier(GtkWindow *Aparent, gboolean new_window);

#endif
