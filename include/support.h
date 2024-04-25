/* main.c
 * vim:et:ts=8:sts=4:sw=4
 */

#ifndef SUPPORT_H_
#define SUPPORT_H_ 1

extern GladeXML *load_widget(gchar *onlyname, GError **error);
extern xmlDocPtr load_xml(gchar *onlyname, GError **error);
extern xmlNodePtr search_entity_doc(xmlDocPtr doc, gchar *name);
extern GSList *lines_from_xml(xmlNodePtr xml);
extern _fichier *file_from_xml(xmlNodePtr xml, gchar *name);
extern gboolean bool_xmlGetProp(xmlNodePtr cur, const xmlChar *name);
extern gint int_xmlGetProp(xmlNodePtr cur, const xmlChar *name);
extern _line *get_line(_fichier *file, gchar *data);
extern GHashTable *alloc_data(gpointer data, gint data_len, gpointer buffer, gint buffer_len);
extern GArray *get_array_from_line(_line *line, gchar *data);
extern gchar *format_positions(_segment *segment);
extern gchar *get_data_from_segment(_segment *segment, gchar *data);
extern gchar *get_details_from_segment(_segment *segment, gchar *value);
extern gchar *get_config_filename();
extern gchar *replace_no_alnum(gchar *buffer, gint with);
extern open_callback_t detect_file_type(gchar *filename, GError **error);
extern gint get_line_length(_line *line);
extern gpointer get_variable_value(_fichier *file, gchar *type, gchar *name);
extern gboolean append_text(GtkTextBuffer *buffer, gchar *txt);
extern void get_cursor(GtkTextView *textview, gint *x, gint *y);
extern void move_cursor(GtkTextView *textview, gint x, gint y);
extern void get_max_cursor(GtkTextView *textview, gint *maxy);

#endif
