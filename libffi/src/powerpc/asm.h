   PowerPC Assembly glue.
  .section ".data";							      \
  stw   %r0,4(%r1);							      \
  lis   %r11,0b@ha;							      \
  stw   %r0,4(%r1);							      \
