# !/usr/bin/python
# -*- coding: UTF-8 -*-

'''
ELGamal 算法
创建时间 Sun Dec  3 11:02:42 CST 2017
作者 张
注释：

'''

import algorithm
import sys
import random

class ELGamal(object):
    plainText = None
    cipherText = None
    primeNumberP = None
    primeRootAlpha = None
    secretKeyD = None
    publicKeyY = None
    __uint32l = 2**31
    __uint32h = 2**32 - 1

    def KeyInit(self):
        '''密钥产生'''
        count = 1
        while True:
            primeNumberP = random.randint(self.__uint32l,self.__uint32h)
            if algorithm.IsAPrimeNumber(primeNumberP) == True :
                if algorithm.IsAPrimeNumber((primeNumberP - 1) / 2) == True :
                    break
        while True:
            primeRootAlpha = random.randint(2 ** 20,primeNumberP)
            if (primeRootAlpha ** 2) % primeNumberP != 1:
                if(algorithm.MultiPowerModule(primeRootAlpha,(primeNumberP - 1) / 2,primeNumberP)) != 1:
                    break
        secretKeyD = random.randint(2**20,primeNumberP-1)
        publicKeyY = algorithm.MultiPowerModule(primeRootAlpha,secretKeyD,primeNumberP)
        self.primeNumberP = primeNumberP
        self.primeRootAlpha = primeRootAlpha
        self.secretKeyD = secretKeyD
        self.publicKeyY = publicKeyY
        return 0
    
    def EnCode(self):
        '''加密过程'''
        intK = random.randint(2**20,self.primeNumberP - 1)
        intU = algorithm.MultiPowerModule(self.publicKeyY,intK,self.primeNumberP)
        cipherC1 = algorithm.MultiPowerModule(self.primeRootAlpha,intK,self.primeNumberP)
        cipherC2 = (intU * self.plainText) % self.primeNumberP
        self.cipherText = (cipherC1 , cipherC2)
        return 0

    def DeCode(self):
        '''解密过程'''
        intV = algorithm.MultiPowerModule(self.cipherText[0],self.secretKeyD,self.primeNumberP)
        message = (self.cipherText[1] * algorithm.InverseOfNumber(intV,self.primeNumberP)) % self.primeNumberP
        self.plainText = message

def main():
    newELGamal = ELGamal()
    plainText = 845
    newELGamal.plainText = plainText
    newELGamal.KeyInit()
    newELGamal.EnCode()
    newELGamal.DeCode()
    print newELGamal.plainText

if __name__ == '__main__':
    main()