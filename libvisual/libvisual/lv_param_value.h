#ifndef _LV_PARAM_VALUE_H
#define _LV_PARAM_VALUE_H

#include <libvisual/lvconfig.h>
#include <libvisual/lv_color.h>
#include <libvisual/lv_palette.h>
#include <libvisual/lv_collection.h>

/**
 * @defgroup VisParam VisParam
 * @{
 */

VISUAL_BEGIN_DECLS

typedef struct _VisParamValue VisParamValue;

/**
 * Different types of parameters that can be used.
 */
typedef enum {
    VISUAL_PARAM_TYPE_NONE,
    VISUAL_PARAM_TYPE_INT,        /**< Integer parameter. */
    VISUAL_PARAM_TYPE_FLOAT,      /**< Floating point parameter. */
    VISUAL_PARAM_TYPE_STRING,     /**< String parameter. */
    VISUAL_PARAM_TYPE_DOUBLE,     /**< Double floating point parameter. */
    VISUAL_PARAM_TYPE_ENUM,       /**< Enum parameters. */
    VISUAL_PARAM_TYPE_COLOR,      /**< VisColor parameter. */
    VISUAL_PARAM_TYPE_PALETTE,    /**< VisPalette parameter. */
    VISUAL_PARAM_TYPE_COLLECTION, /**< VisCollection parameter. */
    VISUAL_PARAM_TYPE_OBJECT      /**< VisObject parameter. */
} VisParamType;

struct _VisParamValue {
	VisParamType type;

	union {
		char	  *string;	   /**< String data. */
		int		   integer;	   /**< Integer data. */
		float	   floating;   /**< Floating point data. */
		double	   doubleflt;  /**< Double floating point data. */
		VisObject *object;	   /**< VisObject data for a VisObject parameter. */
	} v;
};

/**
 * Sets the VisParamValue to VISUAL_PARAM_ENTRY_TYPE_STRING and
 * assigns the string given as argument to it.
 *
 * @param param Pointer to the VisParamValue to which a parameter is set.
 * @param string The string for this parameter.
 *
 * @return VISUAL_OK on success, -VISUAL_ERROR_PARAM_NULL on failure.
 */
void visual_param_value_set_string (VisParamValue *value, const char *string);

/**
 * Sets the VisParamValue to VISUAL_PARAM_ENTRY_TYPE_INTEGER and
 * assigns the integer given as argument to it.
 *
 * @param param Pointer to the VisParamValue to which a parameter is set.
 * @param integer The integer value for this parameter.
 */
void visual_param_value_set_int (VisParamValue *value, int integer);


/**
 * Sets the VisParamValue to VISUAL_PARAM_ENTRY_TYPE_FLOAT and assigns
 * the float given as argument to it.
 *
 * @param param Pointer to the VisParamValue to which a parameter is set.
 * @param floating The float value for this parameter.
 */
void visual_param_value_set_float (VisParamValue *value, float floating);


/**
 * Sets the VisParamValue to VISUAL_PARAM_ENTRY_TYPE_DOUBLE and
 * assigns the double given as argument to it.
 *
 * @param param Pointer to the VisParamValue to which a parameter is set.
 * @param doubleflt The double value for this parameter.
 */
void visual_param_value_set_double (VisParamValue *value, double doubleflt);

/**
 * Sets the VisParamValue to VISUAL_PARAM_ENTRY_TYPE_COLOR and assigns
 * the rgb values given as arguments to it.
 *
 * @param param Pointer to the VisParamValue to which a parameter is set.
 * @param r The red value for this color parameter.
 * @param g The green value for this color parameter.
 * @param b The blue value for this color parameter.
 */
void visual_param_value_set_color (VisParamValue *value, uint8_t r, uint8_t g, uint8_t b);

/**
 * Sets the VisParamValue to VISUAL_PARAM_ENTRY_TYPE_COLOR and assigns
 * the rgb values from the given VisColor as argument to it.
 *
 * @param param Pointer to the VisParamValue to which a parameter is set.
 * @param color Pointer to the VisColor from which the rgb values are copied into the parameter.
 *
 */
