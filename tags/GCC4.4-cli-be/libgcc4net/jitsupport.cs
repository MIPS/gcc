/* Jit hints Attributes

   Copyright (C) 2006-2009 Free Software Foundation, Inc.
   Contributed by STMicroelectronics

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
02110-1301, USA.

Authors:
   Andrea Bona
   Roberto Costa
   Andrea Ornstein
   Erven Rohou
   Gabriele Svelto

Contact information at STMicroelectronics:
Andrea C. Ornstein      <andrea.ornstein@st.com>
Contact information at INRIA:
Erven Rohou             <erven.rohou@inria.fr>
*/

using System;

namespace gcc4net {

    namespace JitCompilationHints {

        [Serializable, AttributeUsage (AttributeTargets.Method)]
        public unsafe sealed class JITMethodAttribute : Attribute {
            public byte* frequencies;
            public byte* probabilities;

            public JITMethodAttribute (byte* frequencies, byte* probabilities) {
                this.frequencies   = frequencies;
                this.probabilities = probabilities;
            }
        }

        [Serializable, AttributeUsage (AttributeTargets.Method)]
        public sealed class BasicBlockFrequenciesAttribute : Attribute {
            byte [] frequencies;

            public BasicBlockFrequenciesAttribute (byte[] frequencies) {
                this.frequencies = frequencies;
            }

            public byte [] Frequencies {
                get {
                    return frequencies;
                }
            }
        }

        [Serializable, AttributeUsage (AttributeTargets.Method)]
        public sealed class BranchProbabilitiesAttribute : Attribute {
            byte [] probabilities;

            public BranchProbabilitiesAttribute (byte[] probabilities) {
                this.probabilities = probabilities;
            }

            public byte [] Probabilities {
                get {
                    return probabilities;
                }
            }
        }
      }

}
