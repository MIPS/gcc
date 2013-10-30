/*
This file is part of GCC.

GCC is free software; you can redistribute it and/or modify it under
the terms of the GNU General Public License as published by the Free
Software Foundation; either version 3, or (at your option) any later
version.

GCC is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or
FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
for more details.

You should have received a copy of the GNU General Public License
along with GCC; see the file COPYING3.  If not see
<http://www.gnu.org/licenses/>.  */

#include "openacc.h"
#include "liboacc-internal.h"

#ifdef __cplusplus
extern "C" {
#endif

/*
in current implementation acc_device_default
  is treated always like acc_device_not_host
*/

int
acc_get_num_devices(acc_device_t devt)
{
  OACC_CHECK_INIT
  switch(devt)
    {
    case acc_device_host:
      return OACC_nhost_devs;
    case acc_device_not_host:
    case acc_device_default:
      return OACC_nacc_devs;
    }
  return 0;
}

void
acc_set_device_type(acc_device_t devt)
{
  OACC_CHECK_INIT
  LOCK_AQUIRE(OACC_dev_lock);
  switch(devt)
    {
    case acc_device_host:
      OACC_curr_dev = OACC_host_devs;
      OACC_curr_num = OACC_host_num;
      break;
    case acc_device_not_host:
    case acc_device_default:
      OACC_curr_dev = OACC_acc_devs;
      OACC_curr_num = OACC_acc_num;
      break;
    default:
      OACC_curr_dev = NULL;
      OACC_curr_num = 0;
      break;
    }
  LOCK_RELEASE(OACC_dev_lock);
}

acc_device_t
acc_get_device_type(void)
{
  acc_device_t ret = acc_device_none;

  OACC_CHECK_INIT
  LOCK_AQUIRE(OACC_dev_lock);
  if(OACC_curr_dev == OACC_host_devs)
    {
      ret = acc_device_host;
    }
  else if(OACC_curr_dev == OACC_acc_devs)
    {
      ret = acc_device_not_host;
    }
  LOCK_RELEASE(OACC_dev_lock);
  return ret;
}

/*
according to OpenACC standard, NUM == 0 means default device of specified type,
DEVT == 0 means for all device types
*/
void
acc_set_device_num(int num, acc_device_t devt)
{
  if(num < 0)
    {
      return;
    }
  OACC_CHECK_INIT
  LOCK_AQUIRE(OACC_dev_lock);

  /* make first device of type default device */
  if(num == 0)
    {
      ++num;
    }

  /*
  we maintain number of current device as an index into corresponding array,
  so before update internal data given number must be decremented
  */
  switch(devt)
    {
    case acc_device_none:
      if(num > OACC_nhost_devs)
        {
          num = OACC_nhost_devs;
        }
      if(num > OACC_nacc_devs)
        {
          num = OACC_nacc_devs;
        }
      OACC_host_num = OACC_acc_num = num - 1;
      break;
    case acc_device_host:
      if(num > OACC_nhost_devs)
        {
          num = OACC_nhost_devs;
        }
      OACC_host_num = num - 1;
      break;
    case acc_device_default:
    case acc_device_not_host:
      if(num > OACC_nacc_devs)
        {
          num = OACC_nacc_devs;
        }
      OACC_acc_num = num - 1;
      break;
    }
  /* update current device index */
  if(OACC_curr_dev == OACC_host_devs)
    {
      OACC_curr_num = OACC_host_num;
    }
  else if(OACC_curr_dev == OACC_acc_devs)
    {
      OACC_curr_num = OACC_acc_num;
    }

  LOCK_RELEASE(OACC_dev_lock);
}

int
acc_get_device_num(acc_device_t devt)
{
  OACC_CHECK_INIT
  switch(devt)
    {
    case acc_device_host:
      return OACC_host_num + 1;
    case acc_device_default:
    case acc_device_not_host:
      return OACC_acc_num + 1;
    }
  return -1;
}

/* device explicit initialization / cleanup */
void
acc_init(acc_device_t devt)
{
  size_t i, n;
  OACC_device_ptr *ptbl = NULL;

  OACC_CHECK_INIT
  switch(devt)
    {
    case acc_device_host:
      ptbl = OACC_host_devs;
      n = OACC_nhost_devs;
      break;
    case acc_device_default:
    case acc_device_not_host:
      ptbl = OACC_acc_devs;
      n = OACC_nacc_devs;
      break;
    }

  LOCK_AQUIRE(OACC_dev_lock);
  if(ptbl != NULL && n > 0)
    {
      for(i = 0 ; i < n; ++i)
        {
          OACC_dev_init(ptbl[i]);
        }
    }
  LOCK_RELEASE(OACC_dev_lock);
}

void
acc_shutdown(acc_device_t devt)
{
  size_t i, n;
  OACC_device_ptr *ptbl = NULL;

  OACC_CHECK_INIT
  switch(devt)
    {
    case acc_device_host:
      ptbl = OACC_host_devs;
      n = OACC_nhost_devs;
      break;
    case acc_device_default:
    case acc_device_not_host:
      ptbl = OACC_acc_devs;
      n = OACC_nacc_devs;
      break;
    }

  LOCK_AQUIRE(OACC_dev_lock);
  if(ptbl != NULL && n > 0)
    {
      for(i = 0 ; i < n; ++i)
        {
          OACC_dev_fini(ptbl[i]);
        }
    }
  LOCK_RELEASE(OACC_dev_lock);
}

void*
acc_malloc(size_t size)
{
  OACC_CHECK_INIT
  OACC_CHECK_DEV_INIT(OACC_curr_dev[OACC_curr_num])
  return OACC_alloc_on_device(NULL, size,
    OACC_curr_dev[OACC_curr_num], 0)->data_ptr;
}

void
acc_free(void* ptr)
{
  OACC_CHECK_INIT
  OACC_CHECK_DEV_INIT(OACC_curr_dev[OACC_curr_num])
  OACC_free_device_mem(ptr, OACC_curr_dev[OACC_curr_num]);
}

int
acc_async_test(int cookie)
{
  struct OACC_queue_data *q = NULL;
  OACC_CHECK_INIT
  q = OACC_find_queue(cookie, NULL);
  if(q == NULL)
    {
      return 1;
    }
  return OACC_test_queue(q) == OACC_QS_COMPLETE;
}

int
acc_async_test_all()
{
  OACC_CHECK_INIT
  return OACC_test_all_global_queues();
}

void
acc_async_wait(int cookie)
{
  struct OACC_queue_data *q = NULL, *qprev = NULL;
  OACC_CHECK_INIT
  q = OACC_find_queue(cookie, &qprev);
  if(q != NULL)
    {
      OACC_unlink_queue(&OACC_async.named, qprev);
      OACC_wait_queue(q);
    }
}

void
acc_async_wait_all()
{
  OACC_CHECK_INIT
  OACC_wait_all_global_queues();
}

int
acc_on_device(acc_device_t devt)
{
  int ret = 0;
  OACC_CHECK_INIT
  LOCK_AQUIRE(OACC_dev_lock);
  ret = OACC_curr_dev == OACC_host_devs;
  LOCK_RELEASE(OACC_dev_lock);
  return ret;
}


#ifdef __cplusplus
}
#endif
