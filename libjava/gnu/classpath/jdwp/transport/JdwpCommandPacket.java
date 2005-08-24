Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.
  protected int myToBytes (byte[] bytes, int index)
    // Need to add command set & command
    int i = 0;
    bytes[index + i++] = getCommandSet ();
    bytes[index + i++] = getCommand ();

    return i;
