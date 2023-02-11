#include <kulina.h>
#include "kgviewCallbacks.h"
#include "CameraCallbacks.h"
#include "images.c"
#define Btype 5
ThumbNail **ReadAlbumList(void);
void WriteAlbumList(void);
char *GetMediaMount(void);
  
extern char *Tmpdir;
char CurDir[300];
extern char AlbumDir[300],Album[100];
extern char HomeDir[300];
char HOME[300],WrkDir[300];
char OldDir[300];
int Evgay,gindex;
PNGIMG *Backgr=NULL;
extern int FULLSCREEN;
extern int DspXsize,DspYsize;
int xsize,ysize;
DIALOG *Parent;

void ModifykgviewGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int kgviewGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char *xpm0=   NULL; /* pixmap info */ 
  DIG g0 = { 
    'g',
    279,54,  
    1077,649,  
    (void *)xpm0,
    15, /* bkgr colour */ 
    kgviewgbox1init,/* void *initgraph(int,void *)  */ 
    NULL,0,0 /* *data border hide*/
  };
  strcpy(g0.Wid,(char *)"");
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*6);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"");
  butn1[0].xpmn=(char *)kgPowerdownImage(32,25,150,250);
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='^';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"!c15\n!f14!z23NEW ");
  butn1[1].xpmn=NULL;
  butn1[1].xpmn=(char *)&album2_str;
//  butn1[1].xpmn=(char *)&book1_str;
//  butn1[1].xpmn=(char *)&new_str;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode='^';
  butn1[2].sw=1;
  strcpy(butn1[2].title,(char *)"");
//  butn1[2].xpmn=(char *)&delete_str;
  butn1[2].xpmn=(char *)&emptytrash_str;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=-1;
  butn1[2].butncode='^';
  butn1[3].sw=1;
  strcpy(butn1[3].title,(char *)"");
  butn1[3].xpmn=(char *)&import_str;
  butn1[3].xpmp=NULL;
  butn1[3].xpmh=NULL;
  butn1[3].bkgr=-1;
  butn1[3].butncode='^';
  butn1[4].sw=1;
  strcpy(butn1[4].title,(char *)"");
  butn1[4].xpmn=(char *)&camera_str;
  butn1[4].xpmp=NULL;
  butn1[4].xpmh=NULL;
  butn1[4].bkgr=-1;
  butn1[4].butncode='^';
  butn1[5].sw=1;
  strcpy(butn1[5].title,(char *)"");
  butn1[5].xpmn=(char *)&slideshow_str;
  butn1[5].xpmp=NULL;
  butn1[5].xpmh=NULL;
  butn1[5].bkgr=-1;
  butn1[5].butncode='^';
  DIL h1 = { 
    'h',
    7,0,  
    268,47,
    0,0,  
    42, 
    42, 
    6,1, 
    9,0.200000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    kgviewsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h1.Wid,(char *)"SplWidget");
  DIY x2 = { 
    'y',
    7,48,  
    268,233,   
    18,20,  
//    106, 
    90, 
    60, 
    1,0, 
    32743,7, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,kgviewbrowser1callback, /* *args, callback */
    8,  /* Border Offset  */
     18,  /* Scroll width  */
     1,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x2.Wid,(char *)"");
  DIY y3 = { 
    'y',
    6,275,  
    268,655,   
    16,16,  
    90, 
    90, 
    1,0, 
    0,3, 
    (int *)v[1], 
    NULL, 
    NULL, 
    NULL,kgviewbrowser2callback, /* *args, callback */
    10,  /* Border Offset  */
     22,  /* Scroll width  */
     1,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y3.Wid,(char *)"");
  char *xpm4=   NULL; /* pixmap info */ 
  DIP p4 = { 
    'p',
    279,54,  
    1077,649,  
    (void *)xpm4,
    -1, /* bkgr colour */ 
      3,0 /* border hide */ 
  };
  strcpy(p4.Wid,(char *)"");
  BUT_STR  *butn5=NULL; 
  butn5= (BUT_STR *)malloc(sizeof(BUT_STR)*7);
  butn5[0].sw=1;
  strcpy(butn5[0].title,(char *)"");
  butn5[0].xpmn=(char *)&previous_str;
  butn5[0].xpmp=NULL;
  butn5[0].xpmh=NULL;
  butn5[0].bkgr=45;
  butn5[0].butncode='^';
  butn5[1].sw=1;
  strcpy(butn5[1].title,(char *)"");
  butn5[1].xpmn=(char *)&next_str;
  butn5[1].xpmp=NULL;
  butn5[1].xpmh=NULL;
  butn5[1].bkgr=45;
  butn5[1].butncode='^';
  butn5[2].sw=1;
  strcpy(butn5[2].title,(char *)"");
  butn5[2].xpmn=(char *)&playback_str;
  butn5[2].xpmp=NULL;
  butn5[2].xpmh=NULL;
  butn5[2].bkgr=45;
  butn5[2].butncode='^';
  butn5[3].sw=1;
  strcpy(butn5[3].title,(char *)"");
  butn5[3].xpmn=(char *)&play_str;
  butn5[3].xpmp=NULL;
  butn5[3].xpmh=NULL;
  butn5[3].bkgr=45;
  butn5[3].butncode='^';
  butn5[4].sw=1;
  strcpy(butn5[4].title,(char *)"");
  butn5[4].xpmn=(char *)&fastback_str;
  butn5[4].xpmp=NULL;
  butn5[4].xpmh=NULL;
  butn5[4].bkgr=45;
  butn5[4].butncode='^';
  butn5[5].sw=1;
  strcpy(butn5[5].title,(char *)"");
  butn5[5].xpmn=(char *)&fastfor_str;
  butn5[5].xpmp=NULL;
  butn5[5].xpmh=NULL;
  butn5[5].bkgr=45;
  butn5[5].butncode='^';
  butn5[6].sw=1;
  strcpy(butn5[6].title,(char *)"");
