struct s;               // ERROR - forward declaration
extern struct s es, *ps;  // ERROR - defined here
extern volatile struct s evs, *pvs; // ERROR - defined here
  *pv;			// ERROR - invalid void
  (i ? *pv : *pv);	// ERROR - invalid void
  *pv, *pv;		// ERROR - invalid void

  *pvv;			// ERROR - invalid void
  (i ? *pvv : *pvv);	// ERROR - invalid void
  *pvv, *pvv;		// ERROR - invalid void

  es;			// ERROR - incomplete
  (i ? es : es);	// ERROR - undefined type
  es, es;		// ERROR - incomplete

  evs;			// ERROR - incomplete
  (i ? evs : evs);	// ERROR - undefined type
  evs, evs;		// ERROR - incomplete

  *ps;			// ERROR - undefined type
  (i ? *ps : *ps);	// ERROR - undefined type
  *ps, *ps;		// ERROR - undefined type

  *pvs;			// ERROR - undefined type
  (i ? *pvs : *pvs);	// ERROR - undefined type
  *pvs, *pvs;		// ERROR - undefined type
