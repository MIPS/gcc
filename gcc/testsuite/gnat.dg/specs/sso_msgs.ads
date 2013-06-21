-- { dg-do compile }
-- { dg-options "-gnat95" }

with System;

package SSO_Msgs is

   type R is record  -- { dg-error "inconsistent with bit order" }
      B : Boolean;
   end record;
   for R'Bit_Order use System.Low_Order_First;
   for R'Scalar_Storage_Order use System.High_Order_First;
   for R use record
      B at 0 range 0 .. 1;
   end record;

   type RW is record
      B : Boolean;
   end record;
   for RW'Bit_Order use System.Low_Order_First;  -- { dg-warning "no effect|no component clause" }
   for RW'Scalar_Storage_Order use System.Low_Order_First;  -- { dg-warning "no component clause" }

end SSO_Msgs;
