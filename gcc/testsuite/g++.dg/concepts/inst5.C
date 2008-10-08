// Contributed by Rolf Bonderer <rolfbo@student.ethz.ch>
concept Visible<typename type>
{
 void show(const type& content);
};

concept_map Visible<double>
{
 void show(const double& content) {}
};

template<Visible type>
void showOff(const type& t) {
 show(t);
}

int main() {
 double t=7.4;
 showOff(t);
 return 0;
}
