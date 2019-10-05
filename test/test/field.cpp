#include <iostream>
#include <conio.h>
#include <ctime>
using namespace std;

class Field
{
private:
	const int height = 12;
	const int wight = 5;
	char cell[12][5];

	bool choice = 0;// chip selected?
	// coordinates of the selected chip (in cursor coordinates)
	int prev_y = 0;
	int prev_x = 0;
	// cursor coordinates
	int y = 0;
	int x = 0;

	unsigned int start_time = 0;
	bool is_start = 0;
	unsigned int count_moves = 0;

public:
	char define_symbol(int i) {
		if (i == 0) return 'К';
		if (i == 1) return 'З';
		if (i == 2) return 'С';
	}
	void random_locat() {
		srand(time(0));
		unsigned int red = 5;
		unsigned int green = 5;
		unsigned int blue = 5;

		cell[0][0] = define_symbol(rand() % 3);
		cell[0][1] = '.';
		for (int i = 0; i < 3; i++) {
			if (define_symbol(i) != cell[0][0]) {
				cell[0][2] = define_symbol(i);
				break;
			}
		}
		cell[0][3] = '.';
		for (int i = 0; i < 3; i++) {
			if (define_symbol(i) != cell[0][0] && define_symbol(i) != cell[0][2]) {
				cell[0][4] = define_symbol(i);
				break;
			}
		}

		for (int i = 0; i < 5; i++) cell[1][i] = '-';

		for (int i = 2; i < 11; i += 2) {
			for (int j = 0; j < 5; j++) {
				if (i == 2 && j == 1 || i == 2 && j == 3 || i == 6 && j == 1 || i == 6 && j == 3 || i == 10 && j == 1 || i == 10 && j == 3) {
					cell[i][j] = '&';
					continue;
				}
				if (i == 4 && j == 1 || i == 4 && j == 3 || i == 8 && j == 1 || i == 8 && j == 3) {
					cell[i][j] = '_';
					continue;
				}
				int r;
				while (1) {
					r = rand() % 3;
					if (define_symbol(r) == 'К' && red > 0) {
						red--;
						break;
					}
					else if (define_symbol(r) == 'З' && green > 0) {
						green--;
						break;
					}
					else if (define_symbol(r) == 'С' && blue > 0) {
						blue--;
						break;
					}
				}
				cell[i][j] = define_symbol(r);
			}
		}

		for (int i = 3; i < 12; i += 2) {
			for (int j = 0; j < 5; j++) {
				cell[i][j] = '\'';
			}
		}
		cell[3][0] = '^';
	}

	void start() {
		cout << endl << endl << endl;
		cout << "      -----Добро пожаловать-----" << endl << endl;
		cout << "     Правила очень просты, собери" << endl;
		cout << " три вертикальных ряда соответственно" << endl;
		cout << "       цветам, стоящим над полем" << endl << endl;
		cout << "      Стрелками управляй курсором" << endl;
		cout << "    С помощью Enter - выборай фишки " << endl;
		cout << "        И ходи, пока не победишь" << endl;
		cout << endl << endl << endl << endl << endl;
		cout << "              нажмите любую клавишу...";
	}
	void show_time() {
		if (is_start == 0) {
			start_time = clock();
			is_start = 1;
		}
		cout << " Времени прошло: " << (clock() - start_time) / CLK_TCK << "сек." << endl;
	}
	void show_count_moves() {
		cout << " Ходов сделано: " << count_moves << endl;
	}
	void show_field() {
		show_time();
		show_count_moves();
		cout << endl;
		for (int i = 0; i < height; i++) {
			cout << "               ";
			for (int j = 0; j < wight; j++) {
				cout << cell[i][j] << " ";
			}
			cout << endl;
		}
	}

	// conversion of cursor y-coordinates to chip y-coordinates
	int y_conv(int _y) {
		return _y * 2 + 3;
	}

	bool is_chip() {
		if (cell[y_conv(y) - 1][x] == 'К' || cell[y_conv(y) - 1][x] == 'С' || cell[y_conv(y) - 1][x] == 'З') return 1;
		else return 0;
	}
	bool cell_is_nearby() {
		if (abs(y - prev_y) == 1 && x == prev_x) return 1;
		else if (y == prev_y && abs(x - prev_x) == 1) return 1;
		else return 0;
	}
	bool cell_is_empty() {
		if (cell[y_conv(y) - 1][x] == '_') return 1;
		else return 0;
	}

	void action() {
		int button_code = _getch();
		// down arrow
		if (button_code == 72 && y > 0) {
			cell[y_conv(y)][x] = '\'';
			cell[y_conv(y) - 2][x] = '^';
			y--;
		}
		// up arrow
		else if (button_code == 80 && y < 4) {
			cell[y_conv(y)][x] = '\'';
			cell[y_conv(y) + 2][x] = '^';
			y++;
		}
		// left arrow
		else if (button_code == 75 && x > 0) {
			cell[y_conv(y)][x] = '\'';
			cell[y_conv(y)][x - 1] = '^';
			x--;
		}
		// righ arrow
		else if (button_code == 77 && x < 4) {
			cell[y_conv(y)][x] = '\'';
			cell[y_conv(y)][x + 1] = '^';
			x++;
		}
		// enter - select chip
		else if (button_code == 13 && choice == 0 && is_chip()) {
			prev_y = y;
			prev_x = x;
			choice = 1;
		}
		// enter - deselect chips
		else if (button_code == 13 && choice == 1 && prev_y == y && prev_x == x) {
			choice = 0;
		}
		// enter - move chip
		else if (button_code == 13 && choice == 1 && cell_is_nearby() && cell_is_empty()) {
			cell[y_conv(y) - 1][x] = cell[y_conv(prev_y) - 1][prev_x];
			cell[y_conv(prev_y) - 1][prev_x] = '_';
			cell[y_conv(prev_y)][prev_x] = '\'';
			count_moves++;
			choice = 0;
		}
		// selected chip is marked
		if (choice == 1) {
			cell[y_conv(prev_y)][prev_x] = '^';
		}
	}

	bool win() {
		for (int i = 0; i < 5; i++) {
			if (cell[y_conv(i) - 1][0] != cell[0][0]) return 0;
			if (cell[y_conv(i) - 1][2] != cell[0][2]) return 0;
			if (cell[y_conv(i) - 1][4] != cell[0][4]) return 0;
		}
		cout << endl << "               ПОБЕДА!!!";
		return 1;
	}
};