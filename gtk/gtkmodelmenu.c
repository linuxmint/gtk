/*
 * Copyright © 2011 Red Hat, Inc.
 * Copyright © 2011 Canonical Limited
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the licence, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Matthias Clasen <mclasen@redhat.com>
 *         Ryan Lortie <desrt@desrt.ca>
 */

#include "config.h"

#include "gtkmenushell.h"
#include "gtkmenubar.h"
#include "gtkmenu.h"

#include "gtkseparatormenuitem.h"
#include "gtkmodelmenuitem.h"
#include "gtkapplicationprivate.h"
#include "gtkwidgetprivate.h"
#include "ubuntumenuitemfactory.h"

#define MODEL_MENU_WIDGET_DATA "gtk-model-menu-widget-data"

typedef struct {
  GMenuModel        *model;
  GtkMenuShell      *shell;
  guint              update_idle;
  GSList            *connected;
  gboolean           with_separators;
  gint               n_items;
  gchar             *action_namespace;
} GtkModelMenuBinding;

static void
gtk_model_menu_binding_items_changed (GMenuModel *model,
                                      gint        position,
                                      gint        removed,
                                      gint        added,
                                      gpointer    user_data);
static void gtk_model_menu_binding_append_model (GtkModelMenuBinding *binding,
                                                 GMenuModel *model,
                                                 const gchar *action_namespace,
                                                 gboolean with_separators);

static void
gtk_model_menu_binding_free (gpointer data)
{
  GtkModelMenuBinding *binding = data;

  /* disconnect all existing signal handlers */
  while (binding->connected)
    {
      g_signal_handlers_disconnect_by_func (binding->connected->data, gtk_model_menu_binding_items_changed, binding);
      g_object_unref (binding->connected->data);

      binding->connected = g_slist_delete_link (binding->connected, binding->connected);
    }

  g_object_unref (binding->model);
  g_free (binding->action_namespace);

  g_slice_free (GtkModelMenuBinding, binding);
}

static gboolean
object_class_has_property (GObjectClass *class,
                           const gchar  *name,
                           GType         type)
{
  GParamSpec *pspec;

  pspec = g_object_class_find_property (class, name);
  return pspec != NULL && G_PARAM_SPEC_VALUE_TYPE (pspec) == type;
}

static GtkMenuItem *
gtk_model_menu_create_item_from_type (GType      type,
                                      GMenuItem *menuitem)
{
  GObjectClass *class;
  GtkMenuItem *item = NULL;

  class = g_type_class_ref (type);

  if (g_type_is_a (type, GTK_TYPE_MENU_ITEM) &&
      object_class_has_property (class, "menu-item", G_TYPE_MENU_ITEM) &&
      object_class_has_property (class, "action-group", G_TYPE_ACTION_GROUP))
    {
      item = g_object_new (type,
                           "menu-item", menuitem,
                           NULL);

      g_object_set (item,
                    "action-group", _gtk_widget_get_action_muxer (GTK_WIDGET (item)),
                    NULL);
    }
  else
    {
      g_warning ("gtk_menu_new_from_model: cannot create menu item from type '%s'.\n"
                 "  Does it derive from GtkMenuItem and have 'menu-item' and 'action-group' properties?",
                 g_type_name (type));
    }

  g_type_class_unref (class);
  return item;
}

static GtkMenuItem *
gtk_model_menu_create_custom_item_from_factory (const gchar  *typename,
                                                GMenuItem    *menuitem,
                                                GActionGroup *actions)
{
  GList *it;
  static GList *factories = NULL;

  if (factories == NULL)
    {
      GIOExtensionPoint *ep;

      g_type_ensure (UBUNTU_TYPE_MENU_ITEM_FACTORY);
      ep = g_io_extension_point_lookup (UBUNTU_MENU_ITEM_FACTORY_EXTENSION_POINT_NAME);
      for (it = g_io_extension_point_get_extensions (ep); it != NULL; it = it->next)
        {
          GIOExtension *ext = it->data;
          UbuntuMenuItemFactory *factory;

          factory = g_object_new (g_io_extension_get_type (ext), NULL);
          factories = g_list_prepend (factories, factory);
        }
      factories = g_list_reverse (factories);
    }

  for (it = factories; it != NULL; it = it->next)
    {
      UbuntuMenuItemFactory *factory = it->data;
      GtkMenuItem *item;

      item = ubuntu_menu_item_factory_create_menu_item (factory, typename, menuitem, actions);
      if (item)
        return item;
    }

  return NULL;
}

