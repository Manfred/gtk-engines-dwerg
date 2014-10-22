#include "dwerg_style.h"
#include "dwerg_rc_style.h"

#include <gtk/gtk.h>
#include <string.h>

static GtkStyleClass *parent_class;

#define DETAIL(xx) ((detail) && (!strcmp(xx, detail)))

static void paint_hline (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail, gint x1, gint x2, gint y);
static void paint_vline (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail, gint y1, gint y2, gint x);
static void paint_shadow (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height);
static void paint_polygon (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  GdkPoint * point, gint npoints, gint fill);
/*
static void paint_arrow (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  GtkArrowType arrow_type,
  gint fill,
  gint x, gint y, gint width, gint height);
static void paint_diamond (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height);
*/
static void paint_box (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height);
static void paint_flat_box (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height);
static void paint_check (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height);
static void paint_option (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height);
static void paint_shadow_gap (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x,
  gint y,
  gint width,
  gint height,
  GtkPositionType gap_side,
  gint gap_x, gint gap_width);
static void paint_box_gap (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x,
  gint y,
  gint width,
  gint height,
  GtkPositionType gap_side,
  gint gap_x, gint gap_width);
static void paint_extension (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x,
  gint y,
  gint width,
  gint height, GtkPositionType gap_side);
static void paint_tab (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height);
static void paint_focus(GtkStyle *style,
  GdkWindow *window,
  GtkStateType state_type,
  GdkRectangle *area,
  GtkWidget *widget,
  const gchar *detail,
  gint x, gint y, gint width, gint height);
static void paint_slider (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x,
  gint y,
  gint width,
  gint height, GtkOrientation orientation);
static void paint_handle (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x,
  gint y,
  gint width,
  gint height, GtkOrientation orientation);

/* ------------------------------------------------------------------------ */
/* Drawing functions                                                        */
/* ------------------------------------------------------------------------ */

static void
sanitize_size(GdkWindow *window,
  gint      *width,
  gint      *height) 
{
    if ((*width == -1)&&(*height == -1))
        gdk_window_get_size(window, width, height);
    else if (*width == -1)
        gdk_window_get_size(window, width, NULL);
    else if (*height == -1)
        gdk_window_get_size(window, NULL, height);
}

static void
paint_hline(GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x1,
  gint x2,
  gint y)
{
    gdk_draw_line(window, style->dark_gc[state_type], x1, y, x2, y);
}

static void
paint_vline(GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint y1,
  gint y2,
  gint x)
{
    gdk_draw_line(window, style->dark_gc[state_type], x, y1, x, y1);
}

static void
paint_shadow(GtkStyle *style,
  GdkWindow    *window,
  GtkStateType  state_type,
  GtkShadowType shadow_type,
  GdkRectangle *area,
  GtkWidget    *widget,
  const gchar  *detail,
  gint          x,
  gint          y,  
  gint          width,
  gint          height)
{
    /*
    gdk_draw_rectangle(window,
      style->bg_gc[state_type], TRUE, x, y, width, height);
    */
}

static void
paint_polygon(GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  GdkPoint * points, gint npoints, gint fill)
{
    gdk_draw_polygon(window, style->bg_gc[state_type], fill, points, npoints);
}

