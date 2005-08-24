Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.
    _errorCode = errorCode;
  protected int myToBytes (byte[] bytes, int index)
  {
    // Need to add error code
    int i = 0;
    bytes[index + i++] = (byte) (getErrorCode () >>> 8);
    bytes[index + i++] = (byte) getErrorCode ();
    
    return i;
