///
/// Copyright(c) 2018 Aimin Huang
/// Distributed under the MIT License (http://opensource.org/licenses/MIT)
///
#include "highlight.h"
#define SHANNON_HIGHLIGHT_RESET_COLOR "\033[0m"

std::string internal_shannon_highlight_color(bool b, const char * title, const char * data, SHANNON_HIGHLIGHT_COLOR c, SHANNON_HIGHLIGHT_TEXT_STYLE ts){
	/// const char reset_color[] = "\033[0m";
	char my_color[] = "\033[0;30m";
    
    my_color[2] = static_cast<char>('0' + ts);
	my_color[5] = static_cast<char>('0' + c);
	std::string d(title); 
		if(title[0]) d.push_back(' ');  /// strlen
	if (b) d.append(my_color);
	d.append(data);
	if (b) d.append(SHANNON_HIGHLIGHT_RESET_COLOR);
	return d;
}

#undef SHANNON_HIGHLIGHT_RESET_COLOR