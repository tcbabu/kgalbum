#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <kulina.h>
#include "rdwrfuns.h"
#include "CameraCallbacks.h"
#include "fstree.h"
FSTREE *cnode=NULL,*curnode;
static int old=-1;
extern DIALOG *Main;
extern char AlbumName[100];
extern char CoverImgFile[200];
extern void *CoverImg;
extern int CameraId;
int RunSelectCamera(void *arg);
int CheckCameras(void);
void CleanCamera(void);
ThumbNail ** GetCameraList(void);
ThumbNail **GetImageList (int CameraId);
int DeleteCameraFile(int CameraId,int ImageId);
int CopyCameraFile_o(int CameraId,int ImageId,char *outfile);
void ** GetCameraFolderList(const char *folder);
void *CopyCameraThumbnail( char *folder,char *fn);
void CopyCameraFile(char *folder,char *fn,char *outfile);

int UpdateImportBrowsers(void *Tmp);
int ImportPictures(void *parent,void *arg);
int ArrayLength(char **m);
void UpdateBrowser(void *Tmp,int i,char *newdir);
void UpdateBrowser3(void *Tmp,char *newdir,char *Album);
extern int pipefd[2];
extern int pipeans[2];
void *RunFullScreenDia(void *arg);
int Runcheckimport(void * parent,GMIMG *png,char *titles[]);
int Runmakealbum(void *arg);


extern int DspXsize,DspYsize;
extern int xsize,ysize,xres,yres;
extern int Evgay,gindex; 
extern char AlbumDir[300],Album[100]; 
extern char CurDir[300],HomeDir[300],OldDir[300]; 
extern char HOME[300],WrkDir[300];
extern Dlink *Alblist,*Photolist;
char Imgfile[500]=" ";
char filter[100]= "*.jpg *.png *.JPG *.PNG *.JPEG";
static GMIMG *Png=NULL,*Pngrz=NULL;
static GMIMG *Img=NULL,*Imgrz=NULL;
extern PNGIMG *Backgr;
int Val=0,Xoff=0,Yoff=0;
int ImgNo=0;

void FreeMem(char **m);
void *Runcameradia(void *parent);
PHOTO * ImportImage(char *Imgfile);
int ImportThumbnailImage(void *Img);


char *GetMediaMount(void) {
  char *pt;
  FILE *fp;
  char buff[300];
  char data[300];
  int ch,i;
  fp = popen("mount","r");
  if(fp != NULL) {
    while (fgets(buff,299,fp) != NULL) {
      i=0;
      while(buff[i]> ' ') i++;
      i++;
      while(buff[i]> ' ') i++;
      i++;
      sscanf(buff+i,"%s",data);
//      printf("%s\n",data);
      ch =data[6];
      data[6]='\0';
      if (strcmp(data,(char *)"/media")==0) {
        data[6]=ch;
        pt = (char *)malloc(strlen(data)+1);
        strcpy(pt,data);
        pclose(fp);
//        printf("%s\n",pt);
        return pt;
      }
    }
    pclose(fp);
  }
  return NULL;
}
int CleanDir(char *Album) {
  char buff[500];
  char **m;
  int k;
  if(Album== NULL) return 0;
  if(Album[0]=='\0') return 0;
#if 1
  sprintf(buff,"rm -r -f  \"%-s\"",Album);
//  printf("%s\n",buff);
  system(buff);
#else
  kgCleanDir(Album);
#endif
  return 1;
}
PHOTO * ImportImage(char *Imgfile) {
  char flname[600];
  PHOTO *ph;
  int width=1000,height=600;
  float xo=10,yo=10;
  float x1,y1,x2,y2;
  int w,h;
  float wfac=1.0,hfac=1.0,fac,sfac=1.0;
  GMIMG *png=NULL,*pngrz=NULL;
   fac = 1.0;
   wfac =1.0;
   hfac =1.0;
   sprintf(flname,"%-s/%-s/img_%-4.4d.jpg",AlbumDir,Album,ImgNo);
   png = (GMIMG *)kgGetImage(Imgfile);
   if(png == NULL) return NULL;
   ph = (PHOTO *) malloc(sizeof(PHOTO));
   sprintf(ph->name,"img_%-4.4d.jpg",ImgNo);
   if(png->image_channels==4){
     sprintf(flname,"%-s/%-s/img_%-4.4d.png",AlbumDir,Album,ImgNo);
     sprintf(ph->name,"img_%-4.4d.png",ImgNo);
   }
   sprintf(ph->orgfile,"%-s",Imgfile);
   w = png->image_width;
   h = png->image_height;
   png->rzfac=1.0;
   if(w>DspXsize*sfac)  wfac = ((float)DspXsize*sfac)/((float)w);
   if(h > DspYsize*sfac ) hfac = ((float)DspYsize*sfac)/((float)h);
   if((wfac==1.0)&&(hfac==1.0)) {
     kgWriteImage(png,flname);
   }
   else {
      fac = hfac;
      if(fac > wfac ) fac = wfac;
//      pngrz = (GMIMG *)kgScaleImage(png,fac);
      pngrz = (GMIMG *)kgResizeImage(png,fac);
      kgWriteImage(pngrz,flname);
      kgFreeImage(pngrz);
      pngrz=NULL;
   }
   kgFreeImage(png);
   png=NULL;
   ImgNo++;
   return ph;
}
int ImportThumbnailImage(void *Img) {
  char flname[300];
  int width=1000,height=600;
  float xo=10,yo=10;
  float x1,y1,x2,y2;
  int w,h;
  float wfac=1.0,hfac=1.0,fac,sfac=1.0;
  GMIMG *png=NULL,*pngrz=NULL;
  png = (GMIMG *)Img;
   fac = 1.0;
   wfac =1.0;
   hfac =1.0;
   if(png->image_channels==4){
     sprintf(flname,"%-s/%-s/.Thumbnail/img_%-4.4d.png",AlbumDir,Album,ImgNo);
   }
   else sprintf(flname,"%-s/%-s/.Thumbnail/img_%-4.4d.jpg",AlbumDir,Album,ImgNo);
   kgWriteImage(Img,flname);
   return ImgNo;
}
void FreeMem(char **m) {
  int i=0;
  char *pt;
  if(m==NULL) return;
  while( (pt=m[i]) != NULL) {free(pt);i++;}
  return;
}
int MakeImgChangePixmap(void * Tmp,char *Imgfile) {
    DIG *G;
    DIHB *H;
    void *tmpimg;
    int w,h;
    float wfac=1.0,hfac=1.0,fac=1.0,ret=1;
    G = ((DIALOG *)Tmp)->d[0].g;
    xsize = G->x2-G->x1+1;
    ysize = G->y2-G->y1+1;
    H = (DIHB *)kgGetWidget(Tmp,6);
#if 0
#else
         if(Img != NULL) {
           kgFreeImage(Img);
           Img = NULL;
         }
         if(Imgrz != NULL) {
           kgFreeImage(Imgrz);
           Imgrz = NULL;
         }
         Val =0;
         Xoff=Yoff=0;
         *(H->df) =0;
         kgUpdateWidget(H);
         Img = (GMIMG *)kgGetImage(Imgfile);
         if(Img != NULL) {
           Img->bkgrclr=15;
           w = Img->image_width;
           h = Img->image_height;
           if(w>xsize)  wfac = ((float)xsize)/((float)w);
           if(h > ysize ) hfac = ((float)ysize)/((float)h);
           fac = hfac;
           if(fac > wfac ) fac = wfac;
           Img->rzfac=fac;
           if(fac > 1.0) {
             tmpimg = kgResizeImage(Img,fac);
             kgImage(Tmp,tmpimg,G->x1,G->y1,xsize,ysize,0.0,1.0);
             kgFreeImage(Img);
             Img=tmpimg;
           }
           else kgImage(Tmp,Img,G->x1,G->y1,xsize,ysize,0.0,1.0);
         }
         else {
           ret=0;
           printf("Img == NULL: %s\n",Imgfile);
         }
         return ret;
#endif
}
void DisplayCurrentImage(void *Tmp) {
  DIY *Y;
  ThumbNail **m;
  int df;
  Y = (DIY *)kgGetWidget(Tmp,3);
  m = (ThumbNail **) (Y->list);
  if(m== NULL) return;
  df = *(Y->df);
  if(m[df-1] != NULL) {
        strcpy(Imgfile,m[df-1]->name);
        MakeImgChangePixmap(Tmp,Imgfile);
        kgUpdateWidget(Y);
        kgUpdateOn(Tmp);
 }
}
void RotateAndShow(void * Tmp) {
    DIG *G;
    DIHB *H;
    int w,h;
    void *Img1;
    float wfac=1.0,hfac=1.0,fac=1.0;
    G = ((DIALOG *)Tmp)->d[0].g;
    xsize = G->x2-G->x1;
    ysize = G->y2-G->y1;
    if(Img != NULL) {
      Img1 = kgRotateImage(Img,90.0);
      kgFreeImage(Img);
      Img = (GMIMG *)Img1;
      kgImage(Tmp,Img,G->x1,G->y1,xsize,ysize,0.0,1.0);
    }
    return ;
}

