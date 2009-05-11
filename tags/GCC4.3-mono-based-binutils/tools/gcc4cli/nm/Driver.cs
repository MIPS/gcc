
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;
using gcc4cli.util;

namespace gcc4cli.nm {

    public class Driver {

        static readonly string prog_name = "GCC4CLI nm";

        public static void Main (string [] args)
        {
            Driver drv = new Driver ();
            drv.Parse (new Queue (args));
            drv.Run ();
            Environment.Exit (0);
        }

        Queue files = new Queue ();

        bool printFilename = false;
        bool printDef = true;
        bool printUndef = true;
        bool printSpecial = false;
//         bool printSize = false;

        Driver ()
        {
        }

        void Parse (Queue q)
        {
            if (q.Count == 0)
                Error (null);

            while (q.Count > 0) {
                string token = (string) q.Dequeue ();

                if (token [0] == '-') {
                    switch (token) {
                    case "-A":
                    case "-o":
                    case"--print-file-name":
                        printFilename = true;
                        break;
                    case "-u":
                    case "--undefined-only":
                        printDef   = false;
                        printUndef = true;
                        break;
                    case "--defined-only":
                        printDef   = true;
                        printUndef = false;
                        break;
//                     case "-S":
//                     case "--print-size":
//                         printSize = true;
//                         break;
                    case "--special-syms":
                        printSpecial = true;
                        break;
                    case "-h":
                    case "--help":
                        Usage ();
                        Environment.Exit (0);
                        break;
                    default:
                        Error ("Unrecognized option '" + token + "'");
                        break;
                    }
                } else {
                    files.Enqueue (token);
                }

            }

            if (files.Count == 0)
                files.Enqueue ("a.out");

            if (files.Count > 1)
                printFilename = true;
        }

        void Run ()
        {
            while (files.Count > 0) {
                string filename = (string) files.Dequeue ();
                if (printFilename)
                    Console.WriteLine (filename + ":");
                try {
                    CFile myfile = CFileFactory.LoadCFile (filename);

                    if (myfile is CStaticLibrary) {
                        CStaticLibrary lib = myfile as CStaticLibrary;
                        foreach (CObject obj in lib.Objects) {
                            Console.WriteLine (obj.Name + ".o:");
                            dump (obj.Name, obj);
                        }
                    } else {
                        dump (filename, myfile);
                    }
                } catch (CFileException e) {
                    Console.WriteLine ("Error : {0}", e.Message);
                    Environment.Exit (1);
                } catch (Exception e) {
                    Console.WriteLine (e);
                    Error ("Fatal error in " + prog_name);
                }
                Console.WriteLine ();
            }
        }

        static void Usage ()
        {
            Console.WriteLine (prog_name);
            Console.WriteLine ("nm [options] <obj_files>");

            Console.WriteLine ("   -o                    print file names");
            Console.WriteLine ("   -A                    print file names");
            Console.WriteLine ("   --print-file-name     print file names");
            Console.WriteLine ("   -u                    print only undefined symbols");
            Console.WriteLine ("   --undefined-only      print only undefined symbols");
            Console.WriteLine ("   --defined-only        print only defined symbols");
            Console.WriteLine ("   -S                    Print Symbol sizes");
            Console.WriteLine ("   --print-size          Print Symbol sizes");
            Console.WriteLine ("   --special-syms        print special symbols");
            Console.WriteLine ("   -h                    this message");
            Console.WriteLine ("");
            Console.WriteLine ("   Sample: nm a.exe input.dll");
        }

        static void Error (string Message)
        {
            if (Message != null)
                Console.WriteLine ("Error : {0}", Message);
            Usage ();
            Environment.Exit (1);
        }

        void dump (string filename, CFile obj)
        {
            if (printDef) {
                foreach (string name in obj.DefinedSymbols ()) {
                    switch (name) {
                    case ".start":
                    case ".init":
                    case ".fini":
                        if (printSpecial) {
                            printLine (0, "T", name);
                        }
                        break;
                    default:
                        IMemberDefinition member =  obj.GetDefMember (name);
                        if (member is MethodDefinition) {
                            printLine (0, "T", name);
                        } else if (member is FieldDefinition) {
                            printLine (0, "D", name);
                        }
                        break;
                    }
                }
                foreach (string name in obj.PrivateSymbols ()) {
                    switch (name) {
                    case "COBJ?init":
                        if (printSpecial) {
                            printLine (0, "T", name);
                        }
                        break;
                    default:
                        IMemberDefinition member =  obj.GetDefMember (name);
                        if (member is MethodDefinition) {
                            printLine (0, "T", name);
                        } else if (member is FieldDefinition) {
                            printLine (0, "D", name);
                        }
                        break;
                    }
                }
            }
            if (printUndef) {
                foreach (string name in obj.UndefinedSymbols ()) {
                    printLine (0, "U", name);
                }
            }
            if (printSpecial) {
                foreach (string name in obj.RefSymbols ()) {
                    printLine (0, "U", name);
                }
            }
        }

        void printLine(int addr, string type, string name)
        {
            Console.WriteLine ("\t0000x\t{1}\t{2}", addr, type, name);
        }
    }
}
