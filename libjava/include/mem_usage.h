void note_memory_usage (const char* note, jsize size);

#undef NOTE_MEMORY_USAGE
#ifdef PRINT_ALLOCATION_INFO
	#define NOTE_MEMORY_USAGE(NOTE, SIZE) note_memory_usage ((NOTE), (SIZE))
#else
	#define NOTE_MEMORY_USAGE(NOTE, SIZE)
#endif