void kgviewgbox1init(int i,void *Tmp) {
  /*********************************** 
    int routine for grahics area 
   ***********************************/ 
  DIALOG *D;void *pt;
  DIG *G;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  G = D->d[i].g;
  G->D = (void *)(Tmp);
  return ;
}
void UpdateBrowser3(void *Tmp,char *AlbumDir,char *Album) {
  ThumbNail **th;
  char **m,**dir;
  void **pt;
  DIALOG *D;
  DIY *X=NULL;
  int k=0;
  D = (DIALOG *)Tmp;
  X = (DIY *)kgGetWidget(Tmp,2);
  th = ReadAlbumList();
  kgFreeThumbNails((ThumbNail **)(X->list));
  kgSetList((void *)X, (void **)th);
  th = (ThumbNail **)X->list;
  if(Album!= NULL) {
    k=0;
    while (th[k]!= NULL) {
      if( strcmp(th[k]->name,Album)==0) {
       *(X->df)=k+1;
        th[k]->sw=1;
        break;
      }
      k++;
    }
  }
  kgUpdateWidget(X);
  return;
}

int checkdup(void *s,void *d) {
  int i=0,j=0,k;
  char so[300],ds[300];
  strncpy(so,(char *)s,300);
  strncpy(ds,(char *)d,300);
  so[299]='\0';
  while(so[i]==' ')i++;
  ds[299]='\0';
  while(ds[j]==' ')j++;
  if(strcmp((char *)(so+i),(char *)(ds+j))==0) {
   return 1;
  }
  else return 0;
}

