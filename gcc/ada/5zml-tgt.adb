--              Copyright (C) 2003 Free Software Foundation, Inc.           --
   --------------------------------
   -- Linker_Library_Path_Option --
   --------------------------------

   function Linker_Library_Path_Option return String_Access is
   begin
      return new String'("-Wl,-R,");
   end Linker_Library_Path_Option;

