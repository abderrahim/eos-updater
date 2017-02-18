/* -*- mode: C; c-file-style: "gnu"; indent-tabs-mode: nil; -*-
 *
 * Copyright © 2016 Endless Mobile
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * Author: Will Thompson <wjt@endlessm.com>
 */

#include "eos-updater-live-boot.h"
#include "eos-updater-object.h"

#define LIVE_BOOT_MESSAGE "Updater disabled on live systems"

gboolean
is_installed_system (GError **error_out)
{
  const gchar *force = g_getenv ("EU_FORCE_LIVE_BOOT");
  GError *error = NULL;
  g_autofree gchar *cmdline = NULL;

  if (!g_file_get_contents ("/proc/cmdline", &cmdline, NULL, &error))
    {
      g_printerr ("unable to read /proc/cmdline: %s\n", error->message);
      g_error_free (error);
      return TRUE;
    }

  if (g_regex_match_simple ("\\bendless\\.live_boot\\b", cmdline, 0, 0) ||
      (force != NULL && *force != '\0'))
    {
      g_set_error_literal (error_out,
        EOS_UPDATER_ERROR, EOS_UPDATER_ERROR_LIVE_BOOT,
        LIVE_BOOT_MESSAGE);
      return FALSE;
    }

  return TRUE;
}

gboolean
handle_on_live_boot (EosUpdater            *updater,
                     GDBusMethodInvocation *call,
                     gpointer               user_data)
{
  g_dbus_method_invocation_return_error_literal (call,
    EOS_UPDATER_ERROR, EOS_UPDATER_ERROR_LIVE_BOOT, LIVE_BOOT_MESSAGE);
  return TRUE;
}