void visual_param_value_set_color_by_color (VisParamValue *value, VisColor *color);

/**
 * Sets the VisParamValue to VISUAL_PARAM_ENTRY_TYPE_PALETTE and
 * assigns a VisPalette to the VisParamValue.  This function does not
 * check if there is a difference between the prior set palette and
 * the new one, and always emits the changed event. so watch out with
 * usage.
 *
 * @param param Pointer to the VisParamValue to which a parameter is set.
 * @param pal Pointer to the VisPalette from which the palette data is retrieved for the VisParamValue.
 */
void visual_param_value_set_palette (VisParamValue *value, VisPalette *pal);

/**
 * Sets the VisParamValue to VISUAL_PARAM_ENTRY_TYPE_OBJECT and
 * assigns a VisObject to the VisParamValue.  With a VisObject
 * VisParamValue, the VisObject is referenced, not cloned.
 *
 * @param param Pointer to the VisParamValue to which a parameter is set.
 * @param object Pointer to the VisObject that is linked to the VisParamValue.
 */
void visual_param_value_set_object (VisParamValue *value, VisObject *object);

/**
 * Get the name of the VisParamValue.
 *
 * @param param Pointer to the VisParamValue from which the name is requested.
 *
 * @return The name of the VisParamValue or NULL.
 */
char *visual_param_value_get_name (VisParamValue *value);

/**
 * Get the string parameter from a VisParamValue.
 *
 * @param param Pointer to the VisParamValue from which the string parameter is requested.
 *
 * @return The string parameter from the VisParamValue or NULL.
 */
const char *visual_param_value_get_string (VisParamValue *value);

/**
 * Get the integer parameter from a VisParamValue.
 *
 * @param param Pointer to the VisParamValue from which the integer parameter is requested.
 *
 * @return The integer parameter from the VisParamValue.
 */
int visual_param_value_get_integer (VisParamValue *value);

/**
 * Get the float parameter from a VisParamValue.
 *
 * @param param Pointer to the VisParamValue from which the float parameter is requested.
 *
 * @return The float parameter from the VisParamValue.
 */
float visual_param_value_get_float (VisParamValue *value);

/**
 * Get the double parameter from a VisParamValue.
 *
 * @param param Pointer to the VisParamValue from which the double parameter is requested.
 *
 * @return The double parameter from the VisParamValue.
 */
double visual_param_value_get_double (VisParamValue *value);

/**
 * Get the object parameter from a VisParamValue.
 *
 * @param param Pointer to the VisParamValue from which the object parameter is requested.
 *
 * @return Pointer to the VisObject parameter from the VisParamValue.
 */
VisObject *visual_param_value_get_object (VisParamValue *value);


/**
 * Get the color parameter from a VisParamValue.
 *
 * @param param Pointer to the VisParamValue from which the color parameter is requested.
 *
 * @return Pointer to the VisColor parameter from the VisParamValue. It's adviced to
 *    use the VisColor that is returned as read only seen changing it directly won't emit events and
 *    can cause synchronous problems between the plugin and the parameter system. Instead use the
 *    visual_param_entry_set_color* methods to change the parameter value.
 */
VisColor *visual_param_value_get_color (VisParamValue *value);


/**
 * Get the palette parameter from a VisParamValue.
 *
 * @param param Pointer to the VisParamValue from which the palette parameter is requested.
 *
 * @return Pointer to the VisPalette parameter from the VisParamValue. The returned VisPalette
 *    should be exclusively used as read only.
 */
VisPalette *visual_param_value_get_palette (VisParamValue *value);

void visual_param_value_unset (VisParamValue *value);

VisCollection *visual_param_value_get_collection (VisParamValue *value);

VISUAL_END_DECLS

/**
 * @}
 */

#endif /* _LV_PARAM_VALUE_H */
