--         Copyright (C) 1992-2003, Free Software Foundation, Inc.          --
with Unchecked_Conversion;
   function To_Address is new Unchecked_Conversion (Task_ID, System.Address);

      T       : Task_ID := Self;
         Result := pthread_sigmask (SIG_UNBLOCK,
           Unblocked_Signal_Mask'Unchecked_Access, Old_Set'Unchecked_Access);
      function Thread_Body_Access is new
        Unchecked_Conversion (System.Address, Thread_Body);

