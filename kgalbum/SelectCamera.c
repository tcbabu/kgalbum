#include <kulina.h>
#include "SelectCameraCallbacks.h"
ThumbNail ** GetCameraList(void);
void ModifySelectCameraGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int SelectCameraGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIX x0 = { 
    'x',
    3,38,  
    253,158,   
    8,2,  
    224, 
    25, 
    1,1, 
    1,4, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,SelectCamerabrowser1callback, /* *args, callback */
    4,  /* Border Offset  */
     16,  /* Scroll width  */
     1,  /* Type  */
     1, /* item highlight */
    0, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x0.Wid,(char *)"SelectCameraWidget2");
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Cancel");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-255000000;
  butn1[0].butncode='';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"Okay");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-177000;
  butn1[1].butncode='';
  DIL h1 = { 
    'h',
    51,184,  
    207,217,
    2,0,  
    72, 
    25, 
    2,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    SelectCamerasplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h1.Wid,(char *)"SelectCameraWidget3");
  DIM m2 = { 
    'm',
    56,9,  
    182,31,  
    1,0  
  };
  strncpy(m2.msg,(char *)"Select Camera",499);
  strcpy(m2.Wid,(char *)"SelectCameraWidget4");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+4));
  d =dtmp+i; 
  d[3].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIX));
  SelectCamerabrowser1init(&x0,pt) ;
  *d[0].x = x0;
  d[0].x->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIL));
  SelectCamerasplbutton1init(&h1,pt) ;
  *d[1].h = h1;
  d[1].h->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIM));
  *d[2].m = m2;
  d[2].m->item = -1;
  d[3].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeSelectCameraGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Selectmenu1  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   void** v=(void **)malloc(sizeof(void*)*2);
   v[1]=NULL;
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = SelectCameraGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int SelectCamera( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = SelectCameraGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 111;   /* Position of Dialog */ 
  D.yo = 73;
  D.xl = 262;    /*  Length of Dialog */
  D.yl = 228;    /*  Width  of Dialog */
  D.Initfun = SelectCamerainit;    /*   init fuction for Dialog */
  D.Cleanupfun = SelectCameracleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 1 
  D.Callback = SelectCameraCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = SelectCameraResizeCallBack;  /*  Resize callback */
#if 1 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = SelectCameraWaitCallBack;  /*  Wait callback */
#endif
  D.Fixpos = 1;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = pt;    /*  any data to be passed by user*/
//  strcpy(D.name,"Kulina Designer ver 1.0");    /*  Dialog name you may change */
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
      // D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
//     D.StackPos = 1; // you may need it
  }    /*  end of fullscreen mode */
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifySelectCameraGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
int RunSelectCamera(void *arg) {
/*************************************************

    Selectmenu1  1 data value

*************************************************/
   int ret;
   int   v0 = 1;
   void* v[1];
   v[0]=(void *)(&v0);
   void *pt=NULL; /* pointer to send any extra information */
   ret = SelectCamera(arg,v,pt ) -1;
   return ret;
}
