#include<Windows.h>
#include<iostream>
#include<conio.h>
#include<chrono>
#include<thread>

using namespace std;
using namespace std::chrono_literals;
#define Escape 27
#define Enter 13

#define MIN_TANK_CAPACITY  20
#define MAX_TANK_CAPACITY  120

class Tank
{
	const int CAPACITY;
	double fuel_level;
public:
	Tank(int capacity): 
		CAPACITY
		(
			capacity < MIN_TANK_CAPACITY ? MIN_TANK_CAPACITY:
			capacity > MAX_TANK_CAPACITY ? MAX_TANK_CAPACITY:
			capacity
		)
	{
		//this->CAPACITY = capacity;
		this->fuel_level = 0;
		cout << "Tank is ready " << this << endl;
	}
	~Tank()
	{
		cout << "Tank is over " << this << endl;
	}
	void fill(double amount)
	{
		if (amount < 0) return;
		fuel_level += amount;
		if (fuel_level > CAPACITY)fuel_level = CAPACITY;
	}
	double give_fuel(double amount)
	{
		if (amount < 0)return fuel_level;
		fuel_level -= amount;
		if (fuel_level < 0) fuel_level = 0;
		return fuel_level;
	}
	void info()const
	{
		cout << "Capacity:\t" << CAPACITY << " liters.\n";
		cout << "Fuel level:" << fuel_level << " liters.\n";
	} 
	double get_fuel_level()const
	{
		return fuel_level;
	}
};

#define MIN_ENGINE_CONSUMPTION 4
#define MAX_ENGINE_CONSUMPTION 30
class Engine
{
	const double CONSUMPTION;
	double consumption_per_second;
	bool is_started;
public:
	Engine(double consumption) : CONSUMPTION
	(
		consumption < MIN_ENGINE_CONSUMPTION ? MIN_ENGINE_CONSUMPTION :
		consumption > MAX_ENGINE_CONSUMPTION ? MAX_ENGINE_CONSUMPTION :
		consumption
	)
	{

		consumption_per_second = CONSUMPTION * 3e-5;
		is_started = false;
		cout << "Engine is ready:\t" << this << endl;
	}
	~Engine()
	{
		cout << "Engine is over:\t" << this << endl;
	}
	void set_consumption_per_second(double speed)
	{
		consumption_per_second = speed;
	}
	void start()
	{
		is_started = true;
	}
	void stop()
	{
		is_started = false;
	}
	bool started()
	{
		return is_started;
	}
	void info()const
	{
		cout << "Consumption:\t" << CONSUMPTION << " liters/km.\n";
		cout << "Consumption per sec:" << consumption_per_second << " liters/sec.\n";
	}
	double get_consumption_per_second()
	{
		return consumption_per_second;
	}
};

class Car
{
	Engine engine;
	Tank tank;
	bool driver_inside;
	int speed_now;
	int check_speed;
	struct
	{
		thread panel_thread;
		thread engine_idle_thread;
		thread speed_thread;
	}car_threads;
public:
	Car(double cunsumtion, int capacity = 50) :engine(cunsumtion), tank(capacity)
	{
		driver_inside = false;
		speed_now = 0;
		check_speed = 0;
		cout << "Your car is ready to go, press Enter to go in " << this << endl;
	}
	~Car()
	{
		cout << "Car is over: " << this << endl;
	}
	void engine_idle()
	{
		while (engine.started() && tank.give_fuel(engine.get_consumption_per_second()))
			this_thread::sleep_for(1s);
	}
	void speed_controle()
	{
		//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		//int x = 0;
		//int y = 1;
		//COORD coord{ x, y };

		//CONSOLE_CURSOR_INFO cursorInfo;
		//GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
		//cursorInfo.bVisible = false;
		//SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

		while (driver_inside)
		{
			//SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			//cout << "Speed is now: " << speed_now << " km/h.";
			if (speed_now = 0) check_speed = 0;
			if (speed_now > 0)
			{
				if (speed_now > 0 && speed_now < 61) check_speed = 1;
				if (speed_now > 60 && speed_now < 101) check_speed = 2;
				if (speed_now > 100 && speed_now < 141) check_speed = 3;
				if (speed_now > 140 && speed_now < 201) check_speed = 4;
				if (speed_now > 200 && speed_now < 251) check_speed = 5;
			}
			//for (int i = 0; i < 23; i++)
			//{
			//	cout << " ";
			//}
			this_thread::sleep_for(100ms);
		}

	}
	void panel()
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		int x = 0;
		int y = 0;
		int x_2 = 15;
		int y_2 = 1;
		COORD coord{ x, y };
		COORD coord_2{ x_2, y_2 };
		system("CLS");


