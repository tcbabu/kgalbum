#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <kulina.h>
#define WAIT 10000
DIALOG *Main;
int xres,yres,DspXsize,DspYsize;
char *Tmpdir=NULL;
char AlbumDir[300],Album[300]="";
char HomeDir[300];
int pipefd[2];
int pipeans[2];
int Timeout=3;
int FULLSCREEN=0;

void *Runkgview(void *);

int wait_event_and_timeout(DIALOG *D,int Timeout) {
       
       KBEVENT kb;               
       int count,i;
       fd_set rfds;
       struct timeval tv;
       int retval,status;
       count = Timeout*(1000000/WAIT);
       for(i=0;i<count;i++) {
#if 0
          FD_ZERO(&rfds);
          FD_SET(0,&rfds);
          tv.tv_sec = 0;
          tv.tv_usec =WAIT;
          retval = select(1,&rfds,NULL,NULL,&tv);
#else
          usleep(WAIT);
#endif
#if 0
          if((retval> 0)&&(FD_ISSET(0,&rfds))){
           continue;
          }
          else {
#else
          {
#endif
            if(kgCheckEvent(D,&kb)==0) continue;
            if(kb.event==0) continue;
//            if(ui_Escape(kb.key)) return 0;
            if(kgEscape(kb)) return 0;
            else {
               if(kb.event==1){
                 kgSkipButtonRelease(D);
                 kgSkipEvents(D);
                 if(kb.button==1) return 1;
                 else return -1;
               }
               if(kb.event==5) {
                 kgSkipEvents(D);
                 if(kgLeftarrow(kb)) return -1;
                 if(kgRightarrow(kb)) return 1;
               }
            }
          }
       } 
       return 1;
}


int fullscreenview(DIALOG *D) {
  int k,ret,df;
  DIY *Y;
  ThumbNail **th;
  GMIMG *img;
  char **m;
  char flname[500],command[500],filter[100]={"*.jpg *.png *.JPG *.JPEG *.png"};
  char *Command;
  Command=(char *)D->pt;
  xres = D->xl;
  yres=  D->yl;
//  printf("%-s\n",Command);
  switch(Command[0]) {
    case 'W':
//      sscanf((Command+1),"%x",&Y);
//      printf("Y= %x\n",Y);
      Y = (DIY *)kgGetWidget(Main,3);
      th = (ThumbNail **)Y->list;
//      printf("Y= %x\n",Y);
      df = *(Y->df);
      if(df<=0)  df=1;
      if(df >Y->nitems) df = Y->nitems;
      *(Y->df)=df;
      while(th[df-1] != NULL) {
        *(Y->df)=df;
        kgSkipEvents(D);
        strcpy(flname,th[df-1]->name);
        img = (GMIMG *)kgGetImage(flname);
        if(img != NULL) {
           img->bkgrclr =0;  
           kgImage(D,img,0,0,xres,yres,0.0,1.0);
           kgFreeImage(img);
           kgUpdateOn(D);
         }
         if((ret=wait_event_and_timeout(D,Timeout)) == 0)  {
              break;
         }
         df +=ret;
         if(df < 1) df=1;
         if(th[df-1]== NULL) df--;
         if(df < 1) df=1;
      }
      kgSkipEvents(D);
      break;
    case 'D':
      m =NULL;
      m = kgFileMenu(Command+1,filter);
      if (m!=NULL) {
        k=0;
        while(m[k]!=NULL) {
//         ChangeCursor(45);
         kgSkipEvents(D);
         strcpy(flname,Command+1);
         strcat(flname,"/");
         strcat(flname,m[k]);
//         _box_fill(0,0,xres,yres,0);
         img = (GMIMG *)kgGetImage(flname);
         if(img != NULL) {
           img->bkgrclr =0;  
           kgImage(D,img,0,0,xres,yres,0.0,1.0);
           kgFreeImage(img);
           kgUpdateOn(D);
         }
         if((ret=wait_event_and_timeout(D,Timeout)) == 0)  {
              break;
         }
         k +=ret;
         if(k < 0) k=0;
         if(m[k]== NULL) k--;
         if(k < 0) k=0;
        }
      }
      kgSkipEvents(D);
      break;
    case 'F':
       kgSkipEvents(D);
//       uiHideCursor();
//       strcpy(flname,command+1);
//       printf("%s\n",Command+1);
       img = (GMIMG *)kgGetImage(Command+1);
       if(img != NULL) {
         img->bkgrclr =0;  
         kgImage(D,img,0,0,xres,yres,0.0,1.0);
         kgFreeImage(img);
         kgUpdateOn(D);
         while(kgCheckEscape(D)!= 1);
       }
      break;
    case 'Q':
      break;
    default:
      break;
  }
}
void FullScreenDiagbox1init(int i,void *Tmp) {
  DIALOG *D;
  DIG *G;
  D = (DIALOG *)Tmp;
  G = D->d[i].g;
  G->D = D;
//  kgDrawingTool(G);
  fullscreenview(D);
  return ;
}
int FullScreenDiainit(void *Tmp) {
  int ret = 1;
  DIALOG *D;
  D = (DIALOG *)Tmp;
  return ret;
}
int FullScreenDiaCallBack(void *Tmp,void *tmp) {
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
int FullScreenDiaGroup( DIALOG *D,void **v,void *pt) {
  int GrpId=0,oitems=0,i,j;
  DIA *d=NULL,*dtmp;
  char *xpm0=   NULL; /* pixmap info */ 
  DIG g0 = { 
    'g',
    0,2,  
    640,482,  
    (void *)xpm0,
    0, /* bkgr colour */ 
    FullScreenDiagbox1init,/* void *initgraph(int,void *)  */ 
    NULL,0,0 /* *data border hide*/
  };
  strcpy(g0.Wid,(char *)"FullScreenDiaWidget1");
  dtmp = D->d;
  i=0;
  if(dtmp!= NULL) while(dtmp[i].t!=NULL)i++;
  dtmp = (DIA *)realloc(dtmp,sizeof(DIA )*(i+2));
  d =dtmp+i; 
  d[1].t=NULL;
  d[0].t = (DIT *)malloc(sizeof(DIG));
  *d[0].g = g0;
  d[0].g->item = -1;
  d[1].t = NULL;
  GrpId=kgOpenGrp(D);
  D->d = dtmp;
  j=0;
  while(d[j].t!=NULL){ kgAddtoGrp(D,GrpId,(void *)(d[j].t));j++;}
  return GrpId;
} 

/* One can also use the following code to add Widgets to an existing Dialog */

int MakeFullScreenDiaGroup(DIALOG *D,void *arg) {
   int GrpId;
   WIDGETGRP *Gpt;
/*************************************************


*************************************************/
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   GrpId = FullScreenDiaGroup(D,v,pt);
   Gpt = kgGetWidgetGrp(D,GrpId);
   Gpt->arg= v;
   return GrpId;
}

int FullScreenDia( void *parent,void **v,void *pt) {
  int ret=1,GrpId,k;
  DIALOG D;
  DIA *d=NULL;
  D.VerId=1401010200;
  kgInitUi(&D);
  D.d=NULL;
  GrpId = FullScreenDiaGroup(&D,v,pt);
  d = D.d;
  D.d = d;
  D.bkup = 1; /* set to 1 for backup */
  D.bor_type = 4;
  D.df = 0;
  D.tw = 4;
  D.bw = 4;
  D.lw = 4;
  D.rw = 4;
  D.xo = 10;   /* Position of Dialog */ 
  D.yo = 10;
  D.xl = 1295;    /*  Length of Dialog */
  D.yl = 636;    /*  Width  of Dialog */
  D.Initfun = FullScreenDiainit;    /*   init fuction for Dialog */
  D.Cleanupfun = NULL;
  D.kbattn = 0;    /*  1 for drawing keyborad attention */
  D.butattn = 0;    /*  1 for drawing button attention */
  D.fullscreen = 1;    /*  1 for for fullscreen mode */
  D.Deco = 0;    /*  1 for Window Decorration */
  D.transparency = 0.000000;    /*  float 1.0 for full transparency */
  D.Newwin = 0;    /*  1 for new window not yet implemented */
  D.DrawBkgr = 0;    /*  1 for drawing background */
  D.Bkpixmap = NULL;    /*  background image */
  D.Sticky = 0;    /*  1 for stickyness */
  D.Resize = 0;    /*  1 for Resize option */
  D.MinWidth = 100;    /*   for Resize option */
  D.MinHeight = 100;    /*   for Resize option */
#if 0 
  D.Callback = FullScreenDiaCallBack;    /*  default callback */
#else 
  D.Callback = NULL;    
#endif
  D.ResizeCallback = NULL;
#if 1 
  D.WaitCallback = NULL;  /*  Wait callback */
#else 
  D.WaitCallback = FullScreenDiaWaitCallBack;  /*  Wait callback */
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
//  ModifyFullScreenDiaGc(&(D.gc));    /*  set colors for gui*/
  ret= kgUi(&D);
  kgCleanUi(&D);
  return ret;
}
void *RunFullScreenDia(void *arg) {
/*************************************************


*************************************************/
   int ret;
   void **v=NULL;
   void *pt=NULL; /* pointer to send any extra information */
   ret = FullScreenDia(NULL,v,arg );
   return NULL;
}
int main(int argc,char *argv[]) {
   void *Rval;
   char **m;
//   uiSetCleantmpdir();
   if((argc>1)&&(strcmp(argv[1],"-f")==0)) FULLSCREEN=1;
   chdir(getenv("HOME"));
   getcwd(HomeDir,299);
   Tmpdir=kgMakeTmpDir();
   mkdir(".Imgview",0700);
   chdir(".Imgview");
   mkdir(".Camera",0700);
   mkdir(".CoverImgs",0700);
   getcwd(AlbumDir,299);
   kgDisplaySize(&DspXsize,&DspYsize);
   Rval = Runkgview(NULL);
   return 1;
}
