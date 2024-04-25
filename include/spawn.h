/* spawn.h
 * vim:et:ts=8:sts=4:sw=4
 */

#ifndef SPAWN_H_
#define SPAWN_H_ 1

extern void process_init(int *argc, char ***argv);
extern GPid open_new_window(gchar *file, GError **error);

#endif
