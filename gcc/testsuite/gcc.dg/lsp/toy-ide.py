# A toy IDE implemented in PyGTK

import pygtk
pygtk.require('2.0')
import gtk
import gtksourceview2
#help(gtksourceview2)

import lsp

class ToyIde:
    def delete_event(self, widget, event, data=None):
        return False

    def destroy(self, widget, data=None):
        gtk.main_quit()

    def quit_cb(self, b):
        gtk.main_quit()

    def get_cursor_position(self):
        """Get the position of the cursor within the buffer
        as an lsp.Position"""
        mark = self.buf.get_insert()
        print(mark)
        iter = self.buf.get_iter_at_mark(mark)
        print(iter)

        print('line: %r' % iter.get_line()) # 0-based line
        print('line_offset: %r' % iter.get_line_offset()) # 0-based offse

        return lsp.Position(iter.get_line(), iter.get_line_offset())

    def goto_definition_cb(self, b):
        print "goto_definition_cb"

        # FIXME: need to sort out paths between the LSP server and client
        FILENAME = '../../src/gcc/testsuite/gcc.dg/lsp/test.c'

        locs = self.lsp.goto_definition(lsp.TextDocumentIdentifier(FILENAME),
                                        self.get_cursor_position())
        print(locs)
        if len(locs) == 1:
            loc = locs[0]
            # Both lsp.Range and gtk.TextBuffer.select_range are inclusive
            # on the start, exclusive on the end-point
            self.buf.select_range(
                self.buf.get_iter_at_line_offset(loc.range.start.line,
                                                 loc.range.start.character),
                self.buf.get_iter_at_line_offset(loc.range.end.line,
                                                 loc.range.end.character))

    def __init__(self, path):
        self.window = gtk.Window(gtk.WINDOW_TOPLEVEL)
        self.window.connect("delete_event", self.delete_event)
        self.window.connect("destroy", self.destroy)
        self.window.set_size_request(640, 480)
        vbox = gtk.VBox()
        self.window.add(vbox)

        uimanager = gtk.UIManager()
        accelgroup = uimanager.get_accel_group()
        self.window.add_accel_group(accelgroup)

        actiongroup = gtk.ActionGroup('UIManagerExample')
        self.actiongroup = actiongroup
        actiongroup.add_actions([('Quit', gtk.STOCK_QUIT, '_Quit', None,
                                  'Quit', self.quit_cb),
                                 ('File', None, '_File'),
                                 ('Test', None, '_Test'),
                                 ('GotoDefinition', None, 'Goto _Definition',
                                  None, 'Goto the definition of this thing',
                                  self.goto_definition_cb)
                             ])
        actiongroup.get_action('Quit').set_property('short-label', '_Quit')
        uimanager.insert_action_group(actiongroup, 0)

        merge_id = uimanager.add_ui_from_string("""
 <ui>
    <menubar name="MenuBar">
      <menu action="File">
        <menuitem action="Quit"/>
      </menu>
      <menu action="Test">
        <menuitem action="GotoDefinition"/>
      </menu>
    </menubar>
  </ui>
""")
        menubar = uimanager.get_widget('/MenuBar')
        vbox.pack_start(menubar, False)

        self.buf = gtksourceview2.Buffer()

        with open(path) as f:
            text = f.read()
        self.buf.set_text(text)

        self.sv = gtksourceview2.View(buffer=self.buf)
        self.sv.set_show_line_numbers(True)

        vbox.add(self.sv)
        self.window.show_all()

        self.lsp = lsp.Proxy('http://localhost:4000/jsonrpc')

    def main(self):
        gtk.main()

ide = ToyIde("gcc/testsuite/gcc.dg/lsp/test.c")
ide.main()