//  butn5[6].xpmn=(char *)&cut_str;
  butn5[6].xpmn=(char *)&emptytrash_str;
  butn5[6].xpmp=NULL;
  butn5[6].xpmh=NULL;
  butn5[6].bkgr=-220100120;
  butn5[6].butncode='^';
  DIN b5 = { 
    'n',
    7,235,  
    267,277,
    0,0,  
    36, 
    36, 
    7,1, 
    Btype,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn5, 
    kgviewbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b5.Wid,(char *)"");
  DIHB d6 = { 
    'P',
    540,14,  
    739,37,   
    -95,205,  
    167,  
    (int *)v[2],
    NULL,
    NULL,kgviewdslide1callback, /* *args, callback */
    0,0,7,-200255, /* bordr,hide,type,color */
  };
  strcpy(d6.Wid,(char *)"");
  BUT_STR  *butn7=NULL; 
  butn7= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn7[0].sw=1;
  strcpy(butn7[0].title,(char *)"");
  butn7[0].xpmn=(char *)&right_str;
  butn7[0].xpmp=NULL;
  butn7[0].xpmh=NULL;
  butn7[0].bkgr=-1;
  butn7[0].butncode='^';
  butn7[1].sw=1;
  strcpy(butn7[1].title,(char *)"");
  butn7[1].xpmn=(char *)&left_str;
  butn7[1].xpmp=NULL;
  butn7[1].xpmh=NULL;
  butn7[1].bkgr=-1;
  butn7[1].butncode='^';
  DIN b7 = { 
    'n',
    469,5,  
    540,41,
    0,0,  
    32, 
    32, 
    2,1, 
    0,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn7, 
    kgviewbutton2callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b7.Wid,(char *)"");
  BUT_STR  *butn8=NULL; 
  butn8= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn8[0].sw=1;
  strcpy(butn8[0].title,(char *)"");
  butn8[0].xpmn=(char *)&down_str;
  butn8[0].xpmp=NULL;
  butn8[0].xpmh=NULL;
  butn8[0].bkgr=-1;
  butn8[0].butncode='^';
  butn8[1].sw=1;
  strcpy(butn8[1].title,(char *)"");
  butn8[1].xpmn=(char *)&up_str;
  butn8[1].xpmp=NULL;
  butn8[1].xpmh=NULL;
  butn8[1].bkgr=-1;
  butn8[1].butncode='^';
  DIN b8 = { 
    'n',
    739,5,  
    808,40,
    0,0,  
    32, 
    32, 
    2,1, 
    0,0.500000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn8, 
    kgviewbutton3callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b8.Wid,(char *)"");
  BUT_STR  *butn9=NULL; 
  butn9= (BUT_STR *)malloc(sizeof(BUT_STR)*2);
  butn9[0].sw=1;
  strcpy(butn9[0].title,(char *)"");
  butn9[0].xpmn=(char *)kgHomeImage(24,250,250,250);
  butn9[0].xpmp=NULL;
  butn9[0].xpmh=NULL;
  butn9[0].bkgr=-1;
  butn9[0].butncode='^';
  butn9[1].sw=1;
  strcpy(butn9[1].title,(char *)"");
  butn9[1].xpmn=(char *)kgUpdirImage(24,250,250,250);
  butn9[1].xpmp=NULL;
  butn9[1].xpmh=NULL;
  butn9[1].bkgr=-1;
  butn9[1].butncode='^';
  DIN b9 = { 
    'n',
    155,5,  
    268,44,
    4,4,  
    36, 
    36, 
    2,1, 
    Btype,0.200000,0,1,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn9, 
    kgviewbutton4callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b9.Wid,(char *)"");
  DII i10 = { 
    'i',
    6,250,  
    268,274,  
    28,0,1   
  };
  strcpy(i10.Wid,(char *)" ");
  DIX x11 = { 
    'x',
    7,48,  
    269,246,   
    10,2,  
    217, 
    20, 
    1,0, 
    0,0, 
    (int *)v[3], 
    NULL, 
    NULL, 
    NULL,kgviewbrowser3callback, /* *args, callback */
    10,  /* Border Offset  */
     22,  /* Scroll width  */
     1,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    1  /* =1 hide  */
   };
  strcpy(x11.Wid,(char *)"");
  BUT_STR  *butn12=NULL; 
  butn12= (BUT_STR *)malloc(sizeof(BUT_STR)*4);
  butn12[0].sw=1;
  strcpy(butn12[0].title,(char *)"Select All");
  butn12[0].xpmn=NULL;
  butn12[0].xpmp=NULL;
  butn12[0].xpmh=NULL;
  butn12[0].bkgr=-1;
  butn12[0].butncode='^';
  butn12[1].sw=1;
  strcpy(butn12[1].title,(char *)"Clear");
  butn12[1].xpmn=NULL;
  butn12[1].xpmp=NULL;
  butn12[1].xpmh=NULL;
  butn12[1].bkgr=-1;
  butn12[1].butncode='^';
  butn12[2].sw=1;
  strcpy(butn12[2].title,(char *)"Import");
  butn12[2].xpmn=NULL;
  butn12[2].xpmp=NULL;
  butn12[2].xpmh=NULL;
  butn12[2].bkgr=-1;
  butn12[2].butncode='^';
  butn12[3].sw=1;
  strcpy(butn12[3].title,(char *)"Finish");
  butn12[3].xpmn=NULL;
  butn12[3].xpmp=NULL;
  butn12[3].xpmh=NULL;
  butn12[3].bkgr=-1;
  butn12[3].butncode='^';
  DIN b12 = { 
    'n',
    424,3,  
    768,43,
    4,0,  
    80, 
    32, 
    4,1, 
    Btype,0.500000,0,1,-1684300643,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn12, 
    kgviewbutton5callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b12.Wid,(char *)"");
  DIY y13 = { 
    'y',
//    279,50,  
//    1081,652,   
    279,54,  
    1077,649,  
    8,20,  
    90, 
    90, 
    1,0, 
    0,0, 
    (int *)v[4], 
    NULL, 
    NULL, 
    NULL,kgviewbrowser4callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    0, /* bordr */
    1, /* bkgr */
    1  /* =1 hide  */
   };
  strcpy(y13.Wid,(char *)"");
  BUT_STR  *butn14=NULL; 
  butn14= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn14[0].sw=1;
  strcpy(butn14[0].title,(char *)"");
  butn14[0].xpmn=(char *)&rotate_str;
  butn14[0].xpmp=NULL;
  butn14[0].xpmh=NULL;
  butn14[0].bkgr=-1;
  butn14[0].butncode='^';
  DIN b14 = { 
    'n',
    424,5,  
    460,42,
    0,0,  
    32, 
    32, 
    1,1, 
    Btype,0.150000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn14, 
    kgviewbutton6callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b14.Wid,(char *)"ImgviewWidget24");
  BUT_STR  *butn15=NULL; 
  butn15= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn15[0].sw=1;
  strcpy(butn15[0].title,(char *)"");
  butn15[0].xpmn=(char *)kgPowerdownImage(32,25,150,250);
  butn15[0].xpmp=NULL;
  butn15[0].xpmh=NULL;
  butn15[0].bkgr=-1;
  butn15[0].butncode='^';
  DIL h15 = { 
    'h',
    7,0,  
    54,47,
    0,0,  
    42, 
    42, 
    1,1, 
    1,0.200000,0,1,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn15, 
    kgviewsplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h15.Wid,(char *)"SplWidget2");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+17));
  d =dtmp+i; 
  d[16].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIG));
  *d[0].g = g0;
  d[0].g->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIL));
  kgviewsplbutton1init(&h1,pt) ;
  *d[1].h = h1;
  d[1].h->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIX));
  kgviewbrowser1init(&x2,pt) ;
  *d[2].y = x2;
  d[2].y->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIY));
  kgviewbrowser2init(&y3,pt) ;
  *d[3].y = y3;
  d[3].y->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DIP));
  *d[4].p = p4;
  d[4].p->item = -1;
  d[5].t = (DIT *)malloc(sizeof(DIN));
  kgviewbutton1init(&b5,pt) ;
  *d[5].N = b5;
  d[5].N->item = -1;
  d[6].t = (DIT *)malloc(sizeof(DIHB));
  *d[6].B = d6;
  d[6].B->item = -1;
  d[7].t = (DIT *)malloc(sizeof(DIN));
  kgviewbutton2init(&b7,pt) ;
  *d[7].N = b7;
  d[7].N->item = -1;
  d[8].t = (DIT *)malloc(sizeof(DIN));
  kgviewbutton3init(&b8,pt) ;
  *d[8].N = b8;
  d[8].N->item = -1;
  d[9].t = (DIT *)malloc(sizeof(DIN));
  kgviewbutton4init(&b9,pt) ;
  *d[9].N = b9;
  d[9].N->item = -1;
  d[10].t = (DIT *)malloc(sizeof(DII));
  *d[10].i = i10;
  d[10].i->item = -1;
  d[11].t = (DIT *)malloc(sizeof(DIX));
  kgviewbrowser3init(&x11,pt) ;
  *d[11].x = x11;
  d[11].x->item = -1;
  d[12].t = (DIT *)malloc(sizeof(DIN));
  kgviewbutton5init(&b12,pt) ;
  *d[12].N = b12;
  d[12].N->item = -1;
  d[13].t = (DIT *)malloc(sizeof(DIY));
  kgviewbrowser4init(&y13,pt) ;
  *d[13].y = y13;
  d[13].y->item = -1;
  d[14].t = (DIT *)malloc(sizeof(DIN));
  kgviewbutton6init(&b14,pt) ;
  *d[14].N = b14;
  d[14].N->item = -1;
  d[15].t = (DIT *)malloc(sizeof(DIL));
  *d[15].h = h15;
  d[15].h->item = -1;
  d[16].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

