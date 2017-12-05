//  Philipp Siedler
//  Bjarne Stroustrup's PP
//  Chapter 16 Exercise 3

#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"
#include <random>

inline int rand_int(int min, int max)
{
	static default_random_engine ran;
	return uniform_int_distribution<>(min, max)(ran);
}

// layout
constexpr int xmax = 400;
constexpr int ymax = 400;

struct My_window : Window {
	My_window(Point xy, int w, int h, const string& title)
		: Window(xy, w, h, title),
		next_button_pushed(false),
		next_button(Point(x_max() - 140, 0), 70, 20, "Next", cb_next),
		quit_button_pushed(false),
		quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit)
	{
		attach(next_button);
		attach(quit_button);
	}

	void wait_for_button()
	{
		while (!next_button_pushed) Fl::wait();
		next_button_pushed = false;
		Fl::redraw();
	}

	Button next_button;
	Button quit_button;

private:
	bool next_button_pushed;
	bool quit_button_pushed;

	static void cb_next(Address, Address addr)
	{
		static_cast<My_window*>(addr)->next();
	}

	static void cb_quit(Address, Address addr)
	{
		static_cast<My_window*>(addr)->quit();
	}

	void next() { next_button_pushed = true; }
	void quit() { hide(); }
};



int main()
try
{
	//cheker_board_buttons win(Point(100, 100), xmax, ymax + 20, "checker board buttons");
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