static void
paint_box (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height)
{
    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    sanitize_size(window, &width, &height);

    if (DETAIL("vscrollbar") || DETAIL("hscrollbar"))
    {
        gdk_draw_rectangle(window,
          style->bg_gc[state_type], TRUE, x, y, width, height);
    }
    else if (DETAIL("slider") || DETAIL("entry_bg"))
    {
        gdk_draw_rectangle(window,
          style->light_gc[state_type], TRUE, x+1, y+1, width-2, height-2);
    }
    else if (DETAIL("menuitem"))
    {
        gdk_draw_rectangle(window,
          style->light_gc[state_type], TRUE, x, y, width, height);
    }
    else if (DETAIL("menubar"))
    {
        gdk_draw_line(window,
          style->dark_gc[state_type], x, y+height-1, x+width, y+height-1);
    }
    else if (DETAIL("menu"))
    {
        gdk_draw_rectangle(window,
          style->dark_gc[state_type], FALSE, x, y, x+width-1, y+height-1);
    }
    else if (DETAIL("trough"))
    {
        gdk_draw_rectangle(window,
          style->bg_gc[state_type], TRUE, x, y, width, height);
    }
    else if (DETAIL("bar"))
    {
        if (state_type == GTK_PROGRESS_DISCRETE)
        {
            gdk_draw_rectangle(window,
              style->light_gc[state_type], FALSE, x, y, x+width-2, y+height-2);
        }
        else
        {
            gdk_draw_rectangle(window,
              style->light_gc[state_type], TRUE, x, y, x+width-2, y+height-2);
            /*gdk_draw_rectangle(window,
              style->dark_gc[state_type], FALSE, x, y, x+width-2, y+height-2);*/
        }
    }
    else if (DETAIL("toolbar"))
    {
        gdk_draw_rectangle(window,
          style->light_gc[state_type], TRUE, x, y, x+width, y+height);
        gdk_draw_line(window,
          style->dark_gc[state_type], x, y+height-1, x+width, y+height-1);
    }
    else if (DETAIL ("buttondefault"))
    {
        /* I don't want no background on default buttons..
           Let's add that cute line (see below) instead... */
    }
    else if (DETAIL("button"))
    {
        gint px, py;

        /* update the coordinates */
        width -= 4; height -= 4; x += 2; y += 2;

        gdk_draw_rectangle(window, style->light_gc[state_type], TRUE, x, y, width, height);

        gdk_draw_line(window, style->dark_gc[state_type], x, y, x + width, y);
        gdk_draw_line(window, style->dark_gc[state_type], x, y, x, y + height);
        gdk_draw_line(window, style->dark_gc[state_type], x, y + height, x + width, y + height);
        gdk_draw_line(window, style->dark_gc[state_type], x + width, y, x + width, y + height);

        /*  Paint some points here instead of in "buttondefault"
            which is drawn _behind_ the current button */
        if (GTK_WIDGET_HAS_DEFAULT (widget)) {
            px = x + width - 1;
            py = y+1;

            for (;py < (y+height); py += 2) {
                gdk_draw_point(window, style->dark_gc[state_type], px, py);
            }
        }
    }
    else if (DETAIL("tab"))
    {
        paint_tab(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height);
    }
    else if DETAIL("notebook")
    {
        gdk_draw_rectangle(window,
          style->bg_gc[state_type], TRUE, x, y, width, height);
        gdk_draw_rectangle(window,
          style->dark_gc[state_type], FALSE, x, y, width-1, height-1);
    }
    else if (DETAIL("dockitem_bin"))
    {
        gdk_draw_line(window, style->dark_gc[state_type], x, y+(height/2), x + width, y+(height/2));
    }
    else
    {
        gdk_draw_rectangle(window,
          style->bg_gc[state_type], TRUE, x, y, width, height);
    }
}

static void
paint_flat_box (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height)
{
    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    sanitize_size(window, &width, &height);

    /* we're only interested in putting a line around the text
       fields */
    if (DETAIL("entry_bg"))
    {
        gdk_draw_rectangle(window, style->light_gc[state_type], TRUE, x, y, width, height);
        gdk_draw_rectangle(window, style->dark_gc[state_type], FALSE, x, y, width-1, height-1);
    }
    else if (DETAIL("tooltip"))
    {
        gdk_draw_rectangle(window, style->light_gc[state_type], TRUE, x, y, width, height);
        gdk_draw_rectangle(window, style->dark_gc[state_type], FALSE, x, y, width-1, height-1);
    }
    else
    {
        gdk_draw_rectangle(window, style->bg_gc[state_type], TRUE, x, y, width, height);
    }
}

static void
paint_check (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height)
{
    /* not a check next to a menuitem */
    /*
    if (!DETAIL("check"))
    */
    gdk_draw_rectangle(window, style->light_gc[state_type], TRUE, x, y, width, height);
    gdk_draw_rectangle(window, style->dark_gc[state_type], FALSE, x, y, width, height);
    if (shadow_type == GTK_SHADOW_IN)
    {
        gdk_draw_rectangle(window, style->dark_gc[state_type], TRUE, x+3, y+3, width-5, height-5);
    }
}

