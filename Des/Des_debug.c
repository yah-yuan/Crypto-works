/*
Des加密解密算法简单实现
创建时间：Mon, 09 Oct 2017 19:08:30 +0800
作者：ZTY
注释：
    a.DEBUG模式下可以自动生成密钥，输入密文
    b.考虑到实用性在RELEASE中可以将明文、密文、密钥用文件模式存储读取
    c.DEBUG MODE明文缓冲区可以使用SIZEOF_PLAIN_TEXT更改
*/

#include "Des_headers.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define SIZEOF_PLAIN_TEXT 64

unsigned char *EnCryptoDes(char *plainText, char *secretKey, int numOf64); //Des加密
unsigned char *DeCryptoDes(char *cipherText, char *secretKey, int numOf64);
//以下实现Des加密
unsigned char *EnKeySon(unsigned char *mainKey); //产生子Key
void IniPermutate(unsigned char *plainText);     //初始置换                                         //将64bit信息左右替换
void EnFFunc(unsigned char *LText, unsigned char *RText, unsigned char *sonKey6, int count); //沙盒加密
void DeIniPeimutate(unsigned char *cipherText);                                              //逆初始IP置换
void Permutate(unsigned char *originText, unsigned char *destText, int permutation[], int times); //矩阵置换
void CpChars(unsigned char *dest, unsigned char *origin, int words);
//Des解密
unsigned long get_file_size(const char *path);

int main(int argc, char *argv[])
{
    unsigned char *plainText;
    unsigned char *cipherText;
    unsigned char *secretKey;
    int i;
    int size;

    plainText = (char *)malloc(SIZEOF_PLAIN_TEXT);
    secretKey = (char *)malloc(8);
    printf("××××× DES加密解密，DEBUG MODE ×××××\n");
    printf("请输入要加密的明文\n");
    scanf("%[^\n]", plainText);
    printf("自动生成的密钥\n");
    for (i = 0; i < 8; i++)
    {
        *(secretKey + i) = (unsigned char)(rand() % 0xff);
        printf("0x%x ", *(secretKey + i));
    }
    printf("\n");
    printf("\n");

    size = strlen(plainText) / 8;
    size += (strlen(plainText)%8!=0)?1:0;
    printf("明文内容\n");
    for (i = 0; i < 8*size; i++)
    {
        printf("0x%x ", (unsigned char)*(plainText + i));
    }
    printf("size=%d\n",size);
    printf("\n");
    printf("\n");

    cipherText = EnCryptoDes(plainText, secretKey, size);
    printf("加密后的密文\n");
    for (i = 0; i < 8*size; i++)
    {
        printf("0x%x ", (unsigned char)*(cipherText + i));
    }
    printf("\n");
    printf("\n");

    plainText = DeCryptoDes(cipherText, secretKey, size);
    printf("解密后的明文\n");
    for (i = 0; i < 8*size; i++)
        printf("0x%x ", (unsigned char)*(plainText + i));
    printf("\n");
    printf("\n");

    printf("字符串\n");
    for (i = 0; i < 8*size; i++)
        printf("%c", (unsigned char)*(plainText + i));
    printf("\n");
    return 0;
}

unsigned char *EnCryptoDes(char *plainText, char *secretKey, int numOf64)
{
    //Des加密
    unsigned char *sonKey8_48;
    unsigned char *cryptoText;
    //int numOf64;
    int count;

    //预处理
    cryptoText = plainText;
    sonKey8_48 = EnKeySon(secretKey);

    //加密过程
    for (count = 0; count < numOf64; count++)
    {
        IniPermutate(cryptoText + count * 8);
        EnFFunc(cryptoText + count * 8, cryptoText + count * 8 + 4, sonKey8_48, 0);
        DeIniPeimutate(cryptoText + count * 8);
    }
    return cryptoText;
}

