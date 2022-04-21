#ifndef DATE_TIME_H
#define DATE_TIME_H

#include "date_time.hpp"

#include <iostream>
#include <string>
#include <cstdint>
#include <bitset>
#include <chrono>
#include <thread>

void date_time::get_date()
{
	int temp;
	
	std::cout << "Month: ";
	std::cin >> temp;
	month = temp;

	std::cout << "Day: ";
	std::cin >> temp;
	day = temp;
}

void date_time::get_time()
{
	int temp;

	std::cout << "Hour: ";
	std::cin >> temp;
	hour = temp;

	std::cout << "Minute: ";
	std::cin >> temp;
	minute = temp;

	std::cout << "Second: ";
	std::cin >> temp;
	second = temp;
}
/*
void date_time::get_time(FROM SYSTEM)
{
	// use time_point to get system time
	const std::chrono::time_point<std::chrono::system_clock> now = std::chrono::system_clock::now();
	// convert time_point to uint8_t (Month, Day, Hour, Minute, and Second seperately)
	// add those values to class
}
*/
void date_time::print_bits()
{
	// print bits
	std::cout << "\n";									// LINE 1
	std::cout << std::bitset<4>(month)[0];				// month bit
	std::cout << 0b0 << 0b0;							// 2 empty bits
	std::cout << std::bitset<5>(day) << "\n";			// day bits

														// LINE 2
	std::cout << std::bitset<4>(month)[1];				// month bit
	std::cout << 0b0 << 0b0;							// empty bit
	std::cout << std::bitset<5>(hour) << "\n";			// hour bits

														// LINE 3
	std::cout << std::bitset<4>(month)[2];				// month bit
	std::cout << 0b0;									// empty bit
	std::cout << std::bitset<6>(minute) << "\n";		// minute bits

														// LINE 4
	std::cout << std::bitset<4>(month)[3];				// month bit
	std::cout << 0b0;									// empty bit
	std::cout << std::bitset<6>(second) << std::endl;	// second bits
}

void date_time::tick()
{
	using namespace std::chrono_literals; // ns, us, ms, s, h...

	std::this_thread::sleep_for(10ns);
	std::this_thread::sleep_until(std::chrono::system_clock::now() + 1s);
	second++;
	date_time::check_time();
}

void date_time::check_time()
{
	if (second >= 60)
	{
		second = 0;
		minute += 1;
	}

	if (minute >= 60)
	{
		minute = 0;
		hour += 1;
	}

	if (hour >= 25)
	{
		hour = 0;
		day++;
	}
}

void date_time::check_date()
{
	// add month ranges
	// check day within current month range
	// if so month++, day = 1
	// check month in year range
}

#endif // !DATE_TIME_H