static void
paint_option (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height)
{
    if (shadow_type == GTK_SHADOW_IN)
    {
        gdk_draw_arc(window, style->light_gc[state_type], TRUE, x, y, width, height, 0, 360*64);
        gdk_draw_arc(window, style->dark_gc[state_type], FALSE, x, y, width, height, 0, 360*64);
        gdk_draw_arc(window, style->dark_gc[state_type], TRUE, x+2, y+2, width-4, height-4, 0, 360*64);
    }
    else
    {
        gdk_draw_arc(window, style->light_gc[state_type], TRUE, x, y, width, height, 0, 360*64);
        gdk_draw_arc(window, style->dark_gc[state_type], FALSE, x, y, width, height, 0, 360*64);
    }
}

static void
paint_shadow_gap (GtkStyle * style,
         GdkWindow * window,
         GtkStateType state_type,
         GtkShadowType shadow_type,
         GdkRectangle * area,
         GtkWidget * widget,
         const gchar * detail,
         gint x,
         gint y,
         gint width,
         gint height,
         GtkPositionType gap_side, gint gap_x, gint gap_width)
{
    sanitize_size(window, &width, &height);

    gdk_draw_rectangle(window, style->dark_gc[state_type], FALSE, x, y, width-1, height-1);
    paint_box_gap(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height, gap_side, gap_x, gap_width);
}

static void
paint_box_gap (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x,
  gint y,
  gint width,
  gint height,
  GtkPositionType gap_side, gint gap_x, gint gap_width)
{
    GdkRectangle rect;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    sanitize_size(window, &width, &height);

    gtk_paint_box (style, window, state_type, shadow_type, area,
               widget, detail, x, y, width, height);

    switch (gap_side) {
    case GTK_POS_TOP:
        rect.x = x + gap_x;
        rect.y = y;
        rect.width = gap_width;
        rect.height = 2;
        break;
    case GTK_POS_BOTTOM:
        rect.x = x + gap_x;
        rect.y = y + height - 2;
        rect.width = gap_width;
        rect.height = 2;
        break;
    case GTK_POS_LEFT:
        rect.x = x;
        rect.y = y + gap_x;
        rect.width = 2;
        rect.height = gap_width;
        break;
    case GTK_POS_RIGHT:
        rect.x = x + width - 2;
        rect.y = y + gap_x;
        rect.width = 2;
        rect.height = gap_width;
        break;
    }
    gtk_style_apply_default_background (style, window,
        widget
        &&
        !GTK_WIDGET_NO_WINDOW (widget),
        state_type, area, rect.x,
        rect.y, rect.width,
        rect.height);
}

static void
paint_extension (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x,
  gint y, gint width, gint height, GtkPositionType gap_side)
{
    GdkRectangle rect;

    g_return_if_fail (style != NULL);
    g_return_if_fail (window != NULL);

    sanitize_size (window, &width, &height);

    if (gap_side)
    {
        paint_box_gap(style, window, state_type, shadow_type, area, widget, detail, x, y, width, height, gap_side, 0, 2);
    }
    
    gtk_style_apply_default_background (style, window,
        widget
        &&
        !GTK_WIDGET_NO_WINDOW (widget),
        state_type, area, rect.x,
        rect.y, rect.width,
        rect.height);
}

