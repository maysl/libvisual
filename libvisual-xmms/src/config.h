#ifndef __LV_XMMS_CONFIG__
#define __LV_XMMS_CONFIG__

#include <glib.h>

#define OPTIONS_MAX_NAME_LEN 256
#define OPTIONS_MAX_ICON_PATH_LEN 256

typedef struct {
	gchar *last_plugin;	/**< Name of the last plugin runned,
				  with length < OPTIONS_MAX_NAME_LEN. */
	gchar *icon_file;	/**< Absolute path of the icon file,
				  with length < OPTIONS_MAX_ICON_PATH_LEN. */
	int width;		/**< Width in pixels. */
	int height;		/**< Height in pixels. */
	int fps;		/**< Maximum frames per second. */
	int depth;		/**< Color depth. */
	gboolean fullscreen;	/**< Say if we are in fullscreen or not. */
} Options;

void lv_xmms_config_window (void);

Options *lv_xmms_config_open (void);
int lv_xmms_config_close (void);

int lv_xmms_config_load_prefs (void);
int lv_xmms_config_save_prefs (void);

void lv_xmms_config_toggle_fullscreen (void);


#endif /* __LV_XMMS_CONFIG__ */
