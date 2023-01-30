/* Target macros for riscv*-elf targets.
   Copyright (C) 1994-2017 Free Software Foundation, Inc.

This file is part of GCC.

GCC is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 3, or (at your option)
any later version.

GCC is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#define LINK_SPEC "\
-melf" XLEN_SPEC "lriscv \
%{mno-relax:--no-relax} \
%{shared}"

/* Link against Newlib libraries, because the ELF backend assumes Newlib.
   Handle the circular dependence between libc and libgloss. */
#undef  LIB_SPEC
// GNU MCU Eclipse
// #define LIB_SPEC "--start-group -lc %{!specs=nosys.specs:-lgloss} --end-group"
// The sequence is copied from arm-none-eabi, might not be exactly right.
#define LIB_SPEC "--start-group -lgcc -lg -lc --end-group"

#undef  STARTFILE_SPEC
#define STARTFILE_SPEC "crt0%O%s crtbegin%O%s"

#undef  ENDFILE_SPEC
#define ENDFILE_SPEC "crtend%O%s"

#define NO_IMPLICIT_EXTERN_C 1