static void
paint_tab(GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x, gint y, gint width, gint height)
{
    GtkNotebook *notebook;
    GdkGC *bg_gc, *light_gc, *dark_gc;
    int orientation;

    sanitize_size (window, &width, &height);

    light_gc = style->light_gc[state_type];
    bg_gc    = style->bg_gc[state_type];
    dark_gc  = style->dark_gc[state_type];

    if (DETAIL ("optionmenutab")) {
        parent_class->draw_tab (style, window, state_type, shadow_type,
            area, widget, detail, x, y, width, height);
        return;
    }

    notebook = GTK_NOTEBOOK (widget);
    orientation = notebook->tab_pos;

    width -= 2;
    switch (orientation) {
        case GTK_POS_TOP:
            gdk_draw_rectangle (window, bg_gc, TRUE, x, y, width, height);
            gdk_draw_line (window, dark_gc, x, y+height, x, y);
            gdk_draw_line (window, dark_gc, x, y, x+width, y);
            gdk_draw_line (window, dark_gc, x+width, y, x+width, y+height);
            break;
        case GTK_POS_BOTTOM:
            y -= 2;
            gdk_draw_rectangle (window, bg_gc, TRUE, x, y+1, width, height);
            gdk_draw_line (window, dark_gc, x, y + height, x, y);
            gdk_draw_line (window, dark_gc, x, y + height, x + width, y + height);
            gdk_draw_line (window, dark_gc, x + width, y, x + width, y + height);
            break;
        case GTK_POS_LEFT:
            gdk_draw_rectangle (window, bg_gc, TRUE, x, y, width, height);
            gdk_draw_line (window, dark_gc, x, y + height, x, y);
            gdk_draw_line (window, dark_gc, x, y, x + width, y);
            gdk_draw_line (window, dark_gc, x , y + height, x + width, y + height);
            break;
        case GTK_POS_RIGHT:
            gdk_draw_rectangle (window, bg_gc, TRUE, x, y, width, height);
            gdk_draw_line (window, dark_gc, x, y, x + width, y);
            gdk_draw_line (window, dark_gc, x, y + height, x + width, y + height);
            gdk_draw_line (window, dark_gc, x + width, y, x + width, y + height);
            break;
    }
}

static void
paint_focus(GtkStyle *style,
  GdkWindow *window,
  GtkStateType state_type,
  GdkRectangle *area,
  GtkWidget *widget,
  const gchar *detail,
  gint x, gint y, gint width, gint height)
{
    gint px, py;

    g_return_if_fail(style != NULL);
    g_return_if_fail(window != NULL);

    sanitize_size (window, &width, &height);

    if (DETAIL ("button")) {
        px = x + width;
        py = y+1;

        for (;py < (y+height); py += 2) {
            gdk_draw_point(window, style->dark_gc[state_type], px, py);
        }
    }
    else
    {
        height--;

        gdk_draw_line(window, style->mid_gc[state_type], x, y+height-2, x+width, y+height-2);
    }
}

static void
paint_slider(GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type, 
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x,
  gint y, gint width, gint height, GtkOrientation orientation)
{
    sanitize_size(window, &width, &height);

    gdk_draw_rectangle(window, style->light_gc[state_type], TRUE, x, y, width, height);

    // update the coordinates
    width -= 1; height -= 1;
    gdk_draw_line(window, style->dark_gc[state_type], x, y, x + width, y);
    gdk_draw_line(window, style->dark_gc[state_type], x, y, x, y + height);
    gdk_draw_line(window, style->dark_gc[state_type], x, y + height, x + width, y + height); 
    gdk_draw_line(window, style->dark_gc[state_type], x + width, y, x + width, y + height);
}

static void
paint_handle (GtkStyle * style,
  GdkWindow * window,
  GtkStateType state_type,
  GtkShadowType shadow_type,
  GdkRectangle * area,
  GtkWidget * widget,
  const gchar * detail,
  gint x,
  gint y,
  gint width,
  gint height, GtkOrientation orientation)
{
    sanitize_size(window, &width, &height);

    gdk_draw_rectangle(window, style->bg_gc[state_type], TRUE, x, y, width, height);
    if (DETAIL("dockitem")||DETAIL("handlebox"))
    {
        gdk_draw_line(window, style->dark_gc[state_type], x, y+height-1, x+width, y+height-1);
        gdk_draw_line(window, style->dark_gc[state_type], x+width-1, y, x+width-1, y+height);
        gdk_draw_line(window, style->dark_gc[state_type], x+width-3, y, x+width-3, y+height);
    }
    else
    {
        if (orientation == GTK_ORIENTATION_HORIZONTAL)
        {
            /* orientation is horizontal */
            gdk_draw_arc(window, style->dark_gc[state_type], FALSE, x+(width/2), y, height-2, height-2, 0, 360*64);
            if (width > 12)
            {
                gdk_draw_arc(window, style->dark_gc[state_type], FALSE, x+(width/2)-height-2, y, height-2, height-2, 0, 360*64);
                gdk_draw_arc(window, style->dark_gc[state_type], FALSE, x+(width/2)+height+2, y, height-2, height-2, 0, 360*64);
            }
        }
        else
        {
            /* orientation is vertical */
            gdk_draw_arc(window, style->dark_gc[state_type], FALSE, x, y+(height/2), width-2, width-2, 0, 360*64);
            if (height > 12)
            {
                gdk_draw_arc(window, style->dark_gc[state_type], FALSE, x, y+(height/2)-width-2, width-2, width-2, 0, 360*64);
                gdk_draw_arc(window, style->dark_gc[state_type], FALSE, x, y+(height/2)+width+2, width-2, width-2, 0, 360*64);
            } 
        }
    }
}   

