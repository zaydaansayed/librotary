#include <iostream>
#include <string>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

using namespace std;

void startscrn() {
	cout << "\x1b[?1049h";
}

void raw_mode() {
	  termios ts;
  tcgetattr(0, &ts);
  ts.c_iflag &= ~IXON;
  ts.c_lflag &= ~(ECHO | ICANON);
  tcsetattr(0, TCSAFLUSH, &ts);
}

void move_curs(int cy, int cx) {
	cout << "\x1b[" + to_string(cy) + ";" + to_string(cx) + "H";
}
