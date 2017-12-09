//  Philipp Siedler
//  Bjarne Stroustrup's PP
//  Chapter 16 Exercise 5

#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"

// layout
constexpr int xmax = 400;
constexpr int ymax = 400;

struct My_window : Window {
	My_window(Point xy, int w, int h, const string& title, string url)
		: Window(xy, w, h, title),
		next_button_pushed(false),
		next_button(Point(x_max() - 140, 0), 70, 20, "Next", cb_next),
		quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit),
		url(url),
		counter(0)
	{
		attach(next_button);
		attach(quit_button);

		// open input stream
		ifstream ist(url);
		int input_x;
		char comma;
		int input_y;

		// store data in vectors
		while (ist >> input_x >> comma >> input_y)
		{
			in_x.push_back(input_x);
			in_y.push_back(input_y);
		}
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
	string url;
	int counter;
	Vector_ref<Shape> s;

	vector<int> in_x;
	vector<int> in_y;

	static void cb_next(Address, Address addr)
	{
		reference_to<My_window>(addr).next();
	}

	static void cb_quit(Address, Address addr)
	{
		reference_to<My_window>(addr).quit();
	}

	void next();
	void quit() { hide(); }
};

void My_window::next()
{
	if (s.size() < in_x.size())
	{
		if (s.size() != 0) { detach(s[s.size() - 1]); }
		s.push_back(new Circle(Point(in_x[counter], in_y[counter]), 20));
		attach(s[s.size() - 1]);
		redraw();
		counter++;
	}
}

int main()
try
{
	My_window win(Point(100, 100), xmax, ymax + 20, "next buttons window", "Data/point_sequence.txt");
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