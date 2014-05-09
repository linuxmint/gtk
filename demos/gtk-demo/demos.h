typedef	GtkWidget *(*GDoDemoFunc) (GtkWidget *do_widget);

typedef struct _Demo Demo;

struct _Demo 
{
  gchar *name;
  gchar *title;
  gchar *filename;
  GDoDemoFunc func;
  Demo *children;
};

GtkWidget *do_application (GtkWidget *do_widget);
GtkWidget *do_assistant (GtkWidget *do_widget);
GtkWidget *do_builder (GtkWidget *do_widget);
GtkWidget *do_button_box (GtkWidget *do_widget);
GtkWidget *do_changedisplay (GtkWidget *do_widget);
GtkWidget *do_clipboard (GtkWidget *do_widget);
GtkWidget *do_colorsel (GtkWidget *do_widget);
GtkWidget *do_combobox (GtkWidget *do_widget);
GtkWidget *do_css_accordion (GtkWidget *do_widget);
GtkWidget *do_css_basics (GtkWidget *do_widget);
GtkWidget *do_css_multiplebgs (GtkWidget *do_widget);
GtkWidget *do_css_pixbufs (GtkWidget *do_widget);
GtkWidget *do_css_shadows (GtkWidget *do_widget);
GtkWidget *do_dialog (GtkWidget *do_widget);
GtkWidget *do_drawingarea (GtkWidget *do_widget);
GtkWidget *do_editable_cells (GtkWidget *do_widget);
GtkWidget *do_entry_buffer (GtkWidget *do_widget);
GtkWidget *do_entry_completion (GtkWidget *do_widget);
GtkWidget *do_expander (GtkWidget *do_widget);
GtkWidget *do_hypertext (GtkWidget *do_widget);
GtkWidget *do_iconview (GtkWidget *do_widget);
GtkWidget *do_iconview_edit (GtkWidget *do_widget);
GtkWidget *do_images (GtkWidget *do_widget);
GtkWidget *do_infobar (GtkWidget *do_widget);
GtkWidget *do_links (GtkWidget *do_widget);
GtkWidget *do_listbox (GtkWidget *do_widget);
GtkWidget *do_list_store (GtkWidget *do_widget);
GtkWidget *do_menus (GtkWidget *do_widget);
GtkWidget *do_offscreen_window (GtkWidget *do_widget);
GtkWidget *do_offscreen_window2 (GtkWidget *do_widget);
GtkWidget *do_overlay (GtkWidget *do_widget);
GtkWidget *do_panes (GtkWidget *do_widget);
GtkWidget *do_pickers (GtkWidget *do_widget);
GtkWidget *do_pixbufs (GtkWidget *do_widget);
GtkWidget *do_printing (GtkWidget *do_widget);
GtkWidget *do_revealer (GtkWidget *do_widget);
GtkWidget *do_rotated_text (GtkWidget *do_widget);
GtkWidget *do_search_entry (GtkWidget *do_widget);
GtkWidget *do_search_entry2 (GtkWidget *do_widget);
GtkWidget *do_sizegroup (GtkWidget *do_widget);
GtkWidget *do_spinner (GtkWidget *do_widget);
GtkWidget *do_stack (GtkWidget *do_widget);
GtkWidget *do_textview (GtkWidget *do_widget);
GtkWidget *do_textscroll (GtkWidget *do_widget);
GtkWidget *do_theming_style_classes (GtkWidget *do_widget);
GtkWidget *do_toolpalette (GtkWidget *do_widget);
GtkWidget *do_transparent (GtkWidget *do_widget);
GtkWidget *do_tree_store (GtkWidget *do_widget);

Demo child0[] = {
  { "css_accordion", "CSS Accordion", "css_accordion.c", do_css_accordion, NULL },
  { "css_basics", "CSS Basics", "css_basics.c", do_css_basics, NULL },
  { "css_multiplebgs", "Multiple Backgrounds", "css_multiplebgs.c", do_css_multiplebgs, NULL },
  { "css_pixbufs", "Animated Backgrounds", "css_pixbufs.c", do_css_pixbufs, NULL },
  { "css_shadows", "Shadows", "css_shadows.c", do_css_shadows, NULL },
  { "theming_style_classes", "Style Classes", "theming_style_classes.c", do_theming_style_classes, NULL },
  { NULL } 
};

