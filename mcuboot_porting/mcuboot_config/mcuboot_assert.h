/*
Copyright 2022 Huckies <https://github.com/Huckies>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

extern void mcuboot_assert_handler(const char *file, int line, const char *func);

#ifdef assert
#undef assert
#endif
#define assert(arg)                                               \
    do {                                                          \
        if (!(arg)) {                                             \
            mcuboot_assert_handler(__FILE__, __LINE__, __func__); \
        }                                                         \
    } while (0)