void ModifyCameraGc(Gclr *gc) {
/*
//  You may change default settings here 
//  probably you can allow the user to create a config in $HOME
//  and try to read that file (if exits); so dynamic configuration is possible
   gc->FontSize =8;
   gc->Font=23;
*/
}
int CameraGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIX x0 = { 
    'x',
    7,48,  
    269,246,   
    10,2,  
    217, 
    20, 
    1,0, 
    -1077952577,8, 
    (int *)v[0], 
    NULL, 
    NULL, 
    NULL,Camerabrowser1callback, /* *args, callback */
    10,  /* Border Offset  */
     22,  /* Scroll width  */
     1,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(x0.Wid,(char *)"CameraX");
  BUT_STR  *butn1=NULL; 
  butn1= (BUT_STR *)malloc(sizeof(BUT_STR)*5);
  butn1[0].sw=1;
  strcpy(butn1[0].title,(char *)"Select All");
  butn1[0].xpmn=NULL;
  butn1[0].xpmp=NULL;
  butn1[0].xpmh=NULL;
  butn1[0].bkgr=-1;
  butn1[0].butncode='^';
  butn1[1].sw=1;
  strcpy(butn1[1].title,(char *)"Clear");
  butn1[1].xpmn=NULL;
  butn1[1].xpmp=NULL;
  butn1[1].xpmh=NULL;
  butn1[1].bkgr=-1;
  butn1[1].butncode='^';
  butn1[2].sw=1;
  strcpy(butn1[2].title,(char *)"Delete");
  butn1[2].xpmn=NULL;
  butn1[2].xpmp=NULL;
  butn1[2].xpmh=NULL;
  butn1[2].bkgr=-1;
  butn1[2].butncode='^';
  butn1[3].sw=1;
  strcpy(butn1[3].title,(char *)"Import");
  butn1[3].xpmn=NULL;
  butn1[3].xpmp=NULL;
  butn1[3].xpmh=NULL;
  butn1[3].bkgr=-1;
  butn1[3].butncode='^';
  butn1[4].sw=1;
  strcpy(butn1[4].title,(char *)"Finish");
  butn1[4].xpmn=NULL;
  butn1[4].xpmp=NULL;
  butn1[4].xpmh=NULL;
  butn1[4].bkgr=-1;
  butn1[4].butncode='^';
  DIN b1 = { 
    'n',
    288,3,  
    712,43,
    4,0,  
    80, 
    32, 
    5,1, 
    Btype,0.500000,0,0,-1684300643,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn1, 
    Camerabutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(b1.Wid,(char *)"");
  DIY y2 = { 
    'y',
    279,50,  
    1081,652,   
    8,20,  
    90, 
    90, 
    1,0, 
    0,3, 
    (int *)v[1], 
    NULL, 
    NULL, 
    NULL,Camerabrowser2callback, /* *args, callback */
    6,  /* Border Offset  */
     22,  /* Scroll width  */
     0,  /* Type  */
     1, /* item highlight */
    1, /* bordr */
    1, /* bkgr */
    0  /* =1 hide  */
   };
  strcpy(y2.Wid,(char *)"CameraY");
  BUT_STR  *butn3=NULL; 
  butn3= (BUT_STR *)malloc(sizeof(BUT_STR)*1);
  butn3[0].sw=1;
  strcpy(butn3[0].title,(char *)"");
//  butn3[0].xpmn=NULL;
  butn3[0].xpmn=(char *)kgPowerdownImage(32,25,150,250);
  butn3[0].xpmp=NULL;
  butn3[0].xpmh=NULL;
  butn3[0].bkgr=-1;
  butn3[0].butncode='^';
  DIL h3 = { 
    'h',
    7,0,  
    54,47,
    0,0,  
    42, 
    42, 
    1,1, 
    1,0.200000,0,0,0,1, /* button type and roundinfg factor(0-0.5),bordr,hide ,nodrawbkgr*/
 
    butn3, 
    Camerasplbutton1callback, /*  Callbak */
      NULL  /* any args */
  };
  strcpy(h3.Wid,(char *)"");
  DII i4 = { 
    'i',
    5,248,  
    270,272,  
    20,1,0   
  };
  strcpy(i4.Wid,(char *)"CameraWidget5");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+6));
  d =dtmp+i; 
  d[5].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIX));
  Camerabrowser1init(&x0,pt) ;
  *d[0].x = x0;
  d[0].x->item = -1;
  d[1].t = (DIT *)malloc(sizeof(DIN));
  Camerabutton1init(&b1,pt) ;
  *d[1].N = b1;
  d[1].N->item = -1;
  d[2].t = (DIT *)malloc(sizeof(DIY));
  Camerabrowser2init(&y2,pt) ;
  *d[2].y = y2;
  d[2].y->item = -1;
  d[3].t = (DIT *)malloc(sizeof(DIL));
  Camerasplbutton1init(&h3,pt) ;
  *d[3].h = h3;
  d[3].h->item = -1;
  d[4].t = (DIT *)malloc(sizeof(DII));
  *d[4].i = i4;
  d[4].i->item = -1;
  d[5].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeCameraGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************

    Selectmenu1  1 data value
    ThumbnailBrowser2  1 data value

