/* error.h
 * vim:et:ts=8:sts=4:sw=4
 */

#ifndef ERROR_H_
#define ERROR_H_ 1

#define G_SESAM_VIEWER sesamviewer_error_quark()

#define _ERR_ERROR_INVALID_DATA     10
#define _ERR_WARNING_INVALID_DATA   20
#define _ERR_NO_MEMORY              30
#define _ERR_SYS_ERROR              40
extern GQuark sesamviewer_error_quark (void);

#define NO_ERROR(error) (error && !*error)

#endif