		CONSOLE_CURSOR_INFO cursorInfo;
		GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
		cursorInfo.bVisible = false;
		SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

		while (driver_inside)
		{
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
			cout << "Fuel level: " << tank.get_fuel_level() << " liters.";
			for (int i = 0; i < 6; i++)
			{
				cout << " ";
			}
			if (tank.get_fuel_level() < 5)
			{
				SetConsoleTextAttribute(hConsole, 0x4F);
				cout << " LOW FUEL ";
				SetConsoleTextAttribute(hConsole, 0x07);
			}
			cout << "Engine is " << (engine.started() ? "started" : "stopped") << endl;
			
			cout << "Speed is now: ";
			if(check_speed > 0 && speed_now > 0)speed_now--;
			for (int i = 0; i < 9; i++)
			{
				cout << " ";
			}
			SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord_2);
			cout << speed_now << " km/h.";
			for (int i = 0; i < 3; i++)
			{
				cout << " ";
			}

			this_thread::sleep_for(100ms);
		}
	}
	void get_in()
	{
		driver_inside = true;
		//panel();
		if (!car_threads.panel_thread.joinable())car_threads.panel_thread = thread(&Car::panel, this);
		if (!car_threads.panel_thread.joinable())car_threads.speed_thread = thread(&Car::speed_controle, this);
	}
	void get_out()
	{
		driver_inside = false;
		if (car_threads.panel_thread.joinable())car_threads.panel_thread.join();
		system("CLS");
		cout << "You are out of the car" << endl;
	}
	void startup()
	{
		if (tank.give_fuel(0))
		{
			engine.start();
			if(!car_threads.engine_idle_thread.joinable())
			car_threads.engine_idle_thread = thread(&Car::engine_idle, this);
		}
	}
	void shutdown()
	{
		engine.stop();
		if (car_threads.engine_idle_thread.joinable())
			car_threads.engine_idle_thread.join();
	}
	void control()
	{
		char key = 0;
		do
		{
			key = 0;
			if(_kbhit())key = _getch();
			switch (key)
			{
			case Enter:
				if (driver_inside)get_out();
				else get_in();
				break;
			case 'F':
			case 'f':
				if (!driver_inside && !engine.started())
				{
					double amount;
					cout << "Введите объём топлива: "; cin >> amount;
					tank.fill(amount);
				}
				else cout << "Нужно заглушить двигатель и выйти из машины, у нас самообслуживание" << endl;
				break;
			case 'I':
			case 'i':
				if (driver_inside && !engine.started())startup();
				else if (driver_inside)shutdown();
				else shutdown();
				break;
			case 'W':
			case 'w':
				if (check_speed > 5) check_speed = 5;
				else
				{
					check_speed++;
					switch (check_speed)
					{
					case 1:
						speed_now = 60;
						engine.set_consumption_per_second(0.0020);
						break;
					case 2:
						speed_now = 100;
						engine.set_consumption_per_second(0.0014);
						break;
					case 3:
						speed_now = 140;
						engine.set_consumption_per_second(0.0020);
						break;
					case 4:
						speed_now = 200;
						engine.set_consumption_per_second(0.0025);
						break;
					case 5:
						speed_now = 250;
						engine.set_consumption_per_second(0.0030);
						break;
					}
				}
				break;
			case 'S':
			case 's':
				if (check_speed < 0) check_speed = 0;
				else
				{
					check_speed--;
					switch (check_speed)
					{
					case 0:
						speed_now = 0;
						engine.set_consumption_per_second(0.0000);
						break;
					case 1:
						speed_now = 60;
						engine.set_consumption_per_second(0.0020);
						break;
					case 2:
						speed_now = 100;
						engine.set_consumption_per_second(0.0014);
						break;
					case 3:
						speed_now = 140;
						engine.set_consumption_per_second(0.0020);
						break;
					case 4:
						speed_now = 200;
						engine.set_consumption_per_second(0.0025);
						break;
					case 5:
						speed_now = 250;
						engine.set_consumption_per_second(0.0030);
						break;
					}
				}
				break;
			case Escape:
				shutdown();
				get_out();
			}
			if (tank.get_fuel_level() == 0 && engine.started())shutdown();
		} while (key != Escape);
	}
};
//#define TANK_CHECK
//#define ENGINE_CHECK
void main()
{
	setlocale(LC_ALL, "");
#ifdef TANK_CHECK
	int amount;
	Tank tank(40);
	cout << "Введите объём топлива: "; cin >> amount;
	tank.fill(amount);
	tank.info();
#endif // TANK_CHECK
#ifdef ENGINE_CHECK
	Engine engine(10);
	engine.info();
#endif // ENGINE_CHECK
	Car bmw(10, 70);
	bmw.control();
}