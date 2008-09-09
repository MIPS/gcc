
using System;
using System.IO;
using System.Text;
using System.Collections;

using Mono.Cecil;
using gcc4cli.util;

namespace gcc4cli.objcopy {

    class Driver {

        static readonly string prog_name = "GCC4CLI objcopy";

        static void Main (string [] args)
        {
            Driver drv = new Driver ();
            drv.Parse (new Queue (args));
            drv.Run ();
            Environment.Exit (0);
        }

        string m_inputFile;
        string m_outputFile;

        Driver ()
        {
        }

        void Run ()
        {
            try {
                CFile ofile = CFileFactory.LoadCFile (m_inputFile);
                if (   !keepSymbols.IsEmpty
                    || !stripSymbols.IsEmpty
                    || !stripUnnededSymbols.IsEmpty
                    || !keepGlobalSymbols.IsEmpty
                    || !localizeTypes.IsEmpty
                    || !weakenSymbols.IsEmpty)
                    Error ("Use of unsupported option");
                if (newAssemblyName != null) {
                    ofile.Assembly.Name.Name = newAssemblyName;
                    ofile.Module.Name = newAssemblyName;
                }
                ofile.AddSymbolPrefix(symbols_prefix);
                ofile.AddTypePrefix(types_prefix);
                foreach (string name in renameSymbols.Keys)  {
                    ofile.renameSymbol (name, (string)renameSymbols[name]);
                }
                foreach (string name in renameTypes.Keys)  {
                    ofile.renameType (name, (string)renameTypes[name]);
                }
                foreach (string name in localizeSymbols)  {
                    ofile.localizeSymbol (name);
                }
                foreach (string name in globalizeSymbols)  {
                    ofile.globalizeSymbol (name);
                }

                ofile.Save (m_outputFile);
            } catch (CFileException e) {
                Console.WriteLine ("Error: {0}", e.Message);
                Environment.Exit (1);
            } catch (Exception e) {
                Error (e.ToString ());
            }
        }

        string InputFileName {
            get { return m_inputFile; }
            set {
                m_inputFile  = value;
                m_outputFile = value;
            }
        }

        string OutputFileName {
            get { return m_outputFile; }
            set { m_outputFile = value; }
        }

        Set keepSymbols         = new Set();        // Set<string>
        Set stripSymbols        = new Set();        // Set<string>
        Set stripUnnededSymbols = new Set();        // Set<string>
        Set keepGlobalSymbols   = new Set();        // Set<string>
        Set localizeSymbols     = new Set();        // Set<string>
        Set localizeTypes       = new Set();        // Set<string>
        Set globalizeSymbols    = new Set();        // Set<string>
        Set weakenSymbols       = new Set();        // Set<string>
        Hashtable renameSymbols = new Hashtable (); // Dictionary<string,string>
        Hashtable renameTypes   = new Hashtable (); // Dictionary<string,string>

        string newAssemblyName = null;
        string symbols_prefix = null;
        string types_prefix = null;

        string prefixSymbols {
            get { return symbols_prefix; }
            set { symbols_prefix = value; }
        }

        string prefixTypes {
            get { return types_prefix; }
            set { types_prefix = value; }
        }

        void AddToSet (string filename, Set myset)
        {
            StreamReader reader = File.OpenText (filename);
            while (reader.Peek () != -1) {
                string line = reader.ReadLine ().Trim ();
                if (line[0] != '#') {
                    myset.Add (line);
                }
            }
        }

