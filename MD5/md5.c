/* md5 实现
Au: T.Zhang
T : Fri Aug  7 10:50:12 CST 2020
*/
#include <stdio.h>
#include <unistd.h> // read()
#include "md5.h"

int hash_round(uint32* md5sum[], uint32 chunk[]){ 
    uint32 a = *md5sum[0];
    uint32 b = *md5sum[1];
    uint32 c = *md5sum[2];
    uint32 d = *md5sum[3];
    //第一轮
    a=FF(a,b,c,d,chunk[0],7,0xd76aa478);
    b=FF(d,a,b,c,chunk[1],12,0xe8c7b756);
    c=FF(c,d,a,b,chunk[2],17,0x242070db);
    d=FF(b,c,d,a,chunk[3],22,0xc1bdceee);
    a=FF(a,b,c,d,chunk[4],7,0xf57c0faf);
    b=FF(d,a,b,c,chunk[5],12,0x4787c62a);
    c=FF(c,d,a,b,chunk[6],17,0xa8304613);
    d=FF(b,c,d,a,chunk[7],22,0xfd469501);
    a=FF(a,b,c,d,chunk[8],7,0x698098d8);
    b=FF(d,a,b,c,chunk[9],12,0x8b44f7af);
    c=FF(c,d,a,b,chunk[10],17,0xffff5bb1);
    d=FF(b,c,d,a,chunk[11],22,0x895cd7be);
    a=FF(a,b,c,d,chunk[12],7,0x6b901122);
    b=FF(d,a,b,c,chunk[13],12,0xfd987193);
    c=FF(c,d,a,b,chunk[14],17,0xa679438e);
    d=FF(b,c,d,a,chunk[15],22,0x49b40821);

    // 第二轮
    a=GG(a,b,c,d,chunk[1],5,0xf61e2562);
    b=GG(d,a,b,c,chunk[6],9,0xc040b340);
    c=GG(c,d,a,b,chunk[11],14,0x265e5a51);
    d=GG(b,c,d,a,chunk[0],20,0xe9b6c7aa);
    a=GG(a,b,c,d,chunk[5],5,0xd62f105d);
    b=GG(d,a,b,c,chunk[10],9,0x02441453);
    c=GG(c,d,a,b,chunk[15],14,0xd8a1e681);
    d=GG(b,c,d,a,chunk[4],20,0xe7d3fbc8);
    a=GG(a,b,c,d,chunk[9],5,0x21e1cde6);
    b=GG(d,a,b,c,chunk[14],9,0xc33707d6);
    c=GG(c,d,a,b,chunk[3],14,0xf4d50d87);
    d=GG(b,c,d,a,chunk[8],20,0x455a14ed);
    a=GG(a,b,c,d,chunk[13],5,0xa9e3e905);
    b=GG(d,a,b,c,chunk[2],9,0xfcefa3f8);
    c=GG(c,d,a,b,chunk[7],14,0x676f02d9);
    d=GG(b,c,d,a,chunk[12],20,0x8d2a4c8a);

    // 第三轮
    a=HH(a,b,c,d,chunk[5],4,0xfffa3942);
    b=HH(d,a,b,c,chunk[8],11,0x8771f681);
    c=HH(c,d,a,b,chunk[11],16,0x6d9d6122);
    d=HH(b,c,d,a,chunk[14],23,0xfde5380c);
    a=HH(a,b,c,d,chunk[1],4,0xa4beea44);
    b=HH(d,a,b,c,chunk[4],11,0x4bdecfa9);
    c=HH(c,d,a,b,chunk[7],16,0xf6bb4b60);
    d=HH(b,c,d,a,chunk[10],23,0xbebfbc70);
    a=HH(a,b,c,d,chunk[13],4,0x289b7ec6);
    b=HH(d,a,b,c,chunk[0],11,0xeaa127fa);
    c=HH(c,d,a,b,chunk[3],16,0xd4ef3085);
    d=HH(b,c,d,a,chunk[6],23,0x04881d05);
    a=HH(a,b,c,d,chunk[9],4,0xd9d4d039);
    b=HH(d,a,b,c,chunk[12],11,0xe6db99e5);
    c=HH(c,d,a,b,chunk[15],16,0x1fa27cf8);
    d=HH(b,c,d,a,chunk[2],23,0xc4ac5665);

    // 第四轮
    a=II(a,b,c,d,chunk[0],6,0xf4292244);
    b=II(d,a,b,c,chunk[7],10,0x432aff97);
    c=II(c,d,a,b,chunk[14],15,0xab9423a7);
    d=II(b,c,d,a,chunk[5],21,0xfc93a039);
    a=II(a,b,c,d,chunk[12],6,0x655b59c3);
    b=II(d,a,b,c,chunk[3],10,0x8f0ccc92);
    c=II(c,d,a,b,chunk[10],15,0xffeff47d);
    d=II(b,c,d,a,chunk[1],21,0x85845dd1);
    a=II(a,b,c,d,chunk[8],6,0x6fa87e4f);
    b=II(d,a,b,c,chunk[15],10,0xfe2ce6e0);
    c=II(c,d,a,b,chunk[6],15,0xa3014314);
    d=II(b,c,d,a,chunk[13],21,0x4e0811a1);
    a=II(a,b,c,d,chunk[4],6,0xf7537e82);
    b=II(d,a,b,c,chunk[11],10,0xbd3af235);
    c=II(c,d,a,b,chunk[2],15,0x2ad7d2bb);
    d=II(b,c,d,a,chunk[9],21,0xeb86d391);

    *md5sum[0] += a;
    *md5sum[1] += b;
    *md5sum[2] += c;
    *md5sum[3] += d;

}

int md5(FILE* f){
    uint32 A = INIT_A;
    uint32 B = INIT_B;
    uint32 C = INIT_C;
    uint32 D = INIT_D;

    uint32* md5sum[4] = {&A,&B,&C,&D};

    uint32 chunk[16];

    int chunk_count = 0;
    uint64 file_size = 0;
    while(1){
        int read_size = fread(chunk,1,64,f);
        if(read_size == 0){
            //返回值
            return 0;
        }
        if(read_size != 64){
            char flag = 1;
            file_size = chunk_count * 64 & 0xffffffff;
            (unsigned char*)chunk;
            if(read_size > 56){
                while(read_size != 64){
                    if (flag){
                        chunk[read_size] = 0x80;
                        flag = 0;
                    }
                    else{
                        chunk[read_size] = 0;
                    }
                    read_size ++;
                }
                hash_round(md5sum, chunk);
                // 额外的 chunk
                read_size = 0;
                while(read_size != 56){
                    chunk[read_size] = 0;
                    read_size ++;
                }
            }
            else{
                while(read_size != 56){
                    if (flag){
                        chunk[read_size] = 0x80;
                        flag = 0;
                    }
                    else{
                        chunk[read_size] = 0;
                    }
                    read_size ++;
                }
            }
            *(uint64*)(chunk + 56) = file_size;
            (uint32*)chunk;
        }
        else{
            chunk_count++;
        }
        hash_round(md5sum, chunk);
        printf("[%d]:readsize=%d A=0x%x B=0x%x C=0x%x D=0x%x\n",chunk_count,read_size,(unsigned int)A,(unsigned int)B,(unsigned int)C,(unsigned int)D);
    }
}

int main(int argv, char* args[]){
    char* filename;
    printf("argv=%d\n",argv);
    if(argv == 2) {
        filename = args[1];
    }
    else{
        return 1;
    }
    FILE* f = fopen(filename,"r");
    md5(f);
}