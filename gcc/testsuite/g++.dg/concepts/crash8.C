#include <concepts>

concept PolytypicSequenceIteratorEnd<typename T> {
};

concept PolytypicSequenceIterator<typename T> {
        typename next_iterator = T::next_iterator;
        requires PolytypicSequenceIterator<next_iterator>; // { dg-error "recursive" }

        next_iterator operator++(const T&);
};

concept PolytypicSequence<typename T> {
        PolytypicSequenceIterator    iterator;
        PolytypicSequenceIteratorEnd iterator_end;

        iterator     T::begin();
        iterator_end T::end();
};

template<PolytypicSequence sequence>
void PolytypicSequenceVisitor(sequence s) { // { dg-error "candidates" }
  PolytypicSequenceVisitor(s.begin()); // { dg-error "no matching|no concept map|you may need" }
}

template<PolytypicSequenceIterator iterator>
void PolytypicSequenceVisitor(iterator i) { // { dg-error "requirements" }
  PolytypicSequenceVisitor(++i); // { dg-error "no matching|no concept map|you may need|no concept map|you may need" }
}

template<PolytypicSequenceIteratorEnd iterator_end>
void PolytypicSequenceVisitor(iterator_end i) {
}