        void Parse (Queue q)
        {
            string token = (string) q.Dequeue ();
            for ( ;q.Count >= 0 && token [0] == '-'; token = (string) q.Dequeue ()) {
                switch (token) {
                case "--rename-assembly":
                    if (q.Count == 0)
                        Error ("Missing argument for option '" + token + "'");
                    newAssemblyName = (string) q.Dequeue ();
                    break;
                case "--remove-gcc4cli-attrib":
                    Error ("Option '" + token + "' not supported");
                    break;
                case "-S":
                case "--strip-all":
                    Error ("Option '" + token + "' not supported");
                    break;
                case "-g":
                case "--strip-debug":
                    Error ("Option '" + token + "' not supported");
                    break;
                case "-K":
                    if (q.Count == 0)
                        Error ("Missing argument for option '" + token + "'");
                    keepSymbols.Add ((string) q.Dequeue ());
                    break;
                case "-N":
                    if (q.Count == 0)
                        Error ("Missing argument for option '" + token + "'");
                    stripSymbols.Add ((string) q.Dequeue ());
                    break;
                case "-G":
                    if (q.Count == 0)
                        Error ("Missing argument for option '" + token + "'");
                    keepGlobalSymbols.Add ((string) q.Dequeue ());
                    break;
//                 case "--localize-hidden":
//                     Error ("Option '" + token + "' not supported");
//                     break;
                case "-L":
                    if (q.Count == 0)
                        Error ("Missing argument for option '" + token + "'");
                    localizeSymbols.Add ((string) q.Dequeue ());
                    break;
                    
                case "--localize-all-types":
                    Error ("Option '" + token + "' not supported");
                    break;
                case "-W":
                    if (q.Count == 0)
                        Error ("Missing argument for option '" + token + "'");
                    weakenSymbols.Add ((string) q.Dequeue ());
                    break;
//                 case "-x":
//                 case "--discard-all":
//                     Error ("Option '" + token + "' not supported");
//                     break;
//                 case "-X":
//                 case "--discard-locals":
//                     Error ("Option '" + token + "' not supported");
//                     break;
//                 case " --debugging":
//                     Error ("Option '" + token + "' not supported");
//                     break;
                case "--redefine-sym":
                    if (q.Count == 0)
                        Error ("Missing argument for option '" + token + "'");
                    else {
                        string argument = (string) q.Dequeue ();
                        int index = argument.IndexOf ('=');
                        if (index == -1)
                            Error ("Invalid argument passed to option '--redefine-sym'");
                        renameSymbols[argument.Substring (0, index)] = argument.Substring (index+1);
                    }
                    break;
                case "--redefine-type":
                    if (q.Count == 0)
                        Error ("Missing argument for option '" + token + "'");
                    else {
                        string argument = (string) q.Dequeue ();
                        int index = argument.IndexOf ('=');
                        if (index == -1)
                            Error ("Invalid argument passed to option '--redefine-type'");
                        renameTypes[argument.Substring (0, index)] = argument.Substring (index+1);
                    }
                    break;
//                 case "--weaken":
//                     Error ("Option '" + token + "' not supported");
//                     break;
//                 case "--keep-file-symbols":
//                     Error ("Option '" + token + "' not supported");
//                     break;
//                 case "--only-keep-debug":
//                     Error ("Option '" + token + "' not supported");
//                     break;
//                 case "--keep-file-symbols":
//                     Error ("Option '" + token + "' not supported");
//                     break;
//                 case "--only-keep-debug":
//                     Error ("Option '" + token + "' not supported");
//                     break;
                case "-v":
                case "--verbose":
                    Tracer.IncreaseVerbosity();
                    break;

                case "-h":
                case "--help":
                    Usage ();
                    Environment.Exit (0);
                    break;

                default:
                    {
                        int index = token.IndexOf ('=');
                        if (index == -1)
                            Error ("Unrecognized option '" + token + "'");
                        string argument = token.Substring (index+1);
                        switch (token.Substring (0, index)) {

                        case "--keep-symbol":
                            keepSymbols.Add (argument);
                            break;
                        case "--strip-symbol":
                            stripSymbols.Add (argument);
                            break;
                        case "--strip-unneeded-symbol":
                            stripUnnededSymbols.Add (argument);
                            break;
                        case "--keep-global-symbol":
                            keepGlobalSymbols.Add (argument);
                            break;
                        case "--localize-symbol":
                            localizeSymbols.Add (argument);
                            break;
                        case "--localize-type":
                            localizeTypes.Add (argument);
                            break;
                        case "--globalize-symbol":
                            globalizeSymbols.Add (argument);
                            break;
                        case "--weaken-symbol":
                            weakenSymbols.Add (argument);
                            break;
                        case "--redefine-syms":
                            {
                                StreamReader reader = File.OpenText (argument);
                                while (reader.Peek () != -1) {
                                    string line = reader.ReadLine ().Trim ();
                                    if (line[0] != '#') {
                                        int lindex = line.IndexOf (' ');
                                        if (lindex == -1)
                                            Error ("Invalid argument passed to option '--redefine-sym'");
                                        renameSymbols[line.Substring (0, lindex)] = line.Substring (lindex+1);
                                    }
                                }
                            }
                            break;
                        case "--redefine-types":
                            {
                                StreamReader reader = File.OpenText (argument);
                                while (reader.Peek () != -1) {
                                    string line = reader.ReadLine ().Trim ();
                                    if (line[0] != '#') {
                                        int lindex = line.IndexOf (' ');
                                        if (lindex == -1)
                                            Error ("Invalid argument passed to option '--redefine-types'");
                                        renameTypes[line.Substring (0, lindex)] = line.Substring (lindex+1);
                                    }
                                }
                            }
                            break;
                        case "--keep-symbols":
                            AddToSet (argument, keepSymbols);
                            break;
                        case "--strip-symbols":
                            AddToSet (argument, stripSymbols);
                            break;
                        case "--strip-unneeded-symbols":
                            AddToSet (argument, stripUnnededSymbols);
                            break;
                        case "--localize-symbols":
                            AddToSet (argument, localizeSymbols);
                            break;
                        case "--localize-types":
                            AddToSet (argument, localizeTypes);
                            break;
                        case "--globalize-symbols":
                            AddToSet (argument, globalizeSymbols);
                            break;
                        case "--keep-global-symbols":
                            Error ("Option '" + token + "' not supported");
                            break;
                        case "--weaken-symbols":
                            AddToSet (argument, weakenSymbols);
                            Error ("Option '" + token + "' not supported");
                            break;
                        case "--prefix-symbols":
                            prefixSymbols  = argument;
                            break;
                        case "--prefix-types":
                            prefixTypes = argument;
                            break;

                        default:
                            Error ("Unrecognized option '" + token + "'");
                            break;
                        }
                    }
                    break;
                }
                if (q.Count == 0)
                    Error ("Missing Input file");
            }
            if (q.Count > 1)
                Error ("Too many Arguments");

            InputFileName = token;
            if (q.Count > 0) {
                OutputFileName = (string) q.Dequeue ();
            }

            if (Tracer.TraceAtLevel(0)) {
                Tracer.Trace (  "InputFileName:  " + InputFileName + "\n"
                              + "OutputFileName: " + OutputFileName);
                if (newAssemblyName != null) {
                    Tracer.Trace ("Assembly renamed to :'" + newAssemblyName + "'");
                }
                if (symbols_prefix != null) {
                    Tracer.Trace ("Added prefix '" + symbols_prefix + "' to all symbols");
                }
                if (types_prefix != null) {
                    Tracer.Trace ("Added prefix '" + types_prefix + "' to all types");
                }
                if (!keepSymbols.IsEmpty) {
                    Tracer.Trace ("Keep Symbols " + keepSymbols.ToString());
                }
                if (!stripSymbols.IsEmpty) {
                    Tracer.Trace ("Strip Symbols " + stripSymbols.ToString());
                }
                if (!stripUnnededSymbols.IsEmpty) {
                    Tracer.Trace ("Strip Unneded Symbols " + stripUnnededSymbols.ToString());
                }
                if (!keepGlobalSymbols.IsEmpty) {
                    Tracer.Trace ("Keep Global Symbols " + keepGlobalSymbols.ToString());
                }
                if (!localizeSymbols.IsEmpty) {
                    Tracer.Trace ("Localize Symbols " + localizeSymbols.ToString());
                }
                if (!localizeTypes.IsEmpty) {
                    Tracer.Trace ("Localize Types " + localizeTypes.ToString());
                }
                if (!globalizeSymbols.IsEmpty) {
                    Tracer.Trace ("Globalize Symbols " + globalizeSymbols.ToString());
                }
                if (!weakenSymbols.IsEmpty) {
                    Tracer.Trace ("Weaken Symbols " + weakenSymbols.ToString());
                }
                if (renameSymbols.Count != 0) {
                    Tracer.Trace ("Symbols renamed:");
                    foreach (string name in renameSymbols.Keys)  {
                        Console.WriteLine ("  {0} \t{1}", name, (string)renameSymbols[name]);
                    }
                }
                if (renameTypes.Count != 0) {
                    Tracer.Trace ("Types renamed");
                    foreach (string name in renameTypes.Keys)  {
                        Console.WriteLine ("  {0} \t{1}", name, (string)renameTypes[name]);
                    }
                }
            }
        }

