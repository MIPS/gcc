--              Copyright (C) 2003, Ada Core Technologies, Inc.             --
with Ada.Text_IO; use Ada.Text_IO;
with Namet;  use Namet;
with Osint;  use Osint;
with Output; use Output;
                  File : Ada.Text_IO.File_Type;
                    (File, In_File,
                     Include_Dir_Default_Prefix & "/s-osinte.ads");
            Lib_Dir : constant String :=
              Get_Name_String (Projects.Table (Project).Library_Dir);
              Get_Name_String (Projects.Table (Project).Library_Name);
   --------------------------------
   -- Linker_Library_Path_Option --
   --------------------------------

   function Linker_Library_Path_Option return String_Access is
   begin
      --  On AIX, any path specify with an -L switch is automatically added
      --  to the library path. So, nothing is needed here.

      return null;
   end Linker_Library_Path_Option;

