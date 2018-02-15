//  Philipp Siedler
//  Bjarne Stroustrup's PP
//  Chapter 16 Exercise 7

#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"
#include <time.h>

// layout
constexpr int xmax = 400;
constexpr int ymax = 400;

// HANDS
struct Hands : Open_polyline
{
	Hands(Point center, Point end, Fl_Color color)
	{
		add(Point(center));
		add(Point(end));
		c = color;
	}

	void draw_lines() const;
	Fl_Color c;
};

void Hands::draw_lines() const
{
	fl_color(c);
	Open_polyline::draw_lines();
}

struct Analog_clock_window : Window {
	Analog_clock_window(Point xy, int w, int h, const string& title)
		: Window(xy, w, h, title),
		quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit)
	{
		attach(quit_button);

		while (true) {
			Fl::wait();
			Sleep(1000);
			cout << "sleep" << endl;
			draw_shape();
			Fl::redraw();
		}
	}

	Vector_ref<Shape> s;
	void draw_shape();

	Button quit_button;

private:
	time_t rawtime;
	bool button_pushed;

	void print_current_time(time_t &rt);

	static void cb_quit(Address, Address addr) { reference_to<Analog_clock_window>(addr).quit(); }
	void quit() { hide(); }
};

void Analog_clock_window::print_current_time(time_t &rt)
{
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

	struct tm * timeinfo;
	timeinfo = localtime(&rt);
	printf("Current local time and date: %s", asctime(timeinfo));

	cout << timeinfo->tm_sec << endl;
}

void Analog_clock_window::draw_shape()
{
	if (s.size() != 0)
	{
		for (int i = 0; i < s.size(); i++)
		{
			detach(s[i]);
		}
	}

	Point center(xmax / 2, ymax / 2);

	for (int i = 0; i < 12; i++)
	{
		Point pt1(0, -100);
		Point pt2(0, -95);
		double rad = ((2 * M_PI) / 12) * i;

		Point rotated_pt1 = Point(cos(rad) * pt1.x - sin(rad) * pt1.y, sin(rad) * pt1.x + cos(rad) * pt1.y);
		Point rotated_pt2 = Point(cos(rad) * pt2.x - sin(rad) * pt2.y, sin(rad) * pt2.x + cos(rad) * pt2.y);

		s.push_back(new Hands(Point(rotated_pt1.x + center.x, rotated_pt1.y + center.y), Point(rotated_pt2.x + center.x, rotated_pt2.y + center.y), Color::black));
		attach(s[s.size() - 1]);
	}

#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
	time_t rawT;
	time(&rawT);

	struct tm * timeinfo;
	timeinfo = localtime(&rawT);
	asctime(timeinfo);

	printf("Current local time and date: %s", asctime(timeinfo));

	// SECOND HAND
	Point origin_Pt1(0, -100);

	double rad = timeinfo->tm_sec * ((2 * M_PI) / 60);
	Point rotated_Pt1 = Point(cos(rad) * origin_Pt1.x - sin(rad) * origin_Pt1.y, sin(rad) * origin_Pt1.x + cos(rad) * origin_Pt1.y);

	s.push_back(new Hands(center, Point(rotated_Pt1.x + center.x, rotated_Pt1.y + center.y), Color::red));
	attach(s[s.size() - 1]);

	// MINUTE HAND
	Point origin_Pt2(0, -80);

	rad = timeinfo->tm_min * ((2 * M_PI) / 60);
	Point rotated_Pt2 = Point(cos(rad) * origin_Pt2.x - sin(rad) * origin_Pt2.y, sin(rad) * origin_Pt2.x + cos(rad) * origin_Pt2.y);

	s.push_back(new Hands(center, Point(rotated_Pt2.x + center.x, rotated_Pt2.y + center.y), Color::blue));
	attach(s[s.size() - 1]);

	// HOUR HAND
	Point origin_Pt3(0, -60);

	rad = timeinfo->tm_hour * ((2 * M_PI) / 12) + (timeinfo->tm_min * ((2 * M_PI) / 60)) / 12;
	Point rotated_Pt3 = Point(cos(rad) * origin_Pt3.x - sin(rad) * origin_Pt3.y, sin(rad) * origin_Pt3.x + cos(rad) * origin_Pt3.y);

	s.push_back(new Hands(center, Point(rotated_Pt3.x + center.x, rotated_Pt3.y + center.y), Color::black));
	attach(s[s.size() - 1]);
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