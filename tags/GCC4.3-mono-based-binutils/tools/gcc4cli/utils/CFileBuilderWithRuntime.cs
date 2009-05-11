
using System;
using System.IO;
using System.Collections;
using System.Threading;

using Mono.Cecil;
using Mono.Cecil.Cil;

namespace gcc4cli.util {

    public abstract class CFileBuilderWithRuntime : CFileBuilder {

        static readonly string StartFunctionName = ".start";
        static readonly string InitFunctionName = ".init";
        static readonly string FiniFunctionName = ".fini";
        static readonly string InitCountName = ".init_count";

        Queue dynamic_libs = new Queue ();

        CDynamicLibrary libstd = null;

        internal bool multithreaded;

        protected CFileBuilderWithRuntime (string name, AssemblyKind kind,
                                           AssemblyResolver asm_resolver, string attrname, bool multithreaded,
                                           bool allowExternalAssembly, bool keepIncompleteTypes)
            : base (name, kind, asm_resolver, attrname, allowExternalAssembly, keepIncompleteTypes)
        {
            this.multithreaded = multithreaded;
            if (multithreaded) {
                TypeDefinition tdef = asm_resolver.GCC4NET.MainModule.Types[CFileAttributes.mt];
                CustomAttribute cattr =
                    new CustomAttribute (Module.Import (tdef.Constructors.GetConstructor (false, new Type [] {})));
                Assembly.CustomAttributes.Add (cattr);
            }
        }
        protected CFileBuilderWithRuntime (AssemblyDefinition assembly, string attrname,
                                           bool allowExternalAssembly, bool keepIncompleteTypes)
            : base (assembly, attrname, allowExternalAssembly, keepIncompleteTypes)
        {
            this.multithreaded = CFileAttributes.AssemblyIsMultiThreaded (assembly);
        }

        protected override bool Add (CDynamicLibrary lib)
        {
            if (lib.Name == "libstd")
                libstd = lib;

            if (base.Add (lib)) {
                if (multithreaded != lib.multithreaded) {
                    if (multithreaded)
                        throw new Exception("Cannot link with '" +lib.Name+ "' that is not a MultiThreaded Library");
                    else
                        throw new Exception("Cannot link with '" +lib.Name+ "' that is a MultiThreaded Library");
                }

                dynamic_libs.Enqueue (lib);
                return true;
            }
            return false;
        }

        protected bool NeedsInitFini ()
        {
            return !def_symbols.Contains (InitFunctionName) || !def_symbols.Contains (FiniFunctionName);
        }

        protected void AddInitFini ()
        {
            if (def_symbols.Contains (InitCountName))
                throw new RedefinitionException ("When adding runtime cannot redefine " + InitCountName,
                                                 InitCountName);

            FieldDefinition fld = new FieldDefinition (InitCountName,
                                                       Module.Import (typeof (int)),
                                                       FieldAttributes.Private
                                                       | FieldAttributes.Static
                                                       | FieldAttributes.SpecialName
                                                       | FieldAttributes.RTSpecialName
                                                       );
            MainType.Fields.Add (fld);
            def_symbols[InitCountName] = fld;

            MethodDefinition myinit = new MethodDefinition (InitFunctionName,
                                                            MethodAttributes.Public
                                                            | MethodAttributes.Static
                                                            | MethodAttributes.SpecialName
                                                            | MethodAttributes.RTSpecialName,
                                                            Module.Import (typeof (void)));
            myinit.Body.CilWorker.Emit (OpCodes.Ret);

            MainType.Methods.Add (myinit);
            def_symbols[InitFunctionName] = myinit;


            MethodDefinition myfini = new MethodDefinition (FiniFunctionName,
                                                            MethodAttributes.Public
                                                            | MethodAttributes.Static
                                                            | MethodAttributes.SpecialName
                                                            | MethodAttributes.RTSpecialName,
                                                            Module.Import (typeof (void)));
            myfini.Body.CilWorker.Emit (OpCodes.Ret);

            MainType.Methods.Add (myfini);
            def_symbols[FiniFunctionName] = myfini;
        }

