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
#include "lv_util.h"

void visual_param_value_set_string (VisParamValue *value, const char *string)
{
    visual_return_if_fail (value != NULL);

	visual_param_value_unset (value);

    value->type     = VISUAL_PARAM_TYPE_STRING;
    value->v.string = visual_strdup (string);
}

void visual_param_value_set_int (VisParamValue *value, int integer)
{
    visual_return_if_fail (value != NULL);

	visual_param_value_unset (value);

    value->type      = VISUAL_PARAM_TYPE_INT;
    value->v.integer = integer;
}

void visual_param_value_set_float (VisParamValue *value, float floating)
{
    visual_return_if_fail (value != NULL);

	visual_param_value_unset (value);

    value->type       = VISUAL_PARAM_TYPE_FLOAT;
    value->v.floating = floating;
}

void visual_param_value_set_double (VisParamValue *value, double doubleflt)
{
    visual_return_if_fail (value != NULL);

	visual_param_value_unset (value);

    value->type        = VISUAL_PARAM_TYPE_DOUBLE;
    value->v.doubleflt = doubleflt;
}

void visual_param_value_set_color (VisParamValue *value, uint8_t r, uint8_t g, uint8_t b)
{
    visual_return_if_fail (value != NULL);

	visual_param_value_unset (value);

    value->type     = VISUAL_PARAM_TYPE_COLOR;
	value->v.object = VISUAL_OBJECT (visual_color_new_rgba (r, g, b, 255));
}

void visual_param_value_set_color_by_color (VisParamValue *value, VisColor *color)
{
    visual_return_if_fail (value != NULL);
	visual_return_if_fail (color != NULL);

	visual_param_value_unset (value);

    value->type     = VISUAL_PARAM_TYPE_COLOR;
	value->v.object = VISUAL_OBJECT (color);

	visual_object_ref (VISUAL_OBJECT (color));
}

void visual_param_value_set_palette (VisParamValue *value, VisPalette *pal)
{
    visual_return_if_fail (value != NULL);
	visual_return_if_fail (pal != NULL);

	visual_param_value_unset (value);

    value->type     = VISUAL_PARAM_TYPE_PALETTE;
	value->v.object = VISUAL_OBJECT (pal);

	visual_object_ref (VISUAL_OBJECT (pal));
}

void visual_param_value_set_object (VisParamValue *value, VisObject *object)
{
    visual_return_if_fail (value != NULL);

    visual_param_value_unset (value);

    value->type     = VISUAL_PARAM_TYPE_OBJECT;
    value->v.object = object;

	visual_object_ref (object);
}

void visual_param_value_unset (VisParamValue *value)
{
    visual_return_if_fail (value != NULL);

    switch (value->type) {
        case VISUAL_PARAM_TYPE_STRING:
            visual_mem_free (value->v.string);
            break;

        case VISUAL_PARAM_TYPE_OBJECT:
        case VISUAL_PARAM_TYPE_COLOR:
        case VISUAL_PARAM_TYPE_PALETTE:
            visual_object_unref (value->v.object);
            break;

        default: /* nothing to do */;
    }

    value->type = VISUAL_PARAM_TYPE_NONE;
}

const char *visual_param_value_get_string (VisParamValue *value)
{
    visual_return_val_if_fail (value != NULL, NULL);
    visual_return_val_if_fail (value->type != VISUAL_PARAM_TYPE_STRING, "<invalid>");

    return value->v.string;
}

int visual_param_value_get_integer (VisParamValue *value)
{
    visual_return_val_if_fail (value != NULL, 0);
    visual_return_val_if_fail (value->type != VISUAL_PARAM_TYPE_INT, 0);

    return value->v.integer;
}

float visual_param_value_get_float (VisParamValue *value)
{
    visual_return_val_if_fail (value != NULL, 0);
    visual_return_val_if_fail (value->type != VISUAL_PARAM_TYPE_FLOAT, 0.0);

    return value->v.floating;
}

double visual_param_value_get_double (VisParamValue *value)
{
    visual_return_val_if_fail (value != NULL, 0.0);
    visual_return_val_if_fail (value->type != VISUAL_PARAM_TYPE_DOUBLE, 0.0);

    return value->v.doubleflt;
}

VisObject *visual_param_value_get_object (VisParamValue *value)
{
    VisObject *object;

    visual_return_val_if_fail (value != NULL, NULL);
    visual_return_val_if_fail (value->type != VISUAL_PARAM_TYPE_COLOR, NULL);

    object = value->v.object;
    visual_object_ref (object);

    return object;
}

VisColor *visual_param_value_get_color (VisParamValue *value)
{
    VisObject *object;

    visual_return_val_if_fail (value != NULL, NULL);
    visual_return_val_if_fail (value->type != VISUAL_PARAM_TYPE_COLOR, NULL);

    object = value->v.object;
    visual_object_ref (object);

    return VISUAL_COLOR (object);
}

VisPalette *visual_param_value_get_palette (VisParamValue *value)
{
    VisObject *object;

    visual_return_val_if_fail (value != NULL, NULL);
    visual_return_val_if_fail (value->type != VISUAL_PARAM_TYPE_PALETTE, NULL);

    object = value->v.object;
    visual_object_ref (object);

    return VISUAL_PALETTE (object);
}

VisCollection *visual_param_value_get_collection (VisParamValue *value)
{
    VisObject *object;

    visual_return_val_if_fail (value != NULL, NULL);
    visual_return_val_if_fail (value->type != VISUAL_PARAM_TYPE_COLLECTION, NULL);

    object = value->v.object;
    visual_object_ref (object);

    return VISUAL_COLLECTION (object);
}
