/* ----------------------------------------------------------------------------
 * This file is part of the xPack distribution.
 *   (https://xpack.github.io)
 * Copyright (c) 2022 Liviu Ionescu.
 *
 * Permission to use, copy, modify, and/or distribute this software
 * for any purpose is hereby granted, under the terms of the MIT license.
 * -------------------------------------------------------------------------- */

#include <iostream>

extern const char* hello();

const char* __attribute__((weak))
world()
{
  return "there";
}

// The test checks if a weak alone is called,
// and if a non-weak overrides the local weak.
int main(int argc, char* argv[]) {
    std::cout << hello() << " " << world() << std::endl;
    return 0;
}
