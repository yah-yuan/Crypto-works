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
#include "AES_header.h"

#define DEBUG 1

#if (DEBUG)
#endif
int nBlock = 4;
int nKey = 4;
int nRound = 10;

unsigned char *EnCodeAES(unsigned char *plainFileName);                              //读取密文-->产生密钥-->开始加密-->写出密文文件和密钥
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
    unsigned char plainText[] = {0x00,0x01,0x00,0x01,0x01,0xa1,0x98,0xaf,0xda,0x78,0x17,0x34,0x86,0x15,0x35,0x66};
    unsigned char *cipherText;
    unsigned char *cipherKey;
    unsigned char *decodedText;
    int i;

    cipherText = EnCodeAES(plainText);
    cipherKey = cipherText + 4 * nBlock;
    printf("coded cipher text is:\n");
    for(i=0;i<nBlock*4;i++)
        printf("0x%x ",cipherText[i]);
    printf("cipher key is:\n");
    for(i=0;i<nKey*4;i++)
        printf("0x%x ",cipherKey[i]);
    printf("\n");
    decodedText = DeCodeAES(cipherText, cipherKey);
    printf("decoded plain text is :\n");
    for(i=0;i<nBlock*4;i++)
        printf("0x%x ",decodedText[i]);
    printf("\n");
    printf("%s", decodedText);
#endif
}

unsigned char *EnCodeAES(unsigned char *plainFileName)
{ //读取密文-->产生密钥-->开始加密-->写出密文文件和密钥
#if (DEBUG)
    unsigned char cipherKey[] = {0x00,0x01,0x20,0x01,0x71,0x01,0x98,0xae,0xda,0x79,0x17,0x14,0x60,0x15,0x35,0x94};
    unsigned char mainKey[4 * nKey];
    unsigned char *state;
    unsigned char *plainText = plainFileName;
    unsigned char *expandedKey;
    unsigned char cipherText[4 * nBlock];
    unsigned char *resMessege;
    int i, j;

    EnDeFlag = 0;
    resMessege = (unsigned char *)malloc(4 * nBlock + 4 * nKey);
    state = (unsigned char *)malloc(4 * nBlock);
    nRound = nRoundTable[(nBlock / 2 - 2) + (nKey / 2 - 2) * 3]; //设置轮数
    /*
    srand((unsigned int)time(NULL));
    for (i = 0; i < 4 * nBlock; i++)
    {
        *(cipherKey + i) = (unsigned char)rand() % 0xff;
    }
    */
    //将密钥写入mainKey
    for (i = 0; i < nKey; i++)
    {
        for (j = 0; j < 4; j++)
        {
            *(mainKey + i * 4 + j) = *(cipherKey + j * nKey + i);
        }
    }
    //mainKey = &cipherKey[0];
    expandedKey = KeyExpansion(mainKey);
    //将明文写入state
    for (i = 0; i < nBlock; i++)
    {
        for (j = 0; j < 4; j++)
        {
            *(state + i * 4 + j) = *(plainText + j * nBlock + i);
        }
    }
    //开始加密
    //CpChars(state,plainText,nBlock*4);
    Controller(state, expandedKey);
    //加密结束，取出密文

    for (i = 0; i < nBlock; i++)
    {
        for (j = 0; j < 4; j++)
        {
            *(cipherText + j * nBlock + i) = *(state + i * nBlock + j);
        }
    }
    CpChars(resMessege, cipherText, 4 * nBlock);
    CpChars(resMessege + 4 * nBlock, mainKey, 4 * nKey);
    return resMessege;
#endif
}
unsigned char *DeCodeAES(unsigned char *cipherFileName, unsigned char *keyFileName)
{
    unsigned char *cipherText = cipherFileName;
    unsigned char *cipherKey = keyFileName;
    unsigned char *expandedKey;
    unsigned char *plainText;
    unsigned char state[nBlock * 4];
    int i, j;

    EnDeFlag = 1; //开始解密
    plainText = (unsigned char *)malloc(nBlock * 4);

    //将密文写入state
    for (i = 0; i < 4 * nBlock; i++)
    {
        for (j = 0; j < 4; j++)
        {
            *(state + i * 4 + j) = *(cipherText + j * nBlock + i);
        }
    }
    //inv密钥扩展
    expandedKey = KeyExpansion(cipherKey);
    //开始解密
    Controller(state, expandedKey);
    //加密结束，取出密文
    for (i = 0; i < 4 * nBlock; i++)
    {
        for (j = 0; j < 4; j++)
        {
            *(plainText + j * nKey + i) = *(state + i * 4 + j);
        }
    }
    return plainText;
}
unsigned char *KeyExpansion(unsigned char *mainKey)
{ //密钥扩展
    unsigned char *expandedKey;
    unsigned char tmp4Char[4];
    unsigned char tmp1Char;
    int i, j;

    expandedKey = (unsigned char *)malloc(nBlock * (nRound + 1) * 4);
    CpChars(expandedKey, mainKey, nKey * 4);
    for (i = nKey; i < nBlock * (nRound + 1); i++)
    {
        CpChars(tmp4Char, expandedKey + (i - 1) * 4, 4);
        if (i % nKey == 0)
        {
            //Rotl(tmp)
            tmp1Char = tmp4Char[0];
            for(j=0;j<3;j++)
                tmp4Char[j] = tmp4Char[j+1];
            tmp4Char[3] = tmp1Char;
            //subByte
            ByteSub(tmp4Char, 4);
            //temp
            tmp4Char[0] ^= RC[i/nKey-1];
            for (j = 1; j < 4; j++)
            {
                *(tmp4Char + j) ^= 0x00;
            }
        }
        for (j = 0; j < 4; j++)
        {
            *(expandedKey + i * 4 + j) = *(expandedKey + (i - nKey) * 4 + j) ^ *(tmp4Char + j);
        }
    }
    return expandedKey;
}
void Controller(unsigned char *state, unsigned char *expandedKey)
{ //加密函数,轮函数控制器
    int i;
    unsigned char *roundKey;
    if (EnDeFlag == 0)
    {
        roundKey = expandedKey;
        //初始轮密钥相加
        AddRoundKey(state, roundKey);
        //进行nRound-1次轮函数
        for (i = 1; i < (nRound - 1); i++)
        {
            roundKey += nBlock * 4;
            ByteSub(state, nBlock * 4);
            ShiftRow(state);
            MixColumn(state);
            AddRoundKey(state, roundKey);
        }
        //进行最后一轮变换
        roundKey += nBlock * 4;
        ByteSub(state, nBlock * 4);
        ShiftRow(state);
        AddRoundKey(state, roundKey);
        //加密结束，state是加密后的密文
    }
    else
    { //解密函数将加密函数倒过来执行
        roundKey = expandedKey + nBlock * nRound * 4;
        //最后一轮变换
        AddRoundKey(state, roundKey);
        ShiftRow(state);
        ByteSub(state, 4 * nBlock);
        //进行nRound-1轮变换
        for (i = nRound - 1; i > 0; i--)
        {
            roundKey -= nBlock * 4;
            AddRoundKey(state, roundKey);
            MixColumn(state);
            ShiftRow(state);
            ByteSub(state, 4 * nBlock);
        }
        //初始相加
        roundKey -= nBlock * 4;
        AddRoundKey(state, roundKey);
    }
}

