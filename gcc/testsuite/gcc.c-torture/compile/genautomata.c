extern struct _IO_FILE *stderr;
extern int have_error;
typedef unsigned long set_el_t;
typedef set_el_t *reserv_sets_t;
typedef struct decl *decl_t;
typedef struct alt_state *alt_state_t;
typedef struct state *state_t;
typedef struct arc *arc_t;
typedef struct ainsn *ainsn_t;
typedef struct automaton *automaton_t;
 typedef struct VEC_state_t_base {
  } VEC_state_t_base;
typedef struct VEC_state_t_heap {
  VEC_state_t_base base;
  } VEC_state_t_heap;
 typedef struct VEC_decl_t_base {
  } VEC_decl_t_base;
typedef struct VEC_decl_t_heap {
  VEC_decl_t_base base;
  } VEC_decl_t_heap;
static reserv_sets_t get_excl_set (reserv_sets_t);
static int ndfa_flag;
static int progress_flag;
static struct description *description;
enum decl_mode {
    dm_unit,   dm_bypass,   dm_automaton,   dm_excl,   dm_presence,   dm_absence,   dm_reserv,   dm_insn_reserv };
struct unit_decl {
  };
struct insn_reserv_decl {
  };
struct decl {
    enum decl_mode mode;
    union   {
      struct unit_decl unit;
      struct insn_reserv_decl insn_reserv;
    }
  decl;
  };
struct description {
    int units_num;
    automaton_t first_automaton;
  };
struct state {
    reserv_sets_t reservs;
    char it_was_placed_in_stack_for_NDFA_forming;
  };
struct alt_state {
    state_t state;
    alt_state_t next_alt_state;
  };
struct ainsn {
    ainsn_t next_ainsn;
    alt_state_t alt_states;
    char first_insn_with_same_reservs;
  };
struct automaton {
    ainsn_t ainsn_list;
    automaton_t next_automaton;
  };
static VEC_decl_t_heap *decls;
static decl_t advance_cycle_insn_decl;
static int max_cycles_num;
static int els_in_cycle_reserv;
static int els_in_reservs;
static reserv_sets_t temp_reserv;
static int reserv_sets_are_intersected (reserv_sets_t operand_1,         reserv_sets_t operand_2) {
    set_el_t *el_ptr_1;
    set_el_t *el_ptr_2;
    set_el_t *cycle_ptr_1;
    set_el_t *cycle_ptr_2;
    for (cycle_ptr_1 = operand_1, cycle_ptr_2 = operand_2;
         cycle_ptr_1 < operand_1 + els_in_reservs;
         cycle_ptr_1 += els_in_cycle_reserv, cycle_ptr_2 += els_in_cycle_reserv)     {
        for (el_ptr_1 = cycle_ptr_1, el_ptr_2 = get_excl_set (cycle_ptr_2);
      el_ptr_1 < cycle_ptr_1 + els_in_cycle_reserv;
      el_ptr_1++, el_ptr_2++)  if (*el_ptr_1 & *el_ptr_2)    return 1;
        if (!check_absence_pattern_sets (temp_reserv + (cycle_ptr_2 - operand_2),            cycle_ptr_2, 1))  return 1;
      }
  }
static int intersected_state_reservs_p (state_t state1, state_t state2) {
    return reserv_sets_are_intersected (state1->reservs, state2->reservs);
  }
static void initiate_states (void) {
    els_in_cycle_reserv     = ((description->units_num + sizeof (set_el_t) * 8 - 1)        / (sizeof (set_el_t) * 8));
    els_in_reservs = els_in_cycle_reserv * max_cycles_num;
  }
static reserv_sets_t excl_set;
static reserv_sets_t get_excl_set (reserv_sets_t in_set) {
    int excl_char_num;
    int chars_num;
    int i;
    int start_unit_num;
    int unit_num;
    chars_num = els_in_cycle_reserv * sizeof (set_el_t);
    for (excl_char_num = 0;
  excl_char_num < chars_num;
  excl_char_num++)     if (((unsigned char *) in_set) [excl_char_num])       for (i = 8 - 1;
  i >= 0;
  i--)  if ((((unsigned char *) in_set) [excl_char_num] >> i) & 1)    {
       if (start_unit_num >= description->units_num)        return excl_set;
       for (unit_num = 0;
  unit_num < els_in_cycle_reserv;
  unit_num++)        {
         }
     }
  }
static void make_automaton (automaton_t automaton) {
    ainsn_t ainsn;
    struct insn_reserv_decl *insn_reserv_decl;
    alt_state_t alt_state;
    state_t state;
    state_t state2;
    arc_t added_arc;
    VEC_state_t_heap *state_stack = (VEC_state_t_heap_alloc(150 ));
    int states_n;
    reserv_sets_t reservs_matter = form_reservs_matter (automaton);
    while ((VEC_state_t_base_length(((state_stack) ? &(state_stack)->base :
  0))) != 0)     {
        state = (VEC_state_t_base_pop(((state_stack) ? &(state_stack)->base :
  0) ,"../../mem-ref/gcc/genautomata.c",5334,__FUNCTION__));
        for (ainsn = automaton->ainsn_list;
      ainsn != ((void *)0);
      ainsn = ainsn->next_ainsn)         if (ainsn->first_insn_with_same_reservs)           {
              if (insn_reserv_decl != __extension__ (({ __typeof (advance_cycle_insn_decl) const _decl = (advance_cycle_insn_decl); if (_decl->mode != dm_insn_reserv) decl_mode_check_failed (_decl->mode, "dm_insn_reserv", "../../mem-ref/gcc/genautomata.c", 5342, __FUNCTION__); &(_decl)->decl.insn_reserv; }
 )))               {   added_arc = ((void *)0);                 for (alt_state = ainsn->alt_states;                      alt_state != ((void *)0);                      alt_state = alt_state->next_alt_state)                   {                     state2 = alt_state->state;                     if (!intersected_state_reservs_p (state, state2))                       {                         state2 = states_union (state, state2, reservs_matter);                         if (!state2->it_was_placed_in_stack_for_NDFA_forming)                           {                             state2->it_was_placed_in_stack_for_NDFA_forming          = 1;                             (VEC_state_t_heap_safe_push(&(state_stack),state2 ,"../../mem-ref/gcc/genautomata.c",5359,__FUNCTION__ ));        states_n++;        if (progress_flag && states_n % 100 == 0)          fprintf (stderr, ".");                           }    added_arc = add_arc (state, state2, ainsn);    if (!ndfa_flag)      break;                       }                   }   if (!ndfa_flag && added_arc != ((void *)0))     {       for (alt_state = ainsn->alt_states;     alt_state != ((void *)0);     alt_state = alt_state->next_alt_state)         state2 = alt_state->state;     }               }
            }
      }
  }
static void build_automaton (automaton_t automaton) {
    make_automaton (automaton);
  }
static void create_automata (void) {
    automaton_t curr_automaton;
    for (curr_automaton = description->first_automaton;
         curr_automaton != ((void *)0);
         curr_automaton = curr_automaton->next_automaton)     {
        build_automaton (curr_automaton);
      }
  }
static void generate (void) {
    initiate_states ();
    create_automata ();
  }
static void expand_automata (void) {
    if (!have_error)     {
        generate ();
      }
  }
int main (int argc, char **argv) {
    if ((VEC_decl_t_base_length(((decls) ? &(decls)->base :
  0))) > 0)     {
        expand_automata ();
      }
  }
