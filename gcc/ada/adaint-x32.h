/****************************************************************************
 *                                                                          *
 *                         GNAT COMPILER COMPONENTS                         *
 *                                                                          *
 *                               A D A I N T                                *
 *                                                                          *
 *                              C Header File                               *
 *                                                                          *
 *             Copyright (C) 2015, Free Software Foundation, Inc.           *
 *                                                                          *
 * GNAT is free software;  you can  redistribute it  and/or modify it under *
 * terms of the  GNU General Public License as published  by the Free Soft- *
 * ware  Foundation;  either version 3,  or (at your option) any later ver- *
 * sion.  GNAT is distributed in the hope that it will be useful, but WITH- *
 * OUT ANY WARRANTY;  without even the  implied warranty of MERCHANTABILITY *
 * or FITNESS FOR A PARTICULAR PURPOSE.                                     *
 *                                                                          *
 * As a special exception under Section 7 of GPL version 3, you are granted *
 * additional permissions described in the GCC Runtime Library Exception,   *
 * version 3.1, as published by the Free Software Foundation.               *
 *                                                                          *
 * You should have received a copy of the GNU General Public License and    *
 * a copy of the GCC Runtime Library Exception along with this program;     *
 * see the files COPYING3 and COPYING.RUNTIME respectively.  If not, see    *
 * <http://www.gnu.org/licenses/>.                                          *
 *                                                                          *
 * GNAT was originally developed  by the GNAT team at  New York University. *
 * Extensive contributions were provided by Ada Core Technologies Inc.      *
 *                                                                          *
 ****************************************************************************/

/* wrappers for run-time functions with pointer arguments.  */

