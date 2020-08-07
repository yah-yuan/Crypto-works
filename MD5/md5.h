typedef unsigned long int uint32;
typedef unsigned long long int uint64;

uint32 INIT_A=0X67452301;
uint32 INIT_B=0XEFCDAB89;
uint32 INIT_C=0X98BADCFE;
uint32 INIT_D=0X10325476;

#define F(X,Y,Z) (X&Y)|((~X)&Z)
#define G(X,Y,Z) (X&Z)|(Y&(~Z))
#define H(X,Y,Z) X^Y^Z
#define I(X,Y,Z) Y^(X|(~Z))

#define RLSHIFT(a,s)\
((a<<s)|(a>>(32-s)))

#define FF(a,b,c,d,M,s,ti) b+(RLSHIFT(a+F(b,c,d)+M+ti,s))
#define GG(a,b,c,d,M,s,ti) b+(RLSHIFT(a+G(b,c,d)+M+ti,s))
#define HH(a,b,c,d,M,s,ti) b+(RLSHIFT(a+H(b,c,d)+M+ti,s))
#define II(a,b,c,d,M,s,ti) b+(RLSHIFT(a+I(b,c,d)+M+ti,s))