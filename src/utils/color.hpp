///*****************************************************************************
/// @date Feb 2021
/// @copyright GPL-3.0-or-later
/// @author Thomas Lepoix <thomas.lepoix@protonmail.ch>
///*****************************************************************************

#pragma once

#include <string>

#ifndef NOCOLOR

#define S_RESET      std::string("\033[m")
#define S_BOLD       std::string("\033[01m")
//#define S_ std::string("\033[02m")
//#define S_ std::string("\033[03m")
#define S_UNDERSCORE std::string("\033[04m")
#define S_BLINK      std::string("\033[05m")
//#define S_ std::string("\033[06m")
#define S_REVERSE    std::string("\033[07m")
#define S_CONCEALED  std::string("\033[08m")

#define F_BLACK   std::string("\033[30m") // color_black
#define F_D_RED   std::string("\033[31m") // color_dark_red
#define F_D_GREEN std::string("\033[32m") // color_dark_green
#define F_ORANGE  std::string("\033[33m") // color_orange
#define F_D_BLUE  std::string("\033[34m") // color_dark_blue
#define F_MAGENTA std::string("\033[35m") // color_magenta
#define F_L_BLUE  std::string("\033[36m") // color_light_blue
#define F_L_GRAY  std::string("\033[37m") // color_light_gray
#define F_GRAY    std::string("\033[90m") // color_gray
#define F_RED     std::string("\033[91m") // color_red
#define F_GREEN   std::string("\033[92m") // color_green
#define F_YELLOW  std::string("\033[93m") // color_yellow
#define F_BLU     std::string("\033[94m") // color_blue
#define F_PINK    std::string("\033[95m") // color_pink
#define F_CYAN    std::string("\033[96m") // color_cyan
#define F_WHITE   std::string("\033[97m") // color_white

#define B_BLACK   std::string("\033[40m") // color_black
#define B_D_RED   std::string("\033[41m") // color_dark_red
#define B_D_GREEN std::string("\033[42m") // color_dark_green
#define B_ORANGE  std::string("\033[43m") // color_orange
#define B_D_BLUE  std::string("\033[44m") // color_dark_blue
#define B_MAGENTA std::string("\033[45m") // color_magenta
#define B_L_BLUE  std::string("\033[46m") // color_light_blue
#define B_L_GRAY  std::string("\033[47m") // color_light_gray
#define B_GRAY    std::string("\033[100m") // color_gray
#define B_RED     std::string("\033[101m") // color_red
#define B_GREEN   std::string("\033[102m") // color_green
#define B_YELLOW  std::string("\033[103m") // color_yellow
#define B_BLU     std::string("\033[104m") // color_blue
#define B_PINK    std::string("\033[105m") // color_pink
#define B_CYAN    std::string("\033[106m") // color_cyan
#define B_WHITE   std::string("\033[107m") // color_white

#else // NOCOLOR

#define S_RESET      std::string()
#define S_BOLD       std::string()
//#define S_UNDERSCORE std::string()
//#define S_UNDERSCORE std::string()
#define S_UNDERSCORE std::string()
#define S_BLINK      std::string()
//#define S_UNDERSCORE std::string()
#define S_REVERSE    std::string()
#define S_CONCEALED  std::string()

#define F_BLACK   std::string()
#define F_D_RED   std::string()
#define F_D_GREEN std::string()
#define F_ORANGE  std::string()
#define F_D_BLUE  std::string()
#define F_MAGENTA std::string()
#define F_L_BLUE  std::string()
#define F_L_GRAY  std::string()
#define F_GRAY    std::string()
#define F_RED     std::string()
#define F_GREEN   std::string()
#define F_YELLOW  std::string()
#define F_BLU     std::string()
#define F_PINK    std::string()
#define F_CYAN    std::string()
#define F_WHITE   std::string()

#define B_BLACK   std::string()
#define B_D_RED   std::string()
#define B_D_GREEN std::string()
#define B_ORANGE  std::string()
#define B_D_BLUE  std::string()
#define B_MAGENTA std::string()
#define B_L_BLUE  std::string()
#define B_L_GRAY  std::string()
#define B_GRAY    std::string()
#define B_RED     std::string()
#define B_GREEN   std::string()
#define B_YELLOW  std::string()
#define B_BLU     std::string()
#define B_PINK    std::string()
#define B_CYAN    std::string()
#define B_WHITE   std::string()

#endif // NOCOLOR
