#ifndef __RESOURCE_gtk_inspector_H__
#define __RESOURCE_gtk_inspector_H__

#include <gio/gio.h>

extern GResource *gtk_inspector_get_resource (void);

extern void gtk_inspector_register_resource (void);
extern void gtk_inspector_unregister_resource (void);

#endif