unsigned char *DeCryptoDes(char *cipherText, char *secretKey, int numOf64)
{
    //Des解密
    unsigned char *sonKey8_48;
    unsigned char *cryptoText;
    unsigned char *deSonKey8_48 = (unsigned char *)malloc(16 * 6);
    //int numOf64;
    int count, i;

    //预处理
    cryptoText = cipherText;
    sonKey8_48 = EnKeySon(secretKey);

    for (count = 0; count < 16; count++)
    {
        for (i = 0; i < 6; i++)
        {
            *(deSonKey8_48 + (15 - count) * 6 + i) = *(sonKey8_48 + count * 6 + i);
        }
    }

    //解密过程
    for (count = 0; count < numOf64; count++)
    {
        IniPermutate(cryptoText + count * 8);
        EnFFunc(cryptoText + count * 8, cryptoText + count * 8 + 4, deSonKey8_48, 0);
        DeIniPeimutate(cryptoText + count * 8);
    }
    return cryptoText;
}
unsigned char *EnKeySon(unsigned char *mainKey)
{
    //产生子Key
    unsigned char *sonKey8_48;
    unsigned char *tmp7Char, *tmp4Char_1, *tmp4Char_2, *tmp1Char_1, *tmp1Char_2, *tmp6Char;
    int i, j;
    int place, row, col;
    sonKey8_48 = (unsigned char *)malloc(6 * 16); //16个48位的子密钥
    tmp7Char = (unsigned char *)malloc(7);
    tmp4Char_1 = (unsigned char *)malloc(4);
    tmp4Char_2 = (unsigned char *)malloc(4);
    tmp1Char_1 = (unsigned char *)malloc(1);
    tmp1Char_2 = (unsigned char *)malloc(1);
    tmp6Char = (unsigned char *)malloc(6);

    Permutate(mainKey, tmp7Char, initial_key_permutaion, 7); //tmp7Char 保存了56位密钥

    for (j = 0; j < 16; j++)
    {
//循环左移+置换选择2 16 times
#if (DEBUG_ENSONKEY)
        unsigned char *oldTmp4Char_1 = (unsigned char *)malloc(4);
        unsigned char *oldTmp4Char_2 = (unsigned char *)malloc(4);
        CpChars(oldTmp4Char_1, tmp4Char_1, 4);
        CpChars(oldTmp4Char_2, tmp4Char_2, 4);
#endif
        switch (key_shift_sizes[j])
        {
        case 1:
        {
            *(tmp1Char_1) = (*tmp7Char & binary_place[0]) >> 3; //0000x000b
            *(tmp1Char_2) = (*(tmp7Char + 3) & binary_place[4]) >> 3;
            for (i = 0; i < 4; i++)
            {
                *(tmp4Char_1 + i) = (i == 3) ? (*(tmp7Char + i) & 0xf0) : *(tmp7Char + i);
            }
            for (i = 0; i < 4; i++)
            {
                *(tmp4Char_1 + i) <<= 1;
                *(tmp4Char_1 + i) |= (i != 3) ? (*(tmp4Char_1 + i + 1) >> 7) : *(tmp1Char_1);
            }

            for (i = 0; i < 4; i++)
            {
                *(tmp4Char_2 + i) = (i == 0) ? *(tmp7Char + 3 + i) & 0x0f : *(tmp7Char + 3 + i);
            }
            for (i = 0; i < 4; i++)
            {
                *(tmp4Char_2 + i) <<= 1;
                *(tmp4Char_2 + i) |= (i != 3) ? (*(tmp4Char_2 + i + 1) >> 7) : *(tmp1Char_2);
            }
        }
        break;

        case 2:
        {
            *(tmp1Char_1) = (*(tmp7Char)&0xc0) >> 2; //0000x000b
            *(tmp1Char_2) = (*(tmp7Char + 3) & 0x0c) >> 2;
            for (i = 0; i < 4; i++)
            {
                *(tmp4Char_1 + i) = (i == 3) ? *(tmp7Char + i) & 0xf0 : *(tmp7Char + i);
            }
            for (i = 0; i < 4; i++)
            {
                *(tmp4Char_1 + i) <<= 2;
                *(tmp4Char_1 + i) |= (i != 3) ? (*(tmp4Char_1 + i + 1) >> 6) : *(tmp1Char_1);
            }

            for (i = 0; i < 4; i++)
            {
                *(tmp4Char_2 + i) = (i == 0) ? *(tmp7Char + 3 + i) & 0x0f : *(tmp7Char + i);
            }
            for (i = 0; i < 4; i++)
            {
                *(tmp4Char_2 + i) <<= 2;
                *(tmp4Char_2 + i) |= (i != 3) ? (*(tmp4Char_2 + i + 1) >> 6) : *(tmp1Char_2);
            }
        }
        }
        /*
        sitch 后，tmp4Char_1存左28位，格式为0 xx xx xx x0 h ；
        tmp4Char_2存右28位，格式为0 0x xx xx xx h ；
        */

        //将循环左移后的结果拼起来
        tmp4Char_1 = (unsigned char *)tmp4Char_1;
        tmp4Char_2 = (unsigned char *)tmp4Char_2;
        *(tmp1Char_1) = *(tmp4Char_1 + 3) | *(tmp4Char_2);

        *(tmp7Char) = *(tmp4Char_1);
        *(tmp7Char + 1) = *(tmp4Char_1 + 1);
        *(tmp7Char + 2) = *(tmp4Char_1 + 2);
        *(tmp7Char + 3) = *(tmp1Char_1);
        *(tmp7Char + 4) = *(tmp4Char_2 + 1);
        *(tmp7Char + 5) = *(tmp4Char_2 + 2);
        *(tmp7Char + 6) = *(tmp4Char_2 + 3);

        //置换选择2
        Permutate(tmp7Char, tmp6Char, sub_key_permutation, 6);
        //成功生成子key
        CpChars((sonKey8_48 + j * 6), tmp6Char, 6);
    }
    return sonKey8_48;
}