static GtkMenuItem *
gtk_model_menu_create_custom_item (GMenuModel  *menu,
                                   gint         item_index,
                                   const gchar *action_namespace,
                                   GtkWidget   *parent)
{
  GMenuItem *menuitem;
  gchar *typename;
  GtkMenuItem *item = NULL;

  menuitem = g_menu_item_new_from_model (menu, item_index);
  if (action_namespace)
    {
      gchar *action = NULL;
      gchar *fullname = NULL;

      g_menu_item_get_attribute (menuitem, G_MENU_ATTRIBUTE_ACTION, "s", &action);
      if (action)
        {
          fullname = g_strconcat (action_namespace, ".", action, NULL);
          g_menu_item_set_attribute (menuitem, G_MENU_ATTRIBUTE_ACTION, "s", fullname);

          g_free (action);
          g_free (fullname);
        }
    }

  if (g_menu_model_get_item_attribute (menu, item_index, "x-canonical-type", "s", &typename))
    {
      GActionGroup *actions;

      /* Passing the parent muxer is wrong, but we'll only have access
       * to the menuitem's muxer after the widget has been created.
       * Thus we'd need some other form of passing the action group to
       * the widget, which would complicate things for no practical
       * reason: the panel service is the only consumer of this API and
       * it will never call gtk_widget_insert_action_group() on the
       * returned menu item.
       */
      actions = G_ACTION_GROUP (_gtk_widget_get_action_muxer (parent));
      item = gtk_model_menu_create_custom_item_from_factory (typename, menuitem, actions);

      /* continue supporting GTypes for now (the "old" behavior) */
      if (item == NULL)
        {
          GType type;

          type = g_type_from_name (typename);
          if (type > 0)
            item = gtk_model_menu_create_item_from_type (type, menuitem);
        }

      if (item == NULL)
        g_warning ("gtk_menu_new_from_model: cannot find type '%s'", typename);

      g_free (typename);
    }

  g_object_unref (menuitem);
  return item;
}

static void
gtk_model_menu_binding_append_item (GtkModelMenuBinding  *binding,
                                    GMenuModel           *model,
                                    const gchar          *action_namespace,
                                    gint                  item_index,
                                    gchar               **heading)
{
  GMenuModel *section;

  if ((section = g_menu_model_get_item_link (model, item_index, "section")))
    {
      gchar *section_namespace = NULL;

      g_menu_model_get_item_attribute (model, item_index, "label", "s", heading);
      g_menu_model_get_item_attribute (model, item_index, "action-namespace", "s", &section_namespace);

      if (action_namespace)
        {
          gchar *namespace = g_strjoin (".", action_namespace, section_namespace, NULL);
          gtk_model_menu_binding_append_model (binding, section, namespace, FALSE);
          g_free (namespace);
        }
      else
        {
          gtk_model_menu_binding_append_model (binding, section, section_namespace, FALSE);
        }

      g_free (section_namespace);
      g_object_unref (section);
    }
  else
    {
      GtkMenuItem *item;

      item = gtk_model_menu_create_custom_item (model, item_index, action_namespace, GTK_WIDGET (binding->shell));
      if (item == NULL)
        item = gtk_model_menu_item_new (model, item_index, action_namespace);

      gtk_menu_shell_append (binding->shell, GTK_WIDGET (item));
      gtk_widget_show (GTK_WIDGET (item));
      binding->n_items++;
    }
}

static void
gtk_model_menu_binding_append_model (GtkModelMenuBinding *binding,
                                     GMenuModel          *model,
                                     const gchar         *action_namespace,
                                     gboolean             with_separators)
{
  gint n, i;

  g_signal_connect (model, "items-changed", G_CALLBACK (gtk_model_menu_binding_items_changed), binding);
  binding->connected = g_slist_prepend (binding->connected, g_object_ref (model));

  /* Deciding if we should show a separator is a bit difficult.  There
   * are two types of separators:
   *
   *  - section headings (when sections have 'label' property)
   *
   *  - normal separators automatically put between sections
   *
   * The easiest way to think about it is that a section usually has a
   * separator (or heading) immediately before it.
   *
   * There are three exceptions to this general rule:
   *
   *  - empty sections don't get separators or headings
   *
   *  - sections only get separators and headings at the toplevel of a
   *    menu (ie: no separators on nested sections or in menubars)
   *
   *  - the first section in the menu doesn't get a normal separator,
   *    but it can get a header (if it's not empty)
   *
   * Unfortunately, we cannot simply check the size of the section in
   * order to determine if we should place a header: the section may
   * contain other sections that are themselves empty.  Instead, we need
   * to append the section, and check if we ended up with any actual
   * content.  If we did, then we need to insert before that content.
   * We use 'our_position' to keep track of this.
   */

  n = g_menu_model_get_n_items (model);

  for (i = 0; i < n; i++)
    {
      gint our_position = binding->n_items;
      gchar *heading = NULL;

      gtk_model_menu_binding_append_item (binding, model, action_namespace, i, &heading);

      if (with_separators && our_position < binding->n_items)
        {
          GtkWidget *separator = NULL;

          if (heading)
            {
              separator = gtk_menu_item_new_with_label (heading);
              gtk_widget_set_sensitive (separator, FALSE);
            }
          else if (our_position > 0)
            separator = gtk_separator_menu_item_new ();

          if (separator)
            {
              gtk_menu_shell_insert (binding->shell, separator, our_position);
              gtk_widget_show (separator);
              binding->n_items++;
            }
        }

      g_free (heading);
    }
}

