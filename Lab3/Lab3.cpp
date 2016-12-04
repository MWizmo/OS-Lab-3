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

class Producer {
private:
	int speed=1500;
	bool condition=true;
public:
	void Turn_Off() {
		condition = false;
	}
	void Turn_On() {
		condition = true;
	}
	void Change_Speed() {
		int _speed;
		system("cls");
		cout << "Введите необходимую скорость работы(от 0 до 10 секунд)>>";
		cin >> _speed;
		speed = _speed;
		system("cls");
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
					case '3': {condition = true; } break;
					case '4': {condition = false; } break;
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
						case '3': {condition = true; } break;
						case '4': {condition = false; } break;
						case '7': exit(0);
						default: {} break;
						}
					}
					cout << "1! ";
					mut.unlock();
					this_thread::sleep_for(chrono::milliseconds(1000));
					mut.lock();
				}
				cout << "1 ";
				x.push(1);
				mut.unlock();
				this_thread::sleep_for(chrono::milliseconds(speed));
			}
		}
	}
};

class Consumer {
private:
	int speed=2000;
	bool condition=true;
public:
	void Turn_Off() {
		condition = false;
	}
	void Turn_On() {
		condition = true;
	}
	void Change_Speed() {
		int _speed;
		system("cls");
		cout << "Введите необходимую скорость работы(от 0 до 10 секунд)>>";
		cin >> _speed;
		speed = _speed;
		system("cls");
		Menu();
	}
	void Work() {
		while (true) {
			if (condition) {
				char keypress;
				if (_kbhit()) {
					keypress = _getch();
					switch (keypress) {
					case '2': {Change_Speed(); }break;
					case '5': {condition = true; } break;
					case '6': {condition = false; } break;
					case '7': exit(0);
					default: {} break;
					}
				}
				mut.lock();
				while (x.empty()) {
					if (_kbhit()) {
						keypress = _getch();
						switch (keypress) {
						case '2': {Change_Speed(); }break;
						case '5': {condition = true; } break;
						case '6': {condition = false; } break;
						case '7': exit(0);
						default: {} break;
						}
					}
					cout << "2! ";
					mut.unlock();
					this_thread::sleep_for(chrono::milliseconds(1000));
					mut.lock();
				}
				cout << "2 ";
				x.pop();
				mut.unlock();
				this_thread::sleep_for(chrono::milliseconds(speed));
			}
		}
	}
};

void Print(stack<int> a) {
	if (a.empty())
		cout << "Стек пуст" << endl;
	else {
		while (!a.empty()) {
			cout << a.top() << " ";
			a.pop();
		}
		cout << endl;
	}
}

void HideCursor() {
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO cursorInfo;
	GetConsoleCursorInfo(out, &cursorInfo);
	cursorInfo.bVisible = false;
	SetConsoleCursorInfo(out, &cursorInfo);
}

void Menu() {
	Consumer Cons;
	Producer Prod;
	cout << "1)Установить скорость для производителя\n2)Установить скорость для потребителя\n3)Запустить производителя\n4)Приостановить производителя\n5)Запустить потребителя\n6)Приостановить работу потребителя\n7)Выйти из программы\n";
	HANDLE hCon; COORD cPos;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	cPos.Y = 9;
	cPos.X = 0;
	SetConsoleCursorPosition(hCon, cPos);
	char keypress;
	thread thr1([&Cons]() {Cons.Work(); });
	thread thr2([&Prod]() {Prod.Work(); });
	for (;;) {
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




