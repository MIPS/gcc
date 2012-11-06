set torture_eval_before_compile {
  # If we have an elf target (most likely newlib without shared libc)
  if { [istarget "*-*-elf*"] } {
    #We cannot use non fat LTO objects to test when there are two symbols are the same
    if {[string match {*-fno-fat-lto-objects*} "$option"]} {
      continue
    }
  }
}

return 0

