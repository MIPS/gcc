typedef struct services
{
   int compId;
   void *globalServices;
}services_t;

typedef struct comp
{
   void *myServices;
   void (*setServices)(struct comp *self, services_t *myServices);
   void *myPort;
}comp_t;


/* prototypes for f90 functions */
extern void ptr_test(comp_t *self, services_t *myServices, double myDouble);

/* c function prototypes */
void test(double myDouble);

int main(int argc, char **argv)
{
   services_t servicesObj;
   comp_t myComp;
   double myDouble;

   servicesObj.compId = 17;
/*    servicesObj.globalServices = NULL; */
   /* nullify the ptr */
   servicesObj.globalServices = 0;
   myComp.myServices = &servicesObj;
/*    myComp.setServices = NULL; */
   myComp.setServices = 0;
/*    myComp.myPort = NULL; */
   myComp.myPort = 0;
   myDouble = 1.23456;
   
   ptr_test(&myComp, &servicesObj, myDouble);
   
   return 0;
}/* end main() */

void test(double myDouble)
{
   return;
}/* end test() */
