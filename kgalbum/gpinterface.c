#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <kulina.h>
#include "fstree.h"

#include <gphoto2/gphoto2-camera.h>
extern DIALOG *Parent;
extern int CameraId;
extern FSTREE *cnode,*curnode;
int CheckExtension(char *name,char *ext);


/* Sample autodetection program.
 *
 * This program can autodetect multiple cameras and then calls a
 * simple function in each of them (summary).
 */
typedef struct _CameraRec {
  char name[200];
  Camera *cam;
  Dlink *Imglist;
} CameraRec;
typedef struct _CameraImage {
//  Camera *cam;
  char folder[1500];
  char file[500];
  void *Timg;
  void *Img;
} CameraImage;
static Dlink *Clist=NULL;
static CameraList	*list=NULL;
static Camera		**cams=NULL;
static GPContext	*context=NULL;

static GPPortInfoList		*portinfolist = NULL;
static CameraAbilitiesList	*abilities = NULL;
static int CameraCount=0,ConError=0;

/*
 * This detects all currently attached cameras and returns
 * them in a list. It avoids the generic usb: entry.
 *
 * This function does not open nor initialize the cameras yet.
 */
int
sample_autodetect (CameraList *list, GPContext *context) {
	gp_list_reset (list);
        return gp_camera_autodetect (list, context);
}

/*
 * This function opens a camera depending on the specified model and port.
 */
int
sample_open_camera (Camera ** camera, const char *model, const char *port, GPContext *context) {
	int		ret, m, p;
	CameraAbilities	a;
	GPPortInfo	pi;

	ret = gp_camera_new (camera);
	if (ret < GP_OK) return ret;

	if (!abilities) {
		/* Load all the camera drivers we have... */
		ret = gp_abilities_list_new (&abilities);
		if (ret < GP_OK) return ret;
		ret = gp_abilities_list_load (abilities, context);
		if (ret < GP_OK) return ret;
	}

	/* First lookup the model / driver */
        m = gp_abilities_list_lookup_model (abilities, model);
	if (m < GP_OK) return ret;
        ret = gp_abilities_list_get_abilities (abilities, m, &a);
	if (ret < GP_OK) return ret;
        ret = gp_camera_set_abilities (*camera, a);
	if (ret < GP_OK) return ret;

	if (!portinfolist) {
		/* Load all the port drivers we have... */
		ret = gp_port_info_list_new (&portinfolist);
		if (ret < GP_OK) return ret;
		ret = gp_port_info_list_load (portinfolist);
		if (ret < 0) return ret;
		ret = gp_port_info_list_count (portinfolist);
		if (ret < 0) return ret;
	}

	/* Then associate the camera with the specified port */
        p = gp_port_info_list_lookup_path (portinfolist, port);
        switch (p) {
        case GP_ERROR_UNKNOWN_PORT:
                fprintf (stderr, "The port you specified "
                        "('%s') can not be found. Please "
                        "specify one of the ports found by "
                        "'gphoto2 --list-ports' and make "
                        "sure the spelling is correct "
                        "(i.e. with prefix 'serial:' or 'usb:').",
                                port);
                break;
        default:
                break;
        }
        if (p < GP_OK) return p;

        ret = gp_port_info_list_get_info (portinfolist, p, &pi);
        if (ret < GP_OK) return ret;
        ret = gp_camera_set_port_info (*camera, pi);
        if (ret < GP_OK) return ret;
	return GP_OK;
}
static void ConnectError(void){
   kgSplashMessage(Parent,50,50,300,36,(char *)"Camera Connection Problem; Try Reconnect",12,-250250250,-120250);
}
static void
ctx_error_func (GPContext *context, const char *str, void *data)
{
//    fprintf  (stderr, "\n*** Contexterror ***              \n%s\n",str);
//    fflush   (stderr);
    
    ConError=1;
}

static void
ctx_status_func (GPContext *context, const char *str, void *data)
{
//        fprintf  (stderr, "%s\n", str);
//        fflush   (stderr);
}

