/* load curses */
var curses = require('../curses')

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
