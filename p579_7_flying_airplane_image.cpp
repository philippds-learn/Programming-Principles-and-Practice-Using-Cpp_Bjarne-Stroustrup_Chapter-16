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
struct Airplane : Shape
{
	Airplane(string content_location, Point& position, int canvas_width, int canvas_height)
		: content_location(content_location)
		, position(position)
		, canvas_width(canvas_width)
		, canvas_height(canvas_height)
	{
		image_width = 250;
		image_height = 150;
		velocity = Point(1, 1);
	}

	void move();
	void draw_lines() const;

	string content_location;
	Point position;

	Point velocity;

	int image_width;
	int image_height;

	int canvas_width;
	int canvas_height;
};

void Airplane::move()
{
	

	if (position.x <= canvas_width - image_width)
	{
		position.x += velocity.x;
	}
	if (position.y <= canvas_height - image_height)
	{
		position.y += velocity.y;
	}

	if (position.x == canvas_width - image_width || position.x == 0)
	{
		velocity.x *= -1;
	}
	if (position.y == canvas_height - image_height || position.y == 0)
	{
		velocity.y *= -1;
	}

}

void Airplane::draw_lines() const
{
	Image image0(position, content_location);
	image0.set_mask(Point(100, 70), image_width, image_height);

	if (color().visibility()) {		
		// draw content image
		image0.draw();
	}
}

struct animation_window : Window {
	animation_window(Point xy, int w, int h, const string& title)
		: Window(xy, w, h, title),
		quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit)
	{
		attach(quit_button);
		//position = Point(0, 0);
		Airplane plane("Data/airplanes-work-1.jpg", Point(0, 0), w, h);

		while (true) {
			Fl::wait();
			Sleep(10);	
			draw_shape(plane);
			plane.move();
			Fl::redraw();
		}
	}

	Vector_ref<Shape> s;
	void move_next(Point& pos);
	void draw_shape(Airplane& plane);

	Button quit_button;

private:

	//Point position;

	static void cb_quit(Address, Address addr) { reference_to<animation_window>(addr).quit(); }
	void quit() { hide(); }
};

void animation_window::draw_shape(Airplane& plane)
{
	if (s.size() != 0)
	{
		for (int i = 0; i < s.size(); i++)
		{
			detach(s[i]);
		}
	}

	s.push_back(plane);
	attach(s[s.size() - 1]);
}

int main()
try
{
	animation_window win(Point(100, 100), xmax, ymax + 20, "flying airplane");

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