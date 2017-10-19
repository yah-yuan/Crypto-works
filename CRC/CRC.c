/*
CRC算法编码实现
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

unsigned int crc_8Check = 0x31;         //100110001
unsigned int test8 = 0x80;

unsigned char * CRC_Encode(char * strAscii  , int size);

unsigned char * CRC_Encode(char * strAscii , int size) {
    unsigned char * res;
    unsigned char jar,tmp;
    unsigned int i = 0,j=0,flag = 1;   //j计算每个字符是否用完
    res = (char *) malloc ((size+8)*sizeof(cha r)); 
    for(i=0;;) {
        unsigned int cmp = jar&test8;
        if(i>=size)
            break;
        if(flag == 1) {
            tmp = *(strAscii+i);
            flag = 0;
        }
            jar <<= 1;
            jar = jar | ((tmp&0x80)>>7);
            tmp = tmp<<1;
            j++;
            if(cmp!=0)
                jar = jar ^ crc_8Check;
        if(j==8) {
            j = 0;
            flag = 1;
            i++;
        }
    }
    strcat(res,strAscii);
    strcat(res,&jar);
    return res;
}

int main()
{
    unsigned char strAscii[256], *res;
    int i, size;
    printf("please input the ascii words:");
    scanf("%[^\n]", strAscii);
    size = strlen(strAscii);
    res = CRC_Encode(strAscii, size);
    printf("%s %x %d\n",res,res,sizeof(res));
}