//  Philipp Siedler
//  Bjarne Stroustrup's PP
//  Chapter 16 Exercise 2

#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"

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
		// modified event loop
		// handle all events (as per default), but quit when button_pushed becomes true
		// this allows graphics without control inversion
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

	static void cb_next(Address, Address addr) // callback for next_button
											   //	{ reference_to<Simple_window>(addr).next(); }
	{
		static_cast<My_window*>(addr)->next();
	}

	static void cb_quit(Address, Address addr) // callback for quit_button
											   //	{ reference_to<Simple_window>(addr).quit(); }
	{
		static_cast<My_window*>(addr)->quit();
	}

	void next() { next_button_pushed = true; }
	void quit() { hide(); }
};

struct cheker_board_buttons : My_window
{
	cheker_board_buttons(Point xy, int w, int h, const string& title)
		:My_window(xy, w, h, title)
	{
		int x_size = xmax / 4;
		int y_size = ymax / 4;

		// initialize buttons
		buttons.push_back(new Button(Point(0, 20), xmax / 4, ymax / 4, "b0", cb_change_0));
		buttons.push_back(new Button(Point(x_size, 20), xmax / 4, ymax / 4, "b1", cb_change_1));
		buttons.push_back(new Button(Point(x_size * 2, 20), xmax / 4, ymax / 4, "b2", cb_change_2));
		buttons.push_back(new Button(Point(x_size * 3, 20), xmax / 4, ymax / 4, "b3", cb_change_3));
		buttons.push_back(new Button(Point(0, y_size + 20), xmax / 4, ymax / 4, "b4", cb_change_4));
		buttons.push_back(new Button(Point(x_size, y_size + 20), xmax / 4, ymax / 4, "b5", cb_change_5));
		buttons.push_back(new Button(Point(x_size * 2, y_size + 20), xmax / 4, ymax / 4, "b6", cb_change_6));
		buttons.push_back(new Button(Point(x_size * 3, y_size + 20), xmax / 4, ymax / 4, "b7", cb_change_7));
		buttons.push_back(new Button(Point(0, y_size * 2 + 20), xmax / 4, ymax / 4, "b8", cb_change_8));
		buttons.push_back(new Button(Point(x_size, y_size * 2 + 20), xmax / 4, ymax / 4, "b9", cb_change_9));
		buttons.push_back(new Button(Point(x_size * 2, y_size * 2 + 20), xmax / 4, ymax / 4, "b10", cb_change_10));
		buttons.push_back(new Button(Point(x_size * 3, y_size * 2 + 20), xmax / 4, ymax / 4, "b11", cb_change_11));
		buttons.push_back(new Button(Point(0, y_size * 3 + 20), xmax / 4, ymax / 4, "b12", cb_change_12));
		buttons.push_back(new Button(Point(x_size, y_size * 3 + 20), xmax / 4, ymax / 4, "b13", cb_change_13));
		buttons.push_back(new Button(Point(x_size * 2, y_size * 3 + 20), xmax / 4, ymax / 4, "b14", cb_change_14));
		buttons.push_back(new Button(Point(x_size * 3, y_size * 3 + 20), xmax / 4, ymax / 4, "b15", cb_change_15));

		// attach buttons
		for (int i = 0; i < buttons.size(); i++) { attach(buttons[i]); }
	}

	// button setup
	Vector_ref<Button> buttons;
	int last = 16;
	void reset_button() { if(last != 16) { buttons[last].show(); } }	

	// button behaviour
	void change_b0() { reset_button(); last = 0; buttons[0].hide(); };
	void change_b1() { reset_button(); last = 1; buttons[1].hide(); };
	void change_b2() { reset_button(); last = 2; buttons[2].hide(); };
	void change_b3() { reset_button(); last = 3; buttons[3].hide(); };
	void change_b4() { reset_button(); last = 4; buttons[4].hide(); };
	void change_b5() { reset_button(); last = 5; buttons[5].hide(); };
	void change_b6() { reset_button(); last = 6; buttons[6].hide(); };
	void change_b7() { reset_button(); last = 7; buttons[7].hide(); };
	void change_b8() { reset_button(); last = 8; buttons[8].hide(); };
	void change_b9() { reset_button(); last = 9; buttons[9].hide(); };
	void change_b10() { reset_button(); last = 10; buttons[10].hide(); };
	void change_b11() { reset_button(); last = 11; buttons[11].hide(); };
	void change_b12() { reset_button(); last = 12; buttons[12].hide(); };
	void change_b13() { reset_button(); last = 13; buttons[13].hide(); };
	void change_b14() { reset_button(); last = 14; buttons[14].hide(); };
	void change_b15() { reset_button(); last = 15; buttons[15].hide(); };

private:
	vector<bool> pressed;

	// call back functions
	static void cb_change_0(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b0(); }
	static void cb_change_1(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b1(); }
	static void cb_change_2(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b2(); }
	static void cb_change_3(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b3(); }
	static void cb_change_4(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b4(); }
	static void cb_change_5(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b5(); }
	static void cb_change_6(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b6(); }
	static void cb_change_7(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b7(); }
	static void cb_change_8(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b8(); }
	static void cb_change_9(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b9(); }
	static void cb_change_10(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b10(); }
	static void cb_change_11(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b11(); }
	static void cb_change_12(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b12(); }
	static void cb_change_13(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b13(); }
	static void cb_change_14(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b14(); }
	static void cb_change_15(Address, Address addr) { reference_to<cheker_board_buttons>(addr).change_b15(); }
};

int main()
try
{
	cheker_board_buttons win(Point(100, 100), xmax, ymax + 20, "checker board buttons");
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