        protected void CheckInitFini()
        {
            if  (!def_symbols.Contains (InitFunctionName))
                throw new NotFoundException ("Broken executable: " + InitFunctionName + " function missing",
                                             InitFunctionName);
            if  (!def_symbols.Contains (FiniFunctionName))
                throw new  NotFoundException ("Broken executable: " + FiniFunctionName + " function missing",
                                              FiniFunctionName);
        }

        void BuildInit ()
        {
            Queue initializers = new Queue ();
            foreach(MethodDefinition method in MainType.Methods) {
                if (CFileAttributes.MethodIsInitializer (method)) {
                    initializers.Enqueue (method);
                }
            }

            if ((dynamic_libs.Count + initializers.Count) > 0) {
                FieldReference init_count = (FieldReference) def_symbols[InitCountName];
                MethodDefinition myinit = (MethodDefinition) def_symbols[InitFunctionName];

                MethodBody body = myinit.Body;
                body.Instructions.Clear ();

                CilWorker worker = body.CilWorker;

                Instruction ret_instr = worker.Create (OpCodes.Ret);
                if (multithreaded) {
                    Type[] types = new Type[] { Type.GetType ("System.Int32&") };
                    MethodReference inc_m = Module.Import (typeof (Interlocked).GetMethod ("Increment", types));
                    worker.Emit (OpCodes.Ldsflda, init_count);
                    worker.Emit (OpCodes.Call, inc_m);
                    worker.Emit (OpCodes.Ldc_I4_1);
                    worker.Emit (OpCodes.Sub);
                }
                else {
                    worker.Emit (OpCodes.Ldsfld, init_count);
                    worker.Emit (OpCodes.Dup);
                    worker.Emit (OpCodes.Ldc_I4_1);
                    worker.Emit (OpCodes.Add);
                    worker.Emit (OpCodes.Stsfld, init_count);
                }

                worker.Emit (OpCodes.Brtrue, ret_instr);

                foreach (CDynamicLibrary library in dynamic_libs) {
                    MethodReference r_method = library.MainType.Methods.GetMethod (InitFunctionName)[0];
                    MethodReference method = Module.Import (r_method);
                    worker.Emit (OpCodes.Call, method);
                }

                foreach (MethodDefinition method in initializers) {
                    worker.Emit (OpCodes.Call, method);
                }

                worker.Append (ret_instr);

                body.Optimize ();
            }
        }

        void BuildFini ()
        {
            Queue finalizers = new Queue ();
            foreach(MethodDefinition method in MainType.Methods) {
                if (CFileAttributes.MethodIsFinalizer (method)) {
                    finalizers.Enqueue (method);
                }
            }

            if ((dynamic_libs.Count + finalizers.Count) > 0) {
                FieldReference init_count = (FieldReference) def_symbols[InitCountName];
                MethodDefinition myfini = (MethodDefinition) def_symbols[FiniFunctionName];

                MethodBody body = myfini.Body;
                body.Instructions.Clear ();

                CilWorker worker = body.CilWorker;

                Instruction ret_instr = worker.Create (OpCodes.Ret);

                if (multithreaded) {
                    Type[] types = new Type[] { Type.GetType ("System.Int32&") };
                    MethodReference dec_m = Module.Import (typeof (Interlocked).GetMethod ("Decrement", types));
                    worker.Emit (OpCodes.Ldsflda, init_count);
                    worker.Emit (OpCodes.Call, dec_m);
                }
                else {
                    worker.Emit (OpCodes.Ldsfld, init_count);
                    worker.Emit (OpCodes.Ldc_I4_1);
                    worker.Emit (OpCodes.Sub);
                    worker.Emit (OpCodes.Dup);
                    worker.Emit (OpCodes.Stsfld, init_count);
                }

                worker.Emit (OpCodes.Brtrue, ret_instr);

                foreach (MethodDefinition method in finalizers) {
                    worker.Emit (OpCodes.Call, method);
                }

                foreach (CDynamicLibrary library in dynamic_libs) {
                    MethodReference r_method = library.MainType.Methods.GetMethod (FiniFunctionName)[0];
                    MethodReference method = Module.Import (r_method);
                    worker.Emit (OpCodes.Call, method);
                }

                worker.Append (ret_instr);
                body.Optimize ();
            }
        }

