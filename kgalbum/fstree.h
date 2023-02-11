#ifndef D_FSTREE
#define D_FSTREE
typedef struct _fstree {
   char name[300];
   struct _fstree *parent;
   Dlink *folders; // NULL means no folders; Empty means stiil to be explored;
   Dlink *files; // NULL means no files; Empty meand still to be explored;
   // folders and files are both null it is file;
   void *Thumb; //thumbnail image; can be NULL
   int type; // 1 folder : 0 file
   int explored; //0 not explored 1 explored
} FSTREE;
FSTREE * InitFoldernode(char *name,void *Thumb,FSTREE *parent);
FSTREE * InitFilenode(char *name,void *Thumb,FSTREE *parent);
int GetFullPath(FSTREE *node,char *buff);
int ExploreFsnode(FSTREE *node,char*filter);
ThumbNail **MakeFolderNails(FSTREE *fs,int l,int h);
ThumbNail **MakeFileNails(FSTREE *fs,int l,int h);
int FreeFstree(FSTREE *fs);
int PrintFstree(FSTREE *fs);
#endif
