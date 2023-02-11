#include <kulina.h>
int  Camerabrowser1callback(int item,int i,void *Tmp) {
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  DIALOG *D;DIX *X;void *pt; 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  X = (DIX *)kgGetWidget(Tmp,i);
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
int  Camerabutton1callback(int butno,int i,void *Tmp) {
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
  switch(butno) {
    case 1: 
      break;
    case 2: 
      break;
    case 3: 
      break;
    case 4: 
      break;
    case 5: 
      break;
  }
  return ret;
}
void  Camerabutton1init(DIN *B,void *pt) {
}
int  Camerabrowser2callback(int item,int i,void *Tmp) {
  DIALOG *D;DIY *Y;void *pt; 
  /*********************************** 
    item : selected item (1 to max_item) 
    i :  Index of Widget  (0 to max_widgets-1) 
    Tmp :  Pointer to DIALOG  
   ***********************************/ 
  int ret=1; 
  D = (DIALOG *)Tmp;
  pt = D->pt;
  Y = (DIY *)kgGetWidget(Tmp,i);
  switch(item) {
    case 1: 
      break;
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
