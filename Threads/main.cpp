//Threads 
#include<Windows.h>
#include<iostream>
#include<thread>
#include<chrono>

using namespace std::chrono_literals;
using namespace std;


BOOL finish = false;


VOID Function()
{
	while (!finish)
	{
		cout << "Hello Threads from " << GetCurrentThreadId() << endl;
		//system("PAUSE");
	}
}

//#define WINDOWS_THREADS_1
#define WINDOWS_THREADS_2

struct Point
{
	Point(int x, int y) :x(x), y(y) {};
	int x;
	int y;
};

VOID Collision(Point* point)
{
	while (point->x != point->y)
	{
		cout << "X = " << point -> x++ << "\tY = " << point -> y-- << endl;
		Sleep(10);
	}
};

VOID Decrement(int i)
{
	while (i)cout << i-- << "\t";
}

void Plus()
{
	while (!finish)
	{
		cout << "+ ";
		this_thread::sleep_for(100ms);
	}
}
void Minus()
{
	while (!finish)
	{
		cout << "- ";
		this_thread::sleep_for(100ms);
	}
}

void main()
{
	setlocale(LC_ALL, "");
#ifdef WINDOWS_THREADS_1
	DWORD dwID = 0;
	HANDLE hThread = CreateThread
	(
		NULL,
		0,
		(LPTHREAD_START_ROUTINE)Function,
		NULL,
		NULL,
		&dwID
	);
	cin.get();
	finish = true;
	cout << "Thread ID from main(): " << dwID << endl;
	WaitForSingleObject(hThread, INFINITE);

#endif
#ifdef WINDOWS_THREADS_2
	Point A{ 0,100000 };
	DWORD dwThreadID = 0;
	int i = 1000;
	HANDLE hThread = CreateThread
	(
		NULL,
		NULL,
		//(LPTHREAD_START_ROUTINE)Collision,
		(LPTHREAD_START_ROUTINE)Decrement,
		//(LPVOID)&A,
		(LPVOID)i,
		NULL,
		&dwThreadID
	);
	WaitForSingleObject(hThread, INFINITE);

#endif
	//Plus();
	//Minus();

	thread plus_thread = thread(Plus);
	thread minus_thread = thread(Minus);
	

	cin.get();
	finish = true;

	if(plus_thread.joinable())plus_thread.join();
	if(minus_thread.joinable())minus_thread.join();
}