static void
gtk_model_menu_binding_populate (GtkModelMenuBinding *binding)
{
  GList *children;

  /* remove current children */
  children = gtk_container_get_children (GTK_CONTAINER (binding->shell));
  while (children)
    {
      gtk_container_remove (GTK_CONTAINER (binding->shell), children->data);
      gtk_widget_destroy (children->data);
      children = g_list_delete_link (children, children);
    }

  binding->n_items = 0;

  /* add new items from the model */
  gtk_model_menu_binding_append_model (binding, binding->model, binding->action_namespace, binding->with_separators);
}

static gboolean
gtk_model_menu_binding_handle_changes (gpointer user_data)
{
  GtkModelMenuBinding *binding = user_data;

  /* disconnect all existing signal handlers */
  while (binding->connected)
    {
      g_signal_handlers_disconnect_by_func (binding->connected->data, gtk_model_menu_binding_items_changed, binding);
      g_object_unref (binding->connected->data);

      binding->connected = g_slist_delete_link (binding->connected, binding->connected);
    }

  gtk_model_menu_binding_populate (binding);

  binding->update_idle = 0;

  g_object_unref (binding->shell);

  return G_SOURCE_REMOVE;
}

static void
gtk_model_menu_binding_items_changed (GMenuModel *model,
                                      gint        position,
                                      gint        removed,
                                      gint        added,
                                      gpointer    user_data)
{
  GtkModelMenuBinding *binding = user_data;

  if (binding->update_idle == 0)
    {
      binding->update_idle = gdk_threads_add_idle (gtk_model_menu_binding_handle_changes, user_data);
      g_object_ref (binding->shell);
    }
}

/**
 * gtk_menu_shell_bind_model:
 * @menu_shell: a #GtkMenuShell
 * @model: (allow-none): the #GMenuModel to bind to or %NULL to remove
 *   binding
 * @action_namespace: (allow-none): the namespace for actions in @model
 * @with_separators: %TRUE if toplevel items in @shell should have
 *   separators between them
 *
 * Establishes a binding between a #GtkMenuShell and a #GMenuModel.
 *
 * The contents of @shell are removed and then refilled with menu items
 * according to @model.  When @model changes, @shell is updated.
 * Calling this function twice on @shell with different @model will
 * cause the first binding to be replaced with a binding to the new
 * model. If @model is %NULL then any previous binding is undone and
 * all children are removed.
 *
 * @with_separators determines if toplevel items (eg: sections) have
 * separators inserted between them.  This is typically desired for
 * menus but doesn't make sense for menubars.
 *
 * If @action_namespace is non-%NULL then the effect is as if all
 * actions mentioned in the @model have their names prefixed with the
 * namespace, plus a dot.  For example, if the action "quit" is
 * mentioned and @action_namespace is "app" then the effective action
 * name is "app.quit".
 *
 * For most cases you are probably better off using
 * gtk_menu_new_from_model() or gtk_menu_bar_new_from_model() or just
 * directly passing the #GMenuModel to gtk_application_set_app_menu() or
 * gtk_application_set_menu_bar().
 *
 * Since: 3.6
 */
void
gtk_menu_shell_bind_model (GtkMenuShell *shell,
                           GMenuModel   *model,
                           const gchar  *action_namespace,
                           gboolean      with_separators)
{
  g_return_if_fail (GTK_IS_MENU_SHELL (shell));
  g_return_if_fail (model == NULL || G_IS_MENU_MODEL (model));

  if (model)
    {
      GtkModelMenuBinding *binding;

      binding = g_slice_new (GtkModelMenuBinding);
      binding->model = g_object_ref (model);
      binding->shell = shell;
      binding->update_idle = 0;
      binding->connected = NULL;
      binding->with_separators = with_separators;
      binding->action_namespace = g_strdup (action_namespace);

      g_object_set_data_full (G_OBJECT (shell), "gtk-model-menu-binding", binding, gtk_model_menu_binding_free);

      gtk_model_menu_binding_populate (binding);
    }

  else
    {
      GList *children;

      /* break existing binding */
      g_object_set_data (G_OBJECT (shell), "gtk-model-menu-binding", NULL);

      /* remove all children */
      children = gtk_container_get_children (GTK_CONTAINER (shell));
      while (children)
        {
          gtk_container_remove (GTK_CONTAINER (shell), children->data);
          children = g_list_delete_link (children, children);
        }
    }
}
