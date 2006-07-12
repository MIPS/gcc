import gnu.javax.net.ssl.provider.Extension;
import gnu.javax.net.ssl.provider.ExtensionList;
import gnu.javax.net.ssl.provider.Extension.Type;

import java.nio.ByteBuffer;
import java.util.Arrays;

class testExtensionList
{
  public static void main (String[] argv)
  {
    try
      {
        check ();
      }
    catch (Exception x)
      {
        System.out.println ("FAIL: caught exception " + x);
        x.printStackTrace (System.err);
      }
  }
  
  static void check () throws Exception
  {
    ByteBuffer buf = ByteBuffer.allocate(4096);
    ExtensionList list = new ExtensionList(buf);
    
    list.setLength (12);
    // Max fragment length of 2^9-1
    list.set (0, Extension.Type.MAX_FRAGMENT_LENGTH, 1); // 2 + 2 + 1
    list.get (0).setValue (new byte[] { 1 });
    // Zero-length server name.
    list.set (1, Extension.Type.SERVER_NAME, 3); // 2 + 2 + 3
    list.get(1).setValue(new byte[3]);
    // This is a valid extension list now.
    
    if (list.length () == 12)
      System.out.println ("PASS: length");
    else
      System.out.println ("FAIL: length");
    
    Extension e = list.get (0);
    if (e.type() == Extension.Type.MAX_FRAGMENT_LENGTH)
      System.out.println ("PASS: get(0).type()");
    else
      System.out.println ("FAIL: get(0).type()");
    if (Arrays.equals (e.valueBytes(), new byte[] { 1 }))
      System.out.println ("PASS: get(0).value()");
    else
      System.out.println ("FAIL: get(0).value()");
    
    e = list.get(1);
    if (e.type () == Extension.Type.SERVER_NAME)
      System.out.println ("PASS: get(1).type()");
    else
      System.out.println ("FAIL: get(1).type()");
    if (Arrays.equals(e.valueBytes(), new byte[3]))
      System.out.println ("PASS: get(1).value()");
    else
      System.out.println ("FAIL: get(1).value()");
    
    System.err.println (list);
  }
}