        static void Usage ()
        {
            Console.WriteLine (prog_name);
            Console.WriteLine ("objcopy [options] infile [outfile]");
// Strip names
            Console.WriteLine ("   --rename-assembly newname                     xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --remove-gcc4cli-attrib                       xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   -S|--strip-all                                xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   -g|--strip-debug                              xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   -K symbolname|--keep-symbol=symbolname        xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   -N symbolname|--strip-symbol=symbolname       xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --strip-unneeded-symbol=symbolname            xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   -G symbolname|--keep-global-symbol=symbolname xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --localize-hidden                             xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   -L symbolname|--localize-symbol=symbolname    xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --localize-type=typename                      xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --localize-all-types                          xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --globalize-symbol=symbolname                 xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   -W symbolname|--weaken-symbol=symbolname      xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   -x|--discard-all                              xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   -X|--discard-locals                           xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --debugging                                   xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --set-start=val                               xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --redefine-sym old=new                        xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --redefine-syms=filename                      xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --redefine-type old=new                       xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --redefine-typess=filename                    xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --weaken                                      xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --keep-symbols=filename                       xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --strip-symbols=filename                      xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --strip-unneeded-symbols=filename             xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --localize-symbols=filename                   xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --localize-types=filename                     xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --globalize-symbols=filename                  xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --keep-file-symbols                           xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --only-keep-debug                             xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --weaken-symbols=filename                     xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --prefix-symbols=string                       xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   --prefix-types=string                         xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --keep-file-symbols                           xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --only-keep-debug                             xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   -v                                            increase verbosity");
//             Console.WriteLine ("   -w|--wildcard                                 xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --change-leading-char                         xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --remove-leading-char                         xxxxxxxxxxxxxxxx");
//             Console.WriteLine ("   --add-gnu-debuglink=path-to-file              xxxxxxxxxxxxxxxx");
            Console.WriteLine ("   -h|--help                                     this message");
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