        protected void BuildInitFini ()
        {
            BuildInit ();
            BuildFini ();
        }

        protected bool NeedsStart ()
        {
            return !def_symbols.Contains (StartFunctionName);
        }

        protected void AddStart ()
        {
            MethodDefinition mystart = new MethodDefinition (StartFunctionName,
                                                             MethodAttributes.Public
                                                             | MethodAttributes.Static
                                                             | MethodAttributes.SpecialName
                                                             | MethodAttributes.RTSpecialName,
                                                             Module.Import (typeof (void)));
            // Add arguments
            ParameterDefinition myargs = new ParameterDefinition ("args",
                                                                  0,
                                                                  (ParameterAttributes) 0,
                                                                  Module.Import (typeof (string[])));
            mystart.Parameters.Add (myargs);

            mystart.Body.CilWorker.Emit (OpCodes.Ret);

            MainType.Methods.Add (mystart);
            def_symbols[StartFunctionName] = mystart;

            Assembly.EntryPoint = mystart;
        }

        protected void CheckStart ()
        {
            if  (!def_symbols.Contains (StartFunctionName))
                throw new NotFoundException ("Broken executable: " + StartFunctionName + " function missing",
                                             StartFunctionName);
        }

        protected void BuildStart ()
        {
            if (!def_symbols.Contains ("main"))
                throw new NotFoundException ("main function not found", "main");

            MethodReference main_method = (MethodReference) def_symbols["main"];
            MethodDefinition mystart = (MethodDefinition) def_symbols[StartFunctionName];

            MethodBody body = mystart.Body;
            body.Instructions.Clear ();

            CilWorker worker = body.CilWorker;

            // Add Local vars
            TypeReference varType = Module.Import (typeof (int)); // int32
            VariableDefinition myargc = new VariableDefinition ("argc", 0, mystart, varType);
            varType = Module.Import (typeof (byte));
            varType = new PointerType (varType);
            varType = new PointerType (varType); // int8**
            VariableDefinition myargv = new VariableDefinition ("argv", 1, mystart, varType);
            body.Variables.Add (myargc);
            body.Variables.Add (myargv);

            TypeDefinition shelper = AssemblyResolver.DefaultResolver.GCC4NET.MainModule.Types["gcc4net.StartupHelper"];

            //Create function body

            worker.Emit (OpCodes.Call, MainType.Methods.GetMethod(InitFunctionName)[0]);

            worker.Emit (OpCodes.Ldloca, myargc);
            worker.Emit (OpCodes.Call,Module.Import (shelper.Methods.GetMethod ("GetArgv")[0]));
            worker.Emit (OpCodes.Stloc, myargv);

            // TODO add call to main
            switch (main_method.Parameters.Count) {
            case 0:
                break;
            case 1:
                worker.Emit (OpCodes.Ldloc, myargc);
                break;
            case 2:
                worker.Emit (OpCodes.Ldloc, myargc);
                worker.Emit (OpCodes.Ldloc, myargv);
                break;
            case 3:
                worker.Emit (OpCodes.Ldloc, myargc);
                worker.Emit (OpCodes.Ldloc, myargv);
                worker.Emit (OpCodes.Call, Module.Import (shelper.Methods.GetMethod ("GetEnvVars")[0]));
                break;
            default:
                throw new NotFoundException ("Function main: wrong number of arguments", "main");
            }
            worker.Emit (OpCodes.Call, main_method);

            MethodReference m_exit = null;

            if (def_symbols.Contains("exit")) {
                m_exit = (MethodReference)def_symbols["exit"];
            }
            else if (libstd != null) {
                m_exit = Module.Import ((MethodReference)(libstd.GetDefMember ("exit")));
            }

            if (m_exit != null) {
                worker.Emit (OpCodes.Dup);
                worker.Emit (OpCodes.Call, m_exit);
            }
            worker.Emit (OpCodes.Call, MainType.Methods.GetMethod(FiniFunctionName)[0]);
            worker.Emit (OpCodes.Call, Module.Import (typeof (Environment).GetMethod ("Exit")));

            worker.Emit (OpCodes.Ret);

            body.Optimize ();
        }

    }

}
