/* JTextComponent.java --
   Copyright (C) 2002, 2004  Free Software Foundation, Inc.

This file is part of GNU Classpath.

GNU Classpath is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

GNU Classpath is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with GNU Classpath; see the file COPYING.  If not, write to the
Free Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
02111-1307 USA.

Linking this library statically or dynamically with other modules is
making a combined work based on this library.  Thus, the terms and
conditions of the GNU General Public License cover the whole
combination.

As a special exception, the copyright holders of this library give you
permission to link this library with independent modules to produce an
executable, regardless of the license terms of these independent
modules, and to copy and distribute the resulting executable under
terms of your choice, provided that you also meet, for each linked
independent module, the terms and conditions of the license of that
module.  An independent module is a module which is not derived from
or based on this library.  If you modify this library, you may extend
this exception to your version of the library, but you are not
obligated to do so.  If you do not wish to do so, delete this
exception statement from your version. */

package javax.swing.text;

import java.awt.AWTEvent;
import java.awt.Component;
import java.awt.Dimension;
import java.awt.Image;
import java.awt.Insets;
import java.awt.Point;
import java.awt.Rectangle;
import java.awt.event.InputMethodListener;

import javax.accessibility.Accessible;
import javax.accessibility.AccessibleContext;
import javax.accessibility.AccessibleRole;
import javax.accessibility.AccessibleStateSet;
import javax.accessibility.AccessibleText;
import javax.swing.Icon;
import javax.swing.JComponent;
import javax.swing.JViewport;
import javax.swing.KeyStroke;
import javax.swing.Scrollable;
import javax.swing.UIManager;
import javax.swing.event.CaretEvent;
import javax.swing.event.CaretListener;
import javax.swing.event.DocumentEvent;
import javax.swing.event.DocumentListener;
import javax.swing.plaf.TextUI;