*************************************************/
   int  *v0 ;
   v0 = (int *)malloc(sizeof(int));
   *v0 = 1;
   int  *v1 ;
   v1 = (int *)malloc(sizeof(int));
   *v1 = 1;
   void** v=(void **)malloc(sizeof(void*)*3);
   v[0]=(void *)(v0);
   v[1]=(void *)(v1);
   v[2]=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = CameraGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int demoGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  DIM m0 = { 
    'M',
    10,3,  
    336,45,  
    0,0  
  };
  strncpy(m0.msg,(char *)"press 'esc' Or click here to interrupt",499);
  strcpy(m0.Wid,(char *)"Intrpt");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+2));
  d =dtmp+i; 
  d[1].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIM));
  *d[0].m = m0;
  d[0].m->item = -1;
  d[1].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakedemoGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = demoGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int kgview( void *parent,void **v,void *pt) {
  char *media;
  int ret=1,GrpId,k,gid,dx,dy;
  void *ptrs[2];
  DIALOG D;
  DIA *d=NULL;
  DIX *x11; DIY *x2; DIY *y13,*y3,*ycam;
  DIP *p4;
  DIG *g0;

  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = kgviewGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  g0  = d[0].g;
  p4  = d[4].p;
  y3  = d[3].y;
  y13 = d[13].y;
  x2  = d[2].y;
  x11 = d[11].x;
  chdir(AlbumDir);
  strcpy(OldDir,HomeDir);
  x2->list = (void **)ReadAlbumList();
  strcpy(HOME,getenv("HOME"));
  strcpy(WrkDir,HOME);
  ptrs[0]=WrkDir;
  ptrs[1]=HOME;
  x11->list = (void **)kgFolderThumbNails(WrkDir);
  y13->list = (void **)kgMakeThumbNails(WrkDir,y13->width);
  gid = kgOpenGrp(&D);
  kgAddtoGrp(&D,gid,kgGetWidget(&D,0));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,1));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,2));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,4));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,5));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,6));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,7));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,8));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,14));
