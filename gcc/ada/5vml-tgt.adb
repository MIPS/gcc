with Ada.Text_IO;             use Ada.Text_IO;
   Macro_Name   : constant String := "macro";
   --  Options to use when invoking gcc to build the dynamic library

   No_Start_Files : aliased String := "-nostartfiles";

   VMS_Options : Argument_List :=
     (No_Start_Files'Access, null);
            return Get_Name_String (Symbol_Data.Symbol_File);
      For_Linker_Opt : constant String_Access :=
                         new String'("--for-linker=" & Opt_File_Name);
      VMS_Options (VMS_Options'First + 1) := For_Linker_Opt;
            Macro_File_Name : constant String := Lib_Filename & "$init.mar";
            Macro_File      : Ada.Text_IO.File_Type;
            --  The command to invoke the macro-assembler on the generated
               Create (Macro_File, Out_File, Macro_File_Name);
               Put_Line (Macro_File, ASCII.HT & ".EXTRN LIB$INITIALIZE");
               Put_Line (Macro_File, ASCII.HT & ".EXTRN " & Init_Proc);
               Put_Line
                 (Macro_File,
                  ASCII.HT & ".PSECT LIB$INITIALIZE USR,GBL,NOEXE,NOWRT,LONG");
               Put_Line (Macro_File, ASCII.HT & ".ADDRESS " & Init_Proc);
               Put_Line (Macro_File, ASCII.HT & ".END");
               Close (Macro_File);
            exception
               when others =>
         Options_2   => Opts (Opts'First .. Last_Opt) &
   --------------------------------
   -- Linker_Library_Path_Option --
   --------------------------------

   function Linker_Library_Path_Option return String_Access is
   begin
      return null;
   end Linker_Library_Path_Option;

