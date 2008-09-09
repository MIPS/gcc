
using System;
using System.IO;
using System.Text;
using System.Collections;

using Mono.Cecil;
using gcc4cli.util;

namespace gcc4cli.objdump {

    class Driver {

        static readonly string prog_name = "GCC4CLI objdump";

        static void Main (string [] args)
        {
            Driver drv = new Driver ();
            drv.Parse (new Queue (args));
            drv.Run ();
            Environment.Exit (0);
        }

        CilWriter m_cilwriter = null;
        Queue files = new Queue ();
        string m_outputFile;

        Driver ()
        {
        }

        void Run ()
        {
            Disassembler sd = new Disassembler (Writer);
            try {
                while (files.Count > 0) {
                    String AssemblyName = (string) files.Dequeue ();
                    if (CFileFactory.CheckIsCFile (AssemblyName)) {
                        CFile cfile = CFileFactory.LoadCFile (AssemblyName);
                        if (cfile is CStaticLibrary) {
                            Writer.WriteLine ("//++++ objdump of CStaticLibrary: " + cfile.Name);
                            foreach (CObject obj in (cfile as CStaticLibrary).Objects) {
                                Writer.WriteLine ("//  ++ CObject: " + obj.Name);
                                sd.DisassembleAssembly (obj.Assembly);
                                Writer.WriteLine ("//  ++ end of " + obj.Name);
                            }
                        } else
                            sd.DisassembleAssembly (cfile.Assembly);
                    }
                    else
                        sd.DisassembleAssembly (CFileFactory.LoadAssembly (AssemblyName));
                }
            } catch (CFileException e) {
                Console.WriteLine ("Error: {0}", e.Message);
                Environment.Exit (1);
            } catch (Exception e) {
                Error (e.ToString ());
            } finally {
                Writer.Close();
            }
        }

        string OutputFileName {
            get { return m_outputFile; }
            set {
                m_outputFile = value;
                FileStream fs = new FileStream (m_outputFile, FileMode.Create, FileAccess.Write, FileShare.None);
                StreamWriter sw = new StreamWriter (fs);
                m_cilwriter = new CilWriter (sw);
            }
        }

        CilWriter Writer {
            get {
                if (m_cilwriter == null)
                    m_cilwriter = new CilWriter (Console.Out);
                return m_cilwriter;
            }
        }

        void AddFile (string file_name)
        {
            if (!File.Exists (file_name)) {
                Error ("Specified '" + file_name + "' file does not exists !");
            }
            else
                files.Enqueue(file_name);
        }

        void Parse (Queue q)
        {
            while (q.Count > 0) {
                string token = (string) q.Dequeue ();

                if (token [0] == '-') {
                    switch (token) {
                    case "-o":
                    case "-out":
                        OutputFileName = (string) q.Dequeue ();
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
                    AddFile (token);
                }
            }
            if (m_cilwriter != null && files.Count > 1)
                Error ("When an output file is specified, the tool can process only one assembly!");

        }

        static void Usage ()
        {
            Console.WriteLine (prog_name);
            Console.WriteLine ("objdump [options] assembly");
            Console.WriteLine ("   -o <filename>         print disassembly into filename");
            Console.WriteLine ("   -h                    this message");
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
