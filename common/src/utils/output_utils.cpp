#include <rcbe-engine/utils/output_utils.hpp>

/**
 * Terminal colors explanation
 *
 * _____________________________________________________________
 *  | Parameter  | Effect                                      |
 *  ____________________________________________________________
 *  | 0          | reset all SGR effects to their default      |
 *  | 1          | bold or increased intensity                 |
 *  | 2          | faint or decreased intensity                |
 *  | 4          | singly underlined                           |
 *  | 5          | slow blink                                  |
 *  | 30-37      | foreground color (8 colors)                 |
 *  | 38;5;x     | foreground color (256 colors, non-standard) |
 *  | 38;2;r;g;b | foreground color (RGB, non-standard)        |
 *  | 40-47      | background color (8 colors)                 |
 *  | 48;5;x     | background color (256 colors, non-standard) |
 *  | 48;2;r;g;b | background color (RGB, non-standard)        |
 *  | 90-97      | bright foreground color (non-standard)      |
 *  | 100-107    | bright background color (non-standard)      |
 *  ____________________________________________________________
 *
 *  The 8 actual colors within the ranges (30-37, 40-47, 90-97, 100-107) are defined by the ANSI standard as follows:
 *  -----------------------------
 *  | Last Digit    |   Color   |
 *  -----------------------------
 *  | 0             |   black   |
 *  | 1             |   red     |
 *  | 2             |   green   |
 *  | 3             |   yellow  |
 *  | 4             |   blue    |
 *  | 5             |   magenta |
 *  | 6             |   cyan    |
 *  | 7             |   white   |
 *  -----------------------------
 *
 * */

#ifdef R_USE_GLOG
#include "glog_output_utils.hpp"
#else 
#include "boost_output_utils.hpp"
#endif