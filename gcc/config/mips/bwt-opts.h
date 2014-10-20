
#ifndef GCC_BWT_OPTS_H
#define GCC_BWT_OPTS_H

/* The intrinsics model defines the code generation strategy. */

enum bwt_intrinsics_model {
  BWT_INTRINSICS_CURRENT,
  BWT_INTRINSICS_I2      // all volatile, minimal optimisations
};

#endif // GCC_BWT_OPTS_H

