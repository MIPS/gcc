
using System;
using System.IO;
using System.Collections;

using Mono.Cecil;
using gcc4cli.util;

namespace gcc4cli.ar {

    public class Driver {

        static readonly string prog_name = "GCC4CLI ar";

        enum ARCommand {
            None,
            Add,
            Delete,
            Test,
            Extract,
            ExtractAll
        };

        bool create = false;
        bool update = false;

        ARCommand command;

        ARCommand Command {
            get { return command; }
            set {
                if (command == ARCommand.None
                    || (command == ARCommand.Extract && value == ARCommand.ExtractAll))
                    command = value;
                else
                    Error ("Multiple commands specified, don't know what to do");
            }
        }

        string ar_name;
        string out_file;

        string ArchiveName {
            get { return ar_name; }
        }

        string ArchiveFileName {
            get { return out_file; }
            set {
                out_file = value;
                ar_name = Path.GetFileNameWithoutExtension(value);
                Tracer.Trace (  "ArchiveName: " + ar_name + "\n"
                              + "ArchiveFileName: " + value);
            }
        }

        Queue files = new Queue ();

        Driver ()
        {
        }

        public static void Main (string [] args)
        {
            Driver drv = new Driver ();
            drv.Parse (new Queue (args));
            if (drv.Command != ARCommand.None) {
                try {
                    drv.Run ();
                } catch (CFileException e) {
                    Console.WriteLine ("Error: {0}", e.Message);
                    Environment.Exit (1);
                } catch (Exception e) {
                    Console.WriteLine (e);
                    Error ("Fatal error in " + prog_name);
                }
            }
            Environment.Exit (0);
        }

        void Parse (Queue q)
        {
            if (q.Count < 2)
                Error ("missing arguments");

            {
                string token = (string) q.Dequeue ();
                int i = 0;
                if (token [0] == '-')
                    i = 1;
                while (i < token.Length) {
                    switch (token[i]) {
                    case 'r':
                        Command = ARCommand.Add;
                        break;
                    case 'd':
                        Command = ARCommand.Delete;
                        break;
                    case 'x':
                        Command = ARCommand.Extract;
                        break;
                    case 't':
                        Command = ARCommand.Test;
                        break;
                    case 'c':
                        create = true;
                        break;
                    case 'u':
                        update = true;
                        break;
                    case 'v':
                        Tracer.IncreaseVerbosity ();
                        break;
                    default:
                        Error ("Unrecognized command '" + token[i] + "' processing argument: " + token);
                        break;
                    }
                    ++i;
                }
            }

            ArchiveFileName = (string) q.Dequeue ();

            if (q.Count == 0 && Command == ARCommand.Extract)
                Command = ARCommand.ExtractAll;

            while (q.Count > 0) {
                files.Enqueue ((string) q.Dequeue ());
            }
        }

        void Run ()
        {
            CStaticLibrary lib = null;
            if (create) {
                    lib = CFileFactory.CreateCStaticLibrary (ArchiveName);
            }
            else {
                CFile myfile = CFileFactory.LoadCFile (ArchiveFileName);
                if (myfile is CStaticLibrary)
                    lib = myfile as CStaticLibrary;
                else
                    throw new WrongFileFormatException ("Unrecognized Library format", ArchiveFileName);
            }
            switch (Command) {
            case ARCommand.Add:
                lib.StartBuilding ();
                while (files.Count > 0) {
                    String filename = (string) files.Dequeue ();
                    CFile obj = CFileFactory.LoadCFile (filename);
                    if (update && lib.Contains (obj.Name))
                        lib.Remove (obj.Name);
                    lib.Add (obj);
                }
                lib.EndBuilding ();
                lib.Save(ArchiveFileName);
                break;
            case ARCommand.Delete:
                lib.StartBuilding ();
                while (files.Count > 0) {
                    string filename = (string) files.Dequeue ();
                    string filename2 = Path.GetFileNameWithoutExtension (filename);
                    if (lib.Contains (filename))
                        lib.Remove (filename);
                    else if  (lib.Contains  (filename2))
                        lib.Remove (filename2);
                }
                lib.EndBuilding ();
                lib.Save (ArchiveFileName);
                break;
            case ARCommand.Test:
                foreach (CObject obj in lib.Objects) {
                    Console.WriteLine (obj.Name + ".o");
                }
                break;
            case ARCommand.Extract:
                while (files.Count > 0) {
                    string filename = (string) files.Dequeue ();
                    string filename2 = Path.GetFileNameWithoutExtension (filename);
                    CFile obj;
                    if (lib.Contains (filename))
                        obj = lib.Get (filename);
                    else if  (lib.Contains (filename2))
                        obj = lib.Get (filename2);
                    else
                        throw new Exception (filename + " not found in " + ArchiveFileName);
                    obj.Save (filename);
                }
                break;
            case ARCommand.ExtractAll:
                foreach (CObject obj in lib.Objects) {
                    obj.Save (obj.Name + ".o");
                }
                break;
            }
        }

        static void Usage ()
        {
            Console.WriteLine (prog_name);
            Console.WriteLine ("ar -p[mod] archive <obj_files>");
            Console.WriteLine ("   p:                   commands");
            Console.WriteLine ("     r                           add");
            Console.WriteLine ("     d                           delete");
            Console.WriteLine ("     x                           exctract");
            Console.WriteLine ("     t                           list");
            Console.WriteLine ("   mod:                 modifiers");
            Console.WriteLine ("     c                           create");
            Console.WriteLine ("     u                           update");
            Console.WriteLine ("     v                           increase verbosity");
            Console.WriteLine ("   archive              archive file");
            Console.WriteLine ("   <obj_files>          list of objects to add, remove, ...");
            Console.WriteLine ("");
            Console.WriteLine ("   Sample: ar -c archive.a a.o b.o ...");
        }

        static void Error (string Message)
        {
            if (Message != null)
                Console.WriteLine ("Error : {0}", Message);
            Usage ();
            Environment.Exit (1);
        }
    }
}
