// PR c++/7566
// { dg-do compile }

struct bar { bar (); ~bar ();};

void foo (int c)
{
  switch (c)
    {
    case 0:
    case 1:
      bar x;	// { dg-error "crosses initialization" }
      break;
    default:	// { dg-error "jump to case label" }
      break;
    }
}

// { dg-warning "destructor needed|where case label|enclose actions" "" { target *-*-* } 14 }
