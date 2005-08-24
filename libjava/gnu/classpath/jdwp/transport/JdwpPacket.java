Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.
  // Put subclass information into bytes
  protected abstract int myToBytes (byte[] bytes, int index);
  // Convert this packet to it byte representation (ready to send on the wire)
  // NOTE: All integers should be big-endian.
  public byte[] toBytes ()
    // Allocate a new array to hold contents of packet
    byte[] bytes = new byte[length];
	
    int i = 0;
    //
    // Packet layout: length, id, flags, packet-specific, data (optional)
    //

    // length
    bytes[i++] = (byte) (length >>> 24);
    bytes[i++] = (byte) (length >>> 16);
    bytes[i++] = (byte) (length >>> 8);
    bytes[i++] = (byte) length;

    // id
    bytes[i++] = (byte) (getId () >>> 24);
    bytes[i++] = (byte) (getId () >>> 16);
    bytes[i++] = (byte) (getId () >>> 8);
    bytes[i++] = (byte) getId ();
    bytes[i++] = getFlags ();
    i += myToBytes (bytes, i);
    if (data.length > 0 && i < length)
      {
	// Would it pay to be over cautious?
	System.arraycopy (data, 0, bytes, i, data.length);
      }

    return bytes;
