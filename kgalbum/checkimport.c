#include <kulina.h>
extern int DspXsize,DspYsize;
int  checkimporthoribar1callback(int key,int i,void *Tmp) {
  DIA *D;DILN *B; 
  int n,ret=1; 
  D = ((DIALOG *)Tmp)->d;
  B = D[i].H;
  n = B->nx;
  switch(key) {
    case 1: 
      break;
    case 2: 
      break;
  }
  return ret;
}
int checkimportinit(void *Tmp) {
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  return ret;
}
int checkimportGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char **titles;
  char *xpm0=   NULL; /* pixmap info */ 
  DIP p0 = { 
    'p',
    5,6,  
    390,288,  
    (void *)xpm0,
    -1, /* bkgr colour */ 
      3,0 /* border hide */ 
  };
  strcpy(p0.Wid,(char *)"checkimportWidget1");
  p0.xpm = (void *)v;
  titles = (char **)pt;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn1[0].sw=1;
  strcpy(butn1[0].title,titles[0]);
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)titles[1]);
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode='';
  DIL h1 = { 
    'h',
    114,298,  
    270,331,
    2,0,  
    72, 
    25, 
    2,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    checkimporthoribar1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h1.Wid,(char *)"checkimportWidget2");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+3));
  d =dtmp+i; 
  d[2].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIP));
  *d[0].p = p0;
  d[0].p->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIL));
  *d[1].h = h1;
  d[1].h->item = -1;
  d[2].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakecheckimportGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = checkimportGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int checkimport( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = checkimportGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 1;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 475;   /* Position of Dialog */ 
  D.yo = 194;
  D.xl = 396;    /*  Length of Dialog */
  D.yl = 336;    /*  Width  of Dialog */
  D.Initfun = checkimportinit;    /*   init fuction for Dialog */
  D.Cleanupfun = NULL;
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
#if 0 
  D.Callback = checkimportCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = NULL;
#if 1 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = checkimportWaitCallBack;  /*  Wait callback */
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
//  ModifycheckimportGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
int Runcheckimport(void *parent,GMIMG *png,char *titles[]) {
/*************************************************


*************************************************/
   int ret;
   GMIMG *pngrz;
   float wfac,hfac,fac;
   wfac = 256.0/png->image_width;
   hfac = 256.0/png->image_height;
   fac = wfac;
   if(hfac < fac ) fac = hfac;
//   pngrz= (GMIMG *)uiResizegmImage(png,fac); 
//   pngrz= (GMIMG *)uiThumbnailgmImage(png,(int)(png->image_width*fac),(int)(png->image_height*fac)); 
   pngrz= (GMIMG *)kgThumbNailImage(png,378,252);
   ret= checkimport(parent,(void **)pngrz,(void *)titles );
   kgFreeImage(pngrz);
   if(ret ==1 ) return 1;
   else return 0;
}
