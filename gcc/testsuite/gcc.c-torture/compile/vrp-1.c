/* PR/11559 */

void
get_capabilities (int i)
{
  int n, len;

  n = 0;
retry_mode_sense_10:
  len = i;
  if (len > 8)
    return;
  if (n < len)
    {
      n = len;
      goto retry_mode_sense_10;
    }
}
