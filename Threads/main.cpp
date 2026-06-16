//Threads 
#include<Windows.h>
#include<iostream>
#include<thread>
#include<mutex>
#include<chrono>

using namespace std::chrono_literals;
using namespace std;


BOOL finish = false;
mutex mtx;
HANDLE ghMutex = NULL;

VOID Function()
{
	while (!finish)
	{
		cout << "Hello Threads from " << GetCurrentThreadId() << endl;
		//system("PAUSE");
	}
}

//#define WINDOWS_THREADS_1
//#define WINDOWS_THREADS_2
//#define CPP_THREADS


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
		//mtx.lock();
		WaitForSingleObject(ghMutex,INFINITE);
		cout << "+ ";
		Sleep(10);
		ReleaseMutex(ghMutex);
		//this_thread::sleep_for(100ms);
		//mtx.unlock();
	}
}
void Minus()
{
	while (!finish)
	{
		//mtx.lock();
		WaitForSingleObject(ghMutex, INFINITE);
		Sleep(10);
		cout << "- ";
		ReleaseMutex(ghMutex);
		//this_thread::sleep_for(100ms);
		//mtx.unlock();
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

#ifdef CPP_THREADS
//Plus();
//Minus();

	thread plus_thread = thread(Plus);
	thread minus_thread = thread(Minus);


	cin.get();
	finish = true;

	if (plus_thread.joinable())plus_thread.join();
	if (minus_thread.joinable())minus_thread.join();
#endif // CPP_THREADS

	ghMutex = CreateMutex(NULL, FALSE, NULL);
	HANDLE hThreads[2] = {};
	hThreads[0] = CreateThread
	(
		NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)Plus,
		NULL,
		NULL,
		0
	);
	hThreads[1] = CreateThread
	(
		NULL,
		NULL,
		(LPTHREAD_START_ROUTINE)Minus,
		NULL,
		NULL,
		0
	);
	WaitForMultipleObjects(2,hThreads,TRUE,INFINITE);
}