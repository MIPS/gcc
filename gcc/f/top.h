   Contributed by James Craig Burley (burley@gnu.org).
extern bool ffe_is_null_version_;
int ffe_decode_option (int argc, char **argv);
#define ffe_is_null_version() ffe_is_null_version_
#define ffe_set_is_do_internal_checks(f) (ffe_is_do_internal_checks_ = (f))
#define ffe_set_is_null_version(f) (ffe_is_null_version_ = (f))
