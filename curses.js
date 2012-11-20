
/* this module loads the native curses module */

/* --- SETUP DEBUGGING --- */
var DEBUG						= false;

/* --- NATIVE MODULE --- */
exports = module.exports		= require('./build/'+(DEBUG?'Debug':'Release')+'/curses');
exports.CURSES_DEBUG			= DEBUG;
