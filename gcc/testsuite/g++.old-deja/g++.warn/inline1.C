// Special g++ Options: -O2 -Winline

#include <stdarg.h>

inline void ok_vararg (int, ...)
{ }
inline void not_vararg (int i, ...)
{va_list a; va_start (a, i); }	// WARNING - cannot inline

inline void ok_label ()
{label:;}

inline void not_label ()
{&&label; label:;}	        // WARNING - cannot inline

inline void ok_goto ()
{goto label; label:;}

inline void not_goto (void *p)
{goto *p;}	                // WARNING - cannot inline

inline void not_stack_1 ()
{__builtin_return_address (0);}	// WARNING - cannot inline
inline void not_stack_2 ()
{__builtin_frame_address (0);}	// WARNING - cannot inline
inline void not_args ()
{__builtin_apply_args ();}	// WARNING - cannot inline

int main ()
{
  ok_vararg (1);
  not_vararg (1);

  ok_label ();
  not_label ();

  ok_goto ();
  not_goto (0);

  not_stack_1 ();
  not_stack_2 ();

  not_args ();

  return 0;
  
}

  
