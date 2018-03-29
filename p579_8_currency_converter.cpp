//  Philipp Siedler
//  Bjarne Stroustrup's PP
//  Chapter 16 Exercise 8

#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"
#include <time.h>

// layout
constexpr int xmax = 400;
constexpr int ymax = 400;

enum currencies
{
	euro = 0, pound
};

struct application_window : Window {
	application_window(Point xy, int w, int h, const string& title, const string& conversion_rate_file_location);

private:

	//------------- DATA

	currencies from;
	currencies to;

	// data
	float conversion_rate;

	// methods
	float get_conversion(const string& crfl);

	//------------- DATA END


	//------------- LAYOUT

	// quit button
	Button quit_button;

	// convert button
	Button convert_button;

	// menu button
	Button from_menu_button;
	Button to_menu_button;
	// menu
	Menu from_menu;
	Menu to_menu;

	// in
	In_box value_in;
	Out_box value_out;
	Out_box value_in_currency;	
	Out_box value_out_currency;

	// actions invoked by callbacks
	void quit() { hide(); }

	void convert();

	void hide_from_menu() { from_menu.hide(); from_menu_button.show(); }
	void hide_to_menu() { to_menu.hide(); to_menu_button.show(); }

	void from_menu_pressed() { from_menu_button.hide(); from_menu.show(); }
	void to_menu_pressed() { to_menu_button.hide(); to_menu.show(); }

	void from_euro_pressed() { hide_from_menu(); value_in_currency.put("euro"); from = euro; }
	void from_pound_pressed() { hide_from_menu(); value_in_currency.put("pound"); from = pound; }

	void to_euro_pressed() { hide_to_menu(); value_out_currency.put("euro"); to = euro; }
	void to_pound_pressed() { hide_to_menu(); value_out_currency.put("pound"); to = pound; }
	
	// callback functions
	static void cb_quit(Address, Address pw) { reference_to<application_window>(pw).quit(); }

	static void cb_convert(Address, Address pw) { reference_to<application_window>(pw).convert(); }

	static void cb_from_menu(Address, Address pw) {	reference_to<application_window>(pw).from_menu_pressed(); }
	static void cb_from_euro(Address, Address pw) { reference_to<application_window>(pw).from_euro_pressed(); }
	static void cb_from_pound(Address, Address pw) { reference_to<application_window>(pw).from_pound_pressed(); }

	static void cb_to_menu(Address, Address pw) { reference_to<application_window>(pw).to_menu_pressed(); }
	static void cb_to_euro(Address, Address pw) { reference_to<application_window>(pw).to_euro_pressed(); }
	static void cb_to_pound(Address, Address pw) { reference_to<application_window>(pw).to_pound_pressed(); }

	//------------- LAYOUT END
};

application_window::application_window(Point xy, int w, int h, const string& title, const string& conversion_rate_file_location)
	: Window(xy, w, h, title)
	, quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit)
	, convert_button(Point(x_max() - 70, 20), 70, 20, "Convert", cb_convert)
	, from_menu_button(Point(0, 20), 70, 20, "From", cb_from_menu)
	, to_menu_button(Point(x_max() / 2, 20), 70, 20, "To", cb_to_menu)
	, from_menu(Point(0, 20), 70, 20, Menu::vertical, "From")
	, to_menu(Point(x_max() / 2, 20), 70, 20, Menu::vertical, "To")
	, value_in(Point(0, 60), 70, 20, "enter value")
	, value_out(Point(x_max() / 2, 60), 70, 20, "")
	, value_in_currency(Point(70, 60), 70, 20, "")
	, value_out_currency(Point(x_max() / 2 + 70, 60), 70, 20, "")
{
	conversion_rate = get_conversion(conversion_rate_file_location);

	from = euro;
	to = euro;

	attach(quit_button);
	attach(convert_button);
	attach(from_menu_button);
	attach(to_menu_button);

	attach(value_in);
	attach(value_out);

	attach(value_in_currency);
	attach(value_out_currency);

	value_in_currency.put("euro");
	value_out_currency.put("euro");

	from_menu.attach(new Button(Point(0, 0), 0, 0, "euro", cb_from_euro));
	from_menu.attach(new Button(Point(0, 0), 0, 0, "pound", cb_from_pound));

	to_menu.attach(new Button(Point(0, 0), 0, 0, "euro", cb_to_euro));
	to_menu.attach(new Button(Point(0, 0), 0, 0, "pound", cb_to_pound));

	attach(to_menu);
	attach(from_menu);

	to_menu.hide();
	from_menu.hide();
}

float application_window::get_conversion(const string& crfl)
{
	string iname = crfl;
	ifstream ist(iname);

	float rate;
	if (!ist) { error("File could not be read: ", iname); }
	else { ist >> rate; }

	return rate;
}

void application_window::convert()
{
	float input = strtof((value_in.get_string()).c_str(), 0);

	if (from == euro && to == pound)
	{
		input *= conversion_rate;

		ostringstream ss;
		ss << input;
		string s(ss.str());

		value_out.put(s);
	}
	else if (from == pound && to == euro)
	{
		input *= 1.0 + (1.0 - conversion_rate);

		ostringstream ss;
		ss << input;
		string s(ss.str());

		value_out.put(s);
	}
	else
	{
		ostringstream ss;
		ss << input;
		string s(ss.str());

		value_out.put(s);
	}
}

int main()
try
{
	application_window win(Point(100, 100), xmax, ymax + 20, "currency converter", "Data/conversion_rate.txt");
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