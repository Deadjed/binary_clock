#pragma once
#include <cstdint>

class date_time
{
public:
	/*
	 * Prompts user for Month and Day
	 */
	void get_date();

	/*
	 * Prompts user for Hour, Minutes, and Seconds
	 */
	void get_time();

	/*
	 * Prints bits in binary grid
	 *
	 * M 0 0 D D D D D
	 * M 0 0 H H H H H
	 * M 0 m m m m m m
	 * M 0 S S S S S S
	 *
	 * Where M - month, D - day, H - hour, m - minute, and S - second
	 * 0 - Currently unused
	 */
	void print_bits();
	
	/*
	 * date_time 'ticks' one second, updating it's values
	 */
	void tick();

private:
	std::uint8_t second = 0;
	std::uint8_t minute = 0;
	std::uint8_t hour = 0;
	std::uint8_t day = 0;
	std::uint8_t month = 0;

	/*
	 * Verifies time values are in range
	 */
	void check_time();

	/*
	 * Verifies date values are in range
	 */
	void check_date();
};
