#ifndef STD_FENV_H
#define STD_FENV_H 1

#include <cpu/fpu_state.h>

typedef struct fpu_state fenv_t;
typedef unsigned long fexcept_t;

#define FE_DIVBYZERO 001
#define FE_INEXACT 002
#define FE_INVALID 004
#define FE_OVERFLOW 010
#define FE_UNDERFLOW 020

#define FE_ALL_EXCEPT (FE_DIVBYZERO | FE_INEXACT | FE_INVALID | FE_OVERFLOW | FE_UNDERFLOW)

#define FE_DOWNWARD 3
#define FE_TONEAREST 1
#define FE_TOWARDZERO 0
#define FE_UPWARD 2

#define FE_DFL_ENV fpu_get_current_state()

int feclearexcept(int);
int fegetenv(fenv_t *);
int fegetexceptflag(fexcept_t *, int);
int fegetround(void);
int feholdexcept(fenv_t *);
int feraiseexcept(int);
int fesetenv(const fenv_t *);
int fesetexceptflag(const fexcept_t *, int);
int fesetround(int);
int fetestexcept(int);
int feupdateenv(const fenv_t *);

#endif