int  kgviewsplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=0; 
  D = (DIALOG *)Tmp;
  DIY *X;DIX *x11;
  DIY *Y,*y13;
  void **pt;
  char buff[500],**m,**dir,*album;
  char *media;
  static int img=0;
  static char flname[300],fltr[200]="*.jpg *.png *.JPG *.JPEG *.PNG *.jpeg";
  char *CurDir;
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  int width=1000,height=600;
  float xo=10,yo=10;
  float x1,y1,x2,y2;
  pt = (void **)(D->pt);
  CurDir=(char *)pt[0];
  X = (DIY *)kgGetWidget(Tmp,2);
  x11 = (DIX *)kgGetWidget(Tmp,11);
  y13 = (DIY *)kgGetWidget(Tmp,13);
  switch(butno) {
    case 1: 
      ret=1;
      break;
    case 2: 
//      strcpy(flname,"NewAlbum");
//      gscanf((void *)Tmp,(void *)"New Album Name: %25s",flname);
      chdir(AlbumDir);
      strcpy(AlbumName,"NewAlbum");
      if(!Runmakealbum(Tmp)) break;
      strcpy(flname,AlbumName);
      strcpy(buff,AlbumDir);
      strcat(buff,"/");
      strcat(buff,flname);
      mkdir(buff,0755);
      album= (char *)malloc(600);
      strcpy(album,flname);
      strcpy(album+300,CoverImgFile);
      Dappend(Alblist,album);
      Drmvdup_cond(Alblist,checkdup);
      WriteAlbumList();
      strcpy(Album,flname);
      kgSetSelection(X,Dcount(Alblist));
      UpdateBrowser3(Tmp,AlbumDir,flname);
      chdir(buff);
      mkdir((char *)".Thumbnail",0755);
      UpdateBrowser(Tmp,3,buff);
      kgUpdateOn((DIALOG *)Tmp);
      old=-1;
      break;
    case 3: 
      if(Album[0]=='\0') break;
        Y = (DIY *)kgGetWidget(Tmp,3);
        sprintf(buff,"Removing Album: %-s",Album);
        if(kgCheckMenu(Tmp,100,100,buff,0)) {
          int df;
          df = kgGetSelection(X);
          Dposition(Alblist,df);
          Ddelete(Alblist);
          chdir(AlbumDir);
          WriteAlbumList();
          CleanDir(Album);
          UpdateBrowser3(Tmp,AlbumDir,NULL);
          Album[0]='\0';
          kgFreeThumbNails((ThumbNail **)Y->list);
          kgSetList(Y,NULL);
          kgUpdateWidget(Y);
        }
      break;
    case 4: 
      if(Album[0]!='\0'){
          CameraId=-1;
          media=GetMediaMount();
          kgSetGrpVisibility(Tmp,2,0);
          kgUpdateGrp(Tmp,2);
          kgSetGrpVisibility(Tmp,3,1);
          if(media != NULL) {
            strcpy(WrkDir,media);
            CurDir=media;
          }
          else strcpy(WrkDir,HOME);
          kgUpdateGrp(Tmp,3);
          kgFreeThumbNails((ThumbNail **)kgGetList(x11));
          kgFreeThumbNails((ThumbNail **)kgGetList(y13));
          x11->list = (void **)kgFolderThumbNails(WrkDir);
          y13->list = (void **)kgMakeThumbNails(WrkDir,y13->width);
          kgUpdateWidget(x11);
          kgUpdateWidget(y13);
          kgPrintf(Tmp,10,CurDir);
          if(media != NULL) free(media);
      }
      else kgSplashMessage(Tmp,10,30,200,26,(char *)"First Select An Album",12,-250250250,-120250);
      break;
    case 5: 
      // Camera code
      if(Album[0]!='\0'){
          int count;
          void * id;
          void **list;
          Main = (DIALOG *)Tmp;
          FreeFstree(cnode);
          cnode=NULL;
          count = CheckCameras();
          if((count > 0)&&RunSelectCamera(Tmp)) {
            DIX *X;
            DIY *Y;
            kgSetGrpVisibility(Tmp,2,0);
            kgUpdateGrp(Tmp,2);
            kgSetGrpVisibility(Tmp,3,1);
            X = x11;
            Y = y13;
            kgFreeThumbNails((ThumbNail **)X->list);
            kgFreeThumbNails((ThumbNail **)Y->list);
            kgSetList(X,NULL);
            kgSetList(Y,NULL);
            kgUpdateGrp(Tmp,3);
            sprintf( buff,"Pl. Wail... Searching");
            kgPrintf(Tmp,10, buff);
            kgUpdateOn(Tmp);
            id = kgOpenBusy(Tmp,200,150);
            cnode = InitFoldernode("/",NULL,NULL);
            curnode=cnode;
            ExploreFsnode(cnode,"*");
            list = (void **)MakeFolderNails(cnode,22,22);
            kgSetList(X,list);
            list = (void **)MakeFileNails(cnode,90,60);
            kgSetList(Y,list);
            kgCloseBusy(id);
            kgUpdateWidget(X);
            kgUpdateWidget(Y);
            sprintf(buff,"%s :  OKAY",curnode->name);
            kgPrintf(Tmp,10,buff);
            kgUpdateGrp(Tmp,3);
          }
          else {
            kgSplashMessage(Tmp,30,100,300,86,(char *)"No Camera Connected\n Or\nUnsupported Camera",12,-250250250,-120250);
          }
      }
      else kgSplashMessage(Tmp,10,30,200,26,(char *)"First Select An Album",12,-250250250,-120250);
      break;
    case 6: 
      if(Album[0]!='\0'){
        int df;
        void *rpt;
        ThumbNail **m;
        Y = (DIY *)kgGetWidget(Tmp,3);
        Main = (DIALOG *)Tmp;
        sprintf(buff,"W%-lx %-lx",Y,Y->list);
        rpt= RunFullScreenDia(buff);
        kgUpdateWidget(Y);
        m = (ThumbNail **)Y->list;
        df = *(Y->df);
        if(m[df-1] != NULL) {
          strcpy(Imgfile,m[df-1]->name);
          sprintf(buff,"%-s : %-s",Album,Imgfile);
          MakeImgChangePixmap(Tmp,Imgfile);
        }
        kgUpdateOn((DIALOG *)Tmp);
      }
      else kgSplashMessage(Tmp,10,30,200,26,(char *)"First Select An Album",12,-250250250,-120250);
      break;
    deafault:
      break;
  }
  return ret;
}
int  Imgviewbutnbox2callback(int key,int i,void *Tmp) {
  DIA *D;DIB *B;
  DIY *X;
  DIY *Y;
  ThumbNail **th;
  char buff[500];
  int n,ret =1,k,ln;
  char **m,**dir,**file;
  D = ((DIALOG *)Tmp)->d;
  B = D[i].b;
  n = B->nx*B->ny;
  switch(key) {
    case  1:
        chdir(AlbumDir);
        Album[0]='\0';
        X = (DIY *)kgGetWidget(Tmp,3);
        Y = (DIY *)kgGetWidget(Tmp,4);
        th = (ThumbNail **)X->list;
        n = X->nitems;
        for(k=0;k<n;k++) { th[k]->sw=0; }
        kgUpdateWidget(X);
        kgFreeThumbNails((ThumbNail **)Y->list);
        kgSetList(Y,NULL);
        kgUpdateWidget(Y);
        old=-1;
      break;
    case  2:
        if(Album[0]=='\0') break;
        X = (DIY *)kgGetWidget(Tmp,3);
        Y = (DIY *)kgGetWidget(Tmp,4);
        kgPrintf(Tmp,7,Album);
        sprintf(buff,"Removing Album: %-s",Album);
        if(kgCheckMenu(Tmp,100,100,buff,0)) {
          int df;
          df = kgGetSelection(X);
          Dposition(Alblist,df);
          Ddelete(Alblist);
          chdir(AlbumDir);
          WriteAlbumList();
          CleanDir(Album);
          UpdateBrowser3(Tmp,AlbumDir,NULL);
          Album[0]='\0';
          kgFreeThumbNails((ThumbNail **)Y->list);
          kgSetList(Y,NULL);
          kgUpdateWidget(Y);
          kgPrintf(Tmp,7,(char *)"Select/Create Album");
        }
      break;
  }
//  UpdateBrowser(Tmp,4,AlbumDir);
  kgUpdateOn(Tmp);
  return ret;

}
void  kgviewsplbutton1init(DIL *B,void *pt) {
}
void UpdateBrowser(void *Tmp,int i,char *newdir) {
  ThumbNail **tb,**tbpt;
  char buf[20];
  void **pt;
  DIALOG *D;
  DIY *Y;
  void **xpm;
  int *sw,df,k,no;
  void *id;
  char **m,**file;
  D = ((DIALOG *)Tmp);
  Y = D->d[i].y;
  id = kgOpenBusy(D,200,150);
  kgFreeThumbNails((ThumbNail **)Y->list);
//  tb =(ThumbNail **)kgMakeThumbNails(newdir,Y->width);
  tb =(ThumbNail **)ReadPhotoList();
//  Y->list = (void **)tb ;
  kgSetList((void *)Y,(void **)tb);
  kgSetSelection((void *)Y,1);
  kgCloseBusy(id);
  kgUpdateWidget(Y);
  ImgNo=0;
  if((tb!=NULL) &&(tb[0]!=NULL)){
    df =1;
    df=*(Y->df);
    strcpy(Imgfile,tb[df-1]->name);
    tb[df-1]->sw=1;
    MakeImgChangePixmap(Tmp,Imgfile);
    k=0;
    while(tb[k]!=NULL) {
       strcpy(buf,(char *)(tb[k]->name));
       buf[8]='\0';
       sscanf(buf+4,"%d",&no);
       if(no>ImgNo) ImgNo=no;
       k++;
    }
    ImgNo++;
  }
  kgUpdateWidget(Y);
}
int  kgviewbrowser1callback(int item,int i,void *Tmp) {
   DIA *D;DIY *B;DIE *E;
   DIY *Y;
   DIR *dp;
  void **pt;
  void **xpm;
  int *sw;
  int ret=1,k,ln,df,pos,x,y;
  char newdir[300];
  char **m,**dir,**file,*folder;
  ThumbNail *tpt,**th;
  void *ph;
  D = ((DIALOG *)Tmp)->d;
  pt = (void **)(((DIALOG *)Tmp)->pt);
  B = D[i].y;
  Y = D[i+1].y;
  th = (ThumbNail **)B->list;
#if 1
  if(kgDragThumbNail(B,item-1,&x,&y)) {
     pos = kgGetThumbNailItem(B,x,y);
     if((pos >=0)&&(pos!=(item-1)) ) {
       tpt = th[item-1];
       if(pos < (item-1) ) {
          for(k=(item-1);k>pos;k--){ th[k]=th[k-1]; }
          th[pos]= tpt;
       }
      else {
          for(k=item;k<pos;k++) {th[k-1]=th[k];}
          th[pos-1]= tpt;
      }
      *(B->df) = pos+1;
      pos = *(B->df);
      Dposition(Alblist,item);
      ph = Dpick(Alblist);
      if(pos > item) {
        pos--;
      }
      Dposition(Alblist,pos);
      Dinsert(Alblist,ph);
      chdir(AlbumDir);
      WriteAlbumList();
      kgSetList((void *)B,(void **)th);
      kgUpdateWidget(B);
      kgUpdateOn(Tmp);
      item=pos;
      old=-1;
    }
    
  }
#endif
  if(item==old) return 0;
  old = item;
  folder=kgGetSelectedString(B);
  strcpy(Album,folder);
  strcpy(newdir,AlbumDir);
  strcat(newdir,"/");
  strcat(newdir,folder);
  dp = opendir(newdir);
  if(dp==NULL) {
    kgUpdateOn(Tmp);
    return 0;
  }
  chdir(newdir);
  *(Y->df) =1;
  UpdateBrowser(Tmp,i+1,newdir);
  kgUpdateOn(Tmp);
  return ret;
}
int  kgviewbrowser2callback(int item,int i,void *Tmp) {
  ThumbNail **th,*tpt;
  DIA *D;DIY *B;
  DIG *G;
  PHOTO *ph;
  char *m;
  char buff[300];
  char flname[30]="/tmp/junk";
  int ret=1,ln,pos,x,y,k;
  D = ((DIALOG *)Tmp)->d;
  G= (DIG *)kgGetWidget(Tmp,0);
  if(G->hide==1) return 0;
  B = D[i].y;
  th =(ThumbNail **)(B->list);
  if((th==NULL)||(th[0]==NULL)) return 0;
#if 1
  if(kgDragThumbNail(B,item-1,&x,&y)) {
     pos = kgGetThumbNailItem(B,x,y);
     if((pos >=0)&&(pos!=(item-1)) ) {
       tpt = th[item-1];
       if(pos < (item-1) ) {
          for(k=(item-1);k>pos;k--){ th[k]=th[k-1]; }
          th[pos]= tpt;
       }
      else {
          for(k=item;k<pos;k++) {th[k-1]=th[k];}
          th[pos-1]= tpt;
      }
      *(B->df) = pos+1;
      pos = *(B->df);
      Dposition(Photolist,item);
      ph = (PHOTO *)Dpick(Photolist);
      if(pos > item) {
        pos--;
      }
      Dposition(Photolist,pos);
      Dinsert(Photolist,ph);
      WritePhotoList();
      kgSetList((void *)B,(void **)th);
//      kgSetList((void *)B, (void **)ReadPhotoList());
      kgUpdateWidget(B);
      kgUpdateOn(Tmp);
    }
  }
#endif
  m=th[*(B->df)-1]->name;
  strcpy(Imgfile,m);
  sprintf(buff,"%-s : %s",Album,Imgfile);
  MakeImgChangePixmap(Tmp,Imgfile);
  switch(item) {
    case 1:
      break;
  }
  kgUpdateOn((DIALOG *)Tmp);
  return ret;
}

