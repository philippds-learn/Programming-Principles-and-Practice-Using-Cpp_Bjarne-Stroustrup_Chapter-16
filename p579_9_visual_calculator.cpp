//	Philipp Siedler
//	Bjarne Stroustrup's PP
//	Chapter 16 Exercise 9

#define _USE_MATH_DEFINES
#include "Simple_window.h"
#include "Graph.h"
#include <cmath>
#include "GUI.h"
#include "std_lib_facilities.h"

// ------------------------------ CALCULATOR ---------------------------

const char let = '#';
const char quitProg = 'Q';
const char print = ';';
const char number = '8';
const char name = 'a';
const char sqroot = 'S';
const char power = 'P';
const char constant = 'C';
const char reset = 'R';
const char help = 'H';

struct Token {
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string val) :kind(ch), name(val) { }
};

class Token_stream {
	bool full;
	Token buffer;
	;
public:
	Token_stream() :full(0), buffer(0) { }
	Token get(stringstream& ist);
	void unget(Token t) { buffer = t; full = true; }
	void ignore(char);
};

Token Token_stream::get(stringstream& ist)
{
	/*
	if (full) {
	full = false;
	return buffer;
	}

	char ch;
	ist >> ch;
	*/

	if (full) {
		full = false; return buffer;
	}
	char ch;
	ist.get(ch);

	while (isspace(ch)) {
		if (ch == '\n') return Token(print); // if newline detected, return print Token
		ist.get(ch);
	}

	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case ';':
	case '=':
	case 'k':
	case ',':
	{
		return Token(ch);
	}
	case '#':
	{
		return Token(let);
	}
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{
		ist.unget();
		double val;
		ist >> val;
		return Token(number, val);
	}
	default:
	{
		if (isalpha(ch) || ch == '_') { //is ch a letter?
			string s;
			s += ch;
			while (ist.get(ch) && (isalpha(ch) || isdigit(ch) || ch == '_')) { //reads chars, strings or digits
				s += ch;
			}
			ist.unget(); //puts the most recently read character back into the stream
			if (s == "quit") return Token(quitProg);
			if (s == "sqrt") return Token(sqroot);
			if (s == "pow") return Token(power);
			if (s == "const") return Token(constant);
			if (s == "reset") return Token(reset);
			if (s == "help" || s == "Help") return Token(help);
			return Token(name, s);
		}
		error("Bad token");
		return Token(' ');
	}
	}
}

void Token_stream::ignore(char c)
{
	if (full && c == buffer.kind) {
		full = false;
		return;
	}
	full = false;
	char ch;
	while (cin >> ch) {
		if (ch == c) return;
	}
}

struct Variable {
	string name;
	double value;
	bool immutable;
	Variable(string n, double v, bool i) :name(n), value(v), immutable(i) { }
};

class Symbol_table {
public:
	vector<Variable> var_table;
	double get(string);
	void set(string, double);
	bool is_declared(string);
	double declare(char, stringstream& ist);
	bool is_immutable(string);
};

Token_stream ts;
double expression(stringstream& ist);
double primary(stringstream& ist);

double Symbol_table::get(string s)
{
	for (int i = 0; i < var_table.size(); ++i) {
		if (var_table[i].name == s) {
			return var_table[i].value;
		}
	}
	error("get: undefined name ", s);
	return 0.0;
}

void Symbol_table::set(string s, double d)
{
	for (int i = 0; i <= var_table.size(); ++i) {
		if (var_table[i].name == s) {
			var_table[i].value = d;
			return;
		}
	}
	error("set: undefined name ", s);
}

bool Symbol_table::is_declared(string s)
{
	for (int i = 0; i < var_table.size(); ++i) {
		if (var_table[i].name == s) return true;
	}
	return false;
}

double Symbol_table::declare(char kind, stringstream& ist)
{
	Token t = ts.get(ist);
	if (t.kind != name) {
		error("name expected in declaration");
	}

	string name = t.name;
	if (kind == let || kind == constant) {
		if (is_declared(name)) error(name, " declared twice");
	}
	else if (kind == reset) {
		if (!is_declared(name))
			error(name, " has not been declared");
		if (is_immutable(name))
			error(name, " is a constant");
	}
	else {
		error("unknown statement");
	}

	Token t2 = ts.get(ist);
	if (t2.kind != '=') error("= missing in declaration of ", name);
	double d = expression(ist);
	if (is_declared(name))
		set(name, d);
	else
		var_table.push_back(Variable(name, d, (kind == constant)));
	return d;
}

bool Symbol_table::is_immutable(string s)
{
	for (int i = 0; i<int(var_table.size()); ++i)
		if (var_table[i].name == s && var_table[i].immutable) return true;
	return false;
}

