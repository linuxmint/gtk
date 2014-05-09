/*
* Copyright 2013 Canonical Ltd.
*
* This program is free software: you can redistribute it and/or modify it
* under the terms of the GNU General Public License version 3, as published
* by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranties of
* MERCHANTABILITY, SATISFACTORY QUALITY, or FITNESS FOR A PARTICULAR
* PURPOSE.  See the GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License along
* with this program.  If not, see <http://www.gnu.org/licenses/>.
*
* Authors:
*     Lars Uebernickel <lars.uebernickel@canonical.com>
*/

#include "config.h"
#include "ubuntumenuitemfactory.h"

G_DEFINE_INTERFACE_WITH_CODE (UbuntuMenuItemFactory, ubuntu_menu_item_factory, G_TYPE_OBJECT,
  GIOExtensionPoint *ep = g_io_extension_point_register (UBUNTU_MENU_ITEM_FACTORY_EXTENSION_POINT_NAME);
  g_io_extension_point_set_required_type (ep, g_define_type_id);)

/*
 * ubuntu_menu_item_factory_get_all:
 *
 * Returns a static list of all registered factories.
 */
GList *
ubuntu_menu_item_factory_get_all (void)
{
  static GList *factories = NULL;

  if (factories == NULL)
    {
      GIOExtensionPoint *ep;
      GList *it;

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

  return factories;
}

static void
ubuntu_menu_item_factory_default_init (UbuntuMenuItemFactoryInterface *iface)
{
}

GtkMenuItem *
ubuntu_menu_item_factory_create_menu_item (UbuntuMenuItemFactory *factory,
                                           const gchar           *type,
                                           GMenuItem             *menuitem,
                                           GActionGroup          *actions)
{
  return UBUNTU_MENU_ITEM_FACTORY_GET_IFACE (factory)->create_menu_item (factory, type, menuitem, actions);
}
