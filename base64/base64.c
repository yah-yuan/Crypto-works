/*
Base64的编码和解码
*/
#include <stdlib.h>
#include <stdio.h>
char base64Dic[64];

char *Encode(char *strAscii, int size);
int Decode(char *strBase64, int size);

char *Encode(char *strAscii, int size)
{
    int i, j, flag1 = 0, flag2 = 0, count = 0;
    unsigned char tmpAscii[3];
    unsigned char charBase64[4];
    unsigned char tmpBase64[4];
    //printf("here");
    unsigned char *res = (unsigned char *)malloc((size / 3 + 1) * 4 * sizeof(unsigned char));
    for (i = 0;;)
    {
        //装入ascii缓冲区
        if (i + 3 == size)
            flag2 = 1;
        if (i + 3 > size)
        {
            for (j = 0; j < size % 3; j++)
            {
                tmpAscii[j] = *(strAscii + i + j);
            }
            for (; j < 3; j++)
            {
                tmpAscii[j] = 0;
            }
            flag1 = size % 3;
        }
        else
        {
            for (j = 0; j < 3; j++)
            {
                tmpAscii[j] = *(strAscii + i + j);
            }
        }
        //printf("装入结束，开始移位操作");
        for (j = 0; j < 4; j++)
        {
            switch (j)
            {
            case 0:
                charBase64[0] = tmpAscii[0]>>2;
                break;
            case 1:
                charBase64[1] = (((tmpAscii[0]&0x3) <<4)& 0x30) | (tmpAscii[1] >> 4);
                break;
            case 2:
                charBase64[2] = (((tmpAscii[1]&0xf)<<2)& 0x3c) | (tmpAscii[2] >> 6);
                break;
            case 3:
                charBase64[3] = tmpAscii[2]&0x3f;
                break;
            }
        }
        //printf("移位结束，读取转码表");
        if (flag1)
        {
            if (flag1 = 1)
            {
                tmpBase64[0] = base64Dic[charBase64[0]];
                tmpBase64[1] = base64Dic[charBase64[1]];
                tmpBase64[2] = '=';
                tmpBase64[3] = '=';
            }
            else
            {
                tmpBase64[0] = base64Dic[charBase64[0]];
                tmpBase64[1] = base64Dic[charBase64[1]];
                tmpBase64[2] = base64Dic[charBase64[2]];
                tmpBase64[3] = '=';
            }
        }
        else
            for (j = 0; j < 4; j++)
                tmpBase64[j] = base64Dic[charBase64[j]];
        //printf("加入结果序列");
        for (j = 0; j < 4; j++)
            *(res + count + j) = tmpBase64[j];
        count = count + 4;
        if (flag1 || flag2)
            break;
        i = i + 3;
    }
    return res;
}

int main()
{
    char strAscii[256], *res;
    int i, size;
    strcpy(base64Dic, "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
    printf("please input the ascii words:");
    scanf("%[^\n]", strAscii);
    size = strlen(strAscii);
    res = Encode(strAscii, size);
    printf("%s\n", res);
}