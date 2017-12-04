//  Philipp Siedler
//  Bjarne Stroustrup's PP
//  Chapter 16 Drill 4

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

	// color menu
	Menu color_menu;
	Button color_menu_button;
	void change_color(Color c) { lines.set_color(c); }
	void hide_color_menu() { color_menu.hide(); color_menu_button.show(); }

	// line style menu
	Menu line_style_menu;
	Button line_style_menu_button;
	void change_line_style(Line_style ls) { lines.set_style(ls); }
	void hide_line_style_menu() { line_style_menu.hide(); line_style_menu_button.show(); }

	// actions invoked by callbacks
	// color menu
	void red_pressed() { change_color(Color::red); hide_color_menu(); }
	void blue_pressed() { change_color(Color::blue); hide_color_menu(); }
	void black_pressed() { change_color(Color::black); hide_color_menu(); }
	void color_menu_pressed() { color_menu_button.hide(); color_menu.show(); }

	// line style menu
	void solid_pressed() { change_line_style(Line_style::solid); hide_line_style_menu(); }
	void dashed_pressed() { change_line_style(Line_style::dash); hide_line_style_menu(); }
	void dotted_pressed() { change_line_style(Line_style::dot); hide_line_style_menu(); }
	void line_style_menu_pressed() { line_style_menu_button.hide(); line_style_menu.show(); }

	void next();
	void quit();

	// callback functions
	// color
	static void cb_red(Address, Address);
	static void cb_blue(Address, Address);
	static void cb_black(Address, Address);
	static void cb_color_menu(Address, Address);

	// line style
	static void cb_solid(Address, Address);
	static void cb_dash(Address, Address);
	static void cb_dot(Address, Address);
	static void cb_line_style_menu(Address, Address);

	static void cb_next(Address, Address);
	static void cb_quit(Address, Address);
};

Lines_window::Lines_window(Point xy, int w, int h, const string& title)
	:Window(xy, w, h, title),
	next_button(Point(x_max() - 150, 0), 70, 20, "Next point", cb_next),
	quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit),
	next_x(Point(x_max() - 310, 0), 50, 20, "next x:"),
	next_y(Point(x_max() - 210, 0), 50, 20, "next y:"),
	xy_out(Point(100, 0), 100, 20, "current (x,y):"),
	color_menu(Point(x_max() - 70, 30), 70, 20, Menu::vertical, "color"),
	color_menu_button(Point(x_max() - 80, 30), 80, 20, "color menu", cb_color_menu),
	line_style_menu(Point(x_max() - 70, 90), 70, 20, Menu::vertical, "line style"),
	line_style_menu_button(Point(x_max() - 80, 90), 80, 20, "style menu", cb_line_style_menu)
{
	attach(next_button);
	attach(quit_button);
	attach(next_x);
	attach(next_y);
	attach(xy_out);

	// initialize out box and line color
	xy_out.put("no point");
	lines.set_color(Color::black);

	color_menu.attach(new Button(Point(0, 0), 0, 0, "red", cb_red));
	color_menu.attach(new Button(Point(0, 0), 0, 0, "blue", cb_blue));
	color_menu.attach(new Button(Point(0, 0), 0, 0, "black", cb_black));

	line_style_menu.attach(new Button(Point(0, 0), 0, 0, "solid", cb_solid));
	line_style_menu.attach(new Button(Point(0, 0), 0, 0, "dashed", cb_dash));
	line_style_menu.attach(new Button(Point(0, 0), 0, 0, "dotted", cb_dot));

	attach(color_menu);
	attach(line_style_menu);

	color_menu.hide();
	line_style_menu.hide();

	attach(line_style_menu_button);
	attach(color_menu_button);
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

// color callback
void Lines_window::cb_red(Address, Address pw)
{
	reference_to<Lines_window>(pw).red_pressed();
}

void Lines_window::cb_blue(Address, Address pw)
{
	reference_to<Lines_window>(pw).blue_pressed();
}

void Lines_window::cb_black(Address, Address pw)
{
	reference_to<Lines_window>(pw).black_pressed();
}

void Lines_window::cb_color_menu(Address, Address pw)
{
	reference_to<Lines_window>(pw).color_menu_pressed();
}

// line style callback
void Lines_window::cb_solid(Address, Address pw)
{
	reference_to<Lines_window>(pw).solid_pressed();
}

void Lines_window::cb_dash(Address, Address pw)
{
	reference_to<Lines_window>(pw).dashed_pressed();
}

void Lines_window::cb_dot(Address, Address pw)
{
	reference_to<Lines_window>(pw).dotted_pressed();
}

void Lines_window::cb_line_style_menu(Address, Address pw)
{
	reference_to<Lines_window>(pw).line_style_menu_pressed();
}

// next + quit callback
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