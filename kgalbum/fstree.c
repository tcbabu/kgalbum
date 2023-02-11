#include <stdio.h>
#include <kulina.h>
#include "Fsimages.c"
#include "fstree.h"
extern void *Main;
extern int CameraId;
void ** GetCameraFileList( const char *folder);
void ** GetCameraFolderList( const char *folder);
void *CopyCameraThumbnail( char *folder,char *fn);
void CopyCameraFile(char *folder,char *fn,char *outfile);
int GetFullPath(FSTREE *node,char *buff);
void *folderimg=NULL,*fileimg=NULL;
FSTREE * InitFoldernode(char *name,void *Thumb,FSTREE *parent) {
  FSTREE *fspt=NULL;
  fspt = (FSTREE *)malloc(sizeof(FSTREE));
  if(fspt== NULL) {
    fprintf(stderr,"Error: Malloc InitFstree\n");
  }
  else {
    strcpy(fspt->name,name);
    fspt->parent = parent;
    fspt->folders=Dopen();
    fspt->files =Dopen();
    fspt->Thumb=Thumb;
    fspt->type = 1;
    fspt->explored = 0;
  }
  return fspt;
}
FSTREE * InitFilenode(char *name,void *Thumb,FSTREE *parent) {
  char buff[1000];
  FSTREE *fspt=NULL;
  fspt = (FSTREE *)malloc(sizeof(FSTREE));
  if(fspt== NULL) {
    fprintf(stderr,"Error: Malloc InitFstree\n");
  }
  else {
    strcpy(fspt->name,name);
    fspt->parent = parent;
    fspt->folders=NULL;
    fspt->files =NULL;
    fspt->explored = 1; // explored
    fspt->type =0; //file
    if(CameraId<=0 ) fspt->Thumb=Thumb;
    else {
      GetFullPath(parent,buff);
//      kgPrintf(Main,10,name);
      fspt->Thumb = CopyCameraThumbnail(buff,name);
    }
  }
  return fspt;
}
int GetFullPath(FSTREE *node,char *buff) {
  char lbuff[2000];
  if( node->parent==NULL) {
   strcpy(buff,node->name);
  }
  else {
    GetFullPath(node->parent,lbuff);
    strcpy(buff,lbuff);
    if(strcmp(lbuff,"/") != 0)strcat(buff,"/");
    strcat(buff,node->name);
  }
  return 1;
}
int ExploreFsnode(FSTREE *node,char*filter) {
  int ret=1;
  char **folders,**files;
  int i=0;
  char buff[2000];
  FSTREE *pt;
  GetFullPath(node,buff);
//  printf("Full Path= %s\n",buff);
  if(node->type==0) return 0;
  if(node->explored==1) return 1;
//  if ((node->folders==NULL)&&(node->files==NULL)) ret= 0;
  else {
#if 0
    folders = kgFolderMenu(buff);
    files   = kgFileMenu(buff,filter);
#else
    folders = (char **)GetCameraFolderList((const char *)buff);
    if(folders==NULL) {
      return 0;
    }
    files = (char **)GetCameraFileList((const char *)buff);
    if(files==NULL) {
      return 0;
    }
    node->explored=1;
#endif
    if( folders[0]==NULL) {
      free(folders);
      Dempty(node->folders);
      node->folders=NULL;
    }
    else {
      i=0;
      while(folders[i]!=NULL) {
        pt = InitFoldernode(folders[i],NULL,node);
        Dadd(node->folders,pt);
        i++;
      }
    }
    if( files[0]==NULL) {
      free(files);
      if(node->files != NULL) Dempty(node->files);
      node->files=NULL;
    }
    else {
      i=0;
      while(files[i]!=NULL) {
        pt = InitFilenode(files[i],NULL,node);
        Dadd(node->files,pt);
        i++;
      }
    }
  }
  return ret;
}
ThumbNail **MakeFolderNails(FSTREE *fs,int l,int h) {
   int count,ln,i;
   ThumbNail **Th=NULL,*tpt;
   FSTREE *fpt;
   Dlink *L;
   if(folderimg==NULL) folderimg=kgGetImageCopy(NULL,(char *)(&Folder_str));
   L = fs->folders;
   if(L!=NULL) {
     count = Dcount(L);
     Th= (ThumbNail **)malloc(sizeof(ThumbNail*)*(count+1));
     Th[count]=NULL;
     Resetlink(L);
     i=0;
     while( (fpt=(FSTREE *)Getrecord(L))!= NULL) {
       tpt=(ThumbNail *)malloc(sizeof(ThumbNail));
       ln = strlen(fpt->name)+1;
       tpt->name = (char *)malloc(sizeof(char)*ln);
       strcpy(tpt->name,fpt->name);
       if(fpt->Thumb!= NULL) {
         tpt->img = (void *) kgThumbNailImage(fpt->Thumb,l,h);
       }
       else tpt->img= (void *) kgThumbNailImage(folderimg,l,h);
       tpt->sw=0;
       Th[i]=tpt;
       i = i+1;
     }
   }
   return Th;
}
ThumbNail **MakeFileNails(FSTREE *fs,int l,int h) {
   int count,ln,i;
   ThumbNail **Th=NULL,*tpt;
   FSTREE *fpt;
   Dlink *L;
   if(fileimg==NULL) fileimg=kgGetImageCopy(NULL,(char *)(&Unknownfile_str));
   L = fs->files;
   if(L!=NULL) {
     count = Dcount(L);
     Th= (ThumbNail **)malloc(sizeof(ThumbNail*)*(count+1));
     Th[count]=NULL;
     Resetlink(L);
     i=0;
     while( (fpt=(FSTREE *)Getrecord(L))!= NULL) {
       tpt=(ThumbNail *)malloc(sizeof(ThumbNail));
       ln = strlen(fpt->name)+1;
       tpt->name = (char *)malloc(sizeof(char)*ln);
       strcpy(tpt->name,fpt->name);
       if(fpt->Thumb!= NULL) {
         tpt->img = (void *) kgThumbNailImage(fpt->Thumb,l,h);
       }
       else tpt->img= (void *) kgThumbNailImage(fileimg,l,h);
       tpt->sw=0;
       Th[i]=tpt;
       i = i+1;
     }
   }
   return Th;
}
int FreeFstree(FSTREE *fs) {
  FSTREE *fpt;
  Dlink *L;
  if(fs==NULL) return 0;
  L = fs->folders;
  if(L!= NULL) {
    Resetlink(L);
    while( (fpt=(FSTREE *)Getrecord(L))!= NULL) {
      FreeFstree(fpt);
    }
    Dempty(L);
    fs->folders=NULL;
  }
  L = fs->files;
  if(L!= NULL) {
    Dempty(L);
    fs->files=NULL;
  }
  return 1;
}
int PrintFstree(FSTREE *fs) {
  FSTREE *fpt;
  Dlink *L;
  if(fs==NULL) return 0;
  L = fs->folders;
  if(L!= NULL) {
    Resetlink(L);
    while( (fpt=(FSTREE *)Getrecord(L))!= NULL) {
      printf("%s\n",fpt->name);
      PrintFstree(fpt);
    }
  }
  L = fs->files;
  if(L!= NULL) {
    Resetlink(L);
    while( (fpt=(FSTREE *)Getrecord(L))!= NULL) {
      printf("   %s\n",fpt->name);
    }
  }
  return 1;
}
#if 0
int main(int argc ,char **argv) {
   FSTREE *fs,*fspt;
   fs = InitFoldernode(argv[1],NULL,NULL);
   if(ExploreFsnode(fs,"*")) {
#if 1
     Resetlink(fs->folders);
     while( (fspt=(FSTREE *)Getrecord(fs->folders))!= NULL) {
       ExploreFsnode(fspt,"*");
     }
#endif
     PrintFstree(fs);
     MakeFolderNails(fs,22);
     MakeFileNails(fs,22);
   }
   FreeFstree(fs);
   free(fs);
}
#endif