/* ------------------------------------------------------------------------ */
/* Dwerg Style and StyleClass                                              */
/* ------------------------------------------------------------------------ */

static void
dwerg_style_init_from_rc(GtkStyle *style,
  GtkRcStyle *rc_style)
{
    /*DwergStyle *dwerg_style = DWERG_STYLE(style);
    DwergRcStyle *dwerg_rc_style = DWERG_RC_STYLE(rc_style);*/

    parent_class->init_from_rc(style, rc_style);
}

/*
static GdkGC *
realize_color(GtkStyle * style,
  GdkColor * color)
{
    GdkGCValues gc_values;

    gdk_colormap_alloc_color(style->colormap, color,
      FALSE, TRUE);
    gc_values.foreground = *color;
    return gtk_gc_get(style->depth, style->colormap,
      &gc_values, GDK_GC_FOREGROUND);
}
*/

static void
dwerg_style_realize(GtkStyle * style)
{
    parent_class->realize(style);
}

static void
dwerg_style_unrealize (GtkStyle * style)
{
    parent_class->unrealize(style);
}

static void
dwerg_style_init(DwergStyle *style)
{
}

static void
dwerg_style_class_init(DwergStyleClass *klass)
{   
    GtkStyleClass *style_class = GTK_STYLE_CLASS(klass);

    parent_class = g_type_class_peek_parent(klass);

    style_class->realize = dwerg_style_realize;
    style_class->unrealize = dwerg_style_unrealize;
    style_class->init_from_rc = dwerg_style_init_from_rc;

    style_class->draw_hline =       paint_hline;
    style_class->draw_vline =       paint_vline;
    style_class->draw_shadow =      paint_shadow;
    style_class->draw_polygon =     paint_polygon;
    /*
    style_class->draw_arrow =       paint_arrow;
    style_class->draw_diamond =     paint_diamond;
    style_class->draw_oval =        paint_oval;
    style_class->draw_string =      paint_string;
    */
    style_class->draw_box =         paint_box;
    style_class->draw_flat_box =    paint_flat_box;
    style_class->draw_check =       paint_check;
    style_class->draw_option =      paint_option;
    /*
    style_class->draw_cross =       paint_cross;
    style_class->draw_ramp =        paint_ramp;
    */
    style_class->draw_tab =         paint_tab;
    style_class->draw_shadow_gap =  paint_shadow_gap;
    style_class->draw_box_gap =     paint_box_gap;
    style_class->draw_extension =   paint_extension;
    style_class->draw_focus =       paint_focus;
    style_class->draw_slider =      paint_slider;
    style_class->draw_handle =      paint_handle;
}

GType dwerg_type_style = 0;

void
dwerg_style_register_type(GTypeModule *module)
{
    static const GTypeInfo object_info = {
        sizeof (DwergStyleClass),
        (GBaseInitFunc) NULL,
        (GBaseFinalizeFunc) NULL,
        (GClassInitFunc) dwerg_style_class_init,
        NULL,                                       /* class_finalize */
        NULL,                                       /* class_data */
        sizeof (DwergStyle),
        0,                                          /* n_preallocs */
        (GInstanceInitFunc) dwerg_style_init,
    };
    dwerg_type_style = g_type_module_register_type (module,
      GTK_TYPE_STYLE, 
      "DwergStyle",
      &object_info,
      0);
}

