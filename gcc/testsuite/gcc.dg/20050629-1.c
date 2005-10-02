/* { dg-options "-O2 -funswitch-loops" } */

void f(int type){
    int tmp;
	
    for(tmp = 0; tmp < 2; tmp++){
        switch(type) {
        case 0: 
		s1(); break;
        case 1: 
		s2(); break;
	case 2:
		s3(); break;
	}
    }
}
