#include "client.h"

PFNGLXGETPROCADDRESSARBPROC lib_glXGetProcAddressARB;
static void* lib_handle_libGL2 = 0;







void GLOMPunpack( )
{
 
  int func;
  int flags;
  fifo_input(&GLOMPcmd_fifo,&func,4);
  fifo_input(&GLOMPcmd_fifo,&flags,4);
  if(DEBUG){printf("CLIENT %d :fnum:%d\n",client_num,func);}


  if(func<OVERRIDE_BASE)
    functable[func]();
  else if (func-OVERRIDE_BASE<sizeof(functable_override)/sizeof(*functable_override))
    functable_override[func-OVERRIDE_BASE]();
  else
    printf("pas trouvé\n");
  
  

   
}

/*initialise un client*/
void GLOMPclient_init()
{

  /*recupere le pointeur de glXGetProcAddressARB pour pouvoir lancer les fonction glX*/
  lib_handle_libGL2 = dlopen("/usr/lib/libGL.so", RTLD_LAZY);
  lib_glXGetProcAddressARB = dlsym(lib_handle_libGL2, "glXGetProcAddressARB");
  initPointers();/*recupere toutes les add des fonction glX (cf dewarped)*/
  creertabfunc();/*creer le tableau des fonction deriver GLOMP...*/ 
}




