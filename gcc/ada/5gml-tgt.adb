--              Copyright (C) 2003, Ada Core Technologies, Inc.             --
   --------------------------------
   -- Linker_Library_Path_Option --
   --------------------------------

   function Linker_Library_Path_Option return String_Access is
   begin
      return new String'("-Wl,-rpath,");
   end Linker_Library_Path_Option;

