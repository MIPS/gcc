struct base {
  int basic;
}; 

struct derived { 
  struct base common;
  char extra;
};

struct derived d;
struct base *bp;

bp = (struct base *)&d;

bp->basic = 10;
((struct derived *)bp)->extra = 'x';
