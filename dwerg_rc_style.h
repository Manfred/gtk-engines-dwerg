#include <gtk/gtkrc.h>

/* -------------------------------------------------------------------------*/
/* RcStyle and RcStyleClass definitions                                     */
/* -------------------------------------------------------------------------*/
typedef struct _DwergRcStyle DwergRcStyle;
typedef struct _DwergRcStyleClass DwergRcStyleClass;

extern GType dwerg_type_rc_style;

#define DWERG_TYPE_RC_STYLE              dwerg_type_rc_style
#define DWERG_RC_STYLE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), DWERG_TYPE_RC_STYLE, DwergRcStyle))
#define DWERG_RC_STYLE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), DWERG_TYPE_RC_STYLE, DwergRcStyleClass))
#define DWERG_IS_RC_STYLE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), DWERG_TYPE_RC_STYLE))
#define DWERG_IS_RC_STYLE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), DWERG_TYPE_RC_STYLE))
#define DWERG_RC_STYLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), DWERG_TYPE_RC_STYLE, DwergRcStyleClass))

struct _DwergRcStyle
{
    GtkRcStyle parent_instance;

    GdkColor *shadow_color_from;
    GdkColor *shadow_color_to;
};

struct _DwergRcStyleClass
{
    GtkRcStyleClass parent_class;
};

void
dwerg_rc_style_register_type(GTypeModule * module);

/* -------------------------------------------------------------------------*/
/* Scanner token definitions                                                */
/* -------------------------------------------------------------------------*/
enum
{
    TOKEN_SHADOW_COLOR_FROM = G_TOKEN_LAST + 1,
    TOKEN_SHADOW_COLOR_TO
};