int  kgviewdslide1callback(int val,int i,void *Tmp) {
  DIA *D;DIHB *SD; 
  int ret=1,w,h; 
  float fac=1.0,oldfac,newfac;
  static void *tmpimg=NULL;
  void *rzimg;
  DIG *G;
  G = ((DIALOG *)Tmp)->d[0].g;
  xsize = G->x2-G->x1+1;
  ysize = G->y2-G->y1+1;
  D = ((DIALOG *)Tmp)->d;
  SD = D[i].B;
  if(Img==NULL) return ret;
  if(val == 0) return 0;
  oldfac = Img->rzfac;
  fac = (val+100.)/100.;
  if(fac==0.0) fac = 0.001;
  if( fabsf(oldfac -fac) > 0.10) {
    Img->rzfac = fac;
    if((fac < oldfac)||(fac < 1)||((oldfac<1)&&(fac >1))) {
      rzimg = kgResizeImage(Img,fac);
    }
    else {
      newfac = fac/oldfac;
      if(oldfac==1.0) {
        rzimg = kgResizeImage(Img,newfac);
      }
      else rzimg = kgResizeImage(tmpimg,newfac);
//    kgImage(Tmp,Img,G->x1,G->y1,xsize,ysize,0.0,1.0);
    }
    kgImage(Tmp,rzimg,G->x1,G->y1,xsize,ysize,0.0,1.0);
    kgFreeImage(tmpimg);
    tmpimg=rzimg;
    kgUpdateOn(Tmp);
  }
  return ret;
}
void  kgviewbrowser1init(DIY *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
}
void  kgviewbrowser2init(DIY *Y,void *pt) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
}
int  kgviewbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  D = (DIALOG *)Tmp;
  DIP *P;
  int n,ret =0,k,ans; 
  void **xpm;
  GMIMG *png;
  GMIMG *img;
  DIY *Y;
  DIM *M,*X;
  char buff[300];
  ThumbNail **m;
  char *titles[]={(char *)"Remove",(char *)"Cancel",NULL};
  int df;
  KBEVENT kb;
  B = (DIN *)kgGetWidget(Tmp,i);
  P = (DIP *)kgGetWidget(Tmp,4);
  n = B->nx*B->ny;
  Y = (DIY *)kgGetWidget(Tmp,3);
  M = (DIM *)kgGetNamedWidget(Tmp,(char *)"Intrpt");
  if(Album[0]=='\0') return ret;
  m = (ThumbNail **)Y->list;
  if((m==NULL)||(m[0]==NULL)) {
   return ret;
  }
  df = *(Y->df);
  if(df==0)  df=1;
  if(df >Y->nitems) df = Y->nitems;
  *(Y->df)=df;
  switch(butno) {
    case 3: 
    case 5: 
      df--;
      kgSetGrpVisibility(Tmp,6,0);
      kgUpdateGrp(Tmp,6);
      kgSetGrpVisibility(Tmp,5,1);
      kgUpdateGrp(Tmp,5);
      while(df> 0) {
        *(Y->df)=df;
        strcpy(Imgfile,m[df-1]->name);
        kgUpdateWidget(Y);
        MakeImgChangePixmap(Tmp,Imgfile);
        sprintf(buff,"%-s : %-s",Album,Imgfile);
        kgUpdateOn(Tmp);
        if(butno==3)usleep(500000);
        if(kgCheckMousePressEvent((DIALOG *)Tmp,&kb)) {
          X=(DIM *)kgGetLocationWidget((DIALOG *)Tmp,kb.x,kb.y);
          if(X==M) { break; }
        }
        if(kgCheckEscape((DIALOG *)Tmp)) break;
        df -=1;
      }
      if(df==0) df=1;
      *(Y->df)=df;
      kgSetGrpVisibility(Tmp,5,0);
      kgUpdateGrp(Tmp,5);
      kgSetGrpVisibility(Tmp,6,1);
      kgUpdateGrp(Tmp,6);
      break;
    case 4: 
    case 6: 
      df++;
      kgSetGrpVisibility(Tmp,6,0);
      kgUpdateGrp(Tmp,6);
      kgSetGrpVisibility(Tmp,5,1);
      kgUpdateGrp(Tmp,5);
      kgUpdateOn(Tmp);
      while(m[df-1] != NULL) {
        *(Y->df)=df;
        strcpy(Imgfile,m[df-1]->name);
        kgUpdateWidget(Y);
        MakeImgChangePixmap(Tmp,Imgfile);
        sprintf(buff,"%-s : %-s",Album,Imgfile);
        kgUpdateOn(Tmp);
        if(butno==4)usleep(500000);
        if(kgCheckMousePressEvent((DIALOG *)Tmp,&kb)) {
          X=(DIM *)kgGetLocationWidget((DIALOG *)Tmp,kb.x,kb.y);
          if(X==M) { break; }
        }
        if(kgCheckEscape((DIALOG *)Tmp)) break;
        df++;
      }
      if(m[df-1]==NULL) df -=1;
      *(Y->df)=df;
      kgSetGrpVisibility(Tmp,5,0);
      kgUpdateGrp(Tmp,5);
      kgSetGrpVisibility(Tmp,6,1);
      kgUpdateGrp(Tmp,6);
      break;
    case 1: 
      df--;
      if(df> 0) {
        *(Y->df)=df;
        strcpy(Imgfile,m[df-1]->name);
        kgUpdateWidget(Y);
        MakeImgChangePixmap(Tmp,Imgfile);
        sprintf(buff,"%-s : %-s",Album,Imgfile);
        kgUpdateOn(Tmp);
      }
      if(df <1 ) df =1;
      break;
    case 2: 
      df++;
      if(m[df-1] != NULL) {
        *(Y->df)=df;
        strcpy(Imgfile,m[df-1]->name);
        kgUpdateWidget(Y);
        MakeImgChangePixmap(Tmp,Imgfile);
        sprintf(buff,"%-s : %-s",Album,Imgfile);
        kgUpdateOn(Tmp);
        df++;
      }
      if(m[df-1]==NULL) df -=1;
      break;
    case 7: 
//      if(Img==NULL) break;
      df = *(Y->df);
      if(df==0)break;
      if(m[df-1] != NULL) {
         strcpy(Imgfile,m[df-1]->name);
         if(Img==NULL) ans=1;
         else ans = Runcheckimport(Tmp,Img,titles);
         if(ans ==1) {
          remove(Imgfile);
          sprintf(buff,".Thumnail/%-s",Imgfile);
          remove(buff);
          kgFreeImage(m[df-1]->img);
//TCB
          free(m[df-1]->name);
          free(m[df-1]);
          k=df;
          do {
            m[df-1]= m[df];
            if(m[df]==NULL) break;
            df++;
          } while (1);
          Dposition(Photolist,k);
          Ddelete(Photolist);
          WritePhotoList();
          kgSetList((void *)Y,(void **)m);
          if(m[k-1]==NULL) k--;
          if(k< 1) k=0;
          if(k> 0) {
            strcpy(Imgfile,m[k-1]->name);
            MakeImgChangePixmap(Tmp,Imgfile);
          }
          else  {
           kgFreeImage(P->xpm);
           P->xpm = Backgr;
           kgUpdateWidget(P);
          }
          if(k< 1) k=1;
          *(Y->df)=k;
          kgUpdateWidget(Y);
          kgUpdateOn(Tmp);
         }
      }
      break;
  }
  kgSkipEvents((DIALOG *)Tmp);
  return ret;
}
void  kgviewbutton1init(DIN *B,void *pt) {
}
void  kgviewbutton2init(DIN *B,void *pt) {
}
void  kgviewbutton3init(DIN *B,void *pt) {
}
int  kgviewbutton2callback(int butno,int i,void *Tmp) {
  DIALOG *D;DIN *B; 
  int n,ret =1; 
  int dx,dy,w,h;
  GMIMG *img;
  DIG *G;
  G = ((DIALOG *)Tmp)->d[0].g;
  xsize = G->x2-G->x1;
  ysize = G->y2-G->y1;
  img = Img;
  if(img == NULL) return ret ;
  w = img->image_width;
  h = img->image_height;
  dx = w/20.0+0.5;
  dy = h/20.0+0.5;
  D = ((DIALOG *)Tmp);
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      img->xoffset -=dx;
      kgImage(Tmp,Img,G->x1,G->y1,xsize,ysize,0.0,1.0);
      break;
    case 2: 
      img->xoffset +=dx;
      kgImage(Tmp,Img,G->x1,G->y1,xsize,ysize,0.0,1.0);
      break;
  }
  kgSkipEvents((DIALOG *)Tmp);
  return ret;
}
int  kgviewbutton3callback(int butno,int i,void *Tmp) {
  DIALOG *D;DIN *B; 
  int n,ret =1; 
  int dx,dy,w,h;
  GMIMG *img;
  DIG *G;
  G = ((DIALOG *)Tmp)->d[0].g;
  xsize = G->x2-G->x1;
  ysize = G->y2-G->y1;
  img = Img;
  if(img == NULL) return ret ;
  w = img->image_width;
  h = img->image_height;
  dx = w/20.0+0.5;
  dy = h/20.0+0.5;
  D = ((DIALOG *)Tmp);
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  switch(butno) {
    case 1: 
      img->yoffset -=dy;
      kgImage(Tmp,Img,G->x1,G->y1,xsize,ysize,0.0,1.0);
      break;
    case 2: 
      img->yoffset +=dy;
      kgImage(Tmp,Img,G->x1,G->y1,xsize,ysize,0.0,1.0);
      break;
  }
  kgSkipEvents((DIALOG *)Tmp);
  return ret;
}
int UpdateImportBrowsers(void *Tmp) {
  DIA *D;DIY *Y; DIX *X;
  int i,k,size;
  char buffer[400];
  char *CurDir;
  char **menu,**m;
  ThumbNail **th;
  int *sw;
  GMIMG *img;
  KBEVENT kbe;
  void **xpm;
  void **pt,**list;
  void *id;
  D = ((DIALOG *)Tmp)->d;
  X = D[11].x;
  Y = D[13].y;
  size=Y->width;
  kgFreeThumbNails((ThumbNail **)X->list);
  kgFreeThumbNails((ThumbNail **)Y->list);
  switch(CameraId) {
  case -1:
    pt = (void **)(((DIALOG *)Tmp)->pt);
    CurDir=(char *)pt[0];
    sprintf(buffer,"%s : Making Thumbnails... Pl. Wait",CurDir);
    kgPrintf(Tmp,10,buffer);
    kgUpdateOn(Tmp);
    kgSetList((void *)X, (void **)kgFolderThumbNails(CurDir));
    kgUpdateWidget(X);
    id = kgOpenBusy(Tmp,500,400);
    th = kgMakeThumbNails(CurDir,Y->width);
    kgCloseBusy(id);
    kgSetList((void *)Y,(void **) th);
    kgPrintf(Tmp,10,CurDir);
    kgUpdateWidget(Y);
  break;
  default:
    sprintf(buffer,"%s :  Pl. Wait",curnode->name);
    kgPrintf(Tmp,10,buffer);
    kgUpdateOn(Tmp);
    id = kgOpenBusy(Tmp,500,400);
    list = (void **)MakeFolderNails(curnode,22,22);
    kgSetList(X,list);
    list = (void **)MakeFileNails(curnode,90,60);
    kgSetList(Y,list);
    kgCloseBusy(id);
    sprintf(buffer,"%s :  OKAY",curnode->name);
    kgPrintf(Tmp,10,buffer);
    kgUpdateWidget(X);
    kgUpdateWidget(Y);
  break;
  }
  kgUpdateOn(Tmp);
}
int  kgviewbutton4callback(int butno,int i,void *Tmp) {
  DIALOG *D;DIN *B; 
  int n,ret =0,k; 
  char *CurDir,**m,**menu0;
  void **pt;
  GMIMG **imgs=NULL;
  int *sw;
  D = ((DIALOG *)Tmp);
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  pt = (void **)(D->pt);
  CurDir=(char *)pt[0];
  switch(butno) {
    case 1: 
      if(CameraId==-1) strcpy((char *)pt[0],(char *)pt[1]);
      else {
       curnode=cnode;
      }
      break;
    case 2: 
      if(CameraId==-1){
        k= strlen(CurDir);
        while((CurDir[k]!='/')&&(k>1)) {CurDir[k]='\0';k--;}
        CurDir[k]='\0';
      }
      else {
        if(curnode->parent != NULL) curnode= curnode->parent;
      }
      break;
  }
  UpdateImportBrowsers(Tmp);
  return ret;
}
void  kgviewbutton4init(DIN *B,void *pt) {
}
void  kgviewbrowser3init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
}
void  kgviewbutton5init(DIN *B,void *pt) {
}
int  kgviewbrowser3callback(int item,int i,void *Tmp) {
  ThumbNail **th;
  DIALOG *D;DIX *X; 
  Dlink *L;
  int ret=1;
  void *id; 
  char buffer[300];
  char *folder,*dir;
  void **pt;
  D = (DIALOG *)Tmp;
  X = (DIX *)kgGetWidget(Tmp,i);
//  printf("Inside Browser\n");
  if(CameraId==-1) {
    th = (ThumbNail **)(X->list);
    pt = (void **)(((DIALOG *)Tmp)->pt);
    folder= th[*(X->df)-1]->name;
    dir=(char *)pt[0];
    if(strcmp(dir,"/")!=0) strcat(dir,"/");
    strcat(dir,folder);
  }
  else {
    L = (Dlink *)(curnode->folders);
    Dposition(L,*(X->df));
    curnode = (FSTREE *)Getrecord(L);
    sprintf(buffer,"%s :  Pl. Wait",curnode->name);
    kgPrintf(Tmp,10,buffer);
    kgUpdateOn(Tmp);
    id = kgOpenBusy(Tmp,500,400);
    ExploreFsnode(curnode,"*");
    kgCloseBusy(id);
  }
  kgSetSelection(X,1);
  UpdateImportBrowsers(Tmp);
  kgUpdateOn(Tmp);

  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
int  kgviewbutton5callback(int butno,int i,void *Tmp) {
  char flname [200],sflname[500],buf[1000],dflname[500];
  char *CurDir,*HomeDir,*dest;
  void **pt;
  int *sw;
  PHOTO *ph;
  ThumbNail **th,**m;
  DIALOG *D;DIB *B; 
  DIY *Y,*A;
  int n,ret =0,k,update=0,df; 
  if(CameraId> 0) return Camerabutton1callback( butno, i,Tmp);
  D = (DIALOG *)Tmp;
  B = (DIB *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  Y= (DIY *)kgGetWidget(Tmp,13);
  A= (DIY *)kgGetWidget(Tmp,3);
  th = (ThumbNail **) Y->list;
  m = (ThumbNail **) A->list;
  pt = (void **)(((DIALOG *)Tmp)->pt);
  HomeDir=(char *)pt[1];
  CurDir=(char *)pt[0];
  n = Y->nitems;
  switch(butno) {
    case 1: 
     ret=0;
     if(th==NULL) ret=0;
     else {
      for(k=0;k<n;k++) {
         th[k]->sw= 1;
      }
      kgUpdateWidget(Y);
     }
     break;
    case 2: 
     ret=0;
     if(th==NULL) ret=0;
     else {
      for(k=0;k<n;k++) {
         th[k]->sw= 0;
      }
      kgUpdateWidget(Y);
     }
     break;
    case 3: 
     if(th==NULL) ret=0;
     else {
      update=0;
      for(k=0;k<n;k++) {
         if(th[k]->sw== 1) {
          update++;
          strcpy(flname,th[k]->name);
          strcpy(sflname,CurDir);
          if(strcmp(CurDir,"/")!=0) strcat(sflname,"/");
          strcat(sflname,flname);
          ImportThumbnailImage(th[k]->img);
          ph = ImportImage(sflname);
          Dappend(Photolist,ph);
         }
      }
      if(update) {
       sprintf(buf,"%-s/%-s",AlbumDir,Album);
       kgFreeThumbNails((ThumbNail **)A->list);
//       kgSetList((void *)A, (void **)kgMakeThumbNails(buf,A->width));
       WritePhotoList();
       kgSetList((void *)A, (void **)ReadPhotoList());
       kgUpdateWidget(A);
      }
     }
     ret=0;
    break;
    case 4: 
      kgSetGrpVisibility(Tmp,3,0);
      kgUpdateGrp(Tmp,3);
      kgSetGrpVisibility(Tmp,2,1);
      kgUpdateGrp(Tmp,2);
      DisplayCurrentImage(Tmp);
      break;
  }
  return ret;
}
int  kgviewbrowser4callback(int item,int i,void *Tmp) {
  char *CurDir,**pt;
  ThumbNail **th,*tpt;
  PHOTO *ph;
  char flname [200],sflname[500],buf[1000];
  void *Img=NULL;
  KBEVENT  kb;
  DIA *Dia;DIY *Y,*A; 
  DIALOG *D;
  int ret=1,pos,k,rv; 
  int x=-100,y=-100,ymin,ymax,axmin,aymin,axmax,aymax;
  int x1,y1,x2,y2;
  if(CameraId> 0) return Camerabrowser2callback( item, i,Tmp);
  D = ((DIALOG *)Tmp);
  pt = (char **)D->pt;
  CurDir=(char *)pt[0];
  Dia = ((DIALOG *)Tmp)->d;
  Y = (DIY *)kgGetWidget(Tmp,i);
  A= (DIY *)kgGetWidget(Tmp,3);
  ymin = Y->y1+D->yo+Y->offset;
  ymax = Y->y2+D->yo-Y->offset;
  axmin =A->x1+D->xo; axmax =A->x2+D->xo;
  aymin =A->y1+D->yo; aymax =A->y2+D->yo;
  th = (ThumbNail **)Y->list;
  if(kgDragThumbNail(Y,item-1,&x,&y)) {
     pos = kgGetThumbNailItem(Y,x,y);
     if((pos >=0)&&(pos!=(item-1)) ) {
       tpt = th[item-1];
       if(pos < (item-1) ) {
          for(k=(item-1);k>pos;k--){ th[k]=th[k-1]; }
          th[pos]= tpt;
       }
      else {
          for(k=item;k<pos;k++) {th[k-1]=th[k];}
          th[pos-1]= tpt;
      }
      *(Y->df) = pos+1;
      kgSetList((void *)Y,(void **)th);
      kgUpdateWidget(Y);
      kgFreeXImage(Img);
      kgUpdateOn(Tmp);
    }
    else {
#if 0
     pos = kgGetThumbNailItem(A,x,y);
     if(pos >= 0) {
#else
     if( (x>axmin)&&(y>aymin)&&(x<axmax)&&(y<aymax)) {
#endif
      tpt = th[item-1];
      strcpy(flname,tpt->name);
      strcpy(sflname,CurDir);
      if(strcmp(CurDir,"/")!=0) strcat(sflname,"/");
      strcat(sflname,flname);
//      ImportImage(sflname);
      ImportThumbnailImage(th[item-1]->img);
      ph = ImportImage(sflname);
      Dappend(Photolist,ph);
      sprintf(buf,"%-s/%-s",AlbumDir,Album);
      kgFreeThumbNails((ThumbNail **)A->list);
//      kgSetList((void *)A, (void **)kgMakeThumbNails(buf,A->width));
      WritePhotoList();
      kgSetList((void *)A, (void **)ReadPhotoList());
      *(A->df) = A->nitems;
      kgUpdateWidget(A);
      kgUpdateOn(Tmp);
     }
    }
  }
  return ret;
}
void  kgviewbrowser4init(DIY *Y,void *pt) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
}
int  kgviewbutton6callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B; 
  int n,ret =0; 
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  n = B->nx*B->ny;
  RotateAndShow(Tmp);
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  kgviewbutton6init(DIN *B,void *pt) {
}
int kgviewinit(void *Tmp) {
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
int kgviewcleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int kgviewCallBack(void *Tmp,void *tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
    tmp :  Pointer to KBEVENT  
   ***********************************/ 
  int ret = 0,df;
  DIG *g0,*g;
  DIALOG *D;
  KBEVENT *kbe;
  DIY *Y;
  char buff[300];
  ThumbNail **m;
  D = (DIALOG *)Tmp;
  kbe = (KBEVENT *)tmp;
  Y = (DIY *)kgGetWidget(Tmp,3);
  if(Album[0]=='\0') return ret;
  m = (ThumbNail **)Y->list;
  if((m==NULL)||(m[0]==NULL)) {
   return ret;
  }
  df = *(Y->df);
  if(df==0)  df=1;
  if(df >Y->nitems) df = Y->nitems;
// need modifications here
  *(Y->df)=df;
  if(kbe->event ==1) {
    g0 = (DIG *)kgGetClickedWidget(Tmp);
    if(g0==NULL) return ret;
    g  = (DIG *)kgGetWidget(Tmp,0);
//    printf("HIDE: %d\n",g->hide);
    if(g0==g) {
    if(kbe->button ==1) {
      df++;
      if(m[df-1] != NULL) {
        *(Y->df)=df;
        strcpy(Imgfile,m[df-1]->name);
        kgUpdateWidget(Y);
        MakeImgChangePixmap(Tmp,Imgfile);
        sprintf(buff,"%-s : %-s",Album,Imgfile);
        kgUpdateOn(Tmp);
        df++;
      }
    }
    else {
      df--;
      if(df> 0) {
        *(Y->df)=df;
        strcpy(Imgfile,m[df-1]->name);
        kgUpdateWidget(Y);
        MakeImgChangePixmap(Tmp,Imgfile);
        sprintf(buff,"%-s : %-s",Album,Imgfile);
        kgUpdateOn(Tmp);
        df -=1;
      }
    }
    }
  }
  return ret;
}
int kgviewResizeCallBack(void *Tmp) {
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int xres,yres,dx,dy; 
  int ret = 0;
  DIALOG *D;
  DIA *d=NULL;
  DIX *x11;DIY *x2; DIY *y13,*y3,*cy;
  DIP *p4;
  DIG *g0;
  D = (DIALOG *)Tmp;
  d = D->d;
  g0  = d[0].g;
  p4  = d[4].p;
  y3  = d[3].y;
  y13 = d[13].y;
  x2  = d[2].y;
  x11 = d[11].x;
  cy = (DIY *)kgGetNamedWidget(Tmp,(char *)"CameraY");
  kgGetWindowSize(D,&xres,&yres); 
//  printf("%d %d\n",xres,yres);
  dx = xres -D->xl;
  dy = yres -D->yl;
  D->xl = xres;
  D->yl = yres;
  y3->y2 +=dy;
  g0->x2 +=dx;
  g0->y2 +=dy;
  p4->x2 +=dx;
  p4->y2 +=dy;
  y13->x2+=dx;
  y13->y2+=dy;
  cy->x2 +=dx;
  cy->y2 +=dy;
  kgRedrawDialog(D);
  if(y13->hide==0) {
     kgUpdateGrp(Tmp,2);
     kgUpdateGrp(Tmp,4);
     kgUpdateGrp(Tmp,3);
  }
  else {
    if(cy->hide==0) {
     kgUpdateGrp(Tmp,2);
     kgUpdateGrp(Tmp,3);
     kgUpdateGrp(Tmp,4);
    }
    else DisplayCurrentImage(D);
  }
  return ret;
}
int  Camerabrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIX *X;void *pt; 
  DIY *Y;
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  X = (DIX *)kgGetWidget(Tmp,i);
  Y = (DIY *)kgGetNamedWidget(Tmp,(char *)"CameraY");
  kgWrite(kgGetNamedWidget(Tmp,(char *)"CameraWidget5"),(char *)kgGetSelectedString(X));
  kgFreeThumbNails((ThumbNail **)Y->list);
  kgSetList(Y,(void **)GetImageList(item));
  kgUpdateWidget(Y);
  kgUpdateOn(Tmp);
  switch(item) {
    case 1: 
      break;
  }
  return ret;
}
void  Camerabrowser1init(DIX *X,void *pt) {
 // One may setup browser list here by setting X->list
 // if it need to be freed set it as X->pt also
}
int DeleteListItem(DIY *Y,int df) {
  int k;
  ThumbNail **m;
  m = (ThumbNail **)Y->list;
  kgFreeImage(m[df-1]->img);
  free(m[df-1]->name);
  free(m[df-1]);
  k=df;
  do {
    m[df-1]= m[df];
    if(m[df]==NULL) break;
    df++;
  } while (1);
  kgSetList((void *)Y,(void **)m);
  if(m[k-1]==NULL) k--;
  if(k< 1) k=1;
  *(Y->df)=k;
  kgUpdateWidget(Y);
  return 1;
}
int  Camerabutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIN *B;DIY *Y,*A;DIX *X; 
  int n,ret =0,k,ix; 
  int update=0;
  PHOTO *ph;
  ThumbNail **th;
  void **list;
  char buff[300],Otfile[100],sflname[300];
  char tbuf[200];
  D = (DIALOG *)Tmp;
  B = (DIN *)kgGetWidget(Tmp,i);
  Y = (DIY *)kgGetWidget(Tmp,13);
  X = (DIX *)kgGetWidget(Tmp,11);
  A= (DIY *)kgGetWidget(Tmp,3);
  th = (ThumbNail **)Y->list;
  n = Y->nitems;
  switch(butno) {
    case 1: 
     ret=0;
     if(th==NULL) ret=0;
     else {
      for(k=0;k<n;k++) {
         th[k]->sw= 1;
      }
      kgUpdateWidget(Y);
     }
     break;
    case 2: 
     ret=0;
     if(th==NULL) ret=0;
     else {
      for(k=0;k<n;k++) {
         th[k]->sw= 0;
      }
      kgUpdateWidget(Y);
     }
     break;
#if 0
    case 3: 
      sprintf(buff,"Delete Selected Photos from Camera ?");
      if(kgCheckMenu(Tmp,100,100,buff,0)) {
        ix = kgGetSelection(X);
        while(1){
          int i,OK;
          i=0;
          OK=1;
          list = Y->list;
          while(list[i]!= NULL) {
            if(kgGetSwitch(Y,i)) {
              if(DeleteCameraFile(ix,i+1)){
                DeleteListItem(Y,i+1);
                OK=0;
              }
              break;
            }
            i++;
          }
          if(OK) break;
        }
      }
//      kgUpdateOn(Tmp);
      break;
#endif
    case 3: 
      sprintf(buff,"%-s/.Camera/%-s",AlbumDir,Album);
//      printf("Buff: %s\n",buff);
      mkdir(buff,0700);
      ix = kgGetSelection(X);
      k=0;
      while(th[k]!= NULL) {
        if(kgGetSwitch(Y,k)) {
          sprintf(Otfile,"img_%4.4d.jpg",ImgNo);
          strcpy(sflname,buff);
          strcat(sflname,"/");
          strcat(sflname,Otfile);
          if(kgCheckEscape((DIALOG *)Main)) break;
#if 1
          if((k%2)==1) sprintf(tbuf,"!c50Copying %s",Otfile);
          else  sprintf(tbuf,"!c51Copying %s",Otfile);
          kgPrintf(Main,10,tbuf);
          kgUpdateOn(Main);
#else
          kgPrintf(Tmp,10,Otfile);
          kgUpdateOn(Main);
#endif
          if(!CopyCameraFile_o(ix,k+1,sflname)) break;
          update++;
          ImportThumbnailImage(th[k]->img);
          ph = ImportImage(sflname);
          Dappend(Photolist,ph);
        }
        k++; 
      }
      if(update) {
       kgFreeThumbNails((ThumbNail **)A->list);
       WritePhotoList();
       kgSetList((void *)A, (void **)ReadPhotoList());
       kgUpdateWidget(A);
      }
      break;
    case 4: 
      CleanCamera();
      kgFreeThumbNails((ThumbNail **)(X->list));
      kgFreeThumbNails((ThumbNail **)(Y->list));
      kgSetList(X,NULL);
      kgSetList(Y,NULL);
      kgSetGrpVisibility(Tmp,3,0);
      kgUpdateGrp(Tmp,3);
      kgSetGrpVisibility(Tmp,2,1);
      kgUpdateGrp(Tmp,2);
      DisplayCurrentImage(Tmp);
      break;
  }
  return ret;
}
void  Camerabutton1init(DIN *B,void *pt) {
}
int  Camerabrowser2callback(int item,int i,void *Tmp) {
  char *CurDir,**pt;
  ThumbNail **th,*tpt;
  PHOTO *ph;
  char flname [200],sflname[500],buff[1000],Otfile[200];
  void *Img=NULL;
  KBEVENT  kb;
  DIA *Dia;DIY *Y,*A; 
  DIALOG *D;
  DIX *X;
  int ret=1,pos,k,rv,ix; 
  int x=-100,y=-100,ymin,ymax,axmin,aymin,axmax,aymax;
  int x1,y1,x2,y2;
  D = ((DIALOG *)Tmp);
  pt = (char **)D->pt;
  CurDir=(char *)pt[0];
  Dia = ((DIALOG *)Tmp)->d;
  Y = (DIY *)kgGetWidget(Tmp,i);
  X = (DIX *)kgGetWidget(Tmp,11);
  A= (DIY *)kgGetWidget(Tmp,3);
  ymin = Y->y1+D->yo+Y->offset;
  ymax = Y->y2+D->yo-Y->offset;
  axmin =A->x1+D->xo; axmax =A->x2+D->xo;
  aymin =A->y1+D->yo; aymax =A->y2+D->yo;
  th = (ThumbNail **)Y->list;
  if(kgDragThumbNail(Y,item-1,&x,&y)) {
     if( (x>axmin)&&(y>aymin)&&(x<axmax)&&(y<aymax)) {
      sprintf(buff,"%-s/.Camera/%-s",AlbumDir,Album);
      mkdir(buff,0700);
      ix = kgGetSelection(X);
      tpt = th[item-1];
      k=item-1;
      sprintf(Otfile,"img_%4.4d.jpg",ImgNo);
      strcpy(sflname,buff);
      strcat(sflname,"/");
      strcat(sflname,Otfile);
      if(CopyCameraFile_o(ix,k+1,sflname)) {
        ImportThumbnailImage(th[k]->img);
        ph = ImportImage(sflname);
        Dappend(Photolist,ph);
        kgFreeThumbNails((ThumbNail **)A->list);
        WritePhotoList();
        kgSetList((void *)A, (void **)ReadPhotoList());
        *(A->df) = A->nitems;
        kgUpdateWidget(A);
        kgUpdateOn(Tmp);
      }
     }
  }
  return ret;
}
void  Camerabrowser2init(DIY *Y,void *pt) {
 // One may setup browser list here by setting Y->list
 // if it need to be freed set it as Y->pt also
}
int  Camerasplbutton1callback(int butno,int i,void *Tmp) {
  /*********************************** 
    butno : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIL *B; 
  int n,ret=1; 
  D = (DIALOG *)Tmp;
  B = (DIL *) kgGetWidget(Tmp,i);
  n = B->nx;
  switch(butno) {
    case 1: 
      break;
  }
  return ret;
}
void  Camerasplbutton1init(DIL *B,void *pt) {
}
int Camerainit(void *Tmp) {
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
int Cameracleanup(void *Tmp) {
  /* you add any cleanup/mem free here */
  /*********************************** 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret = 1;
  DIALOG *D;void *pt;
  D = (DIALOG *)Tmp;
  pt = D->pt;
  return ret;
}
int CameraCallBack(void *Tmp,void *tmp) {
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
int CameraResizeCallBack(void *Tmp) {
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
