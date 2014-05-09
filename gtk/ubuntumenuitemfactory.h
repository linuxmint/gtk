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

#ifndef __UBUNTU_MENU_ITEM_FACTORY_H__
#define __UBUNTU_MENU_ITEM_FACTORY_H__

#include <gtk/gtkmenuitem.h>

G_BEGIN_DECLS

#define UBUNTU_TYPE_MENU_ITEM_FACTORY         (ubuntu_menu_item_factory_get_type ())
#define UBUNTU_MENU_ITEM_FACTORY(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), UBUNTU_TYPE_MENU_ITEM_FACTORY, UbuntuMenuItemFactory))
#define UBUNTU_IS_MENU_ITEM_FACTORY(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), UBUNTU_TYPE_MENU_ITEM_FACTORY))
#define UBUNTU_MENU_ITEM_FACTORY_GET_IFACE(o) (G_TYPE_INSTANCE_GET_INTERFACE ((o), UBUNTU_TYPE_MENU_ITEM_FACTORY, UbuntuMenuItemFactoryInterface))

#define UBUNTU_MENU_ITEM_FACTORY_EXTENSION_POINT_NAME "ubuntu-menu-item-factory"

typedef struct _UbuntuMenuItemFactoryInterface UbuntuMenuItemFactoryInterface;
typedef struct _UbuntuMenuItemFactory          UbuntuMenuItemFactory;

struct _UbuntuMenuItemFactoryInterface
{
  GTypeInterface iface;

  GtkMenuItem * (*create_menu_item)  (UbuntuMenuItemFactory *factory,
                                      const gchar           *type,
                                      GMenuItem             *menuitem,
                                      GActionGroup          *actions);
};

GDK_AVAILABLE_IN_3_10
GList *                 ubuntu_menu_item_factory_get_all                (void);

GDK_AVAILABLE_IN_3_10
GType                   ubuntu_menu_item_factory_get_type               (void);

GDK_AVAILABLE_IN_3_10
GtkMenuItem *           ubuntu_menu_item_factory_create_menu_item       (UbuntuMenuItemFactory *factory,
                                                                         const gchar           *type,
                                                                         GMenuItem             *menuitem,
                                                                         GActionGroup          *actions);

G_END_DECLS

#endif