//  printf("gid= %d\n",gid);

  gid = kgOpenGrp(&D);
  kgAddtoGrp(&D,gid,kgGetWidget(&D,9));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,10));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,11));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,12));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,13));
  kgAddtoGrp(&D,gid,kgGetWidget(&D,15));
  kgSetGrpVisibility(&D,gid,0);
  gid = MakeCameraGroup(&D,NULL);
  kgSetGrpVisibility(&D,gid,0);
  ycam = (DIY *) kgGetNamedWidget(&D,(char *)"CameraY");
  gid = MakedemoGroup(&D,NULL);
  kgSetGrpVisibility(&D,gid,0);
  gid = kgOpenGrp(&D);
  kgAddtoGrp(&D,gid,kgGetWidget(&D,1));

  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 2;
  D.df = 14;
  D.tw = 50;
  D.bw = 7;
  D.lw = 276;
  D.rw = 7;
  D.xo = 0;   /* Position of Dialog */ 
  D.yo = 0;
  D.xl = 1087;    /*  Length of Dialog */
  D.yl = 660;    /*  Width  of Dialog */
  D.Initfun = kgviewinit;    /*   init fuction for Dialog */
  D.Cleanupfun = kgviewcleanup;    /*   init fuction for Dialog */
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 0;    /*  1 for for fullscreen mode */
  D.Deco = 1;    /*  1 for Window Decorration */
  D.transparency = 0.00000;    /*  float 1.0 for full transparency */
  D.Newwin = 1;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 1;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 1;    /*  1 for Resize option */
  D.MinWidth = 800;    /*   for Resize option */
  D.MinHeight = 460;    /*   for Resize option */
