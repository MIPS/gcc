/* FIXME lto: This test is only relevant if the plugin is used. */

void
_cairo_clip_path_reference () {
  int a;
  __sync_fetch_and_add(&a, 1);
}

int main(void) {
  return 0;
}
