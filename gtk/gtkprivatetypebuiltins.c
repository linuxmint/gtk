
/* Generated data (by glib-mkenums) */

#include "config.h"
#include "gtk.h"
#include "gtkprivate.h"
#include "gtkprivatetypebuiltins.h"

/* enumerations from "gtktexthandleprivate.h" */
GType
_gtk_text_handle_position_get_type (void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            { GTK_TEXT_HANDLE_POSITION_CURSOR, "GTK_TEXT_HANDLE_POSITION_CURSOR", "cursor" },
            { GTK_TEXT_HANDLE_POSITION_SELECTION_START, "GTK_TEXT_HANDLE_POSITION_SELECTION_START", "selection-start" },
            { GTK_TEXT_HANDLE_POSITION_SELECTION_END, "GTK_TEXT_HANDLE_POSITION_SELECTION_END", "selection-end" },
            { 0, NULL, NULL }
        };
        etype = g_enum_register_static (g_intern_static_string ("GtkTextHandlePosition"), values);
    }
    return etype;
}

GType
_gtk_text_handle_mode_get_type (void)
{
    static GType etype = 0;
    if (G_UNLIKELY(etype == 0)) {
        static const GEnumValue values[] = {
            { GTK_TEXT_HANDLE_MODE_NONE, "GTK_TEXT_HANDLE_MODE_NONE", "none" },
            { GTK_TEXT_HANDLE_MODE_CURSOR, "GTK_TEXT_HANDLE_MODE_CURSOR", "cursor" },
            { GTK_TEXT_HANDLE_MODE_SELECTION, "GTK_TEXT_HANDLE_MODE_SELECTION", "selection" },
            { 0, NULL, NULL }
        };
        etype = g_enum_register_static (g_intern_static_string ("GtkTextHandleMode"), values);
    }
    return etype;
}



/* Generated data ends here */

