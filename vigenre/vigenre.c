/*
密码学实验：古典密码之Vigenre加密
日期：10/6/2017
作者：张
注释：
a.算法使用了比较耗费内存的查表方式，可以不建立vigenreSqure实表实现算法
b.可定义明文及密钥的长度
c.作者很懒，只能输入大写字母
*/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZEOF_TEXTBUFF 64
#define SIZEOF_CRYPTOBUFF 16

char vigenreSqure[26][26] = {
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ", "BCDEFGHIJKLMNOPQRSTUVWXYZA", "CDEFGHIJKLMNOPQRSTUVWXYZAB", "DEFGHIJKLMNOPQRSTUVWXYZABC",
    "EFGHIJKLMNOPQRSTUVWXYZABCD", "FGHIJKLMNOPQRSTUVWXYZABCDE", "GHIJKLMNOPQRSTUVWXYZABCDEF", "HIJKLMNOPQRSTUVWXYZABCDEFG",
    "IJKLMNOPQRSTUVWXYZABCDEFGH", "JKLMNOPQRSTUVWXYZABCDEFGHI", "KLMNOPQRSTUVWXYZABCDEFGHIJ", "LMNOPQRSTUVWXYZABCDEFGHIJK",
    "MNOPQRSTUVWXYZABCDEFGHIJKL", "NOPQRSTUVWXYZABCDEFGHIJKLM", "OPQRSTUVWXYZABCDEFGHIJKLMN", "PQRSTUVWXYZABCDEFGHIJKLMNO",
    "QRSTUVWXYZABCDEFGHIJKLMNOP", "RSTUVWXYZABCDEFGHIJKLMNOPQ", "STUVWXYZABCDEFGHIJKLMNOPQR", "TUVWXYZABCDEFGHIJKLMNOPQRS",
    "UVWXYZABCDEFGHIJKLMNOPQRST", "VWXYZABCDEFGHIJKLMNOPQRSTU", "WXYZABCDEFGHIJKLMNOPQRSTUV", "XYZABCDEFGHIJKLMNOPQRSTUVW",
    "YZABCDEFGHIJKLMNOPQRSTUVWX", "ZABCDEFGHIJKLMNOPQRSTUVWXY"}; //vigenre方阵
char charactorSqure[26] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";          //字母表

char *Encrypto(char *plainText, char *crypto, int length_plainText, int length_crypto);
char *Decrypto(char *cryptoedText, char *crypto, int length_cryptoedText, int lenght_crypto);

int main()
{
    char *inputText;
    char *crypto;
    char *outputText;
    int length_inputText, length_crypto;

    inputText = (char *)malloc(SIZEOF_TEXTBUFF * sizeof(char));
    crypto = (char *)malloc(SIZEOF_CRYPTOBUFF * sizeof(char));
    //加密提示
    printf("pls input plain text\n");
    scanf("%s", inputText);
    printf("pls input crypto words\n");
    scanf("%s", crypto);
    length_inputText = strlen(inputText);
    length_crypto = strlen(crypto);
    //开始加密
    outputText = Encrypto(inputText, crypto, length_inputText, length_crypto);
    printf("%s\n", outputText);
    //解密提示
    printf("pls input cryptoed text\n");
    scanf("%s", inputText);
    printf("pls input crypto\n");
    scanf("%s", crypto);
    //开始解密
    length_inputText = strlen(inputText);
    length_crypto = strlen(crypto);
    outputText = Decrypto(inputText, crypto, length_inputText, length_crypto);
    printf("%s\n", outputText);
}

char *Encrypto(char *plainText, char *crypto, int length_plainText, int length_crypto)
{   //加密算法
    int i, count;
    char *cryptoedText = (char *)malloc(SIZEOF_TEXTBUFF * sizeof(char));
    for (;;)
    {
        for (i = 0; i < length_crypto; i++, count++)
        {
            if (count < length_plainText)
            {
                cryptoedText[count] = vigenreSqure[plainText[count] - 'A'][crypto[i] - 'A']; //查Vigenre表
            }
            else
                return cryptoedText;
        }
    }
}

char *Decrypto(char *cryptoedText, char *crypto, int length_cryptoedText, int length_crypto)
{   //解密算法，与加密相反
    int i, count;
    char *plainText = (char *)malloc(SIZEOF_TEXTBUFF * sizeof(char));
    for (;;)
    {
        for (i = 0; i < length_crypto; i++, count++)
        {
            if (count < length_cryptoedText)
            {
                plainText[count] = charactorSqure[(cryptoedText[count] + 26 - crypto[i]) % 26]; //查字母表
            }
            else
                return plainText;
        }
    }
}