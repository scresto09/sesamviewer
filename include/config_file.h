/* about_window.h
 * vim:et:ts=8:sts=4:sw=4
 */

#ifndef CONFIG_FILE_WINDOW_H_
#define CONFIG_FILE_WINDOW_H_ 1

extern void read_config(GError **error);
extern gboolean get_boolean_config(gchar *key);
extern void write_config(GError **error);
extern void set_boolean_config(gchar *key, gboolean value);
extern void save_window_position(GtkWindow *window);
extern gboolean restore_window_position(GtkWindow *window);
extern gchar *get_char_config(gchar *key, gchar *value);
extern void set_string_config(gchar *key, gchar *value);
extern gchar *get_string_config(gchar *key);
extern gint get_int_config(gchar *key);
extern void set_int_config(gchar *key, gint value);

#endif
