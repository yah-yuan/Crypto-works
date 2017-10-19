/*
密码学实验：古典密码之置换密码
日期：10/6/2017
作者：张
注释：
a.算法使用的密码列数为自定义的常数SIZEOF_COW
b.demo字符串大小仅为自定义常数SIZEOF_BUFF
c.填充符号为@
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZEOF_COW 6
#define SIZEOF_BUFF 48

char *Encrypto(char *inputText, int length) ;
char *Decrypto(char *cryptoedText, int length) ;

int main()
{
    char *inputText;
    char *cryptoedText;
    char *plainText;
    int length;
    inputText = (char *)malloc(SIZEOF_BUFF * sizeof(char));

    //开始加密
    printf("pls input the plain text with no blanks\n");
    scanf("%s", inputText);
    length = strlen(inputText);
    cryptoedText = Encrypto(inputText, length);
    printf("what encryptoed Text is\n%s\n", cryptoedText);

    //开始解密
    printf("pls input the cryptoedTexted text with no blanks\n");
    scanf("%s", inputText);
    length = strlen(inputText);
    plainText = Decrypto(inputText, length);
    printf("what deCryptoed text is\n%s\n", plainText);

    return 0;
}

char *Encrypto(char *inputText, int length)
{ //加密算法
    int row, i, j, count;
    char *cryptoedText;
    row = length / SIZEOF_COW;
    cryptoedText = (char *)malloc((length % SIZEOF_COW) ? (row * SIZEOF_COW + SIZEOF_COW) : (row * SIZEOF_COW) * sizeof(char)); //设置密文大小
    *(cryptoedText + ((length % SIZEOF_COW) ? (row * SIZEOF_COW + SIZEOF_COW) : (row * SIZEOF_COW) + 1)) = '\000';              //设置结束符，防止打印奇怪的乱码
    for (i = 0; i < SIZEOF_COW; i++)
    {
        for (j = 0; j < (row + 1); j++)
        {
            if ((j == row) && (!(length % SIZEOF_COW))) //特殊情况：明文可以被SIZEOF_COW整除，这时不加填充符@
                break;
            if ((j * SIZEOF_COW + i) < length)
                cryptoedText[count] = inputText[j * SIZEOF_COW + i];
            else
            {
                cryptoedText[count] = '@'; //填充@
                if (i == SIZEOF_COW)
                    break;
            }
            count++;
        }
    }
    return cryptoedText;
}

char *Decrypto(char *cryptoedText, int length)
{ //解密算法，把加密算法倒过来即可
    char *plainText;
    int i, j, count, row;
    row = length / SIZEOF_COW;
    plainText = (char *)malloc(length * sizeof(char));
    *(plainText + length + 1) = '\000';
    for (i = 0; i < SIZEOF_COW; i++)
    {
        for (j = 0; j < row; j++)
        {
            if (cryptoedText[count] == '@')
                ;
            else
            {
                plainText[j * SIZEOF_COW + i] = cryptoedText[count];
            }
            count++;
        }
    }
    return plainText;
}