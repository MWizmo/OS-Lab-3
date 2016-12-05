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
int consumer_speed = 2, producer_speed = 2;

class Producer {
private:
	bool condition=false;
public:
	bool Is_Condition() {
		return condition;
	}
	void Info() {
		HANDLE hCon; COORD cPos;
		hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		cPos.Y = 2;
		cPos.X = 0;
		SetConsoleCursorPosition(hCon, cPos);
		cout << "�������� ������ �������������: " << producer_speed << "   ������ �������������: ";
		condition ? cout << "��������" : cout << "����������";
		cout << endl;
	}
	void Turn_Off() {
		condition = false;
	}
	void Turn_On() {
		condition = true;
	}
	void Change_Speed() {
		bool flag = false;
		if (Is_Condition()) {
			Turn_Off();
			flag = true;
		}
		int _speed;
		system("cls");
		cout << "������� ����������� ����� ������ �������������(�� 0 �� 10 ������)>>";
		cin >> _speed;
		producer_speed = _speed;
		if (flag) {
			Turn_On();
		}
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
						case '3': {condition = true; } break;
						case '4': {condition = false; } break;
						case '8': exit(0);
						default: {} break;
					}
				}
				mut.lock();
				while (x.size() == 10) {
					if (_kbhit()) {
						keypress = _getch();
						switch (keypress) {
						case '3': {condition = true; } break;
						case '4': {condition = false; } break;
						case '7': exit(0);
						default: {} break;
						}
					}
					HANDLE hCon; COORD cPos;
					hCon = GetStdHandle(STD_OUTPUT_HANDLE);
					cPos.Y = 11;
					SetConsoleCursorPosition(hCon, cPos);
					cout << "���� ����� ";
					mut.unlock();
					this_thread::sleep_for(chrono::seconds(1));
					mut.lock();
				}
				HANDLE hCon; COORD cPos;
				hCon = GetStdHandle(STD_OUTPUT_HANDLE);
				cPos.Y = 11;
				SetConsoleCursorPosition(hCon, cPos);
				cout << "����������� ";
				x.push(1);
				mut.unlock();
				this_thread::sleep_for(chrono::seconds(producer_speed));
			}
		}
	}
};

class Consumer {
private:
	bool condition=false;
public:
	bool Is_Condition() {
		return condition;
	}
	void Info() {
		HANDLE hCon; COORD cPos;
		hCon = GetStdHandle(STD_OUTPUT_HANDLE);
		cPos.Y = 1;
		cPos.X = 0;
		SetConsoleCursorPosition(hCon, cPos);
		cout << "�������� ������ �����������: " << consumer_speed << "   ������ �����������: ";
		condition ? cout << "��������" : cout << "����������";
		cout << endl;
	}
	void Turn_Off() {
		condition = false;
	}
	void Turn_On() {
		condition = true; 
	}
	void Change_Speed() {
		int _speed;
		bool flag = false;
		if(Is_Condition()){
			flag = true;
			Turn_Off();
		}
		system("cls");
		cout << "������� ����������� ����� ������ �����������(�� 0 �� 10 ������)>>";
		cin >> _speed;
		consumer_speed = _speed;
		if (flag) {
			Turn_On();
		}
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
						case '5': {condition = true; } break;
						case '6': {condition = false; } break;
						case '8': exit(0);
						default: {} break;
					}
					}
					mut.lock();
					while (x.empty()) {
						if (_kbhit()) {
							keypress = _getch();
							switch (keypress) {
							case '1': {}
							case '5': {condition = true; } break;
							case '6': {condition = false; } break;
							case '7': exit(0);
							default: {} break;
							}
						}
						HANDLE hCon; COORD cPos;
						hCon = GetStdHandle(STD_OUTPUT_HANDLE);
						cPos.Y = 11;
						SetConsoleCursorPosition(hCon, cPos);
						cout << "���� ���� ";
						mut.unlock();
						this_thread::sleep_for(chrono::seconds(1));
						mut.lock();
					}
					HANDLE hCon; COORD cPos;
					hCon = GetStdHandle(STD_OUTPUT_HANDLE);
					cPos.Y = 11;
					SetConsoleCursorPosition(hCon, cPos);
					cout << "���������� ";
					x.pop();
					mut.unlock();
					this_thread::sleep_for(chrono::seconds(consumer_speed));
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

void Info(Consumer &cons, Producer &prod) {
	system("cls");
	prod.Info();
	cons.Info();
	bool flag1 = false, flag2 = false;
	if (cons.Is_Condition()) {
		cons.Turn_Off();
		flag1 = true;
	}
	if (prod.Is_Condition()) {
		prod.Turn_Off();
		flag2 = true;
	}
	HANDLE hCon; COORD cPos;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	cPos.Y = 3;
	cPos.X = 0;
	SetConsoleCursorPosition(hCon, cPos);
	cout << "��������� �����: " << x.size() << "/10\n";
	_getch();
	if(flag1)
		cons.Turn_On();
	if(flag2)
		prod.Turn_On();
	system("cls");
	Menu();
}

void Menu() {
	Consumer Cons;
	Producer Prod;
	cout << "1)���������� ����� ������ �������������  2)���������� ����� ������ �����������\n3)��������� �������������                4)������������� �������������\n5)��������� �����������                  6)������������� ������ �����������\n7)������� ����������                     8)����� �� ���������\n";
	HANDLE hCon; COORD cPos;
	hCon = GetStdHandle(STD_OUTPUT_HANDLE);
	char keypress;
	thread thr1([&Cons]() {Cons.Work(); });
	thread thr2([&Prod]() {Prod.Work(); });
	cPos.Y = 7;
	cPos.X = 0;
	SetConsoleCursorPosition(hCon, cPos);
	for (;;) {
		if (_kbhit()) {
			keypress = _getch();
			switch (keypress) {
			case '1': {
				bool flag = false;
				if (Cons.Is_Condition()) {
					flag = true;
				}
				Prod.Change_Speed();
				if (flag) {
					Cons.Turn_On();
				}
				} break;
			case '2': {
				bool flag = false;
				if (Prod.Is_Condition()) {
					flag = true;
				}
				Cons.Change_Speed();
				if (flag) {
					Prod.Turn_On();
				}
				}break;
			case '3': {Prod.Turn_On(); } break;
			case '4': {Prod.Turn_Off(); } break;
			case '5': {Cons.Turn_On(); } break;
			case '6': {Cons.Turn_Off(); } break;
			case '7': {Info(Cons, Prod); } break;
			case '8': exit(0);
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