#ifdef __cplusplus
extern "C" {
#endif

#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <dirent.h>

/*  Constants used for the form parameter encoding values  */
#define Encoding_UTF8 0         /* UTF-8 */
#define Encoding_8bits 1        /* Standard 8bits, CP_ACP on Windows. */
#define Encoding_Unspecified 2  /* Based on GNAT_CODE_PAGE env variable. */

/* Large file support. It is unclear what portable mechanism we can use to
   determine at compile time what support the system offers for large files.
   For now we just list the platforms we have manually tested. */

#define GNAT_FOPEN fopen64
#define GNAT_OPEN open64
#define GNAT_STAT stat64
#define GNAT_FSTAT fstat64
#define GNAT_LSTAT lstat64
#define GNAT_STRUCT_STAT struct stat64

/* Type corresponding to GNAT.OS_Lib.OS_Time */
typedef long OS_Time;

#define __int64 long long

/* A lazy cache for the attributes of a file. On some systems, a single call to
   stat() will give all this information, so it is better than doing a system
   call every time. On other systems this require several system calls.
*/

struct file_attributes {
  int           error;
  /* Errno value returned by stat()/fstat(). If non-zero, other fields should
   * be considered as invalid.
   */

  unsigned char exists;

  unsigned char writable;
  unsigned char readable;
  unsigned char executable;

  unsigned char symbolic_link;
  unsigned char regular;
  unsigned char directory;

  OS_Time timestamp;
  __int64 file_length;
};
/* WARNING: changing the size here might require changing the constant
 * File_Attributes_Size in osint.ads (which should be big enough to
 * fit the above struct on any system)
 */

extern int    __gnat_max_path_len;
extern OS_Time __gnat_current_time		   (void);
extern void   __gnat_current_time_string           (uintptr_t);
extern void   __gnat_to_gm_time			   (uintptr_t, uintptr_t,
						    uintptr_t, uintptr_t,
						    uintptr_t, uintptr_t,
						    uintptr_t);
extern void   __gnat_to_os_time                    (uintptr_t, int, int,
						    int, int, int, int);
extern int    __gnat_get_maximum_file_name_length  (void);
extern int    __gnat_get_switches_case_sensitive   (void);
extern int    __gnat_get_file_names_case_sensitive (void);
extern int    __gnat_get_env_vars_case_sensitive   (void);
extern char   __gnat_get_default_identifier_character_set (void);
extern void   __gnat_get_current_dir		   (uintptr_t, uintptr_t);
extern void   __gnat_get_object_suffix_ptr         (uintptr_t, uintptr_t);
extern void   __gnat_get_executable_suffix_ptr     (uintptr_t, uintptr_t);
extern void   __gnat_get_debuggable_suffix_ptr     (uintptr_t, uintptr_t);
extern int    __gnat_readlink			   (uintptr_t, uintptr_t,
						    size_t);
extern int    __gnat_symlink                       (uintptr_t, uintptr_t);
extern int    __gnat_try_lock                      (uintptr_t, uintptr_t);
extern int    __gnat_open_new                      (uintptr_t, int);
extern int    __gnat_open_new_temp		   (uintptr_t, int);
extern int    __gnat_mkdir			   (uintptr_t, int);
extern int    __gnat_unlink                        (uintptr_t);
extern int    __gnat_rename                        (uintptr_t, uintptr_t);
extern int    __gnat_chdir                         (uintptr_t);
extern int    __gnat_rmdir                         (uintptr_t);

extern FILE  *__gnat_fopen			   (uintptr_t, uintptr_t,
						    int);
extern FILE  *__gnat_freopen			   (uintptr_t, uintptr_t,
						    uintptr_t, int);
extern int    __gnat_open                          (uintptr_t, int);
extern int    __gnat_open_read                     (uintptr_t, int);
extern int    __gnat_open_rw                       (uintptr_t, int);
extern int    __gnat_open_create                   (uintptr_t, int);
extern int    __gnat_create_output_file            (uintptr_t);
extern int    __gnat_create_output_file_new        (uintptr_t);

extern int    __gnat_open_append                   (uintptr_t, int);
extern long   __gnat_file_length_long              (int);
extern __int64 __gnat_file_length                  (int);
extern __int64 __gnat_named_file_length            (uintptr_t);
extern void   __gnat_tmp_name			   (uintptr_t);
extern DIR   *__gnat_opendir                       (uintptr_t);
extern char  *__gnat_readdir                       (uintptr_t, uintptr_t,
						    uintptr_t);
extern int    __gnat_closedir                      (uintptr_t);
extern int    __gnat_readdir_is_thread_safe        (void);

extern OS_Time __gnat_file_time_name                (uintptr_t);
extern OS_Time __gnat_file_time_fd                  (int);
/* return -1 in case of error */

extern void   __gnat_set_file_time_name		   (uintptr_t, time_t);

extern int    __gnat_dup			   (int);
extern int    __gnat_dup2			   (int, int);
extern int    __gnat_file_exists		   (uintptr_t);
extern int    __gnat_is_regular_file		   (uintptr_t);
extern int    __gnat_is_absolute_path		   (uintptr_t, int);
extern int    __gnat_is_directory		   (uintptr_t);
extern int    __gnat_is_writable_file		   (uintptr_t);
extern int    __gnat_is_readable_file		   (uintptr_t);
extern int    __gnat_is_executable_file		   (uintptr_t);

extern void   __gnat_reset_attributes		   (uintptr_t);
extern int    __gnat_error_attributes		   (uintptr_t);
extern __int64 __gnat_file_length_attr		   (int, uintptr_t,
						    uintptr_t);
extern OS_Time __gnat_file_time_name_attr	   (uintptr_t, uintptr_t);
extern int    __gnat_file_exists_attr		   (uintptr_t, uintptr_t);
extern int    __gnat_is_regular_file_attr	   (uintptr_t, uintptr_t);
extern int    __gnat_is_directory_attr		   (uintptr_t, uintptr_t);
extern int    __gnat_is_readable_file_attr	   (uintptr_t, uintptr_t);
extern int    __gnat_is_writable_file_attr	   (uintptr_t, uintptr_t);
extern int    __gnat_is_executable_file_attr	   (uintptr_t, uintptr_t);
extern int    __gnat_is_symbolic_link_attr	   (uintptr_t, uintptr_t);

extern void   __gnat_set_non_writable              (uintptr_t);
extern void   __gnat_set_writable                  (uintptr_t);
extern void   __gnat_set_executable                (uintptr_t, int);
extern void   __gnat_set_readable                  (uintptr_t);
extern void   __gnat_set_non_readable              (uintptr_t);
extern int    __gnat_is_symbolic_link		   (uintptr_t);
extern int    __gnat_portable_spawn                (uintptr_t);
extern int    __gnat_portable_no_block_spawn       (uintptr_t);
extern int    __gnat_portable_wait                 (uintptr_t);
extern char  *__gnat_locate_exec                   (uintptr_t, uintptr_t);
extern char  *__gnat_locate_exec_on_path	   (uintptr_t);
extern char  *__gnat_locate_regular_file           (uintptr_t, uintptr_t);
extern void   __gnat_maybe_glob_args               (uintptr_t, uintptr_t);
extern void   __gnat_os_exit			   (int);
extern char  *__gnat_get_libraries_from_registry   (void);
extern int    __gnat_to_canonical_file_list_init   (uintptr_t, int);
extern char  *__gnat_to_canonical_file_list_next   (void);
extern void   __gnat_to_canonical_file_list_free   (void);
extern char  *__gnat_to_canonical_dir_spec         (uintptr_t, int);
extern char  *__gnat_to_canonical_file_spec        (uintptr_t);
extern char  *__gnat_to_host_dir_spec              (uintptr_t, int);
extern char  *__gnat_to_host_file_spec             (uintptr_t);
extern char  *__gnat_to_canonical_path_spec	   (uintptr_t);
extern void   __gnat_adjust_os_resource_limits	   (void);
extern void   convert_addresses			   (uintptr_t, uintptr_t,
						    int, uintptr_t,
						    uintptr_t);
extern int    __gnat_copy_attribs		   (uintptr_t, uintptr_t,
						    int);
extern int    __gnat_feof		  	   (uintptr_t);
extern int    __gnat_ferror                        (uintptr_t);
extern int    __gnat_fileno		  	   (uintptr_t);
extern int    __gnat_is_regular_file_fd  	   (int);
extern FILE  *__gnat_constant_stderr	  	   (void);
extern FILE  *__gnat_constant_stdin	  	   (void);
extern FILE  *__gnat_constant_stdout	  	   (void);
extern char  *__gnat_full_name		  	   (uintptr_t, uintptr_t);

extern int    __gnat_arg_count			   (void);
extern int    __gnat_len_arg			   (int);
extern void   __gnat_fill_arg			   (uintptr_t, int);
extern int    __gnat_env_count			   (void);
extern int    __gnat_len_env			   (int);
extern void   __gnat_fill_env			   (uintptr_t, int);

/* Routines for interface to scanf and printf functions for integer values */

extern int    get_int				   (void);
extern void   put_int				   (int);
extern void   put_int_stderr			   (int);
extern int    get_char				   (void);
extern void   put_char				   (int);
extern void   put_char_stderr			   (int);

extern void   __gnat_set_exit_status		   (int);

extern int    __gnat_expect_fork		   (void);
extern void   __gnat_expect_portable_execvp	   (uintptr_t, uintptr_t);
extern int    __gnat_pipe			   (uintptr_t);
extern int    __gnat_expect_poll		   (uintptr_t, int, int,
						    uintptr_t);
extern void   __gnat_set_binary_mode		   (int);
extern void   __gnat_set_text_mode		   (int);
extern void   __gnat_set_mode			   (int,int);
extern char  *__gnat_ttyname			   (int);
extern int    __gnat_lseek			   (int, long, int);
extern int    __gnat_set_close_on_exec		   (int, int);
extern int    __gnat_dup			   (int);
extern int    __gnat_dup2			   (int, int);

/* large file support */
extern __int64 __gnat_ftell64                      (uintptr_t);
extern int     __gnat_fseek64                      (uintptr_t, __int64, int);

extern int    __gnat_number_of_cpus                (void);

extern void   __gnat_os_filename                   (uintptr_t, uintptr_t,
						    uintptr_t, uintptr_t,
						    uintptr_t, uintptr_t);

extern size_t	__gnat_strlen			   (uintptr_t);
extern char *	__gnat_strncpy			   (uintptr_t, uintptr_t,
						    size_t);
extern int	__gnat_atoi			   (uintptr_t);
extern void	__gnat_clearerr			   (uintptr_t);
extern int	__gnat_fclose			   (uintptr_t);
extern FILE *	__gnat_fdopen			   (int, uintptr_t);
extern int	__gnat_fflush			   (uintptr_t);
extern int	__gnat_fgetc			   (uintptr_t);
extern char *	__gnat_fgets			   (uintptr_t, int,
						    uintptr_t);
extern int	__gnat_fputc			   (int, uintptr_t);
extern int	__gnat_fputs			   (uintptr_t, uintptr_t);
extern int	__gnat_fseek			   (uintptr_t, long, int);
extern void *	__gnat_memcpy			   (uintptr_t, uintptr_t,
						    size_t);
extern void *	__gnat_memmove			   (uintptr_t, uintptr_t,
						    size_t);
extern char *	__gnat_mktemp			   (uintptr_t);
extern int	__gnat_pclose			   (uintptr_t);
extern FILE *	__gnat_popen			   (uintptr_t, uintptr_t);
extern void	__gnat_rewind			   (uintptr_t);
extern int	__gnat_setvbuf			   (uintptr_t, uintptr_t,
						    int, size_t);
extern char *	__gnat_tmpnam			   (uintptr_t);
extern int	__gnat_ungetc			   (int, uintptr_t);
extern ssize_t	__gnat_read			   (int, uintptr_t, size_t);
extern ssize_t	__gnat_write			   (int, uintptr_t, size_t);

extern char *	__gnat_x32_getenv		   (uintptr_t);
extern void *	__gnat_x32_realloc		   (uintptr_t, size_t);
extern void	__gnat_x32_free			   (uintptr_t);

extern void   *__gnat_lwp_self			   (void);

/* Routines for interface to required CPU set primitives */
#include <sched.h>

extern cpu_set_t *__gnat_cpu_alloc                 (size_t);
extern size_t __gnat_cpu_alloc_size                (size_t);
extern void   __gnat_cpu_free			   (uintptr_t);
extern void   __gnat_cpu_zero                      (size_t, uintptr_t);
extern void   __gnat_cpu_set                       (int, size_t, uintptr_t);

#ifdef IN_RTS
/* Portable definition of strdup, which is not available on all systems.  */
#define xstrdup(S)  strcpy ((char *) malloc (strlen (S) + 1), S)
#endif

/* This function returns the version of GCC being used.  Here it's GCC 3.  */
extern int    get_gcc_version                      (void);

extern int    __gnat_binder_supports_auto_init     (void);
extern int    __gnat_sals_init_using_constructors  (void);

extern const void * __gnat_get_executable_load_address  (void);

#ifdef __cplusplus
}
#endif
