#include "librotary.h"
#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

namespace rotary {

termios original;

void startscrn() { cout << "\x1b[?1049h\x1b[2J\x1b[?7l\x1b[?1002h\x1b[?1006h"; }

void closescrn() {
  cout << "\x1b[?1006l\x1b[?1002l\x1b[?1049l";
  tcsetattr(0, TCSAFLUSH, &original);
}

void flush() { cout << std::flush; }

void print(int y, int x, string_view text) {
  cout << "\x1b[" + to_string(y + 1) + ";" + to_string(x + 1) + "H" << text
       << "\x1b[K";
}

void p_thin_line(int y, int x) {
  cout << "\x1b[" + to_string(y + 1) + ";" + to_string(x + 1) + "H" << "│";
}

void clrscrn() { cout << "\x1b[2J"; }

int mx = 0, my = 0;
int mouse_x() { return mx - 1; }
int mouse_y() { return my - 1; }

int read_key() {
  char c = 0;
  read(0, &c, 1);
  if (c != '\x1b')
    return c;

  char seq[2];
  read(0, &seq[0], 1);
  read(0, &seq[1], 1);

  if (seq[0] == '[') {
    string params;
    char t = seq[1];
    while (t < 0x40 || t > 0x7E) {
      params += t;
      if (read(0, &t, 1) != 1)
        break;
    }

    if (!params.empty() && params[0] == '<') {
      int b = 0;
      sscanf(params.c_str() + 1, "%d;%d;%d", &b, &mx, &my);
      if (t == 'm' || (b & 32))
        return 0;
      if (b == 64)
        return MOUSE_UP;
      if (b == 65)
        return MOUSE_DOWN;
      return MOUSE_CLICK;
    }

    bool ctrl = (params == "1;5");
    switch (t) {
    case 'A':
      return KEY_UP;
    case 'B':
      return KEY_DOWN;
    case 'C':
      return ctrl ? KEY_CTRL_RIGHT : KEY_RIGHT;
    case 'D':
      return ctrl ? KEY_CTRL_LEFT : KEY_LEFT;
    }
    return 0;
  }
  return '\x1b';
}

void raw_mode() {
  tcgetattr(0, &original);
  termios ts = original;
  ts.c_iflag &= ~IXON;
  ts.c_lflag &= ~(ECHO | ICANON | ISIG);
  tcsetattr(0, TCSAFLUSH, &ts);
}

void move_curs(int y, int x) {
  cout << "\x1b[" + to_string(y + 1) + ";" + to_string(x + 1) + "H"
       << std::flush;
}

int rows() {
  winsize ws{};
  ioctl(1, TIOCGWINSZ, &ws);
  return ws.ws_row;
}

int cols() {
  winsize ws{};
  ioctl(1, TIOCGWINSZ, &ws);
  return ws.ws_col;
}

void invert() { cout << "\x1b[7m"; }

void stop_invert() { cout << "\x1b[0m"; }

} // namespace rotary
