#include <kulina.h>
#include "imagesc.c"
int cameradia(  void * parent) {
  int ret=1;
  DIALOG D;
  DIA d[3];
  DIP p0 = { 
    'p',
    7,6,  
    393,344,  
    &cameramsg_str,
    -1 /* bkgr colour */ 
  };
  int butn1 =1;
  int sw1[1]  = { 
    1  
  };
  char *titles1[]  = { 
    (char *)" ",
    NULL
  };
  void *xpm1[]  = { 
    &gobk_str, /* pixmap info */ 
    NULL,NULL,
    NULL
  };
  int bkgr1[]  = { 
    -1, 
    -1
  };
  char *butncode1  = NULL;
  DILN H1 = { 
    'H',
    148,356,  
    237,389,
    4,0,
    80, 
    25, 
    1,1, 
    &butn1, 
    sw1,
    titles1,
    butncode1,
    NULL,NULL,/* arg, Callbak */
    xpm1,bkgr1 /* pointers to xpms and colors */
    ,2,0.2
  };
  d[0].p = &p0;
  d[1].H = &H1;
  d[2].t = NULL;
  kgInitUi(&D);
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 2;
  D.df = 1;
  D.tw = 4;
  D.bw = 52;
  D.lw = 4;
  D.rw = 4;
  D.xo = 332;   /* Position of Dialog */ 
  D.yo = 75;
  D.xl = 400;    /*  Length of Dialog */
  D.yl = 400;    /*  Width  of Dialog */
  D.Initfun = NULL;    /*  Width  of Dialog */
  D.kbattn = 1;    /*  1 for drawing keyborad attention */
  D.butattn = 1;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
#if 0 
  D.Callback = cameradiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.Fixpos = 0;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 1;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = NULL;    /*  any data to be passed by user*/
  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
  }    /*  end of fullscreen mode */
  kgDefaultGuiTheme(&(D.gc));    /*  set colors for gui*/
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifycameradiaGc(&(D.gc));    /*  set colors for gui*/
  D.SearchList=NULL;
  D.Resize=0;
  ret= kgUi(&D);
  return ret;
}
void *Runcameradia(void *parent) {
/*************************************************


*************************************************/
   cameradia( parent );
   return NULL;
}
