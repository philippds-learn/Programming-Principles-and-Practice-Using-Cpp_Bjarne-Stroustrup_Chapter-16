//  Philipp Siedler
//  Bjarne Stroustrup's PP
//  Chapter 16 Exercise 4

#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"

// layout
constexpr int xmax = 600;
constexpr int ymax = 400;

// QUAD
struct Quad_center : Closed_polyline
{
	Quad_center(Point center, int w, int h)
	{
		add(Point(center.x - w / 2, center.y - h / 2));
		add(Point(center.x + w / 2, center.y - h / 2));
		add(Point(center.x + w / 2, center.y + h / 2));
		add(Point(center.x - w / 2, center.y + h / 2));
		
	}

	void draw_lines() const;
};

void Quad_center::draw_lines() const
{
	fl_color(FL_BLACK);
	Closed_polyline::draw_lines();
}

// TRIANGLE
struct Triangle : Closed_polyline
{
	Triangle(Point origin, int edge_length)
	{
		int h = sqrt(3) / 2 * edge_length;

		Point p(origin.x - edge_length / 2, origin.y + edge_length / 2);
		add(p);		
		add(Point(p.x + edge_length, p.y));
		add(Point(p.x + edge_length / 2, p.y - h));
	}

	void draw_lines() const;
};

void Triangle::draw_lines() const
{
	fl_color(FL_BLACK);
	Closed_polyline::draw_lines();
}

// HEXAGON
struct Hexagon : Closed_polyline
{
	Hexagon(Point origin, int radius)
	{
		int n = 6;
		for (int i = 0; i < n; i++) {
			add(Point(origin.x + radius * cos(2 * M_PI * i / n), origin.y + radius * sin(2 * M_PI * i / n)));
		}
	}

	void draw_lines() const;
};

void Hexagon::draw_lines() const
{
	fl_color(FL_BLACK);
	Closed_polyline::draw_lines();
}

// DRAW SHAPES WINDOW
struct Shapes_window : Window {
	Shapes_window(Point xy, int w, int h, const string& title);
private:
	// data
	enum Shapes
	{
		circle, square, triangle , hexagon
	};

	Vector_ref<Shape> s;

	// widgets
	Button quit_button;	// end program
	In_box draw_pos_x;
	In_box draw_pos_y;
	Out_box xy_out;
	Menu shape_menu;
	Button menu_button;

	void hide_menu() { shape_menu.hide(); menu_button.show(); }

	// actions invoked by callbacks
	void menu_pressed() { menu_button.hide(); shape_menu.show(); }

	void draw_shape(Shapes s);
	void draw_circle();
	void draw_square();
	void draw_triangle();
	void draw_hexagon();

	void quit();

	// callback functions
	static void cb_circle(Address, Address);
	static void cb_square(Address, Address);
	static void cb_triangle(Address, Address);
	static void cb_hexagon(Address, Address);
	static void cb_menu(Address, Address);
	static void cb_quit(Address, Address);
};

Shapes_window::Shapes_window(Point xy, int w, int h, const string& title)
	:Window(xy, w, h, title),
	quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit),
	draw_pos_x(Point(x_max() - 310, 0), 50, 20, "pos x:"),
	draw_pos_y(Point(x_max() - 210, 0), 50, 20, "pos y:"),
	xy_out(Point(100, 0), 100, 20, "drawn at (x,y):"),
	shape_menu(Point(x_max() - 70, 30), 70, 20, Menu::vertical, "shape"),
	menu_button(Point(x_max() - 80, 30), 80, 20, "shape menu", cb_menu)
{
	attach(quit_button);
	attach(draw_pos_x);
	attach(draw_pos_y);
	attach(xy_out);

	xy_out.put("no shape");

	shape_menu.attach(new Button(Point(0, 0), 0, 0, "circle", cb_circle));
	shape_menu.attach(new Button(Point(0, 0), 0, 0, "square", cb_square));
	shape_menu.attach(new Button(Point(0, 0), 0, 0, "triangle", cb_triangle));
	shape_menu.attach(new Button(Point(0, 0), 0, 0, "hexagon", cb_hexagon));
	
	attach(shape_menu);
	shape_menu.hide();
	attach(menu_button);
}

void Shapes_window::draw_shape(Shapes shape)
{
	int x = draw_pos_x.get_int();
	int y = draw_pos_y.get_int();

	switch (shape)
	{
	case circle: s.push_back(new Circle(Point(x, y), 10));
	case square: s.push_back(new Quad_center(Point(x,y), 20, 20));
	case triangle: s.push_back(new Triangle(Point(x, y), 20));
	case hexagon: s.push_back(new Hexagon(Point(x, y), 10));
	}
	
	// update current position readout
	ostringstream ss;
	ss << '(' << x << ',' << y << ')';
	xy_out.put(ss.str());

	attach(s[s.size() - 1]);
	redraw();
	hide_menu();	
}

void Shapes_window::draw_circle()
{
	int x = draw_pos_x.get_int();
	int y = draw_pos_y.get_int();

	s.push_back(new Circle(Point(x, y), 25));

	// update current position readout
	ostringstream ss;
	ss << '(' << x << ',' << y << ')';
	xy_out.put(ss.str());

	attach(s[s.size() - 1]);
	redraw();
	hide_menu();
}

void Shapes_window::draw_square()
{
	int x = draw_pos_x.get_int();
	int y = draw_pos_y.get_int();

	s.push_back(new Quad_center(Point(x, y), 50, 50));

	// update current position readout
	ostringstream ss;
	ss << '(' << x << ',' << y << ')';
	xy_out.put(ss.str());

	attach(s[s.size() - 1]);
	redraw();
	hide_menu();
}

void Shapes_window::draw_triangle()
{
	int x = draw_pos_x.get_int();
	int y = draw_pos_y.get_int();

	s.push_back(new Triangle(Point(x, y), 50));

	// update current position readout
	ostringstream ss;
	ss << '(' << x << ',' << y << ')';
	xy_out.put(ss.str());

	attach(s[s.size() - 1]);
	redraw();
	hide_menu();
}

void Shapes_window::draw_hexagon()
{
	int x = draw_pos_x.get_int();
	int y = draw_pos_y.get_int();

	s.push_back(new Hexagon(Point(x, y), 20));

	// update current position readout
	ostringstream ss;
	ss << '(' << x << ',' << y << ')';
	xy_out.put(ss.str());

	attach(s[s.size() - 1]);
	redraw();
	hide_menu();
}

void Shapes_window::quit()
{
	hide();
}

void Shapes_window::cb_circle(Address, Address pw)
{
	reference_to<Shapes_window>(pw).draw_shape(circle)();
}

void Shapes_window::cb_square(Address, Address pw)
{
	reference_to<Shapes_window>(pw).draw_square();
}

void Shapes_window::cb_triangle(Address, Address pw)
{
	reference_to<Shapes_window>(pw).draw_triangle();
}

void Shapes_window::cb_hexagon(Address, Address pw)
{
	reference_to<Shapes_window>(pw).draw_hexagon();
}

void Shapes_window::cb_menu(Address, Address pw)
{
	reference_to<Shapes_window>(pw).menu_pressed();
}

void Shapes_window::cb_quit(Address, Address pw)
{
	reference_to<Shapes_window>(pw).quit();
}

int main()
try
{
	Shapes_window win(Point(100, 100), xmax, ymax, "lines");
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