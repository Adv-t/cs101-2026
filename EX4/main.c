#include <stdio.h>

int main()
{
   FILE* fp;
   int a[] = {0,1,2};
   char b[] = "ABC";
   float c[] = {1.1,1.2,1.3};
   int ra[3];
   char rb[4];
   float rc[3];
   
   
   fp = fopen("a.bin","wb+");
   fwrite(a,sizeof(int),3,fp);
   fwrite(b,sizeof(char),4,fp);
   fwrite(c,sizeof(float),3,fp);
   
   fseek(fp,0,SEEK_SET);
   
   fread(ra,sizeof(int),3,fp);
   fread(rb,sizeof(char),4,fp);
   fread(rc,sizeof(float),3,fp);
   fclose(fp);
   
   printf("%d %d %d\n",ra[0],ra[1],ra[2]);
   printf("%c %c %c\n",rb[0],rb[1],rb[2],rb[3]);
   printf("%f %f %f\n",rc[0],rc[1],rc[2]);
   
   

    return 0;
}
