
using System;

namespace gcc4cli.util {

    public sealed class Tracer {

        static int verbosity = 0;

        static public void IncreaseVerbosity ()
        {
            ++verbosity;
        }

        static public void SetVerbosity (int value)
        {
            verbosity = value;
        }

        static public void Trace (string msg)
        {
            Trace (0, msg);
        }
        static public void Trace (int level, string msg)
        {
            if (verbosity > level) {
                Console.WriteLine (msg);
            }
        }

        static public bool TraceAtLevel(int level)
        {
            return (verbosity > level);
        }

    }
}
