/* TODO: This is to eventually test dispatch of events to callbacks.  */

#include <openacc.h>

int main()
{
  acc_init(acc_device_default);

  (void) acc_get_cuda_stream(acc_async_default);
  /* The following used to crash the runtime due to acc_get_cuda_stream not
     cleaning up data of the OpenACC Profiling Interface.  */
#pragma acc data
  ;

  return 0;
}
