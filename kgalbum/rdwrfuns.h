typedef struct _Photo {
 char name[200];
 char orgfile[300];
} PHOTO;
ThumbNail **ReadAlbumList(void);
void WriteAlbumList(void);
ThumbNail **ReadPhotoList(void);
void WritePhotoList(void);
