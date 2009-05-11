#ifndef MULTI_TARGET_H
#define MULTI_TARGET_H

#if defined(EXTRA_TARGET) && defined (__cplusplus)
/* Macros to enclose code that should be put in a target-specific namespace.  */
#define START_TARGET_SPECIFIC namespace EXTRA_TARGET {
#define END_TARGET_SPECIFIC } /* Close EXTRA_TARGET namespace.  */
#define TARGET_SCOPE TARGET_SCOPE_
#else /* !EXTRA_TARGET */
#define START_TARGET_SPECIFIC
#define END_TARGET_SPECIFIC
#define TARGET_SCOPE
#endif /* EXTRA_TARGET */

/* Some files contain both code that is target-specific and code which
  is not.  In this case, the latter kind of code is conditionally
  compiled like this: */
#ifndef EXTRA_TARGET
/* Code to be defined only in the main target files.  */
#endif /* !EXTRA_TARGET */

#ifndef TARGET_NUM
#define TARGET_NUM 0
#endif

typedef unsigned char arch_index_t;

#define EXTRA_TARGETS_DECL(DECL) \
  EXTRA_TARGETS_EXPAND(namespace,{ extern DECL; },)
#define COMMA ,
#define EXTRA_TARGETS_CALL(CALL) EXTRA_TARGETS_EXPAND(,::CALL,COMMA)
#define EXTRA_TARGETS_EXPAND_COMMA(PREFIX,TMPL) \
  EXTRA_TARGETS_EXPAND(PREFIX,::TMPL COMMA,)

#endif /* MULTI_TARGET_H */