GPContext* sample_create_context() {
	GPContext *context;

	/* This is the mandatory part */
	context = gp_context_new();

	/* All the parts below are optional! */
        gp_context_set_error_func (context, ctx_error_func, NULL);
        gp_context_set_status_func (context, ctx_status_func, NULL);

	/* also:
	gp_context_set_cancel_func    (p->context, ctx_cancel_func,  p);
        gp_context_set_message_func   (p->context, ctx_message_func, p);
        if (isatty (STDOUT_FILENO))
                gp_context_set_progress_funcs (p->context,
                        ctx_progress_start_func, ctx_progress_update_func,
                        ctx_progress_stop_func, p);
	 */
	return context;
}
int print_folders(Camera *cam,Dlink *Ilist, GPContext *context, const char *folder) {
	const char	*name, *value;
        int j,ret,i;
        CameraImage *cpt;
        CameraList	*folderlist,*filelist;
        char buf[5000];
        int fcount;
        static int entry=0;
        ret = gp_list_new (&folderlist);
        gp_camera_folder_list_folders(cam,folder,folderlist,context);
        if(ConError) {
              printf("folderlist error\n");
              return 0;
        }
        entry++;
        for(j=0;j<gp_list_count(folderlist);j++) {
            ret = gp_list_get_name  (folderlist, j, &name);
            if (ret < GP_OK) break;
            ret = gp_list_get_value  (folderlist, j, &value);
//            for(i=1;i<entry;i++) printf("  ");
//            printf("   %-30s %-16s\n", name, value);
            strcpy(buf,folder);
            if(strcmp(folder,"/")!=0) strcat(buf,"/");
            strcat(buf,name);
//            print_folders(cam,Ilist,context,buf);
        }
        gp_list_free(folderlist);
        ret = gp_list_new (&filelist);
        gp_camera_folder_list_files(cam,folder,filelist,context);
        if(ConError){
           printf("filelist error\n");
           return 0;
        }
        for(j=0;j<gp_list_count(folderlist);j++) {
            ret = gp_list_get_name  (filelist, j, &name);
            if (ret < GP_OK) break;
            if(!CheckExtension(name,"jpg")) {
              if(!CheckExtension(name,"JPG")) {
                if(!CheckExtension(name,"png")) {
                  continue;
                }
              }
            }
            ret = gp_list_get_value  (filelist, j, &value);
            for(i=0;i<entry;i++) printf("  ");
            printf("   %-30s %-16s\n", name, value);
            cpt = (CameraImage *)malloc(sizeof(CameraImage));
        //    cpt->cam = cam;
            strcpy(cpt->folder,folder);
            strcpy(cpt->file,name);
            cpt->Timg = NULL;
            cpt->Img=NULL;
            Dappend(Ilist,cpt);
        }
        entry--;
        gp_list_free(filelist);
        return 1;
}
void ** GetCameraFolderList( const char *folder) {
	const char	*name, *value;
        int j,ret,i;
        Camera *cam;
        CameraRec *cpt;
        CameraImage *Ipt;
        char **list;
//        CameraImage *cpt;
        CameraList	*folderlist,*filelist;
        char buf[5000];
        int fcount;
        static int entry=0;
        Resetlink(Clist);
        printf("CameraId= %d\n",CameraId);
        Dposition(Clist,CameraId);
        cpt = (CameraRec *)Getrecord(Clist);
        cam=cpt->cam;
        ret = gp_list_new (&folderlist);
        gp_camera_folder_list_folders(cam,folder,folderlist,context);
        if(ConError) {
//              printf("folderlist error\n");
              ConnectError();
              return NULL;
        }
        entry++;
        fcount = gp_list_count(folderlist);
        list=(char **)malloc(sizeof(char *)*(fcount+1));
        for(j=0;j<fcount;j++) {
            ret = gp_list_get_name  (folderlist, j, &name);
            if (ret < GP_OK) break;
            ret = gp_list_get_value  (folderlist, j, &value);
            strcpy(buf,folder);
            if(strcmp(folder,"/")!=0) strcat(buf,"/");
            strcat(buf,name);
            list[j]=(char *)malloc(strlen(name)+1);
            strcpy(list[j],name);
        }
        list[fcount]=NULL;
        gp_list_free(folderlist);
        entry--;
        return (void **)list;
}
void ** GetCameraFileList( const char *folder) {
	const char	*name, *value;
        int j,ret,i;
        Camera *cam;
        CameraRec *cpt;
        CameraImage *Ipt;
        char **list;
//        CameraImage *cpt;
        CameraList	*folderlist,*filelist;
        char buf[5000];
        int fcount;
        static int entry=0;
        Resetlink(Clist);
        Dposition(Clist,CameraId);
        cpt = (CameraRec *)Getrecord(Clist);
        cam=cpt->cam;
        ret = gp_list_new (&filelist);
        gp_camera_folder_list_files(cam,folder,filelist,context);
        if(ConError) {
//              printf("filelist error\n");
              ConnectError();
              return NULL;
        }
        entry++;
        fcount = gp_list_count(filelist);
        list=(char **)malloc(sizeof(char *)*(fcount+1));
        i=0;
        for(j=0;j<fcount;j++) {
            ret = gp_list_get_name  (filelist, j, &name);
            if (ret < GP_OK) break;
            if(!CheckExtension(name,"jpg")) {
              if(!CheckExtension(name,"JPG")) {
                if(!CheckExtension(name,"png")) {
                  continue;
                }
              }
            }
            ret = gp_list_get_value  (filelist, j, &value);
            strcpy(buf,folder);
            if(strcmp(folder,"/")!=0) strcat(buf,"/");
            strcat(buf,name);
            list[i]=(char *)malloc(strlen(name)+1);
            strcpy(list[i],name);
            i++;
            list[i]=NULL;
        }
        list[i]=NULL;
        gp_list_free(filelist);
        entry--;
        return (void **)list;
}

