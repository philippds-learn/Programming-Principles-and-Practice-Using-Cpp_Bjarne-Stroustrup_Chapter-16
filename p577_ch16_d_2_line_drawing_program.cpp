//  Philipp Siedler
//  Bjarne Stroustrup's PP
//  Chapter 16 Drill 2

#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"

// layout
constexpr int xmax = 600;
constexpr int ymax = 400;

struct Lines_window : Window {
	Lines_window(Point xy, int w, int h, const string& title);
private:
	// data
	Open_polyline lines;

	// widgets
	Button next_button;	// add (next_x, next_y) to lines
	Button quit_button;	// end program
	In_box next_x;
	In_box next_y;
	Out_box xy_out;

	// actions invoked by callbacks
	void next();
	void quit();

	// callback functions
	static void cb_next(Address, Address);
	static void cb_quit(Address, Address);
};

Lines_window::Lines_window(Point xy, int w, int h, const string& title)
	:Window(xy, w, h, title),
	next_button(Point(x_max() - 150, 0), 70, 20, "Next point", cb_next),
	quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit),
	next_x(Point(x_max() - 310, 0), 50, 20, "next x:"),
	next_y(Point(x_max() - 210, 0), 50, 20, "next y:"),
	xy_out(Point(100, 0), 100, 20, "current (x,y):")
{
	attach(next_button);
	attach(quit_button);
	attach(next_x);
	attach(next_y);
	attach(xy_out);
	xy_out.put("no point");
	attach(lines);
}

void Lines_window::next()
{
	int x = next_x.get_int();
	int y = next_y.get_int();

	lines.add(Point(x, y));

	// update current position readout
	ostringstream ss;
	ss << '(' << x << ',' << y << ')';
	xy_out.put(ss.str());

	redraw();
}

void Lines_window::quit()
{
	hide();
}

void Lines_window::cb_next(Address, Address pw)
{
	reference_to<Lines_window>(pw).next();
}

void Lines_window::cb_quit(Address, Address pw)
{
	reference_to<Lines_window>(pw).quit();
}

int main()
try
{
	Lines_window win(Point(100, 100), xmax, ymax, "lines");
	return gui_main();
}
catch (exception& e) {
	cerr << "error: " << e.what() << '\n';
	keep_window_open();
	return 1;
}

catch (...) {
	cerr << "Unknown exception!\n";
	keep_window_open();
	return 2;
}