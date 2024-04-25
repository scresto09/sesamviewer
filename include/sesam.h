/* sesam.h
 * vim:et:ts=8:sts=4:sw=4
 */

#ifndef SESAM_H_
#define SESAM_H_ 1

#include "config.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <gtk/gtk.h>
#include <glib.h>
#include <glade/glade.h>
#include <libxml/parser.h>
#include <unistd.h>
#include <sys/types.h>

#ifdef ENABLE_NLS
  #include <glib/gi18n-lib.h>
#endif
#include <glib/gstdio.h>

#include "constants.h"
#include "declare.h"
#include "error.h"
#include "b2.h"
#include "noemie.h"
#include "about_window.h"
#include "config_file.h"
#include "main_window.h"
#include "error_window.h"
#include "detail_window.h"
#include "goto_window.h"
#include "properties_window.h"
#include "open_window.h"
#include "spawn.h"
#include "support.h"
#endif
