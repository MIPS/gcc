if { [istarget "mips*-*-linux*"] || [istarget "m68k*-*-linux*"] } {
    set torture_execute_xfail [istarget]
}
return 0