void ByteSub(unsigned char *state, int words)
{
    int i;
    unsigned char tmp1Char;
    const unsigned char *theBox = EnDeFlag ? invsBox : sBox;

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
    int i, j;
    for (i = 1; i < 4; i++)
    {
        shiftNum = !EnDeFlag ? shiftTable[(nBlock / 2 - 2) * 3 + i-1] : (nBlock - shiftTable[(nBlock / 2 - 2) * 3 + i-1]);
        /*
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
        */

        for (j = 0; j < shiftNum; j++)
        {
            tmp1Char = state[i * nBlock];
            CpChars(&state[i * nBlock], &state[i * nBlock + 1], nBlock - 1);
            state[(i + 1) * nBlock - 1] = tmp1Char;
        }
    }
    //循环左移结束
}
void MixColumn(unsigned char *state)
{
    int i, j, k;
    unsigned char *tmp4Char;
    const unsigned char *theBox = EnDeFlag ? invColunmBox : colunmBox;

    tmp4Char = (unsigned char *)malloc(4);
    for (i = 0; i < nBlock; i++)
    {
        for (j = 0; j < 4; j++)
        {
            tmp4Char[j] = state[j * nBlock + i];
        }
        for (j = 0; j < 4; j++)
        {
            state[j * nBlock + i] = 0;
            for (k = 0; k < 4; k++)
            {
                state[j * nBlock + i] += theBox[j * 4 + k] * tmp4Char[k];
            }
        }
    }
}
void AddRoundKey(unsigned char *state, unsigned char *roundKey)
{
    int i;
    for (i = 0; i < 4 * nBlock; i++)
    {
        state[i] ^= roundKey[i];
    }
}
void CpChars(unsigned char *dest, unsigned char *origin, int length)
{ //复制字
    int i;
    for (i = 0; i < length; i++)
    {
        *(dest + i) = *(origin + i);
    }
}
