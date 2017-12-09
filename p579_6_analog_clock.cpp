//  Philipp Siedler
//  Bjarne Stroustrup's PP
//  Chapter 16 Exercise 6

#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"

// layout
constexpr int xmax = 400;
constexpr int ymax = 400;

struct Analog_clock_window : Window {
	Analog_clock_window(Point xy, int w, int h, const string& title)
		: Window(xy, w, h, title),
		quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit)
	{
		attach(quit_button);
	}

	Button quit_button;

private:

	static void cb_quit(Address, Address addr)
	{
		reference_to<Analog_clock_window>(addr).quit();
	}

	void quit() { hide(); }
};

int main()
try
{
	Analog_clock_window win(Point(100, 100), xmax, ymax + 20, "analog clock");
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