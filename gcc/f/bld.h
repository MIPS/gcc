   Contributed by James Craig Burley (burley@gnu.org).
	    ffetargetAlign pad;	/* Initial padding (for DATA, etc.). */
	    ffetargetAlign pad;	/* Initial padding (for DATA, etc.). */
	    ffetargetAlign pad;	/* Initial padding (for DATA, etc.). */
#if FFECOM_targetCURRENT == FFECOM_targetFFE
#endif
#if FFECOM_targetCURRENT == FFECOM_targetFFE
#endif
#if FFECOM_targetCURRENT == FFECOM_targetFFE
#endif
#define ffebld_accter_pad(b) ((b)->u.accter.pad)
#define ffebld_accter_set_pad(b,p) ((b)->u.accter.pad = (p))
#define ffebld_arrter_pad(b) ((b)->u.arrter.pad)
#define ffebld_arrter_set_pad(b,p) ((b)->u.arrter.pad = (p))
#define ffebld_conter_pad(b) ((b)->u.conter.pad)
#define ffebld_conter_set_pad(b,p) ((b)->u.conter.pad = (p))
