
var curses = require('../curses')

	/* top level window */
,	stdwin = curses.initscr()

	/* color pair indices */
,	TITLECOLOR       = 1
,	MAINMENUCOLOR    = (2 | curses.A_BOLD)
,	MAINMENUREVCOLOR = (3 | curses.A_BOLD | curses.A_REVERSE)
,	SUBMENUCOLOR     = (4 | curses.A_BOLD)
,	SUBMENUREVCOLOR  = (5 | curses.A_BOLD | curses.A_REVERSE)
,	BODYCOLOR        = 6
,	STATUSCOLOR      = (7 | curses.A_BOLD)
,	INPUTBOXCOLOR    = 8
,	EDITBOXCOLOR     = (9 | curses.A_BOLD | curses.A_REVERSE)
;

/* initialize color pairs */
if (curses.has_colors()) {
	curses.start_color();																		// cp index (0 = default)
    curses.init_pair(TITLECOLOR       & ~curses.A_ATTR, curses.COLOR_BLACK, curses.COLOR_CYAN);	// 1
    curses.init_pair(MAINMENUCOLOR    & ~curses.A_ATTR, curses.COLOR_WHITE, curses.COLOR_CYAN);	// 2
    curses.init_pair(MAINMENUREVCOLOR & ~curses.A_ATTR, curses.COLOR_WHITE, curses.COLOR_BLACK);// 3
    curses.init_pair(SUBMENUCOLOR     & ~curses.A_ATTR, curses.COLOR_WHITE, curses.COLOR_CYAN);	// 4
    curses.init_pair(SUBMENUREVCOLOR  & ~curses.A_ATTR, curses.COLOR_WHITE, curses.COLOR_BLACK);// 5
    curses.init_pair(BODYCOLOR        & ~curses.A_ATTR, curses.COLOR_WHITE, curses.COLOR_BLUE);	// 6
    curses.init_pair(STATUSCOLOR      & ~curses.A_ATTR, curses.COLOR_WHITE, curses.COLOR_CYAN);	// 7
    curses.init_pair(INPUTBOXCOLOR    & ~curses.A_ATTR, curses.COLOR_BLACK, curses.COLOR_CYAN);	// 8
    curses.init_pair(EDITBOXCOLOR     & ~curses.A_ATTR, curses.COLOR_WHITE, curses.COLOR_BLACK);// 9
}

/* draw border */
curses.wattrset(stdwin, curses.color_pair(8));
curses.wclear(stdwin);
curses.box(stdwin, 0, 0);
curses.wmove(stdwin, 1, 1);

/* create subwindow 20x20 */
var subwin = curses.subwin(stdwin, 20, 20, 1, 1)
,	cp = 0
;

curses.wrefresh(stdwin);
curses.wrefresh(subwin);

/* print to subwindow */
setInterval(function () {
	++cp;
	if (cp % 10 == 0) cp = 1;
	curses.wattrset(subwin, curses.color_pair(cp));
	curses.waddch(subwin, '#'.charCodeAt(0));
	curses.wrefresh(subwin);
}, 25);

/* exit after 10s */
setTimeout(function () {
	curses.endwin();
	process.exit(0);
}, 10000);