double pow_function(Token _t, stringstream& ist) {
	_t = ts.get(ist);
	if (_t.kind != '(') {
		error("'(' expected");
	}

	double x = expression(ist);

	_t = ts.get(ist);
	if (_t.kind != ',') {
		error("',' expected");
	}

	double n = expression(ist);
	_t = ts.get(ist);
	if (_t.kind == ')') {
		return pow(x, n);
	}
	else {
		error("Expected ')'");
	}
}

double sqrt_function(Token _t, stringstream& ist) {
	_t = ts.get(ist);
	if (_t.kind != '(') {
		error("'(' expected");
	}
	ts.unget(_t);
	double d = primary(ist);
	if (d < 0.0) {
		error("negative square root");
	}
	return sqrt(d);
}


Symbol_table st;

double primary(stringstream& ist)
{
	Token t = ts.get(ist);
	switch (t.kind) {
	case '(':
	{
		double d = expression(ist);
		t = ts.get(ist);
		if (t.kind != ')') error("')' expected");
		return d;
	}
	case '-':
	{
		return -primary(ist);
	}
	case '+':
	{
		return primary(ist);
	}
	case number:
	{
		return t.value;
	}
	case sqroot:
	{
		return sqrt_function(t, ist);
	}
	case power:
	{
		return pow_function(t, ist);
	}
	case name:
	{
		return st.get(t.name);
	}
	default:
	{
		error("primary expected");
		return 0.0;
	}
	}
}

double term(stringstream& ist)
{
	double left = primary(ist);
	while (true) {
		Token t = ts.get(ist);
		switch (t.kind) {
		case 'k':
		{
			left *= 1000;
			break;
		}
		case '*':
		{
			left *= primary(ist);
			break;
		}
		case '/':
		{
			double d = primary(ist);
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}
		case '%':
		{
			int i1 = narrow_cast<int>(left);
			int i2 = narrow_cast<int>(primary(ist));
			if (i2 == 0) error("%: divide by zero");
			left = i1%i2;
			break;
		}

		default: {
			ts.unget(t);
			return left;
		}
		}
	}
}

double expression(stringstream& ist)
{
	double left = term(ist);
	while (true) {
		Token t = ts.get(ist);
		switch (t.kind) {
		case '+':
		{
			left += term(ist);
			break;
		}
		case '-':
		{
			left -= term(ist);
			break;
		}
		default:
		{
			ts.unget(t);
			return left;
		}
		}
	}
}

double statement(stringstream& ist)
{
	Token t = ts.get(ist);
	double d;
	if (t.kind == let || t.kind == reset || t.kind == constant) {
		d = st.declare(t.kind, ist);
	}
	else {
		ts.unget(t);
		d = expression(ist);
	}
	t = ts.get(ist);
	if (t.kind != print) {
		error("Missing terminator");
	}

	return d;
}

void clean_up_mess()
{
	ts.ignore(print);
}

static void calculate(stringstream& ist, ostringstream& ost)
{
	while (ist) try {
		Token t = ts.get(ist);
		ts.unget(t);
		ost << statement(ist);
		break;
	}
	catch (runtime_error& e) {
		ost << e.what() << endl;
		clean_up_mess();
	}
}

// ------------------------------ CALCULATOR END------------------------

// ------------------------------ INTERFACE ----------------------------

// layout
constexpr int xmax = 400;
constexpr int ymax = 400;

struct application_window : Window {
	application_window(Point xy, int w, int h, const string& title);

private:

	//------------- LAYOUT

	// quit button
	Button quit_button;

	// convert button
	Button calculate_button;

	// in
	In_box value_in;
	Out_box value_out;

	// actions invoked by callbacks
	void quit() { hide(); }
	void cal();

	// callback functions
	static void cb_quit(Address, Address pw) { reference_to<application_window>(pw).quit(); }
	static void cb_calculate(Address, Address pw) { reference_to<application_window>(pw).cal(); }

	//------------- LAYOUT END
};

application_window::application_window(Point xy, int w, int h, const string& title)
	: Window(xy, w, h, title)
	, quit_button(Point(x_max() - 70, 0), 70, 20, "Quit", cb_quit)
	, calculate_button(Point(x_max() - 70, 20), 70, 20, "Calculate", cb_calculate)
	, value_in(Point(70, 60), 70, 20, "equation")
	, value_out(Point(x_max() / 2, 60), 70, 20, "solution")
{
	attach(quit_button);
	attach(calculate_button);
	attach(value_in);
	attach(value_out);
}

void application_window::cal()
{
	ostringstream os;
	stringstream ss;
	ss.str(value_in.get_string() + ";");
	
	calculate(ss, os);

	string s(os.str());
	value_out.put(s);
}

// ------------------------------ INTERFACE END-------------------------

int main()
try {
	application_window win(Point(100, 100), xmax, ymax + 20, "currency converter");
	return gui_main();
}
catch (exception& e) {
	cerr << "exception: " << e.what() << endl;
	char c;
	while (cin >> c&& c != ';');
	return 1;
}
catch (...) {
	cerr << "exception\n";
	char c;
	while (cin >> c && c != ';');
	return 2;
}