/* Native CPU detection for aarch64.
   Copyright (C) 2015 Free Software Foundation, Inc.

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

#include "config.h"
#include "system.h"

struct arch_extension
{
  const char *ext;
  const char *feat_string;
};

#define AARCH64_OPT_EXTENSION(EXT_NAME, FLAGS_ON, FLAGS_OFF, FEATURE_STRING) \
  { EXT_NAME, FEATURE_STRING },
static struct arch_extension ext_to_feat_string[] =
{
#include "aarch64-option-extensions.def"
};


struct aarch64_core_data
{
  const char* name;
  const char *arch;
  unsigned char implementer_id; /* Exactly 8 bits */
  unsigned int part_no; /* 12 bits + 12 bits */
};

#define AARCH64_BIG_LITTLE(BIG, LITTLE) \
  (((BIG)&0xFFFu) << 12 | ((LITTLE) & 0xFFFu))
#define INVALID_IMP ((unsigned char) -1)
#define INVALID_CORE ((unsigned)-1)

#define AARCH64_CORE(CORE_NAME, CORE_IDENT, SCHED, ARCH, FLAGS, COSTS, IMP, PART) \
  { CORE_NAME, #ARCH, IMP, PART },

static struct aarch64_core_data cpu_data [] =
{
#include "aarch64-cores.def"
  { NULL, NULL, INVALID_IMP, INVALID_CORE }
};


struct aarch64_arch_driver_info
{
  const char* id;
  const char* name;
};

#define AARCH64_ARCH(NAME, CORE, ARCH_IDENT, ARCH_REV, FLAGS) \
  { #ARCH_IDENT, NAME  },

static struct aarch64_arch_driver_info aarch64_arches [] =
{
#include "aarch64-arches.def"
  {NULL, NULL}
};


/* Return the full architecture name string corresponding to the
   identifier ID.  */

static const char*
get_arch_name_from_id (const char* id)
{
  unsigned int i = 0;

  for (i = 0; aarch64_arches[i].id != NULL; i++)
    {
      if (strcmp (id, aarch64_arches[i].id) == 0)
        return aarch64_arches[i].name;
    }

  return NULL;
}


/* Check wether the string CORE contains the same CPU part numbers
   as BL_STRING.  For example CORE="{0xd03, 0xd07}" and BL_STRING="0xd07.0xd03"
   should return true.  */

static bool
valid_bL_core_p (unsigned int *core, unsigned int bL_core)
{
  return AARCH64_BIG_LITTLE (core[0], core[1]) == bL_core;
}


/* Returns the integer that is after ':' for the field. */
static unsigned parse_field (const char *field)
{
  const char *rest = strchr (field, ':');
  char *after;
  unsigned fint = strtol (rest+1, &after, 16);
  if (after == rest+1)
    return -1;
  return fint;
}

/*  Return true iff ARR contains CORE, in either of the two elements. */

static bool
contains_core_p (unsigned *arr, unsigned core)
{
  if (arr[0] != INVALID_CORE)
    {
      if (arr[0] == core)
        return true;

      if (arr[1] != INVALID_CORE)
        return arr[1] == core;
    }

  return false;
}

/* This will be called by the spec parser in gcc.c when it sees
   a %:local_cpu_detect(args) construct.  Currently it will be called
   with either "arch", "cpu" or "tune" as argument depending on if
   -march=native, -mcpu=native or -mtune=native is to be substituted.

   It returns a string containing new command line parameters to be
   put at the place of the above two options, depending on what CPU
   this is executed.  E.g. "-march=armv8-a" on a Cortex-A57 for
   -march=native.  If the routine can't detect a known processor,
   the -march or -mtune option is discarded.

   For -mtune and -mcpu arguments it attempts to detect the CPU or
   a big.LITTLE system.
   ARGC and ARGV are set depending on the actual arguments given
   in the spec.  */

const char *
host_detect_local_cpu (int argc, const char **argv)
{
  const char *arch_id = NULL;
  const char *res = NULL;
  static const int num_exts = ARRAY_SIZE (ext_to_feat_string);
  char buf[128];
  FILE *f = NULL;
  bool arch = false;
  bool tune = false;
  bool cpu = false;
  unsigned int i = 0;
  int core_idx = -1;
  unsigned char imp = INVALID_IMP;
  unsigned int cores[2] = { INVALID_CORE, INVALID_CORE };
  unsigned int n_cores = 0;
  bool processed_exts = false;
  const char *ext_string = "";

  gcc_assert (argc);

  if (!argv[0])
    goto not_found;

  /* Are we processing -march, mtune or mcpu?  */
  arch = strcmp (argv[0], "arch") == 0;
  if (!arch)
    tune = strcmp (argv[0], "tune") == 0;

  if (!arch && !tune)
    cpu = strcmp (argv[0], "cpu") == 0;

  if (!arch && !tune && !cpu)
    goto not_found;

  f = fopen ("/proc/cpuinfo", "r");

  if (f == NULL)
    goto not_found;

  /* Look through /proc/cpuinfo to determine the implementer
     and then the part number that identifies a particular core.  */
  while (fgets (buf, sizeof (buf), f) != NULL)
    {
      if (strstr (buf, "implementer") != NULL)
	{
	  unsigned cimp = parse_field (buf);
	  if (cimp == INVALID_IMP)
	    goto not_found;

	  if (imp == INVALID_IMP)
	    imp = cimp;
	  /* BIG.little implementers are always equal. */
	  else if (imp != cimp)
	    goto not_found;
	}

      if (strstr (buf, "part") != NULL)
	{
	  unsigned ccore = parse_field (buf);
	  if (!contains_core_p (cores, ccore))
	    {
              if (n_cores == 2)
                goto not_found;

              cores[n_cores++] = ccore;
	    }
          continue;
        }
      if (!tune && !processed_exts && strstr (buf, "Features") != NULL)
        {
          for (i = 0; i < num_exts; i++)
            {
              bool enabled = true;
              char *p = NULL;
              char *feat_string = concat (ext_to_feat_string[i].feat_string, NULL);

              p = strtok (feat_string, " ");

              while (p != NULL)
                {
                  if (strstr (buf, p) == NULL)
                    {
                      enabled = false;
                      break;
                    }
                  p = strtok (NULL, " ");
                }
              ext_string = concat (ext_string, "+", enabled ? "" : "no",
                                   ext_to_feat_string[i].ext, NULL);
            }
          processed_exts = true;
        }
    }

  fclose (f);
  f = NULL;

  /* Weird cpuinfo format that we don't know how to handle.  */
  if (n_cores == 0 || n_cores > 2 || imp == INVALID_IMP)
    goto not_found;

  if (arch)
    {
      /* Search for one of the cores in the list. */
      for (i = 0; cpu_data[i].name != NULL; i++)
	if (cpu_data[i].implementer_id == imp
	    && contains_core_p (cores, cpu_data[i].part_no))
	  {
	    arch_id = cpu_data[i].arch;
	    break;
	  }
      if (!arch_id)
	goto not_found;

      const char* arch_name = get_arch_name_from_id (arch_id);

      /* We got some arch indentifier that's not in aarch64-arches.def?  */
      if (!arch_name)
        goto not_found;

      res = concat ("-march=", arch_name, NULL);
    }
  /* We have big.LITTLE.  */
  else if (n_cores == 2)
    {
      for (i = 0; cpu_data[i].name != NULL; i++)
        {
          if (cpu_data[i].implementer_id == imp
              && valid_bL_core_p (cores, cpu_data[i].part_no))
            {
              res = concat ("-m", cpu ? "cpu" : "tune", "=", cpu_data[i].name, NULL);
              break;
            }
        }
      if (!res)
        goto not_found;
    }
  /* The simple, non-big.LITTLE case.  */
  else
    {
      for (i = 0; cpu_data[i].name != NULL; i++)
	if (cores[0] == cpu_data[i].part_no
	    && cpu_data[i].implementer_id == imp)
	  {
	    core_idx = i;
	    break;
	  }
      if (core_idx == -1)
	goto not_found;

      res = concat ("-m", cpu ? "cpu" : "tune", "=",
                      cpu_data[core_idx].name, NULL);
    }

  if (tune)
    return res;

  res = concat (res, ext_string, NULL);

  return res;

not_found:
  {
   /* If detection fails we ignore the option.
      Clean up and return empty string.  */

    if (f)
      fclose (f);

    return "";
  }
}

