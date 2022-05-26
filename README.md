# Binary Clock

## The Problem
**variables** are stored as chunks of memory where the **size** and **format** of **bits** are defined by the **variable type**. Even when storing an **integer** value of **0** the program is using **4 bytes** of memory to represent that value. Thats **INSERT WASTED COMPONENTS** to hold a single binary value, so what if we could control things on a *bit wise* level to make use of that wasted memory. Well it turns out [Bitwise Operators](https://en.cppreference.com/w/cpp/language/operator_arithmetic) exist and utilizing them in conjunction with [Fixed width integer types](https://en.cppreference.com/w/cpp/types/integer) has allowed me to minimise *some* of that wasted memory within this project.

## The Project
The goal for this project is to display the date/time in their binary values on a grid (8 x 4). With each line representing a byte (eight bits) and four lines giving me **32 bits** to work with, I am able to display the month, day, hour, minute, and second with the same amount of bits the integer variable was using to store the single value of 0, and I still have 6 bits to spare.
```
M 0 0 D D D D D 
M 0 0 H H H H H 
M 0 m m m m m m 
M 0 S S S S S S 
```
Where M - month, D - day, H - hour, m - minute, S - second, and 0 - Currently unused

### Variables
The smallest fixed width integer type still uses **8 bits**, which has a maximum value of 255 (```1111 1111``` in binary, when using an **unsigned type**) which is more than enough when our maximum variable values will be 59 for the minutes and seconds (```11 1011``` in binary). In fact the rest of the variables used could be stored in half of that memory as the maximum expected values are less than 16 (```1111``` in binary) but we will talk about that later. In the mean time I have initialised a **date_time** class that holds the date time data in these values.
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

```get_date()``` and ```get_time()``` use simple input output streams to get data from the user.\
```tick()``` uses ```chrono``` and ```thread``` to wait a second before increasing the second variable and checking if time is still in range.
```cpp
    using namespace std::chrono_literals; // ns, us, ms, s, h...

	std::this_thread::sleep_for(10ns);
	std::this_thread::sleep_until(std::chrono::system_clock::now() + 1s);
	second++;
    date_time::check_time();
```
```check_time()``` uses some if statements to increase a minute if seconds > 59, an hour if minute > 59, and check the date if hour > 23. ```if (second >= (0b1111 << 2))``` Is the same thing as ```if (second >= 60)``` as ```11 1100``` is the binary value for 60 and ```<< 2``` *shifts* the bits left two places. This could be optimised further as the compiler will store raw whole numbers as an integer, and then convert the ```second``` variable to an integer to have an equel comparison (as the compiler always stores variables in the larger variables type rather than taking away memory and risk losing data). By storing ```0b1111 << 2``` as an ```uint8_t``` the compiler would not have to convert second to an integer and use less memory during this if statement. 
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
```check_date()``` uses the same ideas but since the amount of days in a month depends on what month it is that value has to be found first. If months are converted into an integral value, the amount of days in each month looks like:
1 - 31\
2 - 28 (with the exception of leap years)\
3 - 31\
4 - 30\
5 - 31\
6 - 30\
7 - 31\
8 - 31\
9 - 30\
10 - 31\
11 - 30\
12 - 31\
#### Bitwise Operators
With only four 30 day months (```4```, ```6```, ```9```, and ```11```) each being a value of less than 16 can be stored in a single **16 bit** variable next to eachother ```std::uint16_t month_check = 0b0100 0110 1001 1011;```. Think of this as an un-mapped array where the variables are one after another in memory. To access the different groups of bits the ones we need will have to be pulled out, this is where bitwise operators will come in use. Lets say we have 100 bits stored as ```...0101010101``` etc, to get four out we can use the ```&``` (bitwise **AND** operator), that will return a 1 where both values are equal. So if we stored the return value of our 100 bit ```...010101010101 & 1111``` (four bits) we would get ```0101```. This is great if we want to access a certain number of bits on the end of our variable, but what if we want to access the next lot of four? I'll use a different input variable here to make it more obvious, but we just add some ```0```'s on the end of the variable we are comparing it against as we don't want to compare against those values e.g. ```10101111 & 11110000``` will return ```10100000``` now. To get rid of out tailing ```0```'s we can use the bitshift operator ```>>``` to move all of those bits to the right and removing values that fall out of our scope. ```10100000 >> 4``` would return ```1010```. Now we know how to access groups of bits within our variable, let's turn that into a loop that will check over the four month values stored in ```month_check```.\
The ```for``` loop loops four times to check the values at ```1111, 11110000, 111100000000, and 1111000000000000``` by bit shifting ```1111``` to the left four times further for each iteration (starting at 0) (```<< (i * 4)```) and compare to the current month value. If the current month is equal to one of those values then it is a 30 day month and ```month_check``` will set it's value to 30. If the for loop ends and ```month_check``` does not equal 30, and is not the month february (which has a unique amount of days) then ```month_check``` equals 31, else the only option left is 28.

```cpp
	// month_check holds binary values for Apr, Jun, Sep, and Nov (30 day months)
	std::uint16_t month_check = 0b0100011010011011; 
	std::uint16_t temp = 0;

	// set day limit depending on month
	for (size_t i = 0; i < 4; i++) 
		month_check = (month == (month_check & (0b1111 << (i * 4))) >> (i * 4)) ? 30 : month_check;
	if (month_check != 30 && month != 2)
		month_check = 31;
	else if (month == 2)
		month_check = 28;
	
	// check if day within month limit
	if (day > month_check)
	{
		month++;
		day = 1;
	}

	// check month in year limit
	if (month > 12)
		month = 1;
```
Printing the bits is pretty self explanatory using ```bitset``` to display the bits of a variable.
```cpp
	// print bits
	std::cout << "\n";								// LINE 1
	std::cout << std::bitset<4>(month)[0];			// month bit
	std::cout << 0b0 << 0b0;						// 2 empty bits
	std::cout << std::bitset<5>(day) << "\n";		// day bits

													// LINE 2
	std::cout << std::bitset<4>(month)[1];			// month bit
	std::cout << 0b0 << 0b0;						// empty bit
	std::cout << std::bitset<5>(hour) << "\n";		// hour bits

													// LINE 3
	std::cout << std::bitset<4>(month)[2];			// month bit
	std::cout << 0b0;								// empty bit
	std::cout << std::bitset<6>(minute) << "\n";	// minute bits

														// LINE 4
	std::cout << std::bitset<4>(month)[3];				// month bit
	std::cout << 0b0;									// empty bit
	std::cout << std::bitset<6>(second) << std::endl;	// second bits
```
