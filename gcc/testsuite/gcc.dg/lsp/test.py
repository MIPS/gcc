from lsp import Proxy, TextDocumentIdentifier, Position, Range, Location

def main():
    # This assumes that we're running this:
    # (hardcoding the particular source file for now):
    # ./xgcc -B. -c ../../src/gcc/testsuite/gcc.dg/lsp/test.c \
    #    -flsp=4000 -fblt -wrapper gdb,--args

    url = "http://localhost:4000/jsonrpc"
    proxy = Proxy(url)

    # FIXME: filename/uri (currently a particular relative location)
    FILENAME = '../../src/gcc/testsuite/gcc.dg/lsp/test.c'

    # Ask for the location of a usage of "struct foo" (0-based lines)
    result = proxy.goto_definition(TextDocumentIdentifier(FILENAME),
                                   Position(8, 16))

    # We expect to get back the location of where "struct foo" is defined
    print(result)
    # FIXME: filename/uri (currently a particular relative location)
    assert result == [Location(FILENAME,
                               Range(Position(2, 0), Position(6, 1)))]
    for loc in result:
        loc.dump("definition")

if __name__ == "__main__":
    main()
