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
		reference_to<My_window>(addr).next();
	}

	static void cb_quit(Address, Address addr)
	{
		reference_to<My_window>(addr).quit();
	}

	void next() { next_button_pushed = true; }
	void quit() { hide(); }
};

struct Random_button_win : My_window
{
	Random_button_win(Point xy, int w, int h, const string& title, const string& url)
		:My_window(xy, w, h, title),
		url(url),
		rand_button(Point(xmax/2, ymax/2), 40, 40, "", cb_rand),
		button_image(Point(xmax / 2, ymax / 2), url)
	{
		attach(rand_button);
		button_image.set_mask(Point(xmax / 2, ymax / 2), 40, 40);
		attach(button_image);
	}
		
	Button rand_button;
	Image button_image;
	
private:
	string url;

	static void cb_rand(Address, Address addr)
	{
		static_cast<Random_button_win*>(addr)->change_button_pos();
	}

	void change_button_pos()
	{
		rand_button.hide();

		int current_x = rand_button.loc.x;
		int current_y = rand_button.loc.y;

		int new_x = rand_int(0, xmax - 40) - current_x;
		int new_y = rand_int(20, ymax - 40) - current_y;
		
		rand_button.move(new_x, new_y);
		button_image.move(new_x, new_y);
		redraw();
	}
};

int main()
try
{
	Random_button_win win(Point(100, 100), xmax, ymax + 20, "random buttons window", "Data/supercell.jpg");
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