/*
AES算法的简单实现
创建时间：Fri Oct 27 14:28:05 CST 2017
作者：张
注释：
    调用者将帧内地址作为参数传递给被调用者是否安全
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "AES_inc.h"

#define DEBUG 1

#if (DEBUG)
#endif
int nBlock = 4;
int nKey = 4;
int nRound = 4

unsigned char *EnCodeAES(char *plainFileName);                                       //读取密文-->产生密钥-->开始加密-->写出密文文件和密钥
unsigned char *DeCodeAES(unsigned char *cipherFileName, unsigned char *keyFileName); //读取密文和密钥-->开始解密-->写出明文文件
unsigned char *KeyExpansion(unsigned char *cipherKey);                               //密钥扩展
unsigned char *RIJNDAEL(unsigned char *plainText, unsigned char *cipherKey);         //加密函数
void Round(unsigned char *state, unsigned char *roundKey, int round);                //轮函数
void ByteSub(unsigned char *state,int words);
void ShiftRow(unsigned char *state);
void MixColumn(unsigned char *state);
void AddRoundKey(unsigned char *state);
void CpChars(unsigned char *dest, unsigned char *origin, int length); //复制字

int EnDeFlag = 0; //声明正在加密(0)解密(1)

int main()
{
    char *plainFileName;
    char *cipherFileName;
    char *keyFileName;
#if (DEBUG)
    unsigned char *plainText;
    unsigned char *cipherText;
    unsigned char *cipherKey;

    plainText = (unsigned char *)malloc(16);

    strcpy(plainText,"abcdef0123456789");
    cipherText =  EnCodeAES(plainText);
    cipherKey = cipherText+16;
    plainText = DeCodeAES(cipherText,cipherKey);
    printf("%s",plainText);
#endif
}
unsigned char *EnCodeAES(unsigned char *plainFileName)
{ //读取密文-->产生密钥-->开始加密-->写出密文文件和密钥
#if (DEBUG)
    unsigned char *plainText = plainFileName;
    unsigned char *cipherKey;
    unsigned char *expandedKey;
    unsigned char *state;
    unsigned char *cipherText;
    int i,j;

    EnDeFlag = 0;
    cipherKey = (unsigned char *)malloc(4*nKey);
    state = (unsigned char *)malloc(4*nBlock);
    srand((void)time(NULL));
    for(i=0;i<4*nBlock;i++) {
        *(cipherKey+i) = (unsigned char)rand()%0xff;
    }//这里的密码没按顺序放
    expandedKey = KeyExpansion(cipherKey);
    //将明文写入state
    for(i=0;i<4*nBlock;i++) {
        for(j=0;j<4;j++) {
            *(state+i*4+j) = *(plainText+j*nBlock+i);
        }
    }
    ByteSub(state,4*nBlock);
#endif
}
unsigned char *DeCodeAES(unsigned char *cipherFileName,unsigned char *keyFileName)

}
unsigned char *KeyExpansion(unsigned char *cipherKey)
{ //密钥扩展
    unsigned char *expandedKey;
    unsigned char *tmp4Char;
    unsigned char *tmp1Char;
    int i,j;
    expandedKey = (unsigned char *)malloc(nBlock*(nRound+1)*4);
    tmp4Char = (unsigned char *)malloc(4);
    tmp1Char = (unsigned char *)malloc(1);
    CpChars(expandedKey,cipherKey,nKey*4);
    for(i=nKey;i<nBlock*(nRound+1);i++) {
        CpChars(tmp4Char,expandedKey+(i-1)*4,4);
        if(i%nKey == 0) {
            //Rotl(tmp)
            *tmp1Char = *(tmp4Char+i*4);
            *(tmp4Char+i*4) = *(tmp4Char+i*4+1);
            *(tmp4Char+i*4+1) = *(tmp4Char+i*4+2);
            *(tmp4Char+i*4+2) = *(tmp4Char+i*4+3);
            *(tmp4Char+i*4+3) = *tmp1Char;
            //subByte
            ByteSub(tmp4Char,4);
            //RC[i]
            *(tmp1Char) = 0x01;
            for(j=0;j<i/nKey;j++) {
                *(tmp1Char) = (*(tmp1Char)<<1)%0x11b;
            }
            //temp
            *(tmp4Char) ^= *(tmp1Char);
            for(i=1;i<4;i++) {
                *(tmp4Char+i) ^= 0x00;
            }
        }
        for(j=0;j<4;j++) {
            *(expandedKey+i*4+j) = *(expandedKey+(i-nKey)*4+j) ^ *(tmp4Char+j);
        }
    }
    if(EnDeFlag == 1) {//解密时将密钥置换位置
    }
    return expandedKey;
}
unsigned char *RIJNDAEL(unsigned char *plainText, unsigned char *cipherKey)
{ //加密函数
}
void Round(unsigned char *state, unsigned char *roundKey, int round)
{ //轮函数
}
void ByteSub(unsigned char *state,int words)
{
    int i;
    unsigned char tmp1Char;
    unsigned char * theBox = EnDeFlag?invsBox:sBox;
    
    for(i=0;i<words;i++) {
        
    }
}
void ShiftRow(unsigned char *state)
{
}
void MixColumn(unsigned char *state)
{
}
void AddRoundKey(unsigned char *state)
{
}
void CpChars(unsigned char *dest, unsigned char *origin, int length)
{ //复制字
    int i;
    for (i = 0; i < length; i++)
    {
        *(dest + i) = *(origin + i);
    }
}