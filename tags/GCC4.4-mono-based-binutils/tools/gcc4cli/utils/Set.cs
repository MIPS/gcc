
using System;
using System.Collections;

namespace gcc4cli.util {

    [Serializable]
    public sealed class Set : ICollection
    {
        static readonly object marker = new object ();

        Hashtable ht = new Hashtable ();

        public Set ()
        {
        }

        public Set (ICollection initialValues)
        {
            AddRange (initialValues);
        }

        public bool Add (object element)
        {
            if (Contains (element))
                return false;

            ht.Add (element, marker);
            return true;
        }

        public bool AddRange (ICollection collection)
        {
            bool changed = false;
            foreach (object element in collection) {
                changed |= Add (element);
            }
            return changed;
        }

        public bool AddRangeNONulls (ICollection collection)
        {
            bool changed = false;
            foreach (object element in collection) {
                if (element != null)
                    changed |= Add (element);
            }
            return changed;
        }

        public void Clear ()
        {
            ht.Clear ();
        }

        public bool Contains (object element)
        {
            return ht.Contains (element);
        }

        public bool ContainsRange (ICollection collection)
        {
            foreach (object element in collection) {
                if (!Contains (element)) {
                    return false;
                }
            }
            return true;
        }

        public bool ContainsAny (ICollection collection)
        {
            foreach (object element in collection) {
                if (Contains (element)) {
                    return true;
                }
            }
            return false;
        }

        public bool IsEmpty
        {
            get { return ht.Count == 0; }
        }

        public bool Remove (object element)
        {
            if (Contains (element)) {
                ht.Remove (element);
                return true;
            }
            return false;
        }

        public bool RemoveRange (ICollection collection)
        {
            bool changed = false;
            foreach (object element in collection) {
                changed |= Remove (element);
            }
            return changed;
        }

        public void CopyTo (Array array, int index)
        {
            int i = index;
            foreach (object o in this) {
                array.SetValue (o, i++);
            }
        }

        public int Count {
            get { return ht.Count; }
        }

        public bool IsSynchronized {
            get { return ht.IsSynchronized; }
        }

        public object SyncRoot {
            get { return ht.SyncRoot; }
        }

        public IEnumerator GetEnumerator ()
        {
            return ht.Keys.GetEnumerator ();
        }

        public void Union (Set setOne)
        {
            AddRange (setOne);
        }

        public void Intersect (Set setOne)
        {
            Set tempSet = new Set ();

            tempSet.AddRange (this);
            tempSet.RemoveRange (setOne);
            RemoveRange (tempSet);
        }

        public void Minus (Set setOne)
        {
            RemoveRange (setOne);
        }

        public void ExclusiveOr (Set setOne)
        {
            foreach (object element in setOne) {
                if (Contains (element)) {
                    Remove (element);
                } else {
                    Add (element);
                }
            }
        }

        public override bool Equals (object obj)
        {
            Set theOtherSet = obj as Set;
            if (theOtherSet == null || theOtherSet.Count != Count) {
                return false;
            } else {
                foreach (object element in theOtherSet) {
                    if (!Contains (element)) {
                        return false;
                    }
                }
                return true;
            }
        }

        public override int GetHashCode()
        {
            int hashCode = 0;
            foreach (object element in this) {
                hashCode += element.GetHashCode();
            }
            return hashCode + Count;
        }

        public override string ToString ()
        {
            string result = "Set(";

            foreach (object element in this) {
                result += element + ",";
            }
            result += ")";
            return result;
        }

    }

}
