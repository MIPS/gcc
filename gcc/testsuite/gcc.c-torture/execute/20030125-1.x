# Only Linux does inlclude all c99 functions at the moment.
if { ! [istarget "*linux*"] } { return 1 }
if { [istarget "*-wrs-linux*"] } {
    set target [target_info name]
    set multilib_flags " [board_info $target multilib_flags] "
    if [string match "* -muclibc *" $multilib_flags] {
	return 1
    }
}
return 0
