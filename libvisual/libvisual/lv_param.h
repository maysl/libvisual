/* Libvisual - The audio visualisation framework.
 * 
 * Copyright (C) 2004, 2005, 2006 Dennis Smit <ds@nerds-incorporated.org>
 *
 * Authors: Dennis Smit <ds@nerds-incorporated.org>
 *
 * $Id: lv_param.h,v 1.32 2006/01/22 13:23:37 synap Exp $
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

#ifndef _LV_PARAM_H
#define _LV_PARAM_H

#include <libvisual/lvconfig.h>
#include <libvisual/lv_defines.h>
#include <libvisual/lv_list.h>
#include <libvisual/lv_param_value.h>

/**
 * @defgroup VisParam VisParam
 * @{
 */

VISUAL_BEGIN_DECLS

#define VISUAL_PARAM_CONTAINER(obj)          (VISUAL_CHECK_CAST ((obj), VisParamContainer))
#define VISUAL_PARAM_CHANGED_FUNC(obj)       (VISUAL_CHECK_CAST ((obj), VisParamEntryCallback))

typedef struct _VisParamContainer VisParamContainer;
typedef struct _VisParamInfo      VisParamInfo;

/**
 * The param changed callback is used to be able to notify of changes within parameters. This should
 * not be used within the plugin itself, instead use the event queue there. This is so it's possible to
 * notify of changes outside the plugin. For example, this is needed by VisUI.
 *
 * @arg param Pointer to the param that has been changed, and to which the callback was set.
 * @arg priv Private argument, that can be set when adding the callback to the callback list.
 */
typedef void (*VisParamChangedFunc)(VisParamInfo *info, VisParamValue *value, void *priv);

/**
 * A parameter entry, used for plugin parameters and such.
 *
 * All members should never be accessed directly, instead methods should be used.
 */
struct _VisParamInfo {
    const char   *name;       /**< Parameter name. */
    const char   *desc;       /**< Parameter annotation. */
    VisParamType  type;       /**< Parameter type. */
};

struct _VisParamContainer {
    VisObject object;
    VisList   entries;
};

/**
 * Creates a new VisParamContainer structure.
 *
 * @return A newly allocated VisParamContainer structure.
 */
VisParamContainer *visual_param_container_new (void);

/**
 * Adds a VisParamEntry to a VisParamContainer.
 *
 * @param paramcontainer A pointer to the VisParamContainer in which the VisParamEntry is added.
 * @param param A pointer to the VisParamEntry that is added to the VisParamContainer.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_PARAM_CONTAINER_NULL, -VISUAL_ERROR_PARAM_NULL or
 *    error values returned by visual_list_add () on failure.
 */
void visual_param_container_add (VisParamContainer *self, VisParamInfo *param);

/**
 * Adds a list of VisParamEntry elements, the list is terminated by an
 * entry of type VISUAL_PARAM_ENTRY_TYPE_END.  All the elements are
 * reallocated, so this function can be used for static param lists.
 *
 * @param paramcontainer A pointer to the VisParamContainer in which the VisParamEntry elements are added.
 * @param params A pointer to the VisParamEntry elements that are added to the VisParamContainer.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_PARAM_CONTAINER_NULL or -VISUAL_ERROR_PARAM_NULL on failure.
 */
void visual_param_container_add_many (VisParamContainer *paramcontainer, VisParamInfo *params, unsigned int num_params);

/**
 * Removes a VisParamEntry from the VisParamContainer by giving the
 * name of the VisParamEntry that needs to be removed.
 *
 * @param paramcontainer A pointer to the VisParamContainer from which a VisParamEntry needs to be removed.
 * @param name The name of the VisParamEntry that needs to be removed from the VisParamContainer.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_PARAM_CONTAINER_NULL, -VISUAL_ERROR_NULL
 *    or -VISUAL_ERROR_PARAM_NOT_FOUND on failure.
 */
int visual_param_container_remove (VisParamContainer *self, const char *name);

void visual_param_container_set_param_value (VisParamContainer *self, const char *name, VisParamValue *value);

/**
 * Retrieve a VisParamEntry from a VisParamContainer by giving the
 * name of the VisParamEntry that is requested.
 *
 * @param paramcontainer A pointer to the VisParamContainer from which a VisParamEntry is requested.
 * @param name The name of the VisParamEntry that is requested from the VisParamContainer.
 *
 * @return Pointer to the VisParamEntry, or NULL.
 */
VisParamValue *visual_param_container_get_param_value (VisParamContainer *self, const char *name);

VisParamType visual_param_container_get_param_type (VisParamContainer *self, const char *name);



VISUAL_END_DECLS

/**
 * @}
 */

#endif /* _LV_PARAM_H */
