
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
var curses = require('./curses')

	/* initialize top level window */
,	stdwin = curses.initscr()
;

// ...

// reset terminal
curses.endwin();

```


See the ncurses manual for descriptions of the functions.

http://tldp.org/HOWTO/NCURSES-Programming-HOWTO/

http://invisible-island.net/ncurses/man/ncurses.3x.html
