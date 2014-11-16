# This option enables -fsanitize=address for stage2 and stage3.

# Suppress LeakSanitizer and odr-violation in bootstrap.
export ASAN_OPTIONS="detect_leaks=0 detect_odr_violation=0"

STAGE2_CFLAGS += -fsanitize=address
STAGE3_CFLAGS += -fsanitize=address
POSTSTAGE1_LDFLAGS += -fsanitize=address -static-libasan \
		      -B$$r/prev-$(TARGET_SUBDIR)/libsanitizer/ \
		      -B$$r/prev-$(TARGET_SUBDIR)/libsanitizer/asan/ \
		      -B$$r/prev-$(TARGET_SUBDIR)/libsanitizer/asan/.libs
