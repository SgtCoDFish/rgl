#include <ncurses.h>
#include <cstring>

int main() {
	::initscr();
	::raw();
	::noecho();
	const char * msg = "Hello world";

	int row, col;
	row = ::getmaxy(stdscr);
	col = ::getmaxx(stdscr);
	::mvprintw(row/2, (col-std::strlen(msg))/2, msg);

	const int ch = ::getch();
	::mvprintw(row/2 + 1, (col-std::strlen(msg))/2, "Pressed %c", ch);
	::refresh();
	::getch();
	::endwin();
	return 0;
}
