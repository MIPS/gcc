/* The compiler always attempts to expand acc_on_device, but if the
   user disables the builtin, or calls it via a pointer, we have this
   version.  */

int
acc_on_device (int dev)
{
  /* Just rely on the compiler builtin.  */
  return __builtin_acc_on_device (dev);
}

int acc_on_device_h_(int *d)
{
  return acc_on_device(*d);
}
