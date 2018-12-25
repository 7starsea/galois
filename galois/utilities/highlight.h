///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#ifndef SHANNON_HIGHLIGHT_H
#define SHANNON_HIGHLIGHT_H

#include <string>


enum SHANNON_HIGHLIGHT_COLOR { BLACK, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN };
enum SHANNON_HIGHLIGHT_TEXT_STYLE { NORMAL, BOLD, DARKER, UNKNOWN1, UNDERLINE, UNKNOWN2, UNKNOWN3, BACKGROUND, UNKNOWN4, STRIKE };

std::string internal_shannon_highlight_color(bool b, const char * title, const char * data, SHANNON_HIGHLIGHT_COLOR c, SHANNON_HIGHLIGHT_TEXT_STYLE ts=SHANNON_HIGHLIGHT_TEXT_STYLE::NORMAL);



/// Reference: http://www.cplusplus.com/forum/unices/36461/
#define SHANNON_HIGHLIGHT_RED(b, data, fmt_str)		internal_shannon_highlight_color(b, data, fmt_str, SHANNON_HIGHLIGHT_COLOR::RED)
#define SHANNON_HIGHLIGHT_GREEN(b, data, fmt_str)	internal_shannon_highlight_color(b, data, fmt_str, SHANNON_HIGHLIGHT_COLOR::GREEN)
#define SHANNON_HIGHLIGHT_YELLOW(b, data, fmt_str)	internal_shannon_highlight_color(b, data, fmt_str, SHANNON_HIGHLIGHT_COLOR::YELLOW)
#define SHANNON_HIGHLIGHT_BLUE(b, data, fmt_str)	internal_shannon_highlight_color(b, data, fmt_str, SHANNON_HIGHLIGHT_COLOR::BLUE)
#define SHANNON_HIGHLIGHT_MAGENTA(b, data, fmt_str)	internal_shannon_highlight_color(b, data, fmt_str, SHANNON_HIGHLIGHT_COLOR::MAGENTA)
#define SHANNON_HIGHLIGHT_CYAN(b, data, fmt_str)	internal_shannon_highlight_color(b, data, fmt_str, SHANNON_HIGHLIGHT_COLOR::CYAN)

#define SHANNON_HIGHLIGHT(b, data) SHANNON_HIGHLIGHT_RED(b, data, "{} ")


#endif