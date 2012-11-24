
# curses

Bindings for the native curses library, a full featured console IO library.
Compiles on Windows with Visual Studio 10 or Linux/UNIX with GCC toolchain.
The curses library for the target system will automatically be downloaded
and compiled into a static library on installation.

On Windows the PDCurses library will be used and on Linux/UNIX it links against
the ncurses library.


## Installation

To install curses just type:

```
npm install curses
```

or install from GIT

```
git clone git://github.com/fresc81/node-curses curses
cd curses
npm install .
```


## Rebuild

If you have changed the C++ sources:

```
node-gyp rebuild
```


## Usage

```javascript
/* load curses */
var curses = require('curses')

/* initialize top level window */
,   stdwin = curses.initscr()

/* color pair definitions */
,   GLYPH_COLOR = 1
,   BORDER_COLOR = 2

;

/* initialize color pairs (id, FG, BG) */
if (curses.has_colors()) {
    curses.start_color();
    
    /* glyph color */
    curses.init_pair(GLYPH_COLOR, curses.COLOR_GREEN, curses.COLOR_BLACK);
    
    /* border color */
    curses.init_pair(BORDER_COLOR, curses.COLOR_WHITE, curses.COLOR_BLACK);
    
}

/* clear toplevel window and draw border */
curses.wattrset(stdwin, curses.color_pair(BORDER_COLOR));
curses.wclear(stdwin);
curses.box(stdwin, 0, 0);
curses.wrefresh(stdwin);

/* create a subwindow */
var sub_height = 20
,   sub_width = 30
,   sub_top = 1
,   sub_left = 1
,   subwin = curses.subwin(stdwin, sub_height, sub_width, sub_top, sub_left)
;

/* setup the subwindow's background and echo Hello world! into it */
curses.wbkgd(subwin, ' '.charCodeAt(0)|curses.color_pair(GLYPH_COLOR)|curses.A_REVERSE);
curses.wclear(subwin);
curses.box(subwin, 0, 0);
curses.wmove(subwin, 1, 8);
curses.wattrset(subwin, curses.color_pair(GLYPH_COLOR));
curses.waddstr(subwin, "Hello world!");
curses.wrefresh(subwin);

/* wait for a keystroke */
curses.wgetch(subwin);

/* reset terminal (never forget this) */
curses.endwin();
```

![hello.js](https://github.com/fresc81/node-curses/wiki/hello.png)

Look at the bottom of [curses.cc](https://github.com/fresc81/node-curses/blob/master/src/curses.cc "src/curses") to
see what already has been implemented.

See the ncurses manual for descriptions of the functions.

[ncurses HOWTO](http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/ "ncurses HOWTO")

[ncurses manual](http://invisible-island.net/ncurses/man/ncurses.3x.html "ncurses manual")
