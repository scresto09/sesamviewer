/* main_window.h
 * vim:et:ts=8:sts=4:sw=4
 */

#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_ 1

extern GdkPixbuf *iconEntity;
extern GladeXML *gladexmlMain;
extern void main_window();
extern void clear_main_window();
extern const gchar *restore_original_title();

#endif
