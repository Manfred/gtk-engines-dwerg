#include <stdlib.h>
#include "dwerg_style.h"
#include "dwerg_rc_style.h"

static GtkRcStyleClass *parent_class;
GType dwerg_type_rc_style = 0;

static void
dwerg_rc_style_init(DwergRcStyle *style);
static void
dwerg_rc_style_class_init(DwergRcStyleClass *klass);
static GtkStyle *
dwerg_rc_style_create_style(GtkRcStyle *rc_style);

static struct {
    gchar *name;
    guint token;
} theme_symbols[] = {
    {"shadow_color_from", TOKEN_SHADOW_COLOR_FROM},
    {"shadow_color_to", TOKEN_SHADOW_COLOR_TO},
};

/* ------------------------------------------------------------------------ */
/* Parsing functions                                                        */
/* ------------------------------------------------------------------------ */

static guint
dwerg_rc_style_parse(GtkRcStyle *rc_style,
  GtkSettings *settings,
  GScanner* scanner)
{
    static GQuark scope_id = 0;
    DwergRcStyle *theme_data = DWERG_RC_STYLE(rc_style);
    theme_data->shadow_color_from = malloc(sizeof(GdkColor));
    theme_data->shadow_color_to = malloc(sizeof(GdkColor));

    guint old_scope;
    guint token;
    guint i;

    /* setup scope, safe old_scope for crash, define tokens */
    if (!scope_id) 
        scope_id = g_quark_from_string("dwerg_theme_engine");
    old_scope = g_scanner_set_scope(scanner, scope_id);
    if (!g_scanner_lookup_symbol(scanner, theme_symbols[0].name))
    {
        g_scanner_freeze_symbol_table(scanner);
        for (i = 0; i < G_N_ELEMENTS(theme_symbols); i++)
            g_scanner_scope_add_symbol(scanner, scope_id,
              theme_symbols[i].name,
              GINT_TO_POINTER(theme_symbols[i].token));
        g_scanner_thaw_symbol_table(scanner);
    }

    token = g_scanner_peek_next_token(scanner);
    while (token != G_TOKEN_RIGHT_CURLY)
    {
        switch(token)
        {
            case TOKEN_SHADOW_COLOR_FROM:
                g_print("vla\n");
                token = g_scanner_get_next_token(scanner); 
                if (token != TOKEN_SHADOW_COLOR_FROM)
                    return token;
                token = g_scanner_get_next_token(scanner); 
                if (token != G_TOKEN_EQUAL_SIGN)
                    return token;
                token = gtk_rc_parse_color(scanner,
                    theme_data->shadow_color_from);
                g_print("ja lekker\n");
                break;
            case TOKEN_SHADOW_COLOR_TO:
                token = g_scanner_get_next_token(scanner);
                if (token != TOKEN_SHADOW_COLOR_TO)
                    return token;
                token = g_scanner_get_next_token(scanner);
                if (token != G_TOKEN_EQUAL_SIGN)
                    return token;
                token = gtk_rc_parse_color(scanner,
                    theme_data->shadow_color_to);
                break;
            default:
                g_scanner_get_next_token(scanner);
                token = G_TOKEN_RIGHT_CURLY;
                break;
        }
        if (token != G_TOKEN_NONE) {
            g_free(theme_data);
            return token;
        }
        token = g_scanner_peek_next_token(scanner);
    }
    /* on no error reset the scope */
    g_scanner_get_next_token(scanner);
    g_scanner_set_scope(scanner, old_scope);
    return G_TOKEN_NONE;
}

static void
dwerg_rc_style_merge(GtkRcStyle *dest, GtkRcStyle *src)
{
    if (DWERG_IS_RC_STYLE(src))
    {
        DwergRcStyle *dwerg_dest = DWERG_RC_STYLE(dest);
        DwergRcStyle *dwerg_src = DWERG_RC_STYLE(src);

        dwerg_dest->shadow_color_from = dwerg_src->shadow_color_from;
        dwerg_dest->shadow_color_to = dwerg_src->shadow_color_to;
    }
    parent_class->merge(dest, src);
}

static GtkStyle *
dwerg_rc_style_create_style(GtkRcStyle *rc_style)
{   
    return GTK_STYLE(g_object_new(DWERG_TYPE_STYLE,NULL));
}

/* ------------------------------------------------------------------------ */
/* Dwerg RcStyle and RcStyleClass                                          */
/* ------------------------------------------------------------------------ */

static void
dwerg_rc_style_init(DwergRcStyle *style)
{
    GdkColor from, to;

    from.red = 0;
    from.green = 0;
    from.blue = 0;
    style->shadow_color_from = &from;

    to.red = 65535;
    to.green = 65535;
    to.blue = 65535;
    style->shadow_color_to = &to;
}

static void
dwerg_rc_style_class_init(DwergRcStyleClass *klass)
{
    GtkRcStyleClass *rc_style_class = GTK_RC_STYLE_CLASS(klass);

    parent_class = g_type_class_peek_parent(klass);

    rc_style_class->parse = dwerg_rc_style_parse;
    rc_style_class->merge = dwerg_rc_style_merge;
    rc_style_class->create_style = dwerg_rc_style_create_style;
}


void
dwerg_rc_style_register_type(GTypeModule *module)
{
    static const GTypeInfo object_info =
    {
        sizeof(DwergRcStyleClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) dwerg_rc_style_class_init,
        NULL,                                           /* class_finalize */
        NULL,                                           /* class_data */
        sizeof(DwergRcStyle),
        0,                                              /* n_preallocs */
        (GInstanceInitFunc)dwerg_rc_style_init,
    };
    dwerg_type_rc_style = g_type_module_register_type(module,
        GTK_TYPE_RC_STYLE,
        "DwergRcStyle",
        &object_info, 0);
}

