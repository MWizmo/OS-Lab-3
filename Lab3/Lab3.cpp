#include "stdafx.h"
#include <iostream>
#include <thread>
#include <Windows.h>
#include <conio.h>
#include <stack>
#include <mutex>

using namespace std;

stack <int> x;
mutex mut;
bool is_something_changed = false;

class Producer {
private:
	int speed=2;
	bool condition=false;
public:
	void Info() {
		HANDLE hCon; COORD cPos;
		hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		cPos.Y = 7;
		cPos.X = 0;
		SetConsoleCursorPosition(hCon, cPos);
		cout << "Скорость работы производителя:                                                                   ";
		cPos.Y = 7;
		SetConsoleCursorPosition(hCon, cPos);
		cout << "Скорость работы производителя: " << speed << "   Статус производителя: ";
		condition ? cout << "работает" : cout << "остановлен";
	}
	void Turn_Off() {
		condition = false;
		is_something_changed = true;
	}
	void Turn_On() {
		condition = true;
		is_something_changed = true;
	}
	void Change_Speed() {
		//Turn_Off();
		int _speed;
		system("cls");
		cout << "Введите необходимую скорость работы(от 0 до 10 секунд)>>";
		cin >> _speed;
		speed = _speed;
		is_something_changed = true;
		system("cls");
		//condition = true;
		Menu();
	}
	void Work() {
		while (true) {
			if (condition) {
				char keypress;
				if (_kbhit()) {
					keypress = _getch();
					switch (keypress) {
					case '1': {Change_Speed(); } break;
					case '3': {condition = true; is_something_changed = true; } break;
					case '4': {condition = false; is_something_changed = true; } break;
					case '7': exit(0);
					default: {} break;
					}
				}
				mut.lock();
				while (x.size() == 10) {
					if (_kbhit()) {
						keypress = _getch();
						switch (keypress) {
						case '1': {Change_Speed(); } break;
						case '3': {condition = true; is_something_changed = true; } break;
						case '4': {condition = false; is_something_changed = true; } break;
						case '7': exit(0);
						default: {} break;
						}
					}
					HANDLE hCon; COORD cPos;
					hCon = GetStdHandle(STD_OUTPUT_HANDLE);
					cPos.Y = 11;
					SetConsoleCursorPosition(hCon, cPos);
					cout << "1! ";
					mut.unlock();
					this_thread::sleep_for(chrono::seconds(1));
					mut.lock();
				}
				HANDLE hCon; COORD cPos;
				hCon = GetStdHandle(STD_OUTPUT_HANDLE);
				cPos.Y = 11;
				SetConsoleCursorPosition(hCon, cPos);
				cout << "1 ";
				x.push(1);
				is_something_changed = true;
				mut.unlock();
				this_thread::sleep_for(chrono::seconds(speed));
			}
		}
	}
};

class Consumer {
private:
	int speed=3;
	bool condition=false;
public:
	void Info() {
		HANDLE hCon; COORD cPos;
		hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		cPos.Y = 8;
		cPos.X = 0;
		SetConsoleCursorPosition(hCon, cPos);
		cout << "Скорость работы потребителя:                                                                       ";
		cPos.Y = 8;
		SetConsoleCursorPosition(hCon, cPos);
		cout << "Скорость работы потребителя: " << speed << "   Статус потребителя: ";
		condition ? cout << "работает" : cout << "остановлен";
	}
	void Turn_Off() {
		condition = false;
		is_something_changed = true;
	}
	void Turn_On() {
		condition = true; is_something_changed = true;
	}
	void Change_Speed() {
		//condition = false;
		int _speed;
		system("cls");
		cout << "Введите необходимую скорость работы(от 0 до 10 секунд)>>";
		cin >> _speed;
		speed = _speed;
		is_something_changed = true;
		system("cls");
		//condition = true;
		Menu();
	}
	void Work() {
		while (true) {
			if (condition) {
				char keypress;
				if (_kbhit()) {
					keypress = _getch();
					switch (keypress) {
					//case'1':{}
					case '2': {Change_Speed(); }break;
					case '5': {condition = true; is_something_changed = true; } break;
					case '6': {condition = false; is_something_changed = true; } break;
					case '7': exit(0);
					default: {} break;
					}
				}
				mut.lock();
				while (x.empty()) {
					if (_kbhit()) {
						keypress = _getch();
						switch (keypress) {
						case '1':{}
						case '2': {Change_Speed(); }break;
						case '5': {condition = true; is_something_changed = true; } break;
						case '6': {condition = false; is_something_changed = true; } break;
						case '7': exit(0);
						default: {} break;
						}
					}
					HANDLE hCon; COORD cPos;
					hCon = GetStdHandle(STD_OUTPUT_HANDLE);
					cPos.Y = 11;
					SetConsoleCursorPosition(hCon, cPos);
					cout << "2! ";
					mut.unlock();
					this_thread::sleep_for(chrono::seconds(1));
					mut.lock();
				}
				HANDLE hCon; COORD cPos;
				hCon = GetStdHandle(STD_OUTPUT_HANDLE);
				cPos.Y = 11;
				SetConsoleCursorPosition(hCon, cPos);
				cout << "2 ";
				x.pop();
				is_something_changed = true;
				mut.unlock();
				this_thread::sleep_for(chrono::seconds(speed));
			}
		}
	}
};

void HideCursor() {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void Info(Consumer cons, Producer prod) {
	HANDLE hCon; COORD cPos;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	cPos.Y = 6;
	cPos.X = 0;
	SetConsoleCursorPosition(hCon, cPos);
	cout << "Занятость стека:             ";
	cPos.Y = 6;
	SetConsoleCursorPosition(hCon, cPos);
	cout << "Занятость стека: " << x.size() << "/10\n";
	prod.Info();
	cons.Info();
	cout << endl;
	is_something_changed = false;
}

void Menu() {
	Consumer Cons;
	Producer Prod;
	cout << "1)Установить скорость для производителя  2)Установить скорость для потребителя\n3)Запустить производителя                4)Приостановить производителя\n5)Запустить потребителя                  6)Приостановить работу потребителя\n7)Выйти из программы\n";
	Info(Cons, Prod);
	HANDLE hCon; COORD cPos;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	char keypress;
	thread thr1([&Cons]() {Cons.Work(); });
	thread thr2([&Prod]() {Prod.Work(); });
	cPos.Y = 12;
	cPos.X = 0;
	SetConsoleCursorPosition(hCon, cPos);
	for (;;) {
		if (is_something_changed) {
			Info(Cons, Prod);
			is_something_changed = false;
		}
		if (_kbhit()) {
			keypress = _getch();
			switch (keypress) {
			case '1': {Prod.Change_Speed(); } break;
			case '2': {Cons.Change_Speed(); }break;
			case '3': {Prod.Turn_On(); } break;
			case '4': {Prod.Turn_Off(); } break;
			case '5': {Cons.Turn_On(); } break;
			case '6': {Cons.Turn_Off(); } break;
			case '7': exit(0);
			default: {} break;
			}
		}
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	HideCursor();
	Menu();
	system("pause");
    return 0;
}




