// { dg-do link }
// { dg-options "-flto-single -O2" }

class Init     {
};

int f(Init *a) {
}

// we should use compile, but flto-single fails with -S
int main(void){
  return 0;
}
