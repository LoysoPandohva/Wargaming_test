#include <iostream>
#include "field.cpp"
using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	system("MODE CON COLS=39 LINES=18");
	Field f;
	f.random_locat();
	f.start();
	_getch();
	system("cls");
	while (1) {
		f.show_field();
		if (f.win()) break;
		f.action();
		system("cls");
	}
	_getch();
}