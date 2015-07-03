#include <dlfcn.h>

#define COST_MODEL	"ix86_cost_model"
#define COST_MODEL_SIZE	COST_MODEL "_size"

static const struct processor_costs *
load_cost_model (const char *model)
{
  const struct processor_costs *cost;
  int *cost_size;
  void *handle;
  char *dso = (char *) alloca (strlen (model) + sizeof ".so");

  strcpy (stpcpy (dso, model), ".so");

  handle = dlopen (dso, RTLD_LAZY);
  if (handle == NULL)
    {
      fprintf(stderr, "load_cost_model: dlopen: %s\n", dlerror ());
      return NULL;
    }

  cost = (const struct processor_costs *) dlsym (handle, COST_MODEL);
  if (cost == NULL)
    {
      fprintf(stderr, "load_cost_model: dlsym: %s\n", dlerror ());
      goto error_return;
    }

  cost_size = (int *) dlsym (handle, COST_MODEL_SIZE);
  if (cost == NULL)
    {
      fprintf(stderr, "load_cost_model: dlsym: %s\n", dlerror ());
      goto error_return;
    }

  if (*cost_size != sizeof (*cost))
    {
      fprintf(stderr, "load_cost_model: cost model size %d != %d\n",
	      *cost_size, (int) sizeof (*cost));
      cost = NULL;
    }

error_return:
  if (cost == NULL)
    dlclose (handle);

  return cost;
}

void
x86_tune_options (void)
{
  if (x86_cost_string)
    {
      if (strcmp (x86_cost_string, "size") == 0)
	ix86_cost = &ix86_size_cost;
      else if (strcmp (x86_cost_string, "i386") == 0)
	ix86_cost = &i386_cost;
      else if (strcmp (x86_cost_string, "i486") == 0)
	ix86_cost = &i486_cost;
      else if (strcmp (x86_cost_string, "pentium") == 0)
	ix86_cost = &pentium_cost;
      else if (strcmp (x86_cost_string, "pentiumpro") == 0)
	ix86_cost = &pentiumpro_cost;
      else if (strcmp (x86_cost_string, "geode") == 0)
	ix86_cost = &geode_cost;
      else if (strcmp (x86_cost_string, "k6") == 0)
	ix86_cost = &k6_cost;
      else if (strcmp (x86_cost_string, "athlon") == 0)
	ix86_cost = &athlon_cost;
      else if (strcmp (x86_cost_string, "k8") == 0)
	ix86_cost = &k8_cost;
      else if (strcmp (x86_cost_string, "amdfam10") == 0)
	ix86_cost = &amdfam10_cost;
      else if (strcmp (x86_cost_string, "bdver1") == 0)
	ix86_cost = &bdver1_cost;
      else if (strcmp (x86_cost_string, "bdver2") == 0)
	ix86_cost = &bdver2_cost;
      else if (strcmp (x86_cost_string, "bdver3") == 0)
	ix86_cost = &bdver3_cost;
      else if (strcmp (x86_cost_string, "bdver4") == 0)
	ix86_cost = &bdver4_cost;
      else if (strcmp (x86_cost_string, "btver1") == 0)
	ix86_cost = &btver1_cost;
      else if (strcmp (x86_cost_string, "btver2") == 0)
	ix86_cost = &btver2_cost;
      else if (strcmp (x86_cost_string, "pentium4") == 0)
	ix86_cost = &pentium4_cost;
      else if (strcmp (x86_cost_string, "nocona") == 0)
	ix86_cost = &nocona_cost;
      else if (strcmp (x86_cost_string, "atom") == 0)
	ix86_cost = &atom_cost;
      else if (strcmp (x86_cost_string, "slm") == 0)
	ix86_cost = &slm_cost;
      else if (strcmp (x86_cost_string, "generic") == 0)
	ix86_cost = &generic_cost;
      else if (strcmp (x86_cost_string, "core") == 0)
	ix86_cost = &core_cost;
      else
	{
	  ix86_cost = load_cost_model (x86_cost_string);
	  if (ix86_cost == NULL)
	    abort ();
	}
    }
  if (x86_schedule_string)
    {
      if (strcmp (x86_schedule_string, "none") == 0)
	ix86_schedule = CPU_NONE;
      else if (strcmp (x86_schedule_string, "pentium") == 0)
	ix86_schedule = CPU_PENTIUM;
      else if (strcmp (x86_schedule_string, "pentiumpro") == 0)
	ix86_schedule = CPU_PENTIUMPRO;
      else if (strcmp (x86_schedule_string, "geode") == 0)
	ix86_schedule = CPU_GEODE;
      else if (strcmp (x86_schedule_string, "k6") == 0)
	ix86_schedule = CPU_K6;
      else if (strcmp (x86_schedule_string, "athlon") == 0)
	ix86_schedule = CPU_ATHLON;
      else if (strcmp (x86_schedule_string, "k8") == 0)
	ix86_schedule = CPU_K8;
      else if (strcmp (x86_schedule_string, "core2") == 0)
	ix86_schedule = CPU_CORE2;
      else if (strcmp (x86_schedule_string, "corei7") == 0)
	ix86_schedule = CPU_NEHALEM;
      else if (strcmp (x86_schedule_string, "atom") == 0)
	ix86_schedule = CPU_ATOM;
      else if (strcmp (x86_schedule_string, "slm") == 0)
	ix86_schedule = CPU_SLM;
      else if (strcmp (x86_schedule_string, "generic") == 0)
	ix86_schedule = CPU_GENERIC;
      else if (strcmp (x86_schedule_string, "amdfam10") == 0)
	ix86_schedule = CPU_AMDFAM10;
      else if (strcmp (x86_schedule_string, "bdver1") == 0)
	ix86_schedule = CPU_BDVER1;
      else if (strcmp (x86_schedule_string, "bdver2") == 0)
	ix86_schedule = CPU_BDVER2;
      else if (strcmp (x86_schedule_string, "bdver3") == 0)
	ix86_schedule = CPU_BDVER3;
      else if (strcmp (x86_schedule_string, "bdver4") == 0)
	ix86_schedule = CPU_BDVER4;
      else if (strcmp (x86_schedule_string, "btver2") == 0)
	ix86_schedule = CPU_BTVER2;
      else
	abort ();
    }
}
