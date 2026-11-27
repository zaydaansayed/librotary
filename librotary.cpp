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
    if (seq[1] == '<') {
      string buf;
      char ch = 0;
      while (read(0, &ch, 1) == 1 && ch != 'M' && ch != 'm')
        buf += ch;

      int b = 0, x = 0, y = 0;
      sscanf(buf.c_str(), "%d;%d;%d", &b, &mx, &my);

      if (b == 64)
        return MOUSE_UP;
      if (b == 65)
        return MOUSE_DOWN;
      else
        return MOUSE_CLICK;
    }
    if (seq[1] == 'A')
      return KEY_UP;
    if (seq[1] == 'B')
      return KEY_DOWN;
    if (seq[1] == 'C')
      return KEY_RIGHT;
    if (seq[1] == 'D')
      return KEY_LEFT;
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