static void
delete_camera_file(Camera *camera, GPContext *context, char *folder,char *fn) {
	int fd, retval;
	retval = gp_camera_file_delete(camera, (const char *)folder, (const char *)fn, context);
//	printf("  Retval: %d\n", retval);

}
void CopyCameraFile(char *folder,char *fn,char *outfile) {
	int fd, retval;
	CameraFile *file;
	CameraFilePath camera_file_path;
        Camera *cam;
        CameraRec *cpt;
        int fcount;
        
        Resetlink(Clist);
        Dposition(Clist,CameraId);
        cpt = (CameraRec *)Getrecord(Clist);
        cam=cpt->cam;

//	printf("Reading.\n");

	/* NOP: This gets overridden in the library to /capt0000.jpg */
	strcpy(camera_file_path.folder, folder);
	strcpy(camera_file_path.name, fn);


	fd = open(outfile, O_CREAT | O_WRONLY, 0644);
	retval = gp_file_new_from_fd(&file, fd);
//	printf("  Retval: %d\n", retval);
	retval = gp_camera_file_get(cam, camera_file_path.folder, camera_file_path.name,
		     GP_FILE_TYPE_NORMAL, file, context);
	printf("  Retval: %d\n %s:%s", retval,folder,fn);
        close(fd);

	gp_file_free(file);
}
static void
copy_camera_file(Camera *camera, GPContext *context, char *folder,char *fn,char *outfile) {
	int fd, retval;
	CameraFile *file;
	CameraFilePath camera_file_path;

//	printf("Reading.\n");

	/* NOP: This gets overridden in the library to /capt0000.jpg */
	strcpy(camera_file_path.folder, folder);
	strcpy(camera_file_path.name, fn);


	fd = open(outfile, O_CREAT | O_WRONLY, 0644);
	retval = gp_file_new_from_fd(&file, fd);
//	printf("  Retval: %d\n", retval);
	retval = gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
		     GP_FILE_TYPE_NORMAL, file, context);
