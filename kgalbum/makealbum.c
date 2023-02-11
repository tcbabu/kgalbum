#include <stdlib.h>
#include <time.h>
#include <kulina.h>
#include "imagesd.c"
#include "pictures.c"
char AlbumName[500];
char CoverImgFile[300];
void *CoverImg;
extern Dlink *Alblist;
int  makealbumtextbox1callback(int cellno,int i,void *Tmp) {
  /************************************************* 
   cellno: current cell counted along column strting with 0 
           ie 0 to (nx*ny-1) 
   i     : widget id starting from 0 
   Tmp   : Pointer to DIALOG 
   *************************************************/ 
  DIALOG *D;DIT *T;T_ELMT *e; 
  int ret=1;
  D = (DIALOG *)Tmp;
  T = (DIT *)kgGetWidget(Tmp,i);
//  strcpy(AlbumName,kgGetString(T,cellno));
//  printf("%s\n",AlbumName);
  e = T->elmt;
  return ret;
}
int  makealbumsplbutton1callback(int butno,int i,void *Tmp) {
/***********************************************
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
 ***********************************************/
  char flname[300];
  DIALOG *D;DIL *B; 
  DIN *N;
  static time_t seed=-1;
  int n,ret=1,count,rno; 
  if(seed==-1) {
    seed = time(NULL);
    srandom(seed);
//    printf("Seed = %d\n",seed);
    random();
  }
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  N = (DIN *) kgGetWidget(Tmp,2);
  n = B->nx;
  count = Dcount(Alblist)+1;
  rno = random()%1000000;
  count+= rno;
  count %= 1000000;
  switch(butno) {
    case 1: 
      break;
    case 2: 
      printf("AlbumName: %s\n",AlbumName);
      sprintf(flname,".CoverImgs/%-s.jpg",AlbumName);
      kgWriteImage(CoverImg,flname);
      strcpy(CoverImgFile,flname);
      break;
  }
  return ret;
}
void  makealbumsplbutton1init(DIL *B,void *pt) {
}
int  makealbumbutton1callback(int butno,int i,void *Tmp) {
/*********************************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
 ***********************************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0,count,rno; 
  char flname[300];
  void *img;
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      if(kgPickImage(NULL,200,200,CoverImgFile)) {
        img = kgGetImage(CoverImgFile);
        CoverImg = kgChangeSizeImage(img,48,36);
        kgChangeButtonNormalImage(B,0,CoverImg);
        kgFreeImage(img);
        kgUpdateWidget(B);
      }
      break;
  }
  return ret;
}
void  makealbumbutton1init(DIN *B,void *pt) {
}
int makealbuminit(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  /* you add any initialisation here */
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int makealbumcleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  fprintf(stderr,"Cleaning ....\n");
  return ret;
}
int makealbumCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0;
  DIALOG *D;
  KBEVENT *kbe;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  if(kbe->event ==1) {
    if(kbe->button ==1) {
    }
  }
  return ret;
}
int makealbumResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 0;
  int xres,yres,dx,dy;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  kgGetWindowSize(D,&xres,&yres);
  dx = xres - D->xl;
  dy = yres - D->yl;
  /* extra code */
  D->xl= xres;
  D->yl= yres;
  kgRedrawDialog(D);
  return ret;
}
void ModifymakealbumGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int makealbumGroup( DIALOG *D,void **v,void *pt) {
  void *img;
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  T_ELMT *e0  ; 
  e0 =(T_ELMT *)malloc(sizeof(T_ELMT)*1);
  e0[0].fmt = (char *)malloc(16);
  strcpy(e0[0].fmt,(char *)"album title%20s");
  e0[0].v=(void *)v[0];
  e0[0].sw=1;
  DIT t0 = { 
    't',
    24,41,  
    356,75,
    20, 
    1,1, 
    e0,
    1,1,
    NULL,makealbumtextbox1callback,1,0,18,9 /* args,Call back */
  };
  strcpy(t0.Wid,(char *)"makealbumWidget2");
  t0.pt=NULL;
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Cancel");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='C';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"Okay");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode='L';
  DIL h1 = { 
    'h',
    125,187,  
    281,220,
    2,0,  
    72, 
    25, 
    2,1, 
    5,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    makealbumsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h1.Wid,(char *)"makealbumWidget4");
  BUT_STR  *butn2=NULL; 
  butn2= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn2[0].sw=1;
  strcpy(butn2[0].title,(char *)"");
  butn2[0].xpmn=(char *)&cover_str;
  butn2[0].xpmp=NULL;
  butn2[0].xpmh=NULL;
//  butn2[0].xpmn= (char *)malloc(100); 
//  strcpy((char *)butn2[0].xpmn,(char *)"##/home/kulina/source/demo/sample.jpg");
  img = kgGetImageCopy(NULL,(char *)&pictures_str);
  CoverImg = kgChangeSizeImage(img,48,36);
  kgFreeImage(img);
  butn2[0].bkgr=-1;
  butn2[0].butncode='';
  DIN b2 = { 
    'n',
    261,98,  
    335,172,
    2,2,  
    64, 
    48, 
    1,1, 
    1,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn2, 
    makealbumbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b2.Wid,(char *)"makealbumWidget6");
  DIM m3 = { 
    'm',
    159,122,  
    259,146,  
    1,0  
  };
  strncpy(m3.msg,(char *)"Cover picture",499);
  strcpy(m3.Wid,(char *)"makealbumWidget4");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+5));
  d =dtmp+i; 
  d[4].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIT));
  *d[0].t = t0;
  d[0].t->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIL));
  makealbumsplbutton1init(&h1,pt) ;
  *d[1].h = h1;
  d[1].h->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIN));
  makealbumbutton1init(&b2,pt) ;
  *d[2].N = b2;
  d[2].N->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIM));
  *d[3].m = m3;
  d[3].m->item = -1;
  d[4].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakemakealbumGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Text_Box1  1 data values

*************************************************/
   char  *v0 ;
   v0 = (char *)malloc(sizeof(100));
   v0[0] = '\0';
   void** v=(void **)malloc(sizeof(void*)*1);
   v[0]=(void *)(v0);
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = makealbumGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int makealbum( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = makealbumGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 2;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 205;   /* Position of Dialog */ 
  D.yo = 85;
  D.xl = 385;    /*  Length of Dialog */
  D.yl = 226;    /*  Width  of Dialog */
  D.Initfun = makealbuminit;    /*   init fuction for Dialog */
  D.Cleanupfun = makealbumcleanup;    /*   init fuction for Dialog */
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
  D.Callback = makealbumCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = makealbumResizeCallBack;  /*  Resize callback */
  D.Fixpos = 0;    /*  1 for Fixing Position */
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
  }    /*  end of fullscreen mode */
//  kgColorTheme(&D,210,210,210);    /*  set colors for gui*/
//  ModifymakealbumGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
int Runmakealbum(void *arg) {
/*************************************************

    Text_Box1  1 data values

*************************************************/
   int ret;
   char  v0[100]=" " ;
   void* v[2];
   v[0]=(void *)(AlbumName);
   v[1]= NULL;
   void *pt=NULL; /* pointer to send any extra information */
   ret=makealbum(arg,v,pt );
   fprintf(stderr,"Return\n");
   return ret-1;
}
