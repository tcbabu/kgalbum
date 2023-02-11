#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <kulina.h>
#include "rdwrfuns.h"
#include "imagesb.c"
extern char AlbumDir[300],Album[100]; 
Dlink *Alblist=NULL,*Photolist=NULL;

ThumbNail **ReadAlbumList(void) {
   int l,i,k;
   static void *img=NULL;
   void *Cimg,*pimg;
   ThumbNail **Albums=NULL;
   char *album,buf[600];
   FILE *fp;
   if(img==NULL){
     img = (void *)kgFMGtogmImage((void *)&book_str);
   }
   if(Alblist!=NULL) {
     Dempty(Alblist);
   }
   Alblist=Dopen();
   fp = fopen(".albums","r");
   if(fp!= NULL) {
     while(fgets(buf,299,fp) != NULL) {
       l = strlen(buf);
       if(buf[l-1]=='\n') buf[l-1]='\0';
       album= (char *)malloc(300);
       strcpy(album,buf);
//       album[300]='\0';
//       strcpy(album+300,"book.png");
       Dappend(Alblist,album);
     }
     fclose(fp);
     l = Dcount(Alblist)+1;
     Albums = (ThumbNail **) malloc(sizeof(ThumbNail *)*(l));
     Albums[l-1]= NULL;
     i =0;
     Resetlink(Alblist);
     while( (album = (char *)Getrecord(Alblist))!= NULL) {
        Albums[i]=(ThumbNail *) malloc(sizeof(ThumbNail));
        l=strlen(album);
        k=l-1;
        while((k>0)&&(album[k]<=' ')) {album[k--]='\0';}
        Albums[i]->name = (char *)malloc(strlen(album)+1);
        strcpy(Albums[i]->name,album);
//        printf("%s\n",album);
        Cimg = kgChangeSizeImage(img,90,60);
        sprintf(buf,"%-s/.CoverImgs/%-s.jpg",AlbumDir,Albums[i]->name);
//          strcat(buf,album+300);
//          printf("%s\n",buf);
        pimg = (void *)kgGetImage(buf);
        Cimg = kgMergeImages(Cimg,pimg,0,0);
        kgFreeImage(pimg);
        Albums[i]->img = Cimg;
        Albums[i]->sw=0;
        i++;
     }   
   }
   return Albums;
}

void WriteAlbumList(void) {
   char *album;
   FILE *fp;
   int k;
   fp =fopen(".albums","w");
   if(Alblist!=NULL){
     Resetlink(Alblist);
     while( (album = (char *)Getrecord(Alblist))!= NULL) {
//       printf("  %s\n",album);
       album[299]='\0';
       k=298;
       while( (k>0)&&(album[k]<=' ')) album[k--]='\0';
       fprintf(fp,"%s\n",album);
     }
   }
   fclose(fp);
}
ThumbNail **ReadPhotoList(void) {
   int l,i;
   ThumbNail **Albums=NULL;
   char *photo,*orgphoto,buf[300],buf1[200],*orgloc;
   PHOTO *ph;
   FILE *fp;
   if(Photolist!=NULL) {
     Dempty(Photolist);
   }
   Photolist=Dopen();
   fp = fopen(".photos","r");
   if(fp!= NULL) {
     while(fgets(buf,299,fp) != NULL) {
       l = strlen(buf);
       buf[l-1]='\0';
       ph = (PHOTO *)malloc(sizeof(PHOTO));
       i=0;
       while((buf[i]>=' ')&&(buf[i]!='|')) {ph->name[i]=buf[i];i++;}
       orgloc = buf+i;
       ph->name[i]='\0';
//       printf("\n%s\n",ph->name);
       if(*orgloc =='|'){
         orgloc++;
         i=0;
         while((orgloc[i]>=' ')) {ph->orgfile[i]=orgloc[i];i++;}
         ph->orgfile[i]='\0';
       }
       else ph->orgfile[0]='\0';
      
       Dappend(Photolist,ph);
     }
     fclose(fp);
     l = Dcount(Photolist)+1;
     Albums = (ThumbNail **) malloc(sizeof(ThumbNail *)*(l));
     Albums[l-1]= NULL;
     i =0;
     Resetlink(Photolist);
     while( (ph = (PHOTO *)Getrecord(Photolist))!= NULL) {
        Albums[i]=(ThumbNail *) malloc(sizeof(ThumbNail));
        Albums[i]->name = (char *)malloc(strlen(ph->name)+1);
        strcpy(Albums[i]->name,ph->name);
//        printf("%s\n",album);
        sprintf(buf,".Thumbnail/%s",Albums[i]->name);
        Albums[i]->img = kgGetImage(buf);
        Albums[i]->sw=0;
        i++;
     }   
   }
   return Albums;
}

void WritePhotoList(void) {
   PHOTO *ph;
   FILE *fp;
   fp =fopen(".photos","w");
   if(Photolist!=NULL){
     Resetlink(Photolist);
     while( (ph = (PHOTO *)Getrecord(Photolist))!= NULL) {
       fprintf(fp,"%s|%s\n",ph->name,ph->orgfile);
//       printf("  %s|%s\n",ph->name,ph->orgfile);
     }
   }
   fclose(fp);
}
