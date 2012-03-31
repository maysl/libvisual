/* Libvisual - The audio visualisation framework.
 *
 * Copyright (C) 2004, 2005, 2006 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *
 * $Id: lv_param.c,v 1.50 2006/01/22 13:23:37 synap Exp $
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation; either version 2.1
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#include "config.h"
#include "lv_param.h"
#include "lv_common.h"
#include "lv_param.h"
#include "lv_util.h"
#include "gettext.h"

int visual_param_container_dtor (VisParamContainer *self);

typedef struct {
    VisParamInfo  info;
    VisParamValue value;
} VisParamEntry;


VisParamContainer *visual_param_container_new (void)
{
    VisParamContainer *self;

    self = visual_mem_new0 (VisParamContainer, 1);

    visual_object_initialize (VISUAL_OBJECT (self), TRUE, (VisObjectDtorFunc) visual_param_container_dtor);

    visual_list_init (&self->entries, NULL);

    return self;
}

int visual_param_container_dtor (VisParamContainer *self)
{
    visual_object_destroy (VISUAL_OBJECT (&self->entries));

    return VISUAL_OK;
}

void visual_param_container_add (VisParamContainer *self, VisParamInfo *param)
{
    visual_return_if_fail (self  != NULL);
    visual_return_if_fail (param != NULL);

    visual_list_add (&self->entries, param);
}

void visual_param_container_add_many (VisParamContainer *self, VisParamInfo *params, unsigned int num_params)
{
    unsigned int i;

    visual_return_if_fail (self != NULL);
    visual_return_if_fail (params != NULL);
    visual_return_if_fail (num_params != 0);

    for (i = 0; i < num_params; i++)
        visual_param_container_add (self, &params[i]);
}

int visual_param_container_remove (VisParamContainer *self, const char *name)
{
    VisParamEntry *entry;
    VisListEntry *iter = NULL;

    visual_return_val_if_fail (self != NULL, FALSE);
    visual_return_val_if_fail (name != NULL, FALSE);

    while ((entry = visual_list_next (&self->entries, &iter)) != NULL) {

        if (strcmp (entry->info.name, name) == 0) {
            visual_list_delete (&self->entries, &iter);

            return TRUE;
        }
    }

    return FALSE;
}

void visual_param_container_set_param_value (VisParamContainer *self, const char *name, VisParamValue *value)
{
    visual_return_if_fail (self != NULL);
    visual_return_if_fail (name != NULL);
    visual_return_if_fail (value != NULL);

    /* FIXME: implement */
}


VisParamValue *visual_param_container_get_param_value (VisParamContainer *self, const char *name)
{
    VisListEntry  *iter = NULL;
    VisParamEntry *entry;

    visual_return_val_if_fail (self != NULL, NULL);
    visual_return_val_if_fail (name != NULL, NULL);

    while ((entry = visual_list_next (&self->entries, &iter)) != NULL) {
        entry = iter->data;

        if (strcmp (entry->info.name, name) == 0)
            return &entry->value;
    }

    return NULL;
}

VisParamType visual_param_container_get_param_type (VisParamContainer *self, const char *name)
{
    visual_return_val_if_fail (self != NULL, VISUAL_PARAM_TYPE_NONE);
    visual_return_val_if_fail (name != NULL, VISUAL_PARAM_TYPE_NONE);

    /* FIXME: Implement */

    return VISUAL_PARAM_TYPE_NONE;
}
