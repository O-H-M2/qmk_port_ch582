/* Generated automatically by the program 'build/genpreds'
   from the machine description file '/Host/home/yyyy/Work/riscv-none-elf-gcc-12.2.0-1/linux-x64/sources/gcc-12.2.0/gcc/config/riscv/riscv.md'.  */

#ifndef GCC_TM_PREDS_H
#define GCC_TM_PREDS_H

#ifdef HAVE_MACHINE_MODES
extern bool general_operand (rtx, machine_mode);
extern bool address_operand (rtx, machine_mode);
extern bool register_operand (rtx, machine_mode);
extern bool pmode_register_operand (rtx, machine_mode);
extern bool scratch_operand (rtx, machine_mode);
extern bool immediate_operand (rtx, machine_mode);
extern bool const_int_operand (rtx, machine_mode);
extern bool const_double_operand (rtx, machine_mode);
extern bool nonimmediate_operand (rtx, machine_mode);
extern bool nonmemory_operand (rtx, machine_mode);
extern bool push_operand (rtx, machine_mode);
extern bool pop_operand (rtx, machine_mode);
extern bool memory_operand (rtx, machine_mode);
extern bool indirect_operand (rtx, machine_mode);
extern bool ordered_comparison_operator (rtx, machine_mode);
extern bool comparison_operator (rtx, machine_mode);
extern bool const_arith_operand (rtx, machine_mode);
extern bool arith_operand (rtx, machine_mode);
extern bool lui_operand (rtx, machine_mode);
extern bool sfb_alu_operand (rtx, machine_mode);
extern bool const_csr_operand (rtx, machine_mode);
extern bool csr_operand (rtx, machine_mode);
extern bool sle_operand (rtx, machine_mode);
extern bool sleu_operand (rtx, machine_mode);
extern bool const_0_operand (rtx, machine_mode);
extern bool reg_or_0_operand (rtx, machine_mode);
extern bool branch_on_bit_operand (rtx, machine_mode);
extern bool splittable_const_int_operand (rtx, machine_mode);
extern bool p2m1_shift_operand (rtx, machine_mode);
extern bool high_mask_shift_operand (rtx, machine_mode);
extern bool move_operand (rtx, machine_mode);
extern bool symbolic_operand (rtx, machine_mode);
extern bool absolute_symbolic_operand (rtx, machine_mode);
extern bool plt_symbolic_operand (rtx, machine_mode);
extern bool call_insn_operand (rtx, machine_mode);
extern bool modular_operator (rtx, machine_mode);
extern bool equality_operator (rtx, machine_mode);
extern bool order_operator (rtx, machine_mode);
extern bool signed_order_operator (rtx, machine_mode);
extern bool subreg_lowpart_operator (rtx, machine_mode);
extern bool fp_native_comparison (rtx, machine_mode);
extern bool fp_scc_comparison (rtx, machine_mode);
extern bool fp_branch_comparison (rtx, machine_mode);
extern bool gpr_save_operation (rtx, machine_mode);
extern bool single_bit_mask_operand (rtx, machine_mode);
extern bool not_single_bit_mask_operand (rtx, machine_mode);
extern bool const31_operand (rtx, machine_mode);
extern bool const63_operand (rtx, machine_mode);
#endif /* HAVE_MACHINE_MODES */

#define CONSTRAINT_NUM_DEFINED_P 1
enum constraint_num
{
  CONSTRAINT__UNKNOWN = 0,
  CONSTRAINT_r,
  CONSTRAINT_f,
  CONSTRAINT_j,
  CONSTRAINT_l,
  CONSTRAINT_I,
  CONSTRAINT_J,
  CONSTRAINT_K,
  CONSTRAINT_L,
  CONSTRAINT_m,
  CONSTRAINT_o,
  CONSTRAINT_A,
  CONSTRAINT_p,
  CONSTRAINT_G,
  CONSTRAINT_S,
  CONSTRAINT_U,
  CONSTRAINT_V,
  CONSTRAINT__l,
  CONSTRAINT__g,
  CONSTRAINT_i,
  CONSTRAINT_s,
  CONSTRAINT_n,
  CONSTRAINT_E,
  CONSTRAINT_F,
  CONSTRAINT_X,
  CONSTRAINT_T,
  CONSTRAINT__LIMIT
};

extern enum constraint_num lookup_constraint_1 (const char *);
extern const unsigned char lookup_constraint_array[];

/* Return the constraint at the beginning of P, or CONSTRAINT__UNKNOWN if it
   isn't recognized.  */

static inline enum constraint_num
lookup_constraint (const char *p)
{
  unsigned int index = lookup_constraint_array[(unsigned char) *p];
  return (index == UCHAR_MAX
          ? lookup_constraint_1 (p)
          : (enum constraint_num) index);
}

extern bool (*constraint_satisfied_p_array[]) (rtx);

/* Return true if X satisfies constraint C.  */

static inline bool
constraint_satisfied_p (rtx x, enum constraint_num c)
{
  int i = (int) c - (int) CONSTRAINT_I;
  return i >= 0 && constraint_satisfied_p_array[i] (x);
}

static inline bool
insn_extra_register_constraint (enum constraint_num c)
{
  return c >= CONSTRAINT_r && c <= CONSTRAINT_l;
}

static inline bool
insn_extra_memory_constraint (enum constraint_num c)
{
  return c >= CONSTRAINT_m && c <= CONSTRAINT_A;
}

static inline bool
insn_extra_special_memory_constraint (enum constraint_num)
{
  return false;
}

static inline bool
insn_extra_relaxed_memory_constraint (enum constraint_num)
{
  return false;
}

static inline bool
insn_extra_address_constraint (enum constraint_num c)
{
  return c >= CONSTRAINT_p && c <= CONSTRAINT_p;
}

static inline void
insn_extra_constraint_allows_reg_mem (enum constraint_num c,
				      bool *allows_reg, bool *allows_mem)
{
  if (c >= CONSTRAINT_G && c <= CONSTRAINT_U)
    return;
  if (c >= CONSTRAINT_V && c <= CONSTRAINT__g)
    {
      *allows_mem = true;
      return;
    }
  (void) c;
  *allows_reg = true;
  *allows_mem = true;
}

#define CONSTRAINT_LEN(c_,s_) 1

extern enum reg_class reg_class_for_constraint_1 (enum constraint_num);

static inline enum reg_class
reg_class_for_constraint (enum constraint_num c)
{
  if (insn_extra_register_constraint (c))
    return reg_class_for_constraint_1 (c);
  return NO_REGS;
}

extern bool insn_const_int_ok_for_constraint (HOST_WIDE_INT, enum constraint_num);
#define CONST_OK_FOR_CONSTRAINT_P(v_,c_,s_) \
    insn_const_int_ok_for_constraint (v_, lookup_constraint (s_))

enum constraint_type
{
  CT_REGISTER,
  CT_CONST_INT,
  CT_MEMORY,
  CT_SPECIAL_MEMORY,
  CT_RELAXED_MEMORY,
  CT_ADDRESS,
  CT_FIXED_FORM
};

static inline enum constraint_type
get_constraint_type (enum constraint_num c)
{
  if (c >= CONSTRAINT_p)
    {
      if (c >= CONSTRAINT_G)
        return CT_FIXED_FORM;
      return CT_ADDRESS;
    }
  if (c >= CONSTRAINT_m)
    return CT_MEMORY;
  if (c >= CONSTRAINT_I)
    return CT_CONST_INT;
  return CT_REGISTER;
}
#endif /* tm-preds.h */
