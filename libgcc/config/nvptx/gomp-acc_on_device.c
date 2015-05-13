int acc_on_device(int d)
{
  return __builtin_acc_on_device(d);
}

int acc_on_device_h_(int *d)
{
  return acc_on_device(*d);
}
