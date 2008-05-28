/* Definitions for various logging mechanisms in GCC.
   Copyright (C) 2008 Free Software Foundation, Inc.
   Contributed by Andrew MacLeod <amacleod@redhat.com>

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


#ifndef GCC_LOGGING_H
#define GCC_LOGGING_H 1


#define ENABLE_LOGGING

/* Max size of a PAL name.  */
#define PAL_NAME_SIZE	200		

/* Define the Pass Activity Log structure.  */
typedef struct pass_activity_log
{
  char name[PAL_NAME_SIZE];
  long stmts_added;
  long stmts_changed;
  long stmts_deleted;
  long stmts_moved;
  long names_created;
  long names_removed;
  long imm_links;
  long imm_delinks;
} pal;

#define EMPTY_PASS_ACTIVITY_LOG               { "root", 0, 0, 0, 0, 0, 0, 0, 0 } 

extern struct pass_activity_log *current_log;


#ifdef ENABLE_LOGGING
#define LOG_PASS_ACTION_ADD_STMT	current_log->stmts_added++
#define LOG_PASS_ACTION_CHANGE_STMT	current_log->stmts_changed++
#define LOG_PASS_ACTION_REMOVE_STMT	current_log->stmts_deleted++
#define LOG_PASS_ACTION_INSERT_STMT	current_log->stmts_added++,	\
					current_log->stmts_changed--
#define LOG_PASS_ACTION_MOVE_STMT	current_log->stmts_moved++,	\
					current_log->stmts_changed--,	\
					current_log->stmts_deleted--,	\
					current_log->stmts_added--
#define LOG_PASS_ACTION_CREATE_NAME	current_log->names_created++
#define LOG_PASS_ACTION_REMOVE_NAME	current_log->names_removed++
#define LOG_PASS_ACTION_IMM_DELINK	current_log->imm_delinks++
#define LOG_PASS_ACTION_IMM_LINK	current_log->imm_links++
#define LOG_PASS_ACTION_BB_TIME(BB,T)	register_bb_time ((BB),(T))

#else

#define LOG_PASS_ACTION_ADD_STMT	0
#define LOG_PASS_ACTION_INSERT_STMT	0
#define LOG_PASS_ACTION_CHANGE_STMT	0
#define LOG_PASS_ACTION_REMOVE_STMT	0
#define LOG_PASS_ACTION_CREATE_NAME	0
#define LOG_PASS_ACTION_REMOVE_NAME	0
#define LOG_PASS_ACTION_IMM_DELINK	0
#define LOG_PASS_ACTION_IMM_LINK	0
#define LOG_PASS_ACTION_BB_TIME(BB,T)	0
#endif


void initialize_logging (tree);
void finalize_logging (void);
void start_pass_logging (const char *);
void finish_pass_logging (const char *);



#endif /* GCC_LOGGING_H */
