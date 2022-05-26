# Binary Clock

## The Problem
**variables** are stored as chunks of memory where the **size** and **format** of **bits** are defined by the **variable type**. Even when storing an **integer** value of **0** the program is using **4 bytes** of memory to represent that value. Thats **INSERT WASTED COMPONENTS** to hold a single binary value, so what if we could control things on a *bit wise* level to make use of that wasted memory. Well it turns out [Bitwise Operators](https://en.cppreference.com/w/cpp/language/operator_arithmetic) exist and utilizing them in conjunction with [Fixed width integer types](https://en.cppreference.com/w/cpp/types/integer) has allowed me to minimise *some* of that wasted memory within this project.

## The Project
The goal for this project is to display the date/time in their binary values on a grid (8 x 4). With each line representing a byte (eight bits) and four lines giving me 32 bits to work with, I am able to display the month, day, hour, minute, and second with the same amount of bits the integer variable was using to store the single value of 0, and I still have 6 bits to spare.
```
M 0 0 D D D D D 
M 0 0 H H H H H 
M 0 m m m m m m 
M 0 S S S S S S 
```
Where M - month, D - day, H - hour, m - minute, S - second, and 0 - Currently unused

### Variables
The smallest fixed width integer type still uses **8 bits**, which has a maximum value of 255 (1111 1111 in binary, when using an **unsigned type**) which is more than enough when our maximum variable values will be 59 for the minutes and seconds (11 1011 in binary). In fact the rest of the variables used could be stored in half of that memory as the maximum expected values are less than 16 (1111 in binary) but we will talk about that later. In the mean time I have initialised a **date_time** class that holds the date time data in these values.
```cpp
	std::uint8_t second = 0;
	std::uint8_t minute = 0;
	std::uint8_t hour = 0;
	std::uint8_t day = 0;
	std::uint8_t month = 0;
```
By using fixed width variable types within the variables expected ranges this program is using a quarter of the memory it would have been if using common integer types (40 bits instead of 160).

### Functions
This program only has a few simple functions to basically **store the date/time** (```get_date()```, ```get_time()```), **tick** like a clock (```tick()```), which also **updates the date/time** (```check_date()```, ```check_time()```), and then **print the current date/time** to the console in the format listed above (```print_bits()```).

```get_date()``` and ```get_time()``` use simple input output streams to get data from the user.
```tick()``` uses ```chrono``` and ```thread``` to wait a second before increasing the second variable and checking if time is still in range.
```cpp
    using namespace std::chrono_literals; // ns, us, ms, s, h...

	std::this_thread::sleep_for(10ns);
	std::this_thread::sleep_until(std::chrono::system_clock::now() + 1s);
	second++;
    date_time::check_time();
```
```check_time()``` uses some if statements to increase a minute if seconds > 59, an hour if minute > 59, and check the date if hour > 23. 
```cpp
	// if second greater than 59 bring to 0 and add a minute
	if (second >= (0b1111 << 2))
	{
		second = 0;
		minute += 1;
	}

	if (minute >= (0b1111 << 2))
	{
		minute = 0;
		hour += 1;
	}
	// if hour greater than 23 bring to 0 and add a day
	if (hour >= (0b11 << 3))
	{
		hour = 0;
		day++;
		date_time::check_date();	// check the date if a day has ended
	}
```

### Storing multiple values in one variable
4 - 0100, 6 - 0110, 9 - 1001, 11 - 1011
```cpp
	// month_check holds binary values for Apr, Jun, Sep, and Nov (30 day months)
	std::uint16_t month_check = 0b0100011010011011; 
```

### The Operators

### Accessing the values with operators
```cpp
	// set day limit depending on month
	for (size_t i = 0; i < 4; i++) 
		month_check = (month == (month_check & (0b1111 << (i * 4))) >> (i * 4)) ? 30 : month_check;
	if (month_check != 30 && month != 2)
		month_check = 31;
	else if (month == 2)
		month_check = 28;
```