public abstract class JTextComponent extends JComponent
  implements Scrollable, Accessible
{
//    public class AccessibleJTextComponent extends AccessibleJComponent
//      implements AccessibleText, CaretListener, DocumentListener,
//                 AccessibleAction, AccessibleEditableText
//    {
//    }

  /**
   * AccessibleJTextComponent
   */
  public class AccessibleJTextComponent extends AccessibleJComponent
    implements AccessibleText, CaretListener, DocumentListener
  {
    private static final long serialVersionUID = 7664188944091413696L;

    /**
     * caretPos
     */
    int caretPos;

    /**
     * Constructor AccessibleJTextComponent
     * @param component TODO
     */
    public AccessibleJTextComponent()
    {
    }

    /**
     * getCaretPosition
     * @return int
     */
    public int getCaretPosition()
    {
      return 0; // TODO
    }

    /**
     * getSelectedText
     * @return String
     */
    public String getSelectedText()
    {
      return null; // TODO
    }

    /**
     * getSelectionStart
     * @return int
     */
    public int getSelectionStart()
    {
      return 0; // TODO
    }

    /**
     * getSelectionEnd
     * @return int
     */
    public int getSelectionEnd()
    {
      return 0; // TODO
    }

    /**
     * caretUpdate
     * @param value0 TODO
     */
    public void caretUpdate(CaretEvent value0)
    {
      // TODO
    }

    /**
     * getAccessibleStateSet
     * @return AccessibleStateSet
     */
    public AccessibleStateSet getAccessibleStateSet()
    {
      return null; // TODO
    }

    /**
     * getAccessibleRole
     * @return AccessibleRole
     */
    public AccessibleRole getAccessibleRole()
    {
      return null; // TODO
    }

    /**
     * getAccessibleText
     * @return AccessibleText
     */
    public AccessibleText getAccessibleText()
    {
      return null; // TODO
    }

    /**
     * insertUpdate
     * @param value0 TODO
     */
    public void insertUpdate(DocumentEvent value0)
    {
      // TODO
    }

    /**
     * removeUpdate
     * @param value0 TODO
     */
    public void removeUpdate(DocumentEvent value0)
    {
      // TODO
    }

    /**
     * changedUpdate
     * @param value0 TODO
     */
    public void changedUpdate(DocumentEvent value0)
    {
      // TODO
    }

    /**
     * getIndexAtPoint
     * @param value0 TODO
     * @return int
     */
    public int getIndexAtPoint(Point value0)
    {
      return 0; // TODO
    }

    /**
     * getRootEditorRect
     * @return Rectangle
     */
    Rectangle getRootEditorRect()
    {
      return null;
    }

    /**
     * getCharacterBounds
     * @param value0 TODO
     * @return Rectangle
     */
    public Rectangle getCharacterBounds(int value0)
    {
      return null; // TODO
    }

    /**
     * getCharCount
     * @return int
     */
    public int getCharCount()
    {
      return 0; // TODO
    }

    /**
     * getCharacterAttribute
     * @param value0 TODO
     * @return AttributeSet
     */
    public AttributeSet getCharacterAttribute(int value0)
    {
      return null; // TODO
    }

    /**
     * getAtIndex
     * @param value0 TODO
     * @param value1 TODO
     * @return String
     */
    public String getAtIndex(int value0, int value1)
    {
      return null; // TODO
    }

    /**
     * getAfterIndex
     * @param value0 TODO
     * @param value1 TODO
     * @return String
     */
    public String getAfterIndex(int value0, int value1)
    {
      return null; // TODO
    }

    /**
     * getBeforeIndex
     * @param value0 TODO
     * @param value1 TODO
     * @return String
     */
    public String getBeforeIndex(int value0, int value1)
    {
      return null; // TODO
    }
  }

  public static class KeyBinding
  {
    public KeyStroke key;
    public String actionName;

    public KeyBinding(KeyStroke key, String actionName)
    {
      this.key = key;
      this.actionName = actionName;
    }
  }

  private static final long serialVersionUID = -8796518220218978795L;
  
  public static final String DEFAULT_KEYMAP = "default";
  public static final String FOCUS_ACCELERATOR_KEY = "focusAcceleratorKey";

  private Document doc;
  private int icon_gap;
  private Icon icon;
  private int align;

  public JTextComponent()
  {
    enableEvents(AWTEvent.KEY_EVENT_MASK);
    updateUI();
  }

  public void setDocument(Document s)
  {
    doc = s;
    revalidate();
    repaint();
  }

  public Document getDocument()
  {
    if (doc == null)
      System.out.println("doc == null !!!");
    return doc;
  }

  /**
   * Verify that key is a legal value for the horizontalAlignment properties.
   */
  protected int checkHorizontalKey(int key, String message)
  {
    return 0;
  }

  /**
   * Verify that key is a legal value for the verticalAlignment
   * or verticalTextPosition properties.
   */
  protected int checkVerticalKey(int key, String message)
  {
    return 0;
  }

  /**
   * Get the AccessibleContext of this object
   */
  public AccessibleContext getAccessibleContext()
  {
    return null;
  }

  public Icon getDisabledIcon()
  {
    return null;
  }

  /**
   * Return the keycode that indicates a mnemonic key.
   */
  public int getDisplayedMnemonic()
  {
    return 0;
  }

  /**
   * Returns the alignment of the label's contents along the X axis.
   */
  public int getHorizontalAlignment()
  {
    return 0;
  }

  /**
   * Returns the horizontal position of the label's text,
   * relative to its image.
   */
  public int getHorizontalTextPosition()
  {
    return 0;
  }

  public Icon getIcon()
  {
    return icon;
  }

  public int getIconTextGap()
  {
    return icon_gap;
  }

  /**
   * Get the component this is labelling.
   */
  Component getLabelFor()
  {
    return null;
  }

  public Insets getMargin()
  {
    // FIXME: Not implemented.
    return null;
  }

  public void setText(String text)
  {
    try
      {
	getDocument().remove(0, doc.getLength());
	getDocument().insertString(0, text, null);
      }
    catch (BadLocationException e)
      {
      }
  }

  public String getText()
  {
    return getDocument().getText(0, getDocument().getLength());
  }

  /**
   * Returns a string that specifies the name of the l&amp;f class
   * that renders this component.
   *
   * @return the string "TextComponentUI"
   */
  public String getUIClassID()
  {
    return "TextComponentUI";
  }

  /**
   * Returns the alignment of the label's contents along the Y axis.
   */
  public int getVerticalAlignment()
  {
    return 0;
  }

  /**
   * Returns the vertical position of the label's text, relative to its image.
   */
  public int getVerticalTextPosition()
  {
    return 0;
  }

  /**
   * This is overriden to return false if the current Icon's Image is not equal to the passed in Image img.
   */
  public boolean imageUpdate(Image img, int infoflags, int x, int y, int w,
                             int h)
  {
    return (img == icon);
  }

  /**
   * Returns a string representation of this JTextComponent.
   */
  protected String paramString()
  {
    return "JTextComponent";
  }

  /**
   * Set the icon to be displayed if this JTextComponent is "disabled" (JTextComponent.setEnabled(false)).
   */
  void setDisabledIcon(Icon disabledIcon)
  {
  }

  /**
   * Specifies the displayedMnemonic as a char value.
   */
  void setDisplayedMnemonic(char aChar)
  {
  }

  /**
   * Specify a keycode that indicates a mnemonic key.
   */
  void setDisplayedMnemonic(int key)
  {
  }

  /**
   * Sets the alignment of the label's contents along the X axis.
   */
  void setHorizontalAlignment(int alignment)
  {
  }

  /**
   * Sets the horizontal position of the label's text, relative to its image.
   */
  void setHorizontalTextPosition(int textPosition)
  {
  }

  /**
   * Defines the icon this component will display.
   */
  void setIcon(Icon icon)
  {
  }

  /**
   * If both the icon and text properties are set, this property defines the space between them.
   */
  public void setIconTextGap(int iconTextGap)
  {
  }

  /**
   * Set the component this is labelling.
   */
  public void setLabelFor(Component c)
  {
  }

  /**
   * Sets the alignment of the label's contents along the Y axis.
   */
  public void setVerticalAlignment(int alignment)
  {
  }

  /**
   * Sets the vertical position of the label's text, relative to its image.
   */
  public void setVerticalTextPosition(int textPosition)
  {
  }

  public TextUI getUI()
  {
    return (TextUI) UIManager.getUI(this);
  }

  public void updateUI()
  {
    setUI(getUI());
  }

  public Dimension getPreferredScrollableViewportSize()
  {
    return null;
  }

  public int getScrollableUnitIncrement(Rectangle visible, int orientation,
                                        int direction)
  {
    return 0;
  }

  public int getScrollableBlockIncrement(Rectangle visible, int orientation,
                                         int direction)
  {
    return 0;
  }

  public boolean getScrollableTracksViewportHeight()
  {
    if (getParent() instanceof JViewport)
      return ((JViewport) getParent()).getHeight() > getPreferredSize().height;

    return false;
  }

  public boolean getScrollableTracksViewportWidth()
  {
    if (getParent() instanceof JViewport)
      return ((JViewport) getParent()).getWidth() > getPreferredSize().width;

    return false;
  }
}
