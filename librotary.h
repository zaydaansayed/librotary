// librotary.h
#pragma once
#include <string>
#include <string_view>

namespace rotary {

enum Key {
  KEY_UP = 1000,
  KEY_DOWN,
  KEY_RIGHT,
  KEY_LEFT,
  KEY_CTRL_RIGHT,
  KEY_CTRL_LEFT,
  MOUSE_UP,
  MOUSE_DOWN,
  MOUSE_CLICK
};

void startscrn();
void closescrn();
void flush();
void print(int y, int x, std::string_view text);
void p_thin_line(int y, int x);
int read_key();
int mouse_x();
int mouse_y();
void raw_mode();
void move_curs(int y, int x);
int rows();
int cols();
void invert();
void stop_invert();
} // namespace rotary
