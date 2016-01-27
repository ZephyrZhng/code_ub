#include <gtk/gtk.h>
//gtk+-2.0
int main(int argc,char *argv[])
{
	gtk_init(&argc, &argv);
	GtkWidget* window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "my gtk+");
	gtk_widget_set_usize(GTK_WINDOW (window), 300, 150);
	GtkWidget* label = gtk_label_new("call me daddy");
	gtk_container_add(GTK_CONTAINER(window), label);
	gtk_widget_show(window);
	gtk_widget_show(label);
	gtk_main();

	return 0;
}

