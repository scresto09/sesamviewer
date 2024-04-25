/* declare.h
 * vim:et:ts=8:sts=4:sw=4
 */

#ifndef DECLARE_H_
#define DECLARE_H_ 1

typedef struct {
    gchar *name;    /* nom du segment */
    gint ordre;     /* position du segment */
    gint debut;     /* debut du segment dans les données */
    gint fin;       /* fin du segment dans les données */
    gint longueur;  /* fin - debut */
    gchar type[2];  /* type de la donnée attendu */
    gchar obligatoire[2];   /* présence de la donnée */
    gchar *valeur;  /* la valeur en elle même */
    gchar *saveas;  /* alias du segment */
    gboolean pkey;  /* es-ce une clef primaire ? */
    GHashTable *descriptions;   /* détails en fonction de la valeur */
} _segment;

typedef struct {
    GSList *pkeylist;	/* liste des pkey */
    GSList *saveaslist;	/* liste des valeurs à sauvegarder */
    GSList *segmentlist; /* liste des segments */
    gchar *name; /* nom court de la ligne */
    gchar *description; /* description de la ligne */
    gint rang;  /* indentation */
} _line;

typedef struct {
    gpointer data;
    gpointer buffer;
} _data;

typedef struct {
    GSList *linelist;
    GHashTable *variables_str;
    GHashTable *variables_int;
    gchar *name;
} _fichier;

typedef gboolean (*(open_callback_t))(gchar *filename, GtkTreeView *treeview, GtkTextView *textview, GError **error);
#endif
