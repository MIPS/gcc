#if __cplusplus
extern "C" {
#endif

#if __cplusplus >= 201103
# define __GOACC_NOTHROW noexcept
#elif __cplusplus
# define __GOACC_NOTHROW throw ()
#else /* Not C++ */
# define __GOACC_NOTHROW __attribute__ ((__nothrow__))
#endif

typedef enum acc_device_t { acc_device_X = 123 } acc_device_t;
int acc_on_device (int) __GOACC_NOTHROW;

#if __cplusplus
}
#endif