void IniPermutate(unsigned char *plainText)
{
    //初始置换
    unsigned char *tmpText = (unsigned char *)malloc(8);
    Permutate(plainText, tmpText, initial_message_permutation, 8);
    CpChars(plainText, tmpText, 8);
}
void EnFFunc(unsigned char *LText, unsigned char *RText, unsigned char *sonKey6, int count)
{
    //S盒加密
    int i, j;
    unsigned char *tmp1Char_1, *tmp1Char_2, *tmp6Char, *tmp4Char, *tmp4Char_p;

    tmp1Char_1 = (unsigned char *)malloc(1);
    tmp1Char_2 = (unsigned char *)malloc(1);
    tmp6Char = (unsigned char *)malloc(6);
    tmp4Char = (unsigned char *)malloc(4);
    tmp4Char_p = (unsigned char *)malloc(4);

    //**************************f函数*******************************//
    //对右侧做f函数，存在tmp4Char中
    //置换运算E，异或
    Permutate(RText, tmp6Char, message_expansion, 6);
    for (i = 0; i < 6; i++)
    {
        *(tmp6Char + i) ^= *(sonKey6 + i);
    }
    //代替函数组S
    //S1
    *(tmp1Char_1) = (*(tmp6Char)&0x78) >> 3;
    *(tmp1Char_2) = ((*(tmp6Char)&0x80) >> 6) | ((*(tmp6Char)&0x04) >> 2);
    *(tmp1Char_1) = (char)S1[*(tmp1Char_2)*16 + *(tmp1Char_1)];
    *(tmp4Char) |= *(tmp1Char_1) << 4;

    //S2
    *(tmp1Char_1) = ((*(tmp6Char)&0x01) << 3) | (((*(tmp6Char + 1)) & 0xe0) >> 5);
    *(tmp1Char_2) = (*(tmp6Char)&0x02) | (((*(tmp6Char + 1)) & 0x10) >> 4);
    *(tmp1Char_1) = (char)S2[*(tmp1Char_2)*16 + *(tmp1Char_1)];
    *(tmp4Char) |= *(tmp1Char_1);

    //S3
    *(tmp1Char_1) = ((*(tmp6Char + 1) & 0x07) << 1) | ((*(tmp6Char + 2) & 0x80) >> 7);
    *(tmp1Char_2) = ((*(tmp6Char + 1) & 0x08) >> 2) | (*(tmp6Char + 2) & 0x40) >> 6;
    *(tmp1Char_1) = (char)S3[*(tmp1Char_2)*16 + *(tmp1Char_1)];
    *(tmp4Char + 1) |= *(tmp1Char_1) << 4;

    //S4
    *(tmp1Char_1) = (*(tmp6Char + 2) & 0x1e) >> 1;
    *(tmp1Char_2) = ((*(tmp6Char + 2) & 0x20) >> 4) | (*(tmp6Char + 2) & 0x01);
    *(tmp1Char_1) = (char)S4[*(tmp1Char_2)*16 + *(tmp1Char_1)];
    *(tmp4Char + 1) |= *(tmp1Char_1);

    //S5
    *(tmp1Char_1) = (*(tmp6Char + 3) & 0x78) >> 3;
    *(tmp1Char_2) = ((*(tmp6Char + 3) & 0x80) >> 6) | ((*(tmp6Char + 3) & 0x04) >> 2);
    *(tmp1Char_1) = (char)S5[*(tmp1Char_2)*16 + *(tmp1Char_1)];
    *(tmp4Char + 2) |= *(tmp1Char_1) << 4;

    //S6
    *(tmp1Char_1) = ((*(tmp6Char + 3) & 0x01) << 3) | (((*(tmp6Char + 4)) & 0xe0) >> 5);
    *(tmp1Char_2) = (*(tmp6Char + 3) & 0x02) | (((*(tmp6Char + 4)) & 0x10) >> 4);
    *(tmp1Char_1) = (char)S6[*(tmp1Char_2)*16 + *(tmp1Char_1)];
    *(tmp4Char + 2) |= *(tmp1Char_1);

    //S7
    *(tmp1Char_1) = ((*(tmp6Char + 4) & 0x07) << 1) | ((*(tmp6Char + 5) & 0x80) >> 7);
    *(tmp1Char_2) = ((*(tmp6Char + 4) & 0x08) >> 2) | (*(tmp6Char + 5) & 0x40) >> 6;
    *(tmp1Char_1) = (char)S7[*(tmp1Char_2)*16 + *(tmp1Char_1)];
    *(tmp4Char + 3) |= *(tmp1Char_1) << 4;

    //S8
    *(tmp1Char_1) = (*(tmp6Char + 5) & 0x1e) >> 1;
    *(tmp1Char_2) = ((*(tmp6Char + 5) & 0x20) >> 4) | (*(tmp6Char + 5) & 0x01);
    *(tmp1Char_1) = (char)S8[*(tmp1Char_2)*16 + *(tmp1Char_1)];
    *(tmp4Char + 3) |= *(tmp1Char_1);

    //置换运算P
    CpChars(tmp4Char_p, tmp4Char, 4);
    Permutate(tmp4Char_p, tmp4Char, right_sub_message_permutation, 4);
    //***************************f函数结束********************************

    //左右替换
    for (i = 0; i < 4; i++)
    {
        *(tmp4Char + i) ^= *(LText + i);
    }
    if (count != 15)
    {
        CpChars(LText, RText, 4);
        CpChars(RText, tmp4Char, 4);
        EnFFunc(LText, RText, sonKey6 + 6, count + 1);
    }
    else
    {
        CpChars(LText, tmp4Char, 4);
    }
}

void DeIniPeimutate(unsigned char *cipherText)
{
    //逆初始IP置换
    unsigned char *tmpText = (unsigned char *)malloc(8);
    Permutate(cipherText, tmpText, final_message_permutation, 8);
    CpChars(cipherText, tmpText, 8);
}
void Permutate(unsigned char *originText, unsigned char *destText, int permutation[], int times)
{
    //置换运算,要求提前malloc好内存
    int i;
    int place, row, col;
    unsigned char *tmp = (unsigned char *)malloc(1);

    for (i = 0; i < times; i++)
    {
        *(destText + i) = (char)0;
    }
    for (i = 0; i < 8 * times; i++)
    {
        place = permutation[i] - 1;
        row = place / 8;
        col = place % 8;
        *(tmp) = *(originText + row);
        *(tmp) = ((col - i % 8) >= 0) ? (*(tmp) << (col - i % 8)) : (*(tmp) >> (i % 8 - col));
        *(tmp) &= binary_place[i % 8];
        *(destText + i / 8) |= *(tmp);
    }
}

void CpChars(unsigned char *dest, unsigned char *origin, int words)
{
    int i;
    for (i = 0; i < words; i++)
    {
        *(dest + i) = *(origin + i);
    }
}