Demo child1[] = {
  { "editable_cells", "Editable Cells", "editable_cells.c", do_editable_cells, NULL },
  { "list_store", "List Store", "list_store.c", do_list_store, NULL },
  { "tree_store", "Tree Store", "tree_store.c", do_tree_store, NULL },
  { NULL } 
};

Demo child2[] = {
  { "entry_buffer", "Entry Buffer", "entry_buffer.c", do_entry_buffer, NULL },
  { "entry_completion", "Entry Completion", "entry_completion.c", do_entry_completion, NULL },
  { "search_entry", "Search Entry", "search_entry.c", do_search_entry, NULL },
  { "search_entry2", "Delayed Search Entry", "search_entry2.c", do_search_entry2, NULL },
  { NULL } 
};

Demo child3[] = {
  { "hypertext", "Hypertext", "hypertext.c", do_hypertext, NULL },
  { "textview", "Multiple Views", "textview.c", do_textview, NULL },
  { "textscroll", "Automatic scrolling", "textscroll.c", do_textscroll, NULL },
  { NULL } 
};

Demo child4[] = {
  { "iconview", "Icon View Basics", "iconview.c", do_iconview, NULL },
  { "iconview_edit", "Editing and Drag-and-Drop", "iconview_edit.c", do_iconview_edit, NULL },
  { NULL } 
};

Demo child5[] = {
  { "offscreen_window", "Rotated button", "offscreen_window.c", do_offscreen_window, NULL },
  { "offscreen_window2", "Effects", "offscreen_window2.c", do_offscreen_window2, NULL },
  { NULL } 
};

Demo gtk_demos[] = {
  { "application", "Application class", "application.c", do_application, NULL }, 
  { "assistant", "Assistant", "assistant.c", do_assistant, NULL }, 
  { "builder", "Builder", "builder.c", do_builder, NULL }, 
  { "button_box", "Button Boxes", "button_box.c", do_button_box, NULL }, 
  { NULL, "CSS Theming", NULL, NULL, child0 }, 
  { "changedisplay", "Change Display", "changedisplay.c", do_changedisplay, NULL }, 
  { "clipboard", "Clipboard", "clipboard.c", do_clipboard, NULL }, 
  { "colorsel", "Color Chooser", "colorsel.c", do_colorsel, NULL }, 
  { "combobox", "Combo boxes", "combobox.c", do_combobox, NULL }, 
  { "dialog", "Dialog and Message Boxes", "dialog.c", do_dialog, NULL }, 
  { "drawingarea", "Drawing Area", "drawingarea.c", do_drawingarea, NULL }, 
  { NULL, "Entry", NULL, NULL, child2 }, 
  { "expander", "Expander", "expander.c", do_expander, NULL }, 
  { NULL, "Icon View", NULL, NULL, child4 }, 
  { "images", "Images", "images.c", do_images, NULL }, 
  { "infobar", "Info bar", "infobar.c", do_infobar, NULL }, 
  { "links", "Links", "links.c", do_links, NULL }, 
  { "listbox", "List Box", "listbox.c", do_listbox, NULL }, 
  { "menus", "Menus", "menus.c", do_menus, NULL }, 
  { NULL, "Offscreen windows", NULL, NULL, child5 }, 
  { "overlay", "Overlay", "overlay.c", do_overlay, NULL }, 
  { "panes", "Paned Widgets", "panes.c", do_panes, NULL }, 
  { "pickers", "Pickers", "pickers.c", do_pickers, NULL }, 
  { "pixbufs", "Pixbufs", "pixbufs.c", do_pixbufs, NULL }, 
  { "printing", "Printing", "printing.c", do_printing, NULL }, 
  { "revealer", "Revealer", "revealer.c", do_revealer, NULL }, 
  { "rotated_text", "Rotated Text", "rotated_text.c", do_rotated_text, NULL }, 
  { "sizegroup", "Size Groups", "sizegroup.c", do_sizegroup, NULL }, 
  { "spinner", "Spinner", "spinner.c", do_spinner, NULL }, 
  { "stack", "Stack", "stack.c", do_stack, NULL }, 
  { NULL, "Text Widget", NULL, NULL, child3 }, 
  { "toolpalette", "Tool Palette", "toolpalette.c", do_toolpalette, NULL }, 
  { "transparent", "Transparent", "transparent.c", do_transparent, NULL }, 
  { NULL, "Tree View", NULL, NULL, child1 },
  { NULL } 
};
