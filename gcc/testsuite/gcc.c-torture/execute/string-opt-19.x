# We can't replace the call to stpcpy with an inlined memcpy due
# to limitations in how we gimplify builtins.  Expect execution
# failures at all optimization levels until we can fix this.
set torture_execute_xfail [istarget]
return 0
