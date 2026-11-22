# librotary

A tiny C++ library for building terminal user interfaces on Linux.

It wraps raw ANSI escape sequences and `termios` into a handful of simple calls,
so you can draw text at a position, read arrow keys and mouse input, and restore
the terminal on exit without touching escape codes yourself.

It was made for use in tui text editors and grid based tui programs.

## Features

- Alternate screen buffer (your shell history comes back on exit)
- Raw keyboard input with arrow key detection
- Mouse click / scroll reporting
- Cursor positioning and screen size queries
- Inverse-video highlighting
- No dependencies beyond the C++ standard library and POSIX

## Requirements

- Linux (or another POSIX system with `termios`, `ioctl` and ANSI terminals)
- A C++ compiler (`g++`) and `ar` (use C++17)
- A terminal that supports ANSI escape sequences (nearly all do)

## Build

```sh
make
```

This produces `librotary.a` and `librotary.o`.

## Install

```sh
sudo make install
```

Installs:

- `/usr/local/lib/librotary.a`
- `/usr/local/include/librotary.h`

To remove them again:

```sh
sudo make uninstall
```

## Usage

Link the static library when compiling your program:

```sh
g++ -std=c++17 main.cpp -lrotary -o main
```
Hello world example:

```cpp
#include "librotary.h"

int main() {
  rotary::startscrn();
  rotary::raw_mode();
  rotary::print(0, 0, "Hello World!");
  rotary::flush();
  rotary::read_key();
  rotary::closescrn();
}
```

Minimal example — show the code of the last key pressed, and quit with `q`:

```cpp
#include "librotary.h"
#include <string>

int main() {
  rotary::startscrn();
  rotary::raw_mode();

  int last = 0;
  while (true) {
    rotary::print(1, 1, "last key: " + std::to_string(last));
    rotary::print(2, 1, "press q to quit");
    rotary::flush();

    last = rotary::read_key();
    if (last == 'q') break;
  }

  rotary::closescrn();
  return 0;
}
```

## API

All functions live in the `rotary` namespace. Coordinates are zero-based; `y` is
the row and `x` is the column, both starting at `0` in the top-left corner.

### Screen

| Function | Description |
| --- | --- |
| `void startscrn()` | Switch to the alternate screen, hide wrapping, enable mouse reporting. |
| `void closescrn()` | Disable mouse reporting, leave the alternate screen, restore terminal settings. |
| `int rows()` | Current terminal height in rows. |
| `int cols()` | Current terminal width in columns. |

### Input

| Function | Description |
| --- | --- |
| `void raw_mode()` | Put the terminal in raw mode (no echo, no line buffering, no signals). |
| `int read_key()` | Block until a key or mouse event arrives and return its code. |

`read_key()` returns a plain `char` for ordinary keys (e.g. `'q'`, `'\n'`), or one
of the following values:

| Constant | Meaning |
| --- | --- |
| `KEY_UP`, `KEY_DOWN`, `KEY_LEFT`, `KEY_RIGHT` | Arrow keys |
| `MOUSE_UP`, `MOUSE_DOWN` | Mouse wheel scroll |
| `MOUSE_CLICK` | Mouse button press or release |

Mouse coordinates for the most recent event are available through
`int mouse_x()` and `int mouse_y()`.

### Drawing

| Function | Description |
| --- | --- |
| `void print(int y, int x, std::string_view text)` | Write `text` at the given position and clear the rest of the line. |
| `void flush()` | Lets the text show instantly without moving the cursor. |
| `void move_curs(int y, int x)` | Move the cursor to the given position. |
| `void invert()` | Start inverse video (swap foreground and background). |
| `void stop_invert()` | Reset all text attributes. |

## Notes

- Always call `closescrn()` before exiting, otherwise the terminal is left in raw
  mode and the alternate screen. It is restored from the settings saved by the
  first `raw_mode()` / `closescrn()` call.
- `raw_mode()` disables `ISIG`, so `Ctrl+C` is delivered as a normal key press
  instead of a signal. Handle it yourself if you want it to quit.
- This is a small, deliberately simple library. It does not do line wrapping,
  colors beyond inverse video, or Unicode width calculations.

## License

MIT — see [LICENSE](LICENSE).
