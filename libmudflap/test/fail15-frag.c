struct base {
  int basic;
}; 

struct derived { 
  struct base common;
  char extra;
};

struct base b;
struct base *bp;

bp = (struct base *)&b;

bp->basic = 10;
((struct derived *)bp)->extra = 'x';
