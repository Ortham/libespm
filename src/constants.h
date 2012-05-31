/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file constants.h
 * @brief Contains the constants for the program.
 * @details Contains the values that need to be referred to for specific format stuff that may need to be changed due to variations such as a change in format or to check ranges.
 * @def RANGE_MIN
 * The lowest hexidecimal value that is allowed. In this case, the lowest allowed would be 0x41 as that corresponds to the capital letter 'A'
 * and all records consist of 3 or 4 capital letters.
 * @def RANGE_MAX
 * The highest hexidecimal value that is allowed. In this case, the highest allowed would be 0x5A as that corresponds to the capital letter 'Z'
 * and all records consist of 3 or 4 capital letters.
 * @def PRINT_START
 * The lowest hexidecimal value that corresponds with a printable character.
 * @def PRINT_END
 * The highest hexidecimal value that corresponds with a printable character.
 */
#pragma once
#define RANGE_MIN 0x41
#define RANGE_MAX 0x5A
#define PRINT_START 0x20
#define PRINT_END 0x7E
/*END OF LINE*/
