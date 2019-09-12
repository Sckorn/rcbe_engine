#ifndef RCBE_ENGINE_WINDOW_UTILS_H
#define RCBE_ENGINE_WINDOW_UTILS_H

#include <data_types/gui_types.hpp>

#include <gtkmm-3.0/gtkmm/dialog.h>

#include <boost/filesystem.hpp>

namespace rcbe::toolkit::gui
{
std::shared_ptr<Gtk::Window> makeWindowFromFile(const std::string &path);
}

#endif //RCBE_ENGINE_WINDOW_UTILS_H
