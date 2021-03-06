#include <iostream>
#include "date_time.hpp"

/* 
 * BINARY TIME GRID
 *
 * M 0 0 D D D D D
 * M 0 0 H H H H H
 * M 0 m m m m m m
 * M 0 S S S S S S
 * 
 * Where M - month, D - day, H - hour, m - minute, and S - second
 *
 */

int main() {

	date_time current;
	
	current.get_date();
	current.get_time();

	do
	{
		current.print_bits();
		current.tick();
	} while (true);

	return 0;
}