#if 1 
  D.Callback = kgviewCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = kgviewResizeCallBack;  /*  Resize callback */
  D.Fixpos = 0;    /*  1 for Fixing Position */
  D.NoTaskBar = 0;    /*  1 for not showing in task bar*/
  D.StackPos = 0;    /* -1,0,1 for for Stack Position -1:below 0:normal 1:above*/
  D.Shapexpm = NULL;    /*  PNG/jpeg file for window shape;Black color will not be drawn */
  D.parent = parent;    /*  1 for not showing in task bar*/
  D.pt = ptrs;    /*  any data to be passed by user*/
  strcpy(D.name,"Kulina Digital Album: ver 1.0");    /*  Dialog name you may change */
  D.fullscreen= FULLSCREEN;
  if(D.fullscreen!=1) {    /*  if not fullscreen mode */
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     if(xres>(D.xl+10)) xres = D.xl+10;
     dx = xres-10 -D.xl;
     dy = yres-80 -D.yl;
#if 1
     if(dy > 0.75*dx )dy = 0.75*dx;
     else dx = dy*4.0/3.0;
     D.xo=D.yo=0; D.xl += dx; D.yl+=dy;
#else
     D.xo=D.yo=0; D.xl = xres-10; D.yl=yres-80;
#endif
     y3->y2 +=dy;
     g0->x2 +=dx;
     g0->y2 +=dy;
     p4->x2 +=dx;
     p4->y2 +=dy;
     y13->x2+=dx;
     y13->y2+=dy;
     ycam->x2+=dx;
     ycam->y2+=dy;
  }
  else {    // for fullscreen
     int xres,yres; 
     kgDisplaySize(&xres,&yres); 
     D.Deco =0;
     D.bor_type = 0;
     dx = xres -D.xl;
     dy = yres -D.yl;
     D.xo=D.yo=0; D.xl = xres; D.yl=yres;
#if 1
     y3->y2 +=dy;
//     g0=d[0].g;
     g0->x2 +=dx;
     g0->y2 +=dy;
     p4->x2 +=dx;
     p4->y2 +=dy;
     y13->x2+=dx;
     y13->y2+=dy;
     ycam->x2+=dx;
     ycam->y2+=dy;
#endif
  }    /*  end of fullscreen mode */
  kgColorTheme(&D,54,69,54);    /*  set colors for gui*/
//  ModifykgviewGc(&(D.gc));    /*  set colors for gui*/
//  Print_gui_data(&D,"/root/kgview.rc");
  Parent = &D;
  ret= kgUi(&D);
//  kgCleanUi(&D);
  return ret;
}
void *Runkgview(void *arg) {
/*************************************************

    Selectmenu1  1 data value
    ThumbnailBrowser2  1 data value
    Integerslidebar1  1 data value
    Selectmenu3  1 data value
    ThumbnailBrowser4  1 data value

*************************************************/
   int   v0 = 1;
   int   v1 = 1;
   int   v2 = 1;
   int   v3 = 1;
   int   v4 = 1;
   void* v[5];
   int ret;
   v[0]=(void *)(&v0);
   v[1]=(void *)(&v1);
   v[2]=(void *)(&v2);
   v[3]=(void *)(&v3);
   v[4]=(void *)(&v4);
   void *pt=NULL; /* pointer to send any extra information */
   ret = kgview(NULL,v,pt );
   return NULL;
}
