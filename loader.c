#include "loader.h"
void loader(int * ram,char * name) {
    FILE * fp = NULL;
    fp = fopen(name,"rb");
    fread(ram,sizeof(int),RAMMAX,fp);
    fclose(fp);
}