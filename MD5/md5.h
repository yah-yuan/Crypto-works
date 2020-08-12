typedef unsigned int uint32;
typedef unsigned long int uint64;
typedef char uint8;

uint32 INIT_A=0X67452301;
uint32 INIT_B=0XEFCDAB89;
uint32 INIT_C=0X98BADCFE;
uint32 INIT_D=0X10325476;

#define F(X,Y,Z) ((X&Y)|((~X)&Z))
#define G(X,Y,Z) ((X&Z)|(Y&(~Z)))
#define H(X,Y,Z) (X^Y^Z)
#define I(X,Y,Z) (Y^(X|(~Z)))

#define RLSHIFT(a,s)\
((a<<s)|(a>>(32-s)))

#define FF(a,b,c,d,M,s,ti) a=b+(RLSHIFT(a+F(b,c,d)+M+ti,s))
#define GG(a,b,c,d,M,s,ti) a=b+(RLSHIFT(a+G(b,c,d)+M+ti,s))
#define HH(a,b,c,d,M,s,ti) a=b+(RLSHIFT(a+H(b,c,d)+M+ti,s))
#define II(a,b,c,d,M,s,ti) a=b+(RLSHIFT(a+I(b,c,d)+M+ti,s))

int _hash_round(uint32* md5sum[], uint32 chunk[]){ 
    uint32 a = *md5sum[0];
    uint32 b = *md5sum[1];
    uint32 c = *md5sum[2];
    uint32 d = *md5sum[3];
    //第一轮
    FF(a,b,c,d,chunk[0],7,0xd76aa478);
    FF(d,a,b,c,chunk[1],12,0xe8c7b756);
    FF(c,d,a,b,chunk[2],17,0x242070db);
    FF(b,c,d,a,chunk[3],22,0xc1bdceee);
    FF(a,b,c,d,chunk[4],7,0xf57c0faf);
    FF(d,a,b,c,chunk[5],12,0x4787c62a);
    FF(c,d,a,b,chunk[6],17,0xa8304613);
    FF(b,c,d,a,chunk[7],22,0xfd469501);
    FF(a,b,c,d,chunk[8],7,0x698098d8);
    FF(d,a,b,c,chunk[9],12,0x8b44f7af);
    FF(c,d,a,b,chunk[10],17,0xffff5bb1);
    FF(b,c,d,a,chunk[11],22,0x895cd7be);
    FF(a,b,c,d,chunk[12],7,0x6b901122);
    FF(d,a,b,c,chunk[13],12,0xfd987193);
    FF(c,d,a,b,chunk[14],17,0xa679438e);
    FF(b,c,d,a,chunk[15],22,0x49b40821);

    // 第二轮
    GG(a,b,c,d,chunk[1],5,0xf61e2562);
    GG(d,a,b,c,chunk[6],9,0xc040b340);
    GG(c,d,a,b,chunk[11],14,0x265e5a51);
    GG(b,c,d,a,chunk[0],20,0xe9b6c7aa);
    GG(a,b,c,d,chunk[5],5,0xd62f105d);
    GG(d,a,b,c,chunk[10],9,0x02441453);
    GG(c,d,a,b,chunk[15],14,0xd8a1e681);
    GG(b,c,d,a,chunk[4],20,0xe7d3fbc8);
    GG(a,b,c,d,chunk[9],5,0x21e1cde6);
    GG(d,a,b,c,chunk[14],9,0xc33707d6);
    GG(c,d,a,b,chunk[3],14,0xf4d50d87);
    GG(b,c,d,a,chunk[8],20,0x455a14ed);
    GG(a,b,c,d,chunk[13],5,0xa9e3e905);
    GG(d,a,b,c,chunk[2],9,0xfcefa3f8);
    GG(c,d,a,b,chunk[7],14,0x676f02d9);
    GG(b,c,d,a,chunk[12],20,0x8d2a4c8a);

    // 第三轮
    HH(a,b,c,d,chunk[5],4,0xfffa3942);
    HH(d,a,b,c,chunk[8],11,0x8771f681);
    HH(c,d,a,b,chunk[11],16,0x6d9d6122);
    HH(b,c,d,a,chunk[14],23,0xfde5380c);
    HH(a,b,c,d,chunk[1],4,0xa4beea44);
    HH(d,a,b,c,chunk[4],11,0x4bdecfa9);
    HH(c,d,a,b,chunk[7],16,0xf6bb4b60);
    HH(b,c,d,a,chunk[10],23,0xbebfbc70);
    HH(a,b,c,d,chunk[13],4,0x289b7ec6);
    HH(d,a,b,c,chunk[0],11,0xeaa127fa);
    HH(c,d,a,b,chunk[3],16,0xd4ef3085);
    HH(b,c,d,a,chunk[6],23,0x04881d05);
    HH(a,b,c,d,chunk[9],4,0xd9d4d039);
    HH(d,a,b,c,chunk[12],11,0xe6db99e5);
    HH(c,d,a,b,chunk[15],16,0x1fa27cf8);
    HH(b,c,d,a,chunk[2],23,0xc4ac5665);

    // 第四轮
    II(a,b,c,d,chunk[0],6,0xf4292244);
    II(d,a,b,c,chunk[7],10,0x432aff97);
    II(c,d,a,b,chunk[14],15,0xab9423a7);
    II(b,c,d,a,chunk[5],21,0xfc93a039);
    II(a,b,c,d,chunk[12],6,0x655b59c3);
    II(d,a,b,c,chunk[3],10,0x8f0ccc92);
    II(c,d,a,b,chunk[10],15,0xffeff47d);
    II(b,c,d,a,chunk[1],21,0x85845dd1);
    II(a,b,c,d,chunk[8],6,0x6fa87e4f);
    II(d,a,b,c,chunk[15],10,0xfe2ce6e0);
    II(c,d,a,b,chunk[6],15,0xa3014314);
    II(b,c,d,a,chunk[13],21,0x4e0811a1);
    II(a,b,c,d,chunk[4],6,0xf7537e82);
    II(d,a,b,c,chunk[11],10,0xbd3af235);
    II(c,d,a,b,chunk[2],15,0x2ad7d2bb);
    II(b,c,d,a,chunk[9],21,0xeb86d391);

    *md5sum[0] += a;
    *md5sum[1] += b;
    *md5sum[2] += c;
    *md5sum[3] += d;
    return 0;
}