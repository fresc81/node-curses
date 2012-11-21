
/***********************\
*  ENTER THE MATRIX 8-) *
\***********************/

var curses = require('../curses')

	/* top level window */
,	stdwin = curses.initscr()

	/* get screen size */
,	maxyx = curses.getmaxyx(stdwin)

;

/* initialize color pairs (id, FG, BG) */
if (curses.has_colors()) {
	curses.start_color();
	
	/* glyph color */
    curses.init_pair(1, curses.COLOR_GREEN, curses.COLOR_BLACK);
	
	/* border color */
	curses.init_pair(2, curses.COLOR_WHITE, curses.COLOR_BLACK);
	
}

/* draw border */
curses.wattrset(stdwin, curses.color_pair(2));
curses.wclear(stdwin);
curses.box(stdwin, 0, 0);
curses.wrefresh(stdwin);

/* draw bands */
maxyx.x -= 2;
maxyx.y -= 2;
var subwins = new Array(maxyx.x);
for (var i=0; i<maxyx.x; i++) {
	subwins[i] = curses.subwin(stdwin, maxyx.y, 1, 1, i+1);
	curses.wattrset(subwins[i], curses.color_pair(1));
	curses.scrollok(subwins[i], true);
	
	setInterval(function (subwin) {
		curses.waddch(subwin, 0xff & Math.floor(Math.random() * 0xff));
		/* tag subwin as dirty */
		curses.wnoutrefresh(subwin);
	}, 100 + Math.floor(Math.random() * 250), subwins[i]);
	
}

/* refresh every 100ms */
setInterval(function () {
	/* repaint dirty subwins */
	curses.doupdate();
}, 100);

/* exit after 25s */
setTimeout(function () {
	curses.endwin();
	process.exit(0);
}, 25000);
