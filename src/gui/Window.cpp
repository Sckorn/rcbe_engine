#include "gui/Window.h"

namespace rcbe::toolkit::gui
{
void Window::initWindow()
{
  set_title(caption);
}

/*void Window::show()
{
  g_signal_connect(gtk_window_ptr.get(), "destroy", G_CALLBACK(gtk_main_quit), NULL);
  gtk_widget_show_all(gtk_window_ptr.get());
}*/
}