#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

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
        printf("%s\n",pt);
        return pt;
      }
    }
    pclose(fp);
  }
  return NULL;
}
int main(void) {GetMediaMount();}
