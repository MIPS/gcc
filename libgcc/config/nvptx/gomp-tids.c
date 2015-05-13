/* Each gang consists of 'worker' threads.  Each worker has 'vector'
   threads.

   gang, worker and vector mapping functions:

   *tid (0) => vector dimension
   *tid (1) => worker dimension
   *ctaid (0) = gang dimension

   FIXME: these functions assume that the gang, worker and vector parameters
   are 0 or 1.  To generalize these functions, we should use -1 to indicate,
   say, that a gang clause was used without its optional argument.  In this
   case, gang should correspond to ctaid(0), i.e., the num_gangs parameter
   passed to cuLaunchKernel.

   tid = [0, ntid-1]
   ntid = [1...threads_per_dimension]
*/

int __attribute__ ((used))
GOACC_get_num_threads (int gang, int worker, int vector)
{
  int vsize = vector * __builtin_GOACC_ntid (0);
  int wsize = worker * __builtin_GOACC_ntid (1);
  int gsize = gang * __builtin_GOACC_nctaid (0);
  int size = 1;

  if (vector)
    size *= __builtin_GOACC_ntid (0);

  if (worker)
    size *= __builtin_GOACC_ntid (1);

  if (gang)
    size *= __builtin_GOACC_nctaid (0);

  return size;
}

int __attribute__ ((used))
GOACC_get_thread_num (int gang, int worker, int vector)
{
  int tid = 0;
  int ws = __builtin_GOACC_ntid (1);
  int vs = __builtin_GOACC_ntid (0);
  int gid = __builtin_GOACC_ctaid (0);
  int wid = __builtin_GOACC_tid (1);
  int vid = __builtin_GOACC_tid (0);

  if (gang && worker && vector)
    tid = gid * ws * vs + vs * wid + vid;
  else if (gang && !worker && vector)
    tid = vs * gid + vid;
  else if (gang && worker && !vector)
    tid = ws * gid + wid;
  else if (!gang && worker && vector)
    tid = vs * wid + vid;
  else if (!gang && !worker && vector)
    tid = vid;
  else if (!gang && worker && !vector)
    tid = wid;
  else if (gang && !worker && !vector)
    tid = gid;

  return tid;
}
