#include <stdlib.h>
#include <gtk/gtk.h>
#include <goocanvas.h>

static gboolean on_rect_button_press (GooCanvasItem	 *view,
                                      GooCanvasItem  *target,
                                      GdkEventButton *event,
                                      gpointer        data);

static gboolean on_delete_event (GtkWidget *window,
                                 GdkEvent  *event,
                                 gpointer   unused_data);


int
main (int argc, char *argv[])
{
	GtkWidget *window, *scrolled_win, *canvas;
	GooCanvasItem *root, *rect_item;

	gtk_init (&argc, &argv);

	/* Create the window and widgets. */
	window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
	gtk_window_set_default_size (GTK_WINDOW (window), 640, 600);


	scrolled_win = gtk_scrolled_window_new (NULL, NULL);


	canvas = goo_canvas_new ();
	goo_canvas_set_bounds (GOO_CANVAS (canvas), 0, 0, 1000, 1000);



	root = goo_canvas_get_root_item (GOO_CANVAS (canvas));

	/* Add a few simple items. */
	rect_item = goo_canvas_rect_new (root, 100, 100, 400, 400,
																					 NULL);

	/* Connect a signal handler for the rectangle item. */
	g_signal_connect (rect_item, "button-press-event", (GCallback)on_rect_button_press, NULL);

	g_signal_connect (window, "delete-event", (GCallback)on_delete_event, NULL);



	gtk_container_add (GTK_CONTAINER (window), scrolled_win);

	gtk_container_add (GTK_CONTAINER (scrolled_win), canvas);


	gtk_widget_show_all (GTK_WIDGET (window));


	/* Pass control to the GTK+ main event loop. */
	gtk_main ();

	return 0;
}

/* This handles button presses in item views. We simply output a message to
	 the console. */
static gboolean
on_rect_button_press (GooCanvasItem  *item,
                      GooCanvasItem  *target,
                      GdkEventButton *event,
                      gpointer        data)
{
	g_print ("rect item received button press event\n");
	return TRUE;
}

/* This is our handler for the "delete-event" signal of the window, which
	 is emitted when the 'x' close button is clicked. We just exit here. */
static gboolean
on_delete_event (GtkWidget *window,
                 GdkEvent  *event,
                 gpointer   unused_data)
{
	exit (0);
}
