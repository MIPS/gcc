enum debug_counter {
   new_dce,
   debug_counter_number_of_counters
};

extern bool dbg_cnt (enum debug_counter index);
extern void dbg_cnt_process_opt (const char *arg);
