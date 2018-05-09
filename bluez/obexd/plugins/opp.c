/*
 *
 *  OBEX Server
 *
 *  Copyright (C) 2007-2010  Nokia Corporation
 *  Copyright (C) 2007-2010  Marcel Holtmann <marcel@holtmann.org>
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>

#include <glib.h>

#include "obexd.h"
#include "plugin.h"
#include "obex.h"
#include "service.h"
#include "log.h"
#include "manager.h"
#include "filesystem.h"

#define VCARD_TYPE "text/x-vcard"
#define VCARD_FILE CONFIGDIR "/vcard.vcf"

static void *opp_connect(struct obex_session *os, int *err)
{
	if (err)
		*err = 0;

	return manager_register_transfer(os);
}

static void opp_progress(struct obex_session *os, void *user_data)
{
	manager_emit_transfer_progress(user_data);
}

static int opp_chkput(struct obex_session *os, void *user_data)
{
	char *folder, *name, *path;
	int32_t time;
	const char *t;
	int err;

	if (obex_get_size(os) == OBJECT_SIZE_DELETE)
		return -ENOSYS;

	t = obex_get_name(os);
	if (t != NULL && !is_filename(t))
		return -EBADR;

	if (obex_option_auto_accept()) {
		folder = g_strdup(obex_option_root_folder());
		name = g_strdup(obex_get_name(os));
		goto skip_auth;
	}

	time = 0;
	err = manager_request_authorization(user_data, time, &folder, &name);
	if (err < 0)
		return -EPERM;

	if (folder == NULL)
		folder = g_strdup(obex_option_root_folder());

	if (name == NULL)
		name = g_strdup(obex_get_name(os));

skip_auth:
	if (name == NULL || strlen(name) == 0) {
		err = -EBADR;
		goto failed;
	}

	if (g_strcmp0(name, obex_get_name(os)) != 0)
		obex_set_name(os, name);

	path = g_build_filename(folder, name, NULL);

	err = obex_put_stream_start(os, path);

	g_free(path);

	if (err < 0)
		goto failed;

	manager_emit_transfer_started(user_data);

failed:
	g_free(folder);
	g_free(name);

	return err;
}

static int opp_put(struct obex_session *os, void *user_data)
{
	const char *name = obex_get_name(os);
	const char *folder = obex_option_root_folder();

	if (folder == NULL)
		return -EPERM;

	if (name == NULL)
		return -EBADR;

	return 0;
}

static int opp_get(struct obex_session *os, void *user_data)
{
	const char *type;

	if (obex_get_name(os))
		return -EPERM;

	type = obex_get_type(os);

	if (type == NULL)
		return -EPERM;

	if (g_ascii_strcasecmp(type, VCARD_TYPE) == 0) {
		if (obex_get_stream_start(os, VCARD_FILE) < 0)
			return -ENOENT;

	} else
		return -EPERM;

	return 0;
}

static void opp_disconnect(struct obex_session *os, void *user_data)
{
	manager_unregister_transfer(user_data);
}

static void opp_reset(struct obex_session *os, void *user_data)
{
	manager_emit_transfer_completed(user_data);
}

static struct obex_service_driver driver = {
	.name = "Object Push server",
	.service = OBEX_OPP,
	.connect = opp_connect,
	.progress = opp_progress,
	.disconnect = opp_disconnect,
	.get = opp_get,
	.put = opp_put,
	.chkput = opp_chkput,
	.reset = opp_reset
};

static int opp_init(void)
{
	return obex_service_driver_register(&driver);
}

static void opp_exit(void)
{
	obex_service_driver_unregister(&driver);
}

OBEX_PLUGIN_DEFINE(opp, opp_init, opp_exit)
