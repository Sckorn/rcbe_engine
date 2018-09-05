#ifndef RCBE_ENGINE_WINDOW_UTILS_H
#define RCBE_ENGINE_WINDOW_UTILS_H

#include "gui/Window.h"

#include <gtkmm/dialog.h>

#include <boost/filesystem.hpp>

namespace rcbe::toolkit::gui
{
std::shared_ptr<Gtk::Window> makeWindowFromFile(const std::string &path);
}

#endif //RCBE_ENGINE_WINDOW_UTILS_H
