/* vim: set et ts=4 sw=4 sts=4 fdm=marker syntax=c.doxygen: */

/** \file   main.c
 * \brief   Example code to show bug with menus being hidden behind GtkGlArea
 */

#include <gtk/gtk.h>
#include <GL/gl.h>

static GtkWidget *main_window;
static int fullscreen_state = 0;


static void on_fullscreen_clicked(GtkWidget *widget, gpointer data)
{
    g_print("fullscreen clicked\n");

    fullscreen_state ^= 1;

    if (fullscreen_state) {
        gtk_window_fullscreen(GTK_WINDOW(main_window));
    } else {
        gtk_window_unfullscreen(GTK_WINDOW(main_window));
    }
}


static void on_menu_item_activate(GtkWidget *widget, gpointer data)
{
    g_print("Menu item %d.\n", GPOINTER_TO_INT(data));
}


static GtkWidget *create_menu(void)
{
    GtkWidget *menu;
    GtkWidget *submenu;
    GtkWidget *item;
    int i;

    menu = gtk_menu_bar_new();

    submenu = gtk_menu_new();

    item = gtk_menu_item_new_with_mnemonic("_Menu");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(item), submenu);


    for (i = 0; i < 4; i++) {
        gchar buffer[256];
        GtkWidget *foo;

        g_snprintf(buffer, 256, "Item %d", i);
        foo = gtk_menu_item_new_with_label(buffer);
        g_signal_connect(foo, "activate",
                G_CALLBACK(on_menu_item_activate), GINT_TO_POINTER(i));
        gtk_container_add(GTK_CONTAINER(submenu), foo);
    }

    gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);

    return menu;
}


static gboolean on_render(GtkGLArea *area, GdkGLContext *context)
{
#if 0
    g_print("on_render() called\n");
#endif
    glClearColor(0, 0, 0, 0);
    glClear(GL_COLOR_BUFFER_BIT);
    return TRUE;
}


static void on_realize(GtkGLArea *area)
{
    g_print("on_realize() called\n");

    gtk_gl_area_make_current(area);
}


static GtkWidget *create_render_area(void)
{
    GtkWidget *area;

    area = gtk_gl_area_new();
    g_signal_connect(area, "realize", G_CALLBACK(on_realize), NULL);
    g_signal_connect(area, "render", G_CALLBACK(on_render), NULL);
    return area;

}


static GtkWidget *create_main_window(GtkApplication *app)
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *menu;
    GtkWidget *render;
    GtkWidget *button;


    window = gtk_application_window_new(app);

    grid = gtk_grid_new();
    gtk_container_add(GTK_CONTAINER(window), grid);


    menu = create_menu();
    gtk_widget_set_hexpand(menu, TRUE);
    gtk_widget_set_vexpand(menu, FALSE);

    render = create_render_area();
    gtk_widget_set_hexpand(render, TRUE);
    gtk_widget_set_vexpand(render, TRUE);

    button = gtk_button_new_with_label("Fullscreen");
    g_signal_connect(button, "clicked",
            G_CALLBACK(on_fullscreen_clicked), NULL);

    gtk_grid_attach(GTK_GRID(grid), menu, 0, 0, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), render, 0, 1, 1, 1);
    gtk_grid_attach(GTK_GRID(grid), button, 0, 2, 1, 1);

    gtk_window_set_title(GTK_WINDOW(window), "GtkGlArea over menu items bug");
    gtk_window_set_default_size(GTK_WINDOW(window), 600, 400);
    return window;
}



static void on_application_activate(GtkApplication *app,
                                    gpointer        data)
{
    main_window = create_main_window(app);
    gtk_widget_show_all(main_window);
}




/** \brief  Entry point
 *
 * \param[in    argc    argument count
 * \param[in]   argv    argument vector
 *
 * \return  EXIT_SUCCESS or EXIT_FAILURE
 */
int main(int argc, char **argv)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new(
            "nl.compyx.gtkglarea-menus-example",
            G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(on_application_activate), NULL);

    /* run application */
    status = g_application_run(G_APPLICATION(app), argc, argv);
    g_object_unref(app);
    return status;
}
