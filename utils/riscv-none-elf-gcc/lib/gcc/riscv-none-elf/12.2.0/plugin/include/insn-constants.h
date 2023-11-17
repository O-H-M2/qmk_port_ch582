/* Generated automatically by the program `genconstants'
   from the machine description file `md'.  */

#ifndef GCC_INSN_CONSTANTS_H
#define GCC_INSN_CONSTANTS_H

#define S3_REGNUM 19
#define S8_REGNUM 24
#define S2_REGNUM 18
#define S7_REGNUM 23
#define S4_REGNUM 20
#define S1_REGNUM 9
#define S11_REGNUM 27
#define S9_REGNUM 25
#define S6_REGNUM 22
#define S0_REGNUM 8
#define NORMAL_RETURN 0
#define S10_REGNUM 26
#define S5_REGNUM 21
#define T1_REGNUM 6
#define RETURN_ADDR_REGNUM 1
#define EXCEPTION_RETURN 2
#define GP_REGNUM 3
#define TP_REGNUM 4
#define SIBCALL_RETURN 1
#define T0_REGNUM 5

enum unspec {
  UNSPEC_EH_RETURN = 0,
  UNSPEC_ADDRESS_FIRST = 1,
  UNSPEC_PCREL = 2,
  UNSPEC_LOAD_GOT = 3,
  UNSPEC_TLS = 4,
  UNSPEC_TLS_LE = 5,
  UNSPEC_TLS_IE = 6,
  UNSPEC_TLS_GD = 7,
  UNSPEC_AUIPC = 8,
  UNSPEC_FLT_QUIET = 9,
  UNSPEC_FLE_QUIET = 10,
  UNSPEC_COPYSIGN = 11,
  UNSPEC_LRINT = 12,
  UNSPEC_LROUND = 13,
  UNSPEC_TIE = 14,
  UNSPEC_COMPARE_AND_SWAP = 15,
  UNSPEC_SYNC_OLD_OP = 16,
  UNSPEC_SYNC_EXCHANGE = 17,
  UNSPEC_ATOMIC_STORE = 18,
  UNSPEC_MEMORY_BARRIER = 19
};
#define NUM_UNSPEC_VALUES 20
extern const char *const unspec_strings[];

enum unspecv {
  UNSPECV_GPR_SAVE = 0,
  UNSPECV_GPR_RESTORE = 1,
  UNSPECV_FRFLAGS = 2,
  UNSPECV_FSFLAGS = 3,
  UNSPECV_MRET = 4,
  UNSPECV_SRET = 5,
  UNSPECV_URET = 6,
  UNSPECV_BLOCKAGE = 7,
  UNSPECV_FENCE = 8,
  UNSPECV_FENCE_I = 9,
  UNSPEC_SSP_SET = 10,
  UNSPEC_SSP_TEST = 11
};
#define NUM_UNSPECV_VALUES 12
extern const char *const unspecv_strings[];

#endif /* GCC_INSN_CONSTANTS_H */
