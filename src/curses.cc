/* Public Domain */

#ifndef BUILDING_NODE_EXTENSION
#	define BUILDING_NODE_EXTENSION
#endif

#include <node.h>
#include <v8.h>

// the windows header already defines MOUSE_MOVED
#ifdef MOUSE_MOVED
#	undef MOUSE_MOVED
#endif

#include <curses.h>
#include <stdint.h>

#include "ptrwrap.h"

using namespace v8;


// checks if the number of arguments is n
#define NODE_ARGS(n)																							\
	if (args.Length() != n) {																					\
		ThrowException(Exception::TypeError(String::New("Wrong number of arguments.")));						\
		return scope.Close(Undefined());																		\
	}																											\

// checks if the i'th argument is of type t
#define NODE_ARG(i, t)																							\
	if (!args[i]->Is##t()) {																					\
		ThrowException(Exception::TypeError(String::New("Wrong type for argument " #i ", expected " #t ".")));	\
		return scope.Close(Undefined());																		\
	}																											\

// checks if the i'th argument is pointer to t
#define NODE_PARG(i, t)																							\
	if (																										\
		!args[i]->IsObject() ||																					\
		!PtrWrap< t >::InstanceOf(args[i])																		\
	) {																											\
		ThrowException(Exception::TypeError(String::New("Wrong type for argument " #i ", expected " #t ".")));	\
		return scope.Close(Undefined());																		\
	}																											\

#define CAST_INT8(i)	static_cast<int8_t>( i )
#define CAST_INT16(i)	static_cast<int16_t>( i )
#define CAST_INT32(i)	static_cast<int32_t>( i )
#define CAST_INT64(i)	static_cast<int64_t>( i )

#define CAST_CHTYPE(c)	static_cast<chtype>( c )
#define CAST_BOOL(b)	static_cast<bool>( b )

#define CAST_PWINDOW(w)	PtrWrap<WINDOW>::Unwrap( w )
#define CAST_PSCREEN(s)	PtrWrap<SCREEN>::Unwrap( s )

#define CAST_POINTER(t, p)	PtrWrap<t>::New( p )

PTRWRAP_DECLARE(WINDOW)
PTRWRAP_DECLARE(SCREEN)

Handle<Value> node_color_pair(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_ARG(0, Number)
	int32_t n			= CAST_INT32(args[0]->Int32Value());
	int32_t result		= COLOR_PAIR(n);
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_addch(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_ARG(0, Number)
	chtype ch		= CAST_CHTYPE(args[0]->Int32Value());
	int32_t result	= addch( ch );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_box(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(3)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	NODE_ARG(2, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	chtype v		= CAST_CHTYPE(args[1]->Int32Value());
	chtype h		= CAST_CHTYPE(args[2]->Int32Value());
	int32_t result	= box( win, v, h );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_cbreak(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	int32_t result	= cbreak(  );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_delwin(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_PARG(0, WINDOW)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t result	= delwin( win );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_derwin(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(5)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	NODE_ARG(2, Number)
	NODE_ARG(3, Number)
	NODE_ARG(4, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	chtype h		= CAST_CHTYPE(args[1]->Int32Value());
	chtype w		= CAST_CHTYPE(args[2]->Int32Value());
	chtype y		= CAST_CHTYPE(args[3]->Int32Value());
	chtype x		= CAST_CHTYPE(args[4]->Int32Value());
	WINDOW* result	= derwin( win, h, w, y, x );
	return scope.Close( CAST_POINTER(WINDOW, result) );
}

Handle<Value> node_doupdate(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	int32_t result	= doupdate( );
	return scope.Close(Number::New( result ));
}

Handle<Value> node_dupwin(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_PARG(0, WINDOW)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	WINDOW* result	= dupwin( win );
	return scope.Close(CAST_POINTER(WINDOW, result));
}

Handle<Value> node_echo(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	int32_t result	= echo( );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_endwin(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	int32_t result	= endwin( );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_getmaxyx(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_PARG(0, WINDOW)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t y		= 0,
			x		= 0;
	getmaxyx( win, y, x );
	Local<Object> result = Object::New();
	result->Set(
		String::NewSymbol("y"),
		Int32::New(y)
	);
	result->Set(
		String::NewSymbol("x"),
		Int32::New(x)
	);
	return scope.Close(result);
}

Handle<Value> node_has_colors(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	bool result		= (has_colors() != 0);
	return scope.Close(Boolean::New( result ));
}

Handle<Value> node_init_color(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(4)
	NODE_ARG(0, Number)
	NODE_ARG(1, Number)
	NODE_ARG(2, Number)
	NODE_ARG(3, Number)
	int16_t a		= CAST_INT16(args[0]->Int32Value());
	int16_t b		= CAST_INT16(args[1]->Int32Value());
	int16_t c		= CAST_INT16(args[2]->Int32Value());
	int16_t d		= CAST_INT16(args[3]->Int32Value());
	int result		= init_color( a, b, c, d );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_init_pair(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(3)
	NODE_ARG(0, Number)
	NODE_ARG(1, Number)
	NODE_ARG(2, Number)
	int16_t a		= CAST_INT16(args[0]->Int32Value());
	int16_t b		= CAST_INT16(args[1]->Int32Value());
	int16_t c		= CAST_INT16(args[2]->Int32Value());
	int result		= init_pair( a, b, c );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_initscr(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	WINDOW* result	= initscr( );
	return scope.Close(CAST_POINTER(WINDOW, result));
}

Handle<Value> node_keyname(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_ARG(0, Number)
	int32_t k		= CAST_INT32(args[0]->Int32Value());
	char* result	= keyname( k );
	return scope.Close(String::New( result ));
}

Handle<Value> node_keypad(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Boolean)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	bool b			= CAST_BOOL(args[1]->BooleanValue());
	int32_t result	= keypad( win, b );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_killchar(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	char result	= killchar( );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_leaveok(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Boolean)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	bool b			= CAST_BOOL(args[1]->BooleanValue());
	int32_t result	= leaveok( win, b );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_longname(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	char* result	= longname( );
	return scope.Close(String::New( result ));
}

Handle<Value> node_meta(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Boolean)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	bool b			= CAST_BOOL(args[1]->BooleanValue());
	int32_t result	= meta( win, b );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_newpad(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_ARG(0, Number)
	NODE_ARG(1, Number)
	int32_t h		= CAST_INT32(args[0]->Int32Value());
	int32_t w		= CAST_INT32(args[1]->Int32Value());
	WINDOW* result	= newpad( h, w );
	return scope.Close(CAST_POINTER(WINDOW, result));
}

Handle<Value> node_newwin(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(4)
	NODE_ARG(0, Number)
	NODE_ARG(1, Number)
	NODE_ARG(2, Number)
	NODE_ARG(3, Number)
	int32_t h		= CAST_INT32(args[0]->Int32Value());
	int32_t w		= CAST_INT32(args[1]->Int32Value());
	int32_t y		= CAST_INT32(args[2]->Int32Value());
	int32_t x		= CAST_INT32(args[3]->Int32Value());
	WINDOW* result	= newwin( h, w, y, x );
	return scope.Close(CAST_POINTER(WINDOW, result));
}

Handle<Value> node_nocbreak(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	int32_t result	= nocbreak( );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_nodelay(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Boolean)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	bool b			= CAST_BOOL(args[1]->BooleanValue());
	int32_t result	= nodelay( win, b );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_noecho(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	int32_t result	= noecho( );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_scrollok(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Boolean)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	bool b			= CAST_BOOL(args[1]->BooleanValue());
	int32_t result	= scrollok( win, b );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_start_color(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	int32_t result	= start_color( );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_subpad(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(5)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	NODE_ARG(2, Number)
	NODE_ARG(3, Number)
	NODE_ARG(4, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t h		= CAST_INT32(args[1]->Int32Value());
	int32_t w		= CAST_INT32(args[2]->Int32Value());
	int32_t y		= CAST_INT32(args[3]->Int32Value());
	int32_t x		= CAST_INT32(args[4]->Int32Value());
	WINDOW* result	= subpad( win, h, w, y, x );
	return scope.Close(CAST_POINTER(WINDOW, result));
}

Handle<Value> node_subwin(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(5)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	NODE_ARG(2, Number)
	NODE_ARG(3, Number)
	NODE_ARG(4, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t h		= CAST_INT32(args[1]->Int32Value());
	int32_t w		= CAST_INT32(args[2]->Int32Value());
	int32_t y		= CAST_INT32(args[3]->Int32Value());
	int32_t x		= CAST_INT32(args[4]->Int32Value());
	WINDOW* result	= subwin( win, h, w, y, x );
	return scope.Close(CAST_POINTER(WINDOW, result));
}

Handle<Value> node_waddchnstr(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(3)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, String)
	NODE_ARG(2, Number)
	WINDOW *win		= CAST_PWINDOW(args[0]);
	String::Value str(args[1]);
	int32_t n		= CAST_INT32(args[2]->Int32Value());
	int32_t result	= waddchnstr( win, (chtype*) *str, n );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_waddchstr(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, String)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	String::Value str(args[1]);
	int32_t result	= waddchstr( win, (chtype*) *str );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_waddch(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	chtype ch		= CAST_CHTYPE(args[1]->Int32Value());
	int32_t result	= waddch( win, ch );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_waddnstr(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(3)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, String)
	NODE_ARG(2, Number)
	WINDOW *win		= CAST_PWINDOW(args[0]);
	String::Utf8Value str(args[1]);
	int32_t n		= CAST_INT32(args[2]->Int32Value());
	int32_t result	= waddnstr( win, *str, n );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_waddstr(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, String)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	String::Utf8Value str(args[1]);
	int32_t result	= waddstr( win, (char*) *str );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wattroff(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	chtype ch		= CAST_CHTYPE(args[1]->Int32Value());
	int32_t result	= wattroff( win, ch );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wattron(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	chtype ch		= CAST_CHTYPE(args[1]->Int32Value());
	int32_t result	= wattron( win, ch );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wattrset(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	chtype ch		= CAST_CHTYPE(args[1]->Int32Value());
	int32_t result	= wattrset( win, ch );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wbkgd(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	chtype ch		= CAST_CHTYPE(args[1]->Int32Value());
	int32_t result	= wbkgd( win, ch );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wclear(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_PARG(0, WINDOW)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t result	= wclear( win );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wdelch(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_PARG(0, WINDOW)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t result	= wdelch( win );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wdeleteln(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_PARG(0, WINDOW)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t result	= wdeleteln( win );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wechochar(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	chtype c		= CAST_CHTYPE(args[0]->Int32Value());
	int32_t result	= wechochar( win, c );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_werase(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_PARG(0, WINDOW)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t result	= werase( win );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wgetch(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_PARG(0, WINDOW)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t result	= wgetch( win );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wmove(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(3)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	NODE_ARG(2, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t y		= CAST_INT32(args[1]->Int32Value());
	int32_t x		= CAST_INT32(args[2]->Int32Value());
	int32_t result	= wmove( win, y, x );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wnoutrefresh(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_PARG(0, WINDOW)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t result	= wnoutrefresh( win );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wrefresh(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_PARG(0, WINDOW)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t result	= wrefresh( win );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wscrl(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t i		= CAST_INT32(args[1]->Int32Value());
	int32_t result	= wscrl( win, i );
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_curses_version(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	const char * result = curses_version();
	return scope.Close(String::New( result ));
}

Handle<Value> node_assume_default_colors(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_ARG(0, Number)
	NODE_ARG(1, Number)
	int32_t a		= CAST_INT32(args[0]->Int32Value());
	int32_t b		= CAST_INT32(args[1]->Int32Value());
	int32_t result	= assume_default_colors(a, b);
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_has_key(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_ARG(0, Number)
	int32_t k		= CAST_INT32(args[0]->Int32Value());
	bool result		= has_key(k)!=0;
	return scope.Close(Boolean::New( result ));
}

Handle<Value> node_use_default_colors(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	int32_t result	= use_default_colors();
	return scope.Close(Int32::New( result ));
}

Handle<Value> node_wresize(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(3)
	NODE_PARG(0, WINDOW)
	NODE_ARG(1, Number)
	NODE_ARG(2, Number)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t h		= CAST_INT32(args[1]->Int32Value());
	int32_t w		= CAST_INT32(args[2]->Int32Value());
	int32_t result	= wresize(win, h, w);
	return scope.Close(Int32::New( result ));
}

#ifdef NODE_CURSES_WINDOWS

Handle<Value> node_mouse_set(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_ARG(0, Number)
	NODE_ARG(1, Number)
	int32_t fh = CAST_INT32(args[0]->IntegerValue());
	int32_t fl = CAST_INT32(args[1]->IntegerValue());
	uint64_t f = ((uint64_t)fl) & (((uint64_t)fh) << 32);
	int32_t result = mouse_set(static_cast<unsigned long>(f));
	return scope.Close(Int32::New(result));
}

Handle<Value> node_mouse_on(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_ARG(0, Number)
	NODE_ARG(1, Number)
	int32_t fh = CAST_INT32(args[0]->IntegerValue());
	int32_t fl = CAST_INT32(args[1]->IntegerValue());
	uint64_t f = ((uint64_t)fl) & (((uint64_t)fh) << 32);
	int32_t result = mouse_on(static_cast<unsigned long>(f));
	return scope.Close(Int32::New(result));
}

Handle<Value> node_mouse_off(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_ARG(0, Number)
	NODE_ARG(1, Number)
	int32_t fh = CAST_INT32(args[0]->IntegerValue());
	int32_t fl = CAST_INT32(args[1]->IntegerValue());
	uint64_t f = ((uint64_t)fl) & (((uint64_t)fh) << 32);
	int32_t result = mouse_off(static_cast<unsigned long>(f));
	return scope.Close(Int32::New(result));
}

Handle<Value> node_request_mouse_pos(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	int32_t result = request_mouse_pos();
	return scope.Close(Int32::New(result));
}

Handle<Value> node_map_button(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(2)
	NODE_ARG(0, Number)
	NODE_ARG(1, Number)
	int32_t fh = CAST_INT32(args[0]->IntegerValue());
	int32_t fl = CAST_INT32(args[1]->IntegerValue());
	uint64_t f = ((uint64_t)fl) & (((uint64_t)fh) << 32);
	int32_t result = map_button(static_cast<unsigned long>(f));
	return scope.Close(Int32::New(result));
}

Handle<Value> node_wmouse_position(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(1)
	NODE_PARG(0, WINDOW)
	WINDOW* win		= CAST_PWINDOW(args[0]);
	int32_t y		= 0,
			x		= 0;
	wmouse_position( win, &y, &x );
	Local<Object> result = Object::New();
	result->Set(
		String::NewSymbol("y"),
		Int32::New(y)
	);
	result->Set(
		String::NewSymbol("x"),
		Int32::New(x)
	);
	return scope.Close(result);
}

Handle<Value> node_getmouse(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	uint64_t m = getmouse();
	int32_t mh = (int32_t)((m >> 32) & 0xffffffff);
	int32_t ml = (int32_t)(m & 0xffffffff);
	Local<Object> result = Object::New();
	result->Set(
		String::NewSymbol("high"),
		Int32::New(mh)
	);
	result->Set(
		String::NewSymbol("low"),
		Int32::New(ml)
	);
	return scope.Close(result);
}

Handle<Value> node_getbmap(const Arguments& args) {
	HandleScope scope;
	NODE_ARGS(0)
	uint64_t m = getbmap();
	int32_t mh = (int32_t)((m >> 32) & 0xffffffff);
	int32_t ml = (int32_t)(m & 0xffffffff);
	Local<Object> result = Object::New();
	result->Set(
		String::NewSymbol("high"),
		Int32::New(mh)
	);
	result->Set(
		String::NewSymbol("low"),
		Int32::New(ml)
	);
	return scope.Close(result);
}

#endif

void init(Handle<Object> target) {
	
	target->Set(
		String::NewSymbol("CURSES_RGB"),
#ifdef PDC_RGB
		Boolean::New(true)
#else
#	ifdef NCURSES_VERSION
		Boolean::New(true)
#	else
		Boolean::New(false)
#	endif
#endif
	);
	
	target->Set(
		String::NewSymbol("CHTYPE_SIZE"),
		Int32::New(sizeof(chtype))
	);
	
	target->Set(
		String::NewSymbol("COLOR_BLACK"),
		Int32::New(COLOR_BLACK)
	);
	
	target->Set(
		String::NewSymbol("COLOR_RED"),
		Int32::New(COLOR_RED)
	);
	
	target->Set(
		String::NewSymbol("COLOR_GREEN"),
		Int32::New(COLOR_GREEN)
	);
	
	target->Set(
		String::NewSymbol("COLOR_BLUE"),
		Int32::New(COLOR_BLUE)
	);
	
	target->Set(
		String::NewSymbol("COLOR_CYAN"),
		Int32::New(COLOR_CYAN)
	);
	
	target->Set(
		String::NewSymbol("COLOR_MAGENTA"),
		Int32::New(COLOR_MAGENTA)
	);
	
	target->Set(
		String::NewSymbol("COLOR_YELLOW"),
		Int32::New(COLOR_YELLOW)
	);
	
	target->Set(
		String::NewSymbol("COLOR_WHITE"),
		Int32::New(COLOR_WHITE)
	);
	
	target->Set(
		String::NewSymbol("A_NORMAL"),
		Int32::New(A_NORMAL)
	);
	
	target->Set(
		String::NewSymbol("A_ALTCHARSET"),
		Int32::New(A_ALTCHARSET)
	);
	
	target->Set(
		String::NewSymbol("A_INVIS"),
		Int32::New(A_INVIS)
	);
	
	target->Set(
		String::NewSymbol("A_UNDERLINE"),
		Int32::New(A_UNDERLINE)
	);
	
	target->Set(
		String::NewSymbol("A_REVERSE"),
		Int32::New(A_REVERSE)
	);
	
	target->Set(
		String::NewSymbol("A_BLINK"),
		Int32::New(A_BLINK)
	);
	
	target->Set(
		String::NewSymbol("A_BOLD"),
		Int32::New(A_BOLD)
	);
	
	target->Set(
		String::NewSymbol("A_ATTRIBUTES"),
		Int32::New(A_ATTRIBUTES)
	);
	
	target->Set(
		String::NewSymbol("A_CHARTEXT"),
		Int32::New(A_CHARTEXT)
	);
	
	target->Set(
		String::NewSymbol("A_COLOR"),
		Int32::New(A_COLOR)
	);
	
	target->Set(
		String::NewSymbol("A_PROTECT"),
		Int32::New(A_PROTECT)
	);
	
	target->Set(
		String::NewSymbol("ATTR_SHIFT"),
#ifdef NCURSES_VERSION
		Int32::New(NCURSES_ATTR_SHIFT)
#else
		Int32::New(PDC_ATTR_SHIFT)
#endif
	);
	
	target->Set(
		String::NewSymbol("A_STANDOUT"),
		Int32::New(A_STANDOUT)
	);
	
	target->Set(
		String::NewSymbol("A_DIM"),
		Int32::New(A_DIM)
	);
	
	target->Set(
		String::NewSymbol("color_pair"),
		FunctionTemplate::New(node_color_pair)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("addch"),
		FunctionTemplate::New(node_addch)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("box"),
		FunctionTemplate::New(node_box)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("cbreak"),
		FunctionTemplate::New(node_cbreak)->GetFunction()
	);

	target->Set(
		String::NewSymbol("delwin"),
		FunctionTemplate::New(node_delwin)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("derwin"),
		FunctionTemplate::New(node_derwin)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("doupdate"),
		FunctionTemplate::New(node_doupdate)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("dupwin"),
		FunctionTemplate::New(node_dupwin)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("echo"),
		FunctionTemplate::New(node_echo)->GetFunction()
	);

	target->Set(
		String::NewSymbol("endwin"),
		FunctionTemplate::New(node_endwin)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("getmaxyx"),
		FunctionTemplate::New(node_getmaxyx)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("has_colors"),
		FunctionTemplate::New(node_has_colors)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("init_color"),
		FunctionTemplate::New(node_init_color)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("init_pair"),
		FunctionTemplate::New(node_init_pair)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("initscr"),
		FunctionTemplate::New(node_initscr)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("keyname"),
		FunctionTemplate::New(node_keyname)->GetFunction()
	);

	target->Set(
		String::NewSymbol("keypad"),
		FunctionTemplate::New(node_keypad)->GetFunction()
	);

	target->Set(
		String::NewSymbol("killchar"),
		FunctionTemplate::New(node_killchar)->GetFunction()
	);

	target->Set(
		String::NewSymbol("leaveok"),
		FunctionTemplate::New(node_leaveok)->GetFunction()
	);

	target->Set(
		String::NewSymbol("longname"),
		FunctionTemplate::New(node_longname)->GetFunction()
	);

	target->Set(
		String::NewSymbol("meta"),
		FunctionTemplate::New(node_meta)->GetFunction()
	);

	target->Set(
		String::NewSymbol("newpad"),
		FunctionTemplate::New(node_newpad)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("newwin"),
		FunctionTemplate::New(node_newwin)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("nocbreak"),
		FunctionTemplate::New(node_nocbreak)->GetFunction()
	);

	target->Set(
		String::NewSymbol("nodelay"),
		FunctionTemplate::New(node_nodelay)->GetFunction()
	);

	target->Set(
		String::NewSymbol("noecho"),
		FunctionTemplate::New(node_noecho)->GetFunction()
	);

	target->Set(
		String::NewSymbol("scrollok"),
		FunctionTemplate::New(node_scrollok)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("start_color"),
		FunctionTemplate::New(node_start_color)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("subpad"),
		FunctionTemplate::New(node_subpad)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("subwin"),
		FunctionTemplate::New(node_subwin)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("waddchnstr"),
		FunctionTemplate::New(node_waddchnstr)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("waddchstr"),
		FunctionTemplate::New(node_waddchstr)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("waddch"),
		FunctionTemplate::New(node_waddch)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("waddnstr"),
		FunctionTemplate::New(node_waddnstr)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("waddstr"),
		FunctionTemplate::New(node_waddstr)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("wattroff"),
		FunctionTemplate::New(node_wattroff)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("wattron"),
		FunctionTemplate::New(node_wattron)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("wattrset"),
		FunctionTemplate::New(node_wattrset)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("wbkgd"),
		FunctionTemplate::New(node_wbkgd)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("wclear"),
		FunctionTemplate::New(node_wclear)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("wdelch"),
		FunctionTemplate::New(node_wdelch)->GetFunction()
	);

	target->Set(
		String::NewSymbol("wdeleteln"),
		FunctionTemplate::New(node_wdeleteln)->GetFunction()
	);

	target->Set(
		String::NewSymbol("wechochar"),
		FunctionTemplate::New(node_wechochar)->GetFunction()
	);

	target->Set(
		String::NewSymbol("werase"),
		FunctionTemplate::New(node_werase)->GetFunction()
	);

	target->Set(
		String::NewSymbol("wgetch"),
		FunctionTemplate::New(node_wgetch)->GetFunction()
	);

	target->Set(
		String::NewSymbol("wmove"),
		FunctionTemplate::New(node_wmove)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("wnoutrefresh"),
		FunctionTemplate::New(node_wnoutrefresh)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("wrefresh"),
		FunctionTemplate::New(node_wrefresh)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("wscrl"),
		FunctionTemplate::New(node_wscrl)->GetFunction()
	);
	
	target->Set(
		String::NewSymbol("assume_default_colors"),
		FunctionTemplate::New(node_assume_default_colors)->GetFunction()
	);

	target->Set(
		String::NewSymbol("curses_version"),
		FunctionTemplate::New(node_curses_version)->GetFunction()
	);

	target->Set(
		String::NewSymbol("has_key"),
		FunctionTemplate::New(node_has_key)->GetFunction()
	);

	target->Set(
		String::NewSymbol("use_default_colors"),
		FunctionTemplate::New(node_use_default_colors)->GetFunction()
	);

	target->Set(
		String::NewSymbol("wresize"),
		FunctionTemplate::New(node_wresize)->GetFunction()
	);

#ifdef NODE_CURSES_WINDOWS

	target->Set(
		String::NewSymbol("mouse_set"),
		FunctionTemplate::New(node_mouse_set)->GetFunction()
	);

	target->Set(
		String::NewSymbol("mouse_on"),
		FunctionTemplate::New(node_mouse_on)->GetFunction()
	);

	target->Set(
		String::NewSymbol("mouse_off"),
		FunctionTemplate::New(node_mouse_off)->GetFunction()
	);

	target->Set(
		String::NewSymbol("request_mouse_pos"),
		FunctionTemplate::New(node_request_mouse_pos)->GetFunction()
	);

	target->Set(
		String::NewSymbol("map_button"),
		FunctionTemplate::New(node_map_button)->GetFunction()
	);

	target->Set(
		String::NewSymbol("wmouse_position"),
		FunctionTemplate::New(node_wmouse_position)->GetFunction()
	);

	target->Set(
		String::NewSymbol("getmouse"),
		FunctionTemplate::New(node_getmouse)->GetFunction()
	);

	target->Set(
		String::NewSymbol("getbmap"),
		FunctionTemplate::New(node_getbmap)->GetFunction()
	);

#endif

}

NODE_MODULE(curses, init)


/*

	TODO: implement functions and tag with #
	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

#	int     addch(const chtype);
	int     addchnstr(const chtype *, int);
	int     addchstr(const chtype *);
	int     addnstr(const char *, int);
	int     addstr(const char *);
	int     attroff(chtype);
	int     attron(chtype);
	int     attrset(chtype);
	int     attr_get(attr_t *, short *, void *);
	int     attr_off(attr_t, void *);
	int     attr_on(attr_t, void *);
	int     attr_set(attr_t, short, void *);
	int     baudrate(void);
	int     beep(void);
	int     bkgd(chtype);
	void    bkgdset(chtype);
	int     border(chtype, chtype, chtype, chtype, chtype, chtype, chtype, chtype);
#	int     box(WINDOW *, chtype, chtype);
	bool    can_change_color(void);
#	int     cbreak(void);
	int     chgat(int, attr_t, short, const void *);
	int     clearok(WINDOW *, bool);
	int     clear(void);
	int     clrtobot(void);
	int     clrtoeol(void);
	int     color_content(short, short *, short *, short *);
	int     color_set(short, void *);
	int     copywin(const WINDOW *, WINDOW *, int, int, int, int, int, int, int);
	int     curs_set(int);
	int     def_prog_mode(void);
	int     def_shell_mode(void);
	int     delay_output(int);
	int     delch(void);
	int     deleteln(void);
	void    delscreen(SCREEN *); 
#	int     delwin(WINDOW *);
#	WINDOW *derwin(WINDOW *, int, int, int, int);
#	int     doupdate(void);
#	WINDOW *dupwin(WINDOW *);
	int     echochar(const chtype);
#	int     echo(void);
#	int     endwin(void);
	char    erasechar(void);
	int     erase(void);
	void    filter(void);
	int     flash(void);
	int     flushinp(void);
	chtype  getbkgd(WINDOW *);
	int     getnstr(char *, int);
	int     getstr(char *);
	WINDOW *getwin(FILE *);
	int     halfdelay(int);
#	bool    has_colors(void);
	bool    has_ic(void);
	bool    has_il(void);
	int     hline(chtype, int);
	void    idcok(WINDOW *, bool);
	int     idlok(WINDOW *, bool);
	void    immedok(WINDOW *, bool);
	int     inchnstr(chtype *, int);
	int     inchstr(chtype *);
	chtype  inch(void);
#	int     init_color(short, short, short, short);
#	int     init_pair(short, short, short);
#	WINDOW *initscr(void);
	int     innstr(char *, int);
	int     insch(chtype);
	int     insdelln(int);
	int     insertln(void);
	int     insnstr(const char *, int);
	int     insstr(const char *);
	int     instr(char *);
	int     intrflush(WINDOW *, bool);
	bool    isendwin(void);
	bool    is_linetouched(WINDOW *, int);
	bool    is_wintouched(WINDOW *);
#	char   *keyname(int);
#	int     keypad(WINDOW *, bool);
#	char    killchar(void);
#	int     leaveok(WINDOW *, bool);
#	char   *longname(void);
#	int     meta(WINDOW *, bool);
	int     move(int, int);
	int     mvaddch(int, int, const chtype);
	int     mvaddchnstr(int, int, const chtype *, int);
	int     mvaddchstr(int, int, const chtype *);
	int     mvaddnstr(int, int, const char *, int);
	int     mvaddstr(int, int, const char *);
	int     mvchgat(int, int, int, attr_t, short, const void *);
	int     mvcur(int, int, int, int);
	int     mvdelch(int, int);
	int     mvderwin(WINDOW *, int, int);
	int     mvgetch(int, int);
	int     mvgetnstr(int, int, char *, int);
	int     mvgetstr(int, int, char *);
	int     mvhline(int, int, chtype, int);
	chtype  mvinch(int, int);
	int     mvinchnstr(int, int, chtype *, int);
	int     mvinchstr(int, int, chtype *);
	int     mvinnstr(int, int, char *, int);
	int     mvinsch(int, int, chtype);
	int     mvinsnstr(int, int, const char *, int);
	int     mvinsstr(int, int, const char *);
	int     mvinstr(int, int, char *);
	int     mvprintw(int, int, const char *, ...);
	int     mvscanw(int, int, const char *, ...);
	int     mvvline(int, int, chtype, int);
	int     mvwaddchnstr(WINDOW *, int, int, const chtype *, int);
	int     mvwaddchstr(WINDOW *, int, int, const chtype *);
	int     mvwaddch(WINDOW *, int, int, const chtype);
	int     mvwaddnstr(WINDOW *, int, int, const char *, int);
	int     mvwaddstr(WINDOW *, int, int, const char *);
	int     mvwchgat(WINDOW *, int, int, int, attr_t, short, const void *);
	int     mvwdelch(WINDOW *, int, int);
	int     mvwgetch(WINDOW *, int, int);
	int     mvwgetnstr(WINDOW *, int, int, char *, int);
	int     mvwgetstr(WINDOW *, int, int, char *);
	int     mvwhline(WINDOW *, int, int, chtype, int);
	int     mvwinchnstr(WINDOW *, int, int, chtype *, int);
	int     mvwinchstr(WINDOW *, int, int, chtype *);
	chtype  mvwinch(WINDOW *, int, int);
	int     mvwinnstr(WINDOW *, int, int, char *, int);
	int     mvwinsch(WINDOW *, int, int, chtype);
	int     mvwinsnstr(WINDOW *, int, int, const char *, int);
	int     mvwinsstr(WINDOW *, int, int, const char *);
	int     mvwinstr(WINDOW *, int, int, char *);
	int     mvwin(WINDOW *, int, int);
	int     mvwprintw(WINDOW *, int, int, const char *, ...);
	int     mvwscanw(WINDOW *, int, int, const char *, ...);
	int     mvwvline(WINDOW *, int, int, chtype, int);
	int     napms(int);
#	WINDOW *newpad(int, int);
	SCREEN *newterm(const char *, FILE *, FILE *);
#	WINDOW *newwin(int, int, int, int);
	int     nl(void);
#	int     nocbreak(void);
#	int     nodelay(WINDOW *, bool);
#	int     noecho(void);
	int     nonl(void);
	void    noqiflush(void);
	int     noraw(void);
	int     notimeout(WINDOW *, bool);
	int     overlay(const WINDOW *, WINDOW *);
	int     overwrite(const WINDOW *, WINDOW *);
	int     pair_content(short, short *, short *);
	int     pechochar(WINDOW *, chtype);
	int     pnoutrefresh(WINDOW *, int, int, int, int, int, int);
	int     prefresh(WINDOW *, int, int, int, int, int, int);
	int     printw(const char *, ...);
	int     putwin(WINDOW *, FILE *);
	void    qiflush(void);
	int     raw(void);
	int     redrawwin(WINDOW *);
	int     refresh(void);
	int     reset_prog_mode(void);
	int     reset_shell_mode(void);
	int     resetty(void);
	int     ripoffline(int, int (*)(WINDOW *, int));
	int     savetty(void);
	int     scanw(const char *, ...);
	int     scr_dump(const char *);
	int     scr_init(const char *);
	int     scr_restore(const char *);
	int     scr_set(const char *);
	int     scrl(int);
	int     scroll(WINDOW *);
#	int     scrollok(WINDOW *, bool);
	SCREEN *set_term(SCREEN *);
	int     setscrreg(int, int);
	int     slk_attroff(const chtype);
	int     slk_attr_off(const attr_t, void *);
	int     slk_attron(const chtype);
	int     slk_attr_on(const attr_t, void *);
	int     slk_attrset(const chtype);
	int     slk_attr_set(const attr_t, short, void *);
	int     slk_clear(void);
	int     slk_color(short);
	int     slk_init(int);
	char   *slk_label(int);
	int     slk_noutrefresh(void);
	int     slk_refresh(void);
	int     slk_restore(void);
	int     slk_set(int, const char *, int);
	int     slk_touch(void);
	int     standend(void);
	int     standout(void);
	int     start_color(void);
#	WINDOW *subpad(WINDOW *, int, int, int, int);
#	WINDOW *subwin(WINDOW *, int, int, int, int);
	int     syncok(WINDOW *, bool);
	chtype  termattrs(void);
	attr_t  term_attrs(void);
	char   *termname(void);
	void    timeout(int);
	int     touchline(WINDOW *, int, int);
	int     touchwin(WINDOW *);
	int     typeahead(int);
	int     untouchwin(WINDOW *);
	void    use_env(bool);
	int     vidattr(chtype);
	int     vid_attr(attr_t, short, void *);
	int     vidputs(chtype, int (*)(int));
	int     vid_puts(attr_t, short, void *, int (*)(int));
	int     vline(chtype, int);
	int     vw_printw(WINDOW *, const char *, va_list);
	int     vwprintw(WINDOW *, const char *, va_list);
	int     vw_scanw(WINDOW *, const char *, va_list);
	int     vwscanw(WINDOW *, const char *, va_list);
#	int     waddchnstr(WINDOW *, const chtype *, int);
#	int     waddchstr(WINDOW *, const chtype *);
#	int     waddch(WINDOW *, const chtype);
#	int     waddnstr(WINDOW *, const char *, int);
#	int     waddstr(WINDOW *, const char *);
#	int     wattroff(WINDOW *, chtype);
#	int     wattron(WINDOW *, chtype);
#	int     wattrset(WINDOW *, chtype);
	int     wattr_get(WINDOW *, attr_t *, short *, void *);
	int     wattr_off(WINDOW *, attr_t, void *);
	int     wattr_on(WINDOW *, attr_t, void *);
	int     wattr_set(WINDOW *, attr_t, short, void *);
	void    wbkgdset(WINDOW *, chtype);
#	int     wbkgd(WINDOW *, chtype);
	int     wborder(WINDOW *, chtype, chtype, chtype, chtype, chtype, chtype, chtype, chtype);
	int     wchgat(WINDOW *, int, attr_t, short, const void *);
#	int     wclear(WINDOW *);
	int     wclrtobot(WINDOW *);
	int     wclrtoeol(WINDOW *);
	int     wcolor_set(WINDOW *, short, void *);
	void    wcursyncup(WINDOW *);
#	int     wdelch(WINDOW *);
#	int     wdeleteln(WINDOW *);
#	int     wechochar(WINDOW *, const chtype);
#	int     werase(WINDOW *);
#	int     wgetch(WINDOW *);
	int     wgetnstr(WINDOW *, char *, int);
	int     wgetstr(WINDOW *, char *);
	int     whline(WINDOW *, chtype, int);
	int     winchnstr(WINDOW *, chtype *, int);
	int     winchstr(WINDOW *, chtype *);
	chtype  winch(WINDOW *);
	int     winnstr(WINDOW *, char *, int);
	int     winsch(WINDOW *, chtype);
	int     winsdelln(WINDOW *, int);
	int     winsertln(WINDOW *);
	int     winsnstr(WINDOW *, const char *, int);
	int     winsstr(WINDOW *, const char *);
	int     winstr(WINDOW *, char *);
#	int     wmove(WINDOW *, int, int);
#	int     wnoutrefresh(WINDOW *);
	int     wprintw(WINDOW *, const char *, ...);
	int     wredrawln(WINDOW *, int, int);
#	int     wrefresh(WINDOW *);
	int     wscanw(WINDOW *, const char *, ...);
#	int     wscrl(WINDOW *, int);
	int     wsetscrreg(WINDOW *, int, int);
	int     wstandend(WINDOW *);
	int     wstandout(WINDOW *);
	void    wsyncdown(WINDOW *);
	void    wsyncup(WINDOW *);
	void    wtimeout(WINDOW *, int);
	int     wtouchln(WINDOW *, int, int, int);
	int     wvline(WINDOW *, chtype, int);

	chtype  getattrs(WINDOW *);
	int     getbegx(WINDOW *);
	int     getbegy(WINDOW *);
	int     getmaxx(WINDOW *);
	int     getmaxy(WINDOW *);
	int     getparx(WINDOW *);
	int     getpary(WINDOW *);
	int     getcurx(WINDOW *);
	int     getcury(WINDOW *);
	void    traceoff(void);
	void    traceon(void);
	char   *unctrl(chtype);

	int     crmode(void);
	int     nocrmode(void);
	int     draino(int);
	int     resetterm(void);
	int     fixterm(void);
	int     saveterm(void);
	int     setsyx(int, int);

#	int     mouse_set(unsigned long);
#	int     mouse_on(unsigned long);
#	int     mouse_off(unsigned long);
#	int     request_mouse_pos(void);
#	int     map_button(unsigned long);
#	void    wmouse_position(WINDOW *, int *, int *);
#	unsigned long getmouse(void);
#	unsigned long getbmap(void);

#	int     assume_default_colors(int, int);
#	const char *curses_version(void);
#	bool    has_key(int);
#	int     use_default_colors(void);
#	int     wresize(WINDOW *, int, int);

	int     mouseinterval(int);
	mmask_t mousemask(mmask_t, mmask_t *);
	bool    mouse_trafo(int *, int *, bool);
	int     nc_getmouse(MEVENT *);
	int     ungetmouse(MEVENT *);
	bool    wenclose(const WINDOW *, int, int);
	bool    wmouse_trafo(const WINDOW *, int *, int *, bool);

*/
