## binary_clock

Prints date/time in a binary grid \
 M 0 0 D D D D D \
 M 0 0 H H H H H \
 M 0 m m m m m m \
 M 0 S S S S S S \
 Where M - month, D - day, H - hour, m - minute, S - second, and 0 - Currently unused

# The Problem
We have learnt that **variables** are stored as chunks of memory where the **size** and **format** of **bits** are defined by the **variable type**. Even if we are storing an **integer** value of **0** our program is using **4 bytes** of memory to represent that value. Thats **INSERT WASTED COMPONENTS** to hold a single binary value, so what if we could control things on a *bit wise* level to make use of that wasted memory. Well it turns out [Bitwise Operators](https://en.cppreference.com/w/cpp/language/operator_arithmetic) exist and utilizing them in conjunction with [Fixed width integer types](https://en.cppreference.com/w/cpp/types/integer) has allowed me to minimise some of that wasted memory in this project.

# The Project
The goal for this project is to display the date/time in their binary values on a grid (8 x 4) which just so happens to have the same amount of binary values as our integer was using to store the value of 0 mentioned above. \
```
M 0 0 D D D D D \
M 0 0 H H H H H \
M 0 m m m m m m \
M 0 S S S S S S \
```
Where M - month, D - day, H - hour, m - minute, S - second, and 0 - Currently unused

# Variable sizes and bits


# Storing multiple values in one variable
4 - 0100, 6 - 0110, 9 - 1001, 11 - 1011
```cpp
	// month_check holds binary values for Apr, Jun, Sep, and Nov (30 day months)
	std::uint16_t month_check = 0b0100011010011011; 
```

# The Operators

# Accessing the values with operators
```cpp
	// set day limit depending on month
	for (size_t i = 0; i < 4; i++) 
		month_check = (month == (month_check & (0b1111 << (i * 4))) >> (i * 4)) ? 30 : month_check;
	if (month_check != 30 && month != 2)
		month_check = 31;
	else if (month == 2)
		month_check = 28;
```
