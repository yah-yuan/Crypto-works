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
int nRound = nRoundTable[(nBlock / 2 - 2) + (nBlock / 2 - 2) * 3];

unsigned char *EnCodeAES(char *plainFileName);                                       //读取密文-->产生密钥-->开始加密-->写出密文文件和密钥
unsigned char *DeCodeAES(unsigned char *cipherFileName, unsigned char *keyFileName); //读取密文和密钥-->开始解密-->写出明文文件
unsigned char *KeyExpansion(unsigned char *mainKey);                                 //密钥扩展
void Controller(unsigned char *state, unsigned char *expandedKey);                   //加密函数
void ByteSub(unsigned char *state, int words);
void ShiftRow(unsigned char *state);
void MixColumn(unsigned char *state);
void AddRoundKey(unsigned char *state, unsigned char *roundKey);
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

    strcpy(plainText, "abcdef0123456789");
    cipherText = EnCodeAES(plainText);
    cipherKey = cipherText + 16;
    plainText = DeCodeAES(cipherText, cipherKey);
    printf("%s", plainText);
#endif
}

unsigned char *EnCodeAES(unsigned char *plainFileName)
{ //读取密文-->产生密钥-->开始加密-->写出密文文件和密钥
#if (DEBUG)
    unsigned char cipherKey[4 * nKey];
    unsigned char mainKey[4 * nKey];
    unsigned char state[4 * nBlock];
    unsigned char *plainText = plainFileName;
    unsigned char *expandedKey;
    unsigned char *cipherText;
    int i, j;

    EnDeFlag = 0;
    srand((void)time(NULL));
    for (i = 0; i < 4 * nBlock; i++)
    {
        *(cipherKey + i) = (unsigned char)rand() % 0xff;
    }
    //将密钥写入mainKey
    for (i = 0; i < 4 * nKey; i++)
    {
        for (j = 0; j < 4; j++)
        {
            *(mainKey + i * 4 + j) = *(cipherKey + j * nKey + i);
        }
    }
    expandedKey = KeyExpansion(mainKey);
    //将明文写入state
    for (i = 0; i < 4 * nBlock; i++)
    {
        for (j = 0; j < 4; j++)
        {
            *(state + i * 4 + j) = *(plainText + j * nBlock + i);
        }
    }
    //开始加密
    Controller(state, expandedKey);
    //加密结束，取出密文
    //这里是归位还是再按照其顺序写一遍？暂时归位
    for (i = 0; i < 4 * nBlock; i++)
    {
        for (j = 0; j < 4; j++)
        {
            *(cipherText + j * nKey + i) = *(state + i * 4 + j);
        }
#endif
    }
    unsigned char *DeCodeAES(unsigned char *cipherFileName, unsigned char *keyFileName)
}
unsigned char *KeyExpansion(unsigned char *mainKey)
{ //密钥扩展
    unsigned char expandedKey[nBlock * (nRound + 1) * 4];
    unsigned char tmp4Char[4];
    unsigned char tmp1Char[1];
    int i, j;

    CpChars(expandedKey, mainKey, nKey * 4);
    for (i = nKey; i < nBlock * (nRound + 1); i++)
    {
        CpChars(tmp4Char, expandedKey + (i - 1) * 4, 4);
        if (i % nKey == 0)
        {
            //Rotl(tmp)
            *tmp1Char = *(tmp4Char + i * 4);
            *(tmp4Char + i * 4) = *(tmp4Char + i * 4 + 1);
            *(tmp4Char + i * 4 + 1) = *(tmp4Char + i * 4 + 2);
            *(tmp4Char + i * 4 + 2) = *(tmp4Char + i * 4 + 3);
            *(tmp4Char + i * 4 + 3) = *tmp1Char;
            //subByte
            ByteSub(tmp4Char, 4);
            //temp
            *(tmp4Char) ^= RC[i];
            for (i = 1; i < 4; i++)
            {
                *(tmp4Char + i) ^= 0x00;
            }
        }
        for (j = 0; j < 4; j++)
        {
            *(expandedKey + i * 4 + j) = *(expandedKey + (i - nKey) * 4 + j) ^ *(tmp4Char + j);
        }
    }
    if (EnDeFlag == 1)
    { //解密时将密钥置换位置
    }
    return expandedKey;
}
void Controller(unsigned char *state, unsigned char *expandedKey)
{ //加密函数,轮函数控制器
    int i;
    unsigned char *roundKey = expandedKey;
    //初始轮密钥相加
    AddRoundKey(state, roundKey);
    //进行nRound-1次轮函数
    for (i = 1; i < (nRound - 1); i++)
    {
        roundKey += 4;
        ByteSub(state, nBlock * 4);
        ShiftRow(state);
        MixColumn(state);
        AddRoundKey(state, roundKey);
    }
    //进行最后一轮变换
    roundKey += 4;
    ByteSub(state, nBlock * 4);
    ShiftRow(state);
    AddRoundKey(state, roundKey);
    //加密结束，state是加密后的密文
}

void ByteSub(unsigned char *state, int words)
{
    int i;
    unsigned char tmp1Char;
    unsigned char *theBox = EnDeFlag ? invsBox : sBox;

    for (i = 0; i < words; i++)
    {
        tmp1Char = state[i] >> 4;
        state[i] &= 0x0f;
        state[i] = theBox[state[i] + tmp1Char * 16];
    }
}
void ShiftRow(unsigned char *state)
{
    unsigned char tmp1Char, tmpnChar[nBlock];
    int shiftNum;
    int i,j;
    for (i = 1; i < 4; i++)
    {
        shiftNum = EnDeFlag ? shiftTable[(nBlock / 2 - 2) * 3 + i] : (nBlock - shiftTable[(nBlock / 2 - 2) * 3 + i]);
        tmp1Char = state[i*nBlock] >> (8-shiftNum);
        for(j=0;j<nBlock;j++) {
            state[i*nBlock+j] <<= shiftNum;
            if(j==(nBlock-1)) {
                state[i*nBlock+j] |= tmp1Char;
            }
            else {
                state[i*nBlock+j] |= state[i*nBlock+j+1] >> (8-shiftNum);
            }
        }
    }
    //循环左移结束
}
void MixColumn(unsigned char *state)
{
}
void AddRoundKey(unsigned char *state, unsigned char *roundKey)
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