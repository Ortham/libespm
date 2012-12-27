/*
 * constants.h
 * This file is part of libespm
 *
 * Copyright (C) 2012 - deaths_soul (MCP)
 *
 * libespm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * libespm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with libespm. If not, see <http://www.gnu.org/licenses/>.
 */

/**
 * @mainpage
 * @author deaths_soul AKA MCP
 * @file constants.h
 * @brief Contains the constants for the program.
 * @details Contains the values that need to be referred to for specific format stuff that may need to be changed due to variations such as a change in format or to check ranges.
 * @def RANGE_MIN
 * The lowest hexadecimal value that is allowed. In this case, the lowest allowed would be 0x41 as that corresponds to the capital letter 'A'
 * and all records consist of 3 or 4 capital letters.
 * @def RANGE_MAX
 * The highest hexadecimal value that is allowed. In this case, the highest allowed would be 0x5A as that corresponds to the capital letter 'Z'
 * and all records consist of 3 or 4 capital letters.
 * @def PRINT_START
 * The lowest hexadecimal value that corresponds with a printable character.
 * @def PRINT_END
 * The highest hexadecimal value that corresponds with a printable character.
 */
#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#define RANGE_MIN 0x41
#define RANGE_MAX 0x5A
#define PRINT_START 0x20
#define PRINT_END 0x7E
/*END OF LINE*/

#endif /* __CONSTANTS_H__ */
