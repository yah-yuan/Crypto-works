/* md5 实现
Au: T.Zhang
T : Fri Aug  7 10:50:12 CST 2020
*/
#include <stdio.h>
#include <unistd.h> // read()
#include <errno.h> //errno
#include <string.h> //strerror

#include "md5.h"

int hash_round(uint32* md5sum[], uint32 chunk[]){
    static int count = 0;
    int ret = _hash_round(md5sum,chunk);
    // printf("[%d]:A=0x%x B=0x%x C=0x%x D=0x%x\n",count,(unsigned int)*md5sum[0],(unsigned int)*md5sum[1],(unsigned int)*md5sum[2],(unsigned int)*md5sum[3]);
    count++;
    return ret;
}

int md5(FILE* f){
    uint32 A = INIT_A;
    uint32 B = INIT_B;
    uint32 C = INIT_C;
    uint32 D = INIT_D;

    uint32* md5sum[4] = {&A,&B,&C,&D};

    uint32 chunk[16] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

    fseek(f,0,SEEK_END);
    uint32 file_size = ftell(f); // maximum to 2GB
    fseek(f,0,SEEK_SET);
    uint32 read_size = 0;
    uint32 file_size_remainder;
    int i;
    while(1){
        fread(chunk,4,16,f);
        if(file_size-read_size<64){
            file_size_remainder = file_size-read_size;
            if(file_size_remainder < 56){
                *((char*)chunk+file_size_remainder) = 0x80;
                file_size_remainder++;
                while (file_size_remainder != 56){
                    *((char*)chunk+file_size_remainder) = 0;
                    file_size_remainder++;
                }
            }
            else if(file_size_remainder >= 56){
                *((char*)chunk+file_size_remainder) = 0x80;
                file_size_remainder++;
                while (file_size_remainder != 64){
                    *((char*)chunk+file_size_remainder) = 0;
                    file_size_remainder++;
                }
                hash_round(md5sum, chunk);
                for(i=0;i<14;i++){
                    *(chunk+i) = 0;
                }
            }
            *(chunk+14)=(file_size<<3)&0xffffffff;
            *(chunk+15)=(file_size>>29)&0xffffffff;
        }
        hash_round(md5sum, chunk);
        if(file_size-read_size<64){
            break;
        }
        else{
            read_size += 64;
        }
    }
    printf("The md5 of this file is:\n");
    for(i=0;i<4;i++){
        printf("%02x",*((uint8*)&A+i)&0xff);
    }
    for(i=0;i<4;i++){
        printf("%02x",*((uint8*)&B+i)&0xff);
    }
    for(i=0;i<4;i++){
        printf("%02x",*((uint8*)&C+i)&0xff);
    }
    for(i=0;i<4;i++){
        printf("%02x",*((uint8*)&D+i)&0xff);
    }
    printf("\n");
    return 0;
}

int main(int argv, char* args[]){
    char* filename;
    if(argv == 2) {
        filename = args[1];
    }
    else{
        return 1;
    }
    FILE* f = fopen(filename,"r");
    if(!f){
        printf("open file error, code:%d, readable:%s\n",errno,strerror(errno));
        return 1;
    }
    md5(f);
    fclose(f);
    return 0;
}