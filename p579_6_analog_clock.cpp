//  Philipp Siedler
//  Bjarne Stroustrup's PP
//  Chapter 16 Exercise 6

#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"
#include <time.h>

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
	void get_time();
	void draw_hands();


	static void cb_quit(Address, Address addr)
	{
		reference_to<Analog_clock_window>(addr).quit();
	}

	void quit() { hide(); }
};

void Analog_clock_window::get_time()
{
	#pragma warning(disable : 4996)

	time_t rawtime;
	struct tm * timeinfo;
	char buffer[80];

	time(&rawtime);
	timeinfo = localtime(&rawtime);

	strftime(buffer, sizeof(buffer), "%d-%m-%Y %I:%M:%S", timeinfo);
	std::string str(buffer);
	std::cout << str << endl;
	cout << timeinfo->tm_sec << endl;
}

void Analog_clock_window::draw_hands()
{
	int seconds;
	while (true) {
		if (clock() / CLOCKS_PER_SEC == seconds) {
			seconds++;

		}
	}
}

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