
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;
using gcc4cli.util;

namespace gcc4cli.ld {

    public class Driver {

        static readonly string prog_name = "GCC4CLI Linker";

        string outputPath = null;
        string outputName = null;
        bool outputIsExecutable = true;
        bool incrementalLink = false;
        bool multithreaded = false;

        Queue files = new Queue ();

        public string OutputPath {
            set {
                outputPath = value;
                if (outputName ==  null)
                    OutputName = Path.GetFileNameWithoutExtension (value);
                if (Path.GetExtension(value) == ".dll")
                    OutputIsExecutable = false;
            }
        }
        public string OutputName {
            set { outputName = value; }
        }

        public bool OutputIsExecutable {
            set { outputIsExecutable = value; }
        }

        public bool IncrementalLink {
            set {
                incrementalLink = value;
                if (value)
                    OutputIsExecutable = false;
            }
        }

        public bool OutputIsMThreaded {
            set { multithreaded = value; }
        }

        Driver ()
        {
        }

        public static void Main (string [] args)
        {
            Driver drv = new Driver ();
            try {
                drv.Parse (new Queue (args));
                drv.Run ();
            } catch (CFileException e) {
                Console.WriteLine ("Error: {0}", e.Message);
                Environment.Exit (1);
            } catch (Exception e) {
                Console.WriteLine (e);
                Error ("Fatal error in " + prog_name);
            }
            Environment.Exit (0);
        }

        void Parse (Queue q)
        {
            while (q.Count > 0) {
                string token = (string) q.Dequeue ();

                if (token [0] == '-') {
                    if (token.Length > 2
                        && (token [1] == 'L' || token [1] == 'l' || token [1] == 'v')) {
                        switch (token[1]) {
                        case 'L':
                            AssemblyResolver.DefaultResolver.AddSearchDirectory (token.Substring (2));
                            break;

                        case 'l':
                            AddLibrary (token.Substring (2));
                            break;

                        case 'v':
                            Tracer.SetVerbosity (Int32.Parse (token.Substring (2)));
                            break;

                        default:
                            Error ("Unrecognized option '" + token + "'");
                            break;
                        }
                    } else {
                        switch (token) {
                        case "-o":
                        case "-out":
                            OutputPath = (string) q.Dequeue ();
                            break;
                        case "-e":
                        case "-exe":
                            OutputIsExecutable = true;
                            break;
                        case "-d":
                        case "-dll":
                        case "--shared":
                            OutputIsExecutable = false;
                            break;
                        case "-r":
                            IncrementalLink = true;
                            break;
                        case "-v":
                            Tracer.IncreaseVerbosity();
                            break;
                        case "-L":
                            AssemblyResolver.DefaultResolver.AddSearchDirectory ((string) q.Dequeue ());
                            break;
                        case "-l":
                            AddLibrary ((string) q.Dequeue ());
                            break;
                        case "-mt":
                            OutputIsMThreaded = true;
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
                    }
                } else {
                    // we assume is an object file to link
                    AddObject (token);
                }

            }

            if (files.Count == 0)
                Error ("At least one object file needed");
        }

        void AddObject (string asm_name)
        {
                files.Enqueue (CFileFactory.LoadCFile (asm_name));
        }

        void AddLibrary (string lib_name)
        {
            if (CFileFactory.CheckIsCLibrary (lib_name))
                files.Enqueue (CFileFactory.LoadCLibrary (lib_name));
            else
                Tracer.Trace ("Skip library " + lib_name);
        }

        void Run ()
        {
            CFileBuilder ofile;

            if (outputPath == null)
                OutputPath = "a.out";

            if (outputIsExecutable)
                ofile = CFileFactory.CreateCExecutable (outputName, multithreaded);
            else if (incrementalLink)
                ofile = CFileFactory.CreateCObject (outputName);
            else
                ofile = CFileFactory.CreateCDynamicLibrary (outputName, multithreaded);

            ofile.StartBuilding ();
            while (files.Count > 0) {
                ofile.Add ((CFile)files.Dequeue ());
            }
            ofile.EndBuilding ();

            ofile.Save (outputPath);
            if (outputIsExecutable) {
                //
                // The constant 0x80000000 is internal to Mono, it means `make executable'
                //
                File.SetAttributes (outputPath, (System.IO.FileAttributes) (unchecked ((int) 0x80000000)));
            }
        }

        static void Usage ()
        {
            Console.WriteLine (prog_name);
            Console.WriteLine ("ld [options] <obj_files>");

            Console.WriteLine ("   -o <filename>        Specify the output file");
            Console.WriteLine ("   -e | -exe            output is an executable");
            Console.WriteLine ("   -d | -dll | --shared output is a dll");
            Console.WriteLine ("   -L <searchdir>       search for Libraries also in searchdir");
            Console.WriteLine ("   -l <libname>         link with libname");
            Console.WriteLine ("   -r                   Perform an incremental link");
            Console.WriteLine ("   -v                   increase verbosity");
            Console.WriteLine ("   -h                   this message");
            Console.WriteLine ("");
            Console.WriteLine ("   Sample: ld -o a.out input.dll");
        }

        static void Error (string Message)
        {
            if (Message != null)
                Console.WriteLine ("Error: {0}", Message);
            Usage ();
            Environment.Exit (1);
        }
    }
}
