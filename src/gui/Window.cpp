#include "gui/Window.h"

namespace rcbe::toolkit::gui
{
void Window::initWindow()
{
  gtk_window_set_title(GTK_WINDOW(gtk_window_ptr.get()), caption.c_str());
  gtk_window_set_position(GTK_WINDOW(gtk_window_ptr.get()), GTK_WIN_POS_CENTER);
  gtk_window_set_default_size(GTK_WINDOW(gtk_window_ptr.get()), dimensions.x, dimensions.y);
}

void Window::show()
{
  g_signal_connect(gtk_window_ptr.get(), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(gtk_window_ptr.get());
}
}