//	printf("  Retval: %d\n", retval);

	gp_file_free(file);
}
 void *CopyCameraThumbnail( char *folder,char *fn) {
        void *timg,*img;
	int fd, retval;
        char buf[300];
	CameraFile *file;
	CameraFilePath camera_file_path;
        Camera *cam;
        CameraRec *cpt;
        CameraImage *Ipt;
        int fcount;
        Resetlink(Clist);
        Dposition(Clist,CameraId);
        cpt = (CameraRec *)Getrecord(Clist);
        cam=cpt->cam;

//	printf("Reading.\n");

	/* NOP: This gets overridden in the library to /capt0000.jpg */
	strcpy(camera_file_path.folder, folder);
	strcpy(camera_file_path.name, fn);
	strcpy(buf, "thumbnail.jpg");
//        strcat(buf,fn);


//	fd = open(fn, O_CREAT | O_WRONLY, 0644);
	fd = open(buf, O_CREAT | O_WRONLY, 0644);
	retval = gp_file_new_from_fd(&file, fd);
//	printf("  Retval: %d\n", retval);
	retval = gp_camera_file_get(cam, camera_file_path.folder, camera_file_path.name,
		     GP_FILE_TYPE_NORMAL, file, context);
//		     GP_FILE_TYPE_PREVIEW, file, context);
//	printf("  Retval: %d\n", retval);
        close(fd);
	gp_file_free(file);
//        printf("getting image copy\n");
        strcpy(buf,"thumbnail.jpg");
        img =  kgGetImage(buf);
        timg = kgThumbNailImage(img,90,60);
        kgFreeImage(img);
//        printf("got\n");
        remove("thumbnail.jpg");
        return timg;
}
static void
copy_camera_thumbnail(Camera *camera, GPContext *context, char *folder,char *fn) {
	int fd, retval;
        char buf[300];
	CameraFile *file;
	CameraFilePath camera_file_path;

//	printf("Reading.\n");

	/* NOP: This gets overridden in the library to /capt0000.jpg */
	strcpy(camera_file_path.folder, folder);
	strcpy(camera_file_path.name, fn);
	strcpy(buf, "thumbnail.jpg");
//        strcat(buf,fn);


//	fd = open(fn, O_CREAT | O_WRONLY, 0644);
	fd = open(buf, O_CREAT | O_WRONLY, 0644);
	retval = gp_file_new_from_fd(&file, fd);
//	printf("  Retval: %d\n", retval);
	retval = gp_camera_file_get(camera, camera_file_path.folder, camera_file_path.name,
		     GP_FILE_TYPE_PREVIEW, file, context);
//	printf("  Retval: %d\n", retval);

	gp_file_free(file);
}
void CleanCamera(void) {
  int i;
  Dlink *Ilist=NULL;
  CameraRec *cpt;
  CameraImage *ipt;
  if(Clist== NULL)return;
  Resetlink(Clist);
  while ( (cpt=(CameraRec *)Getrecord(Clist))!= NULL){
     Ilist=cpt->Imglist;
     Resetlink(Ilist);
     while ( (ipt=(CameraImage *)Getrecord(Ilist))!= NULL){
/*
//  Shoulf not be done since it is done in kgFreeThumbNails
       kgFreeImage(ipt->Timg);
*/
       kgFreeImage(ipt->Img);
     }
     Dempty(cpt->Imglist);
  }
  Dempty(Clist);
  Clist=NULL;
  if(cams != NULL) {
    
    for(i=0;i<CameraCount;i++) {
      gp_camera_exit(cams[i],context);
    }
    free(cams);
  }
  CameraCount=0;
  gp_list_free(list);
  gp_abilities_list_free (abilities);
  gp_port_info_list_free (portinfolist);
  list=NULL;
  portinfolist=NULL;
  abilities=NULL;
  cams=NULL;
}
int CheckCameraStatus(void) {
  int count;
  count= sample_autodetect (list, context);
  if(count!= CameraCount ){
      return 0;
  }
  else return count;
}
int CheckCameras(void) {
  void *id;
  Dlink *Ilist=NULL;
  CameraRec *cpt;
  const char	*name, *value;
  int count,ret=1,i,j;
  id = kgOpenBusy(Parent,200,50);
  CleanCamera();
  Clist = Dopen();
  if(context == NULL) context = sample_create_context (); /* see context.c */
 /* Detect all the cameras that can be autodetected... */
  ret = gp_list_new (&list);
  if (ret < GP_OK) return 0;
  ConError=0;
  count = sample_autodetect (list, context);
  kgCloseBusy(id);
  j=0;
  if(!ConError) {
    id = kgOpenBusy(Parent,200,50);
    cams = (Camera **)calloc (sizeof (Camera*),count);
    for (i = 0; i < count; i++) {
      gp_list_get_name  (list, i, &name);
      gp_list_get_value (list, i, &value);
      ret = sample_open_camera (&cams[i], name, value, context);
      if (ret < GP_OK) continue;
      if(ConError) { ConError=0;continue;}
      cpt = (CameraRec *)malloc(sizeof(CameraRec));
      sprintf(cpt->name,"%s:%s",name,value);
      cpt->cam=cams[i];
      cpt->Imglist=Dopen();
//      print_folders(cams[i],cpt->Imglist,context,"/");
//      if(ConError) { Dempty(cpt->Imglist);free(cpt);break;}
      Dappend(Clist,cpt);
      j++;
    }
    CameraCount=j;
    if(j==0) {CleanCamera();}
    kgCloseBusy(id);
#if 0
    if(ConError) {
      ConnectError();
      ConError=0;
    }
#endif
  }
  return j;
}
int CheckExtension(char *name,char *ext) {
  int i=0;
  i = strlen(name);
  while( (i>=0)&&(name[i]<=' ')) name[i--]='\0';
  if(i<0) return 0;
  while( (i>=0)&&(name[i]!='.')) i--;
  if(i<0) return 0;
  if(strcmp(name+i+1,ext) == 0) return 1;
  else return 0;
}
ThumbNail ** GetCameraList(void) {
  CameraRec *cpt;
  ThumbNail **th=NULL;
  int count,i;
  ConError=0;
  if(Clist != NULL) {
   CleanCamera();
   Clist=NULL;
  }
  count = CheckCameras();
//  else if(!CheckCameraStatus()) return NULL;
//  if((count==0)||ConError) { ConError=0;return NULL;}
  if((count==0)) { ConError=0;return NULL;}
  count = Dcount(Clist);
  if(count > 0) {
    th = (ThumbNail **)malloc(sizeof(ThumbNail *)*(count+1));
    th[count]=NULL;
    Resetlink(Clist);
    i=0;
    while( (cpt=(CameraRec *)Getrecord(Clist))!= NULL) {
      th[i]=(ThumbNail *)malloc(sizeof(ThumbNail));
      th[i]->name=(char *)malloc(strlen(cpt->name)+1);
      strcpy(th[i]->name,cpt->name);
      th[i]->img=NULL;
      th[i]->sw=0;
      th[i+1]= NULL;
      i++;
    }
  }
  return th;
}
ThumbNail **GetImageList (int CameraId,char *folder) {
  void *id;
  GMIMG *Timg;
  Camera *cam;
  CameraRec *cpt;
  CameraImage *Ipt;
  ThumbNail **th=NULL;
  int count,i;
  Dlink *Ilist;
  count= CameraCount;
  if(Clist == NULL) {
    count = CheckCameras();
  }
  else if(!CheckCameraStatus()) return NULL;
  if(count==0 )return th;
  Resetlink(Clist);
  Dposition(Clist,CameraId);
  cpt = (CameraRec *)Getrecord(Clist);
  cam=cpt->cam;
  Ilist = cpt->Imglist;
  if(Ilist == NULL) return NULL;
  count=Dcount(Ilist);
  th=(ThumbNail **)malloc(sizeof(ThumbNail *)*(count+1));
  for(i=0;i<=count;i++) th[i]=NULL;
  Resetlink(Ilist);
  i=0;
  ConError=0;
  id = kgOpenBusy(Parent,200,150);
  while ( (Ipt=(CameraImage *)Getrecord(Ilist))!= NULL) {
     copy_camera_thumbnail(cam,context,Ipt->folder,Ipt->file);
     if(ConError) {
         break;
     }
     Timg= (GMIMG *)kgGetImage((char *)"thumbnail.jpg");
     Ipt->Timg = kgResizeImage(Timg,(float)(90.0/Timg->image_width));
     kgFreeImage(Timg);
     th[i]=(ThumbNail *)malloc(sizeof(ThumbNail));
     th[i]->name = (char *)malloc(strlen(Ipt->file)+1);
     strcpy(th[i]->name,Ipt->file);
     th[i]->img= Ipt->Timg;
     th[i]->sw=0;
     i++;
  }
  kgCloseBusy(id);
  if(ConError) {
         kgFreeThumbNails(th);
         th = NULL;
         ConnectError();
         ConError=0;
  }
  return th;
}
int DeleteCameraFile(int CameraId,int ImageId) {
   CameraRec *cpt;
   CameraImage *ipt;
   Dlink *Ilist;
   if(Clist==NULL) {
    ConnectError();
    return 0;
   }
   if((CameraId< 1)||(CameraId> Dcount(Clist))) {
    ConnectError();
    return 0;
   }
   ConError=0;
   if(!CheckCameraStatus()) {
     ConnectError();
     return 0;
   }
   Resetlink(Clist);
   Dposition(Clist,CameraId);
   cpt = (CameraRec *)Getrecord(Clist);
   Ilist = cpt->Imglist;
   Resetlink(Ilist);
   Dposition(Ilist,ImageId);
   ipt = (CameraImage *)Dpick(Ilist);
   delete_camera_file(cpt->cam,context,ipt->folder,ipt->file);
   kgFreeImage(ipt->Img);
//   kgFreeImage(ipt->Timg); // done elsewhere
   if(ConError) {ConnectError();ConError=0;return 0;}
   else return 1;
}
int CopyCameraFile_o(int CameraIddum,int ImageId,char *outfile) {
   CameraRec *cpt;
   FSTREE *ipt;
   char folder[1000];
   Dlink *Ilist;
   if(Clist==NULL){
    ConnectError();
    return 0;
   }
   if((CameraId< 1)||(CameraId> Dcount(Clist))) return 0;
   if(!CheckCameraStatus()){
      ConnectError();
      return 0;
   }
   ConError=0;
   Resetlink(Clist);
   Dposition(Clist,CameraId);
   cpt = (CameraRec *)Getrecord(Clist);
   Ilist = curnode->files;
   Resetlink(Ilist);
   Dposition(Ilist,ImageId);
   ipt = (FSTREE *)Getrecord(Ilist);
   GetFullPath(ipt->parent,folder);
   CopyCameraFile(folder,ipt->name,outfile);
   if(ConError) {ConnectError();ConError=0;return 0;}
   else return 1;
}
