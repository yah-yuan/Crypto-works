# !/usr/bin/python
# -*- coding: UTF-8 -*-

# python实现简单的RSA加密运算
# 暂时不考虑速度问题^_^

import random
import os
import sys
import shelve
import binascii


class _Rsa(object):
    uint1024_l = 2**1023
    uint1024_h = 2**1024 - 1
    _numberX = 1
    _numberY = 0
    enKeyPath = './enRSA.key'
    deKeyPath = './deRSA.key'
    cipherTextPath = './cipherText'
    plainTextPath = './plainText'

    def MultiPowerModule(self, base, index, module):
        lengthOfIndex = len(bin(index)) - 2
        res = 1
        multiBase = base
        for x in range(lengthOfIndex):
            multiedNum = (index >> x) & 1
            if multiedNum == 1:
                res = (res * (multiBase)) % module
            multiBase = (multiBase**2) % module
        return res

    # 递归求解欧几里得扩展算法
    def Bezout(self, number1, number2):
        if number2 == 0:
            self._numberX = 1
            self._numberY = 0
            return number1

        res = self.Bezout(number2, number1 % number2)
        temp = self._numberX
        self._numberX = self._numberY
        self._numberY = temp - number1 / number2 * self._numberY
        return res
    # miller_rabin算法
    def Miller_rabin(self, n, k):
        '''测试二,用miller_rabin算法对n进行k次检测'''
        if n < 2:
                return False
        d = n - 1
        r = 0
        while not (d & 1):
                r += 1
                d >>= 1
        for _ in range(k):
                a = random.randint(120,2**10)        #随机数
                x = self.MultiPowerModule(a, d, n)
                if x == 1 or x == n - 1:
                        continue
                for _ in range(r - 1):
                        x = self.MultiPowerModule(x, 2, n)
                if x == 1:
                        return False
                if x == n - 1:
                        break
        else:
                return False
        return True 

class Rsa_Cryption(_Rsa):
    primeNumberP = 0
    primeNumberQ = 0
    phiN = 0
    keyE = 0
    keyD = 0
    publicNumberN = 0
    cipherText = 0
    plainText = 0

    def PrimeNumberInit(self):  #产生两个素数、N、phiN
        # 产生随机素数P
        testNumber = random.randint(self.uint1024_l, self.uint1024_h)
        while True:
            if self.MultiPowerModule(
                    random.randint(2**10, 2**20), (testNumber - 1),
                    testNumber) == 1:
            # if self.Miller_rabin(testNumber,10):
                primeNumberP = testNumber
                break
            testNumber += 1
        self.primeNumberP = primeNumberP

        # 产生随机素数Q
        testNumber = random.randint(self.uint1024_l, self.uint1024_h)
        while True:
            if self.MultiPowerModule(
                    random.randint(2**10, 2**20), (testNumber - 1),
                    testNumber) == 1:
            # if self.Miller_rabin(testNumber,10):
                if testNumber != self.primeNumberP:
                    primeNumberQ = testNumber
                    break
            testNumber += 1
        self.primeNumberQ = primeNumberQ

        self.publicNumberN = self.primeNumberP * self.primeNumberQ
        self.phiN = (self.primeNumberP - 1) * (self.primeNumberQ - 1)

    def KeyInitial(self):  #生成密钥
        #产生密钥E
        keyE = 2 ** 16 -1
        while True:
            tmpA = self.phiN
            tmpB = keyE
            while tmpB:
                tmp = tmpA
                tmpA = tmpB
                tmpB = tmp % tmpA
            if tmpA == 1:
                break
            else:
                keyE += 1
        self.keyE = keyE

        # sys.setrecursionlimit(1000000) #设置递归深度
        self.Bezout(self.phiN, self.keyE)
        self.keyD = self._numberY
        if (self.keyD * self.keyE) % self.phiN == 1:
            print 'ok,,length of keyD is :', (len(bin(self.keyD)) - 2)

    def SaveKeyFile(self):
        #公钥
        try:
            os.remove(self.enKeyPath)
        except NameError:
            pass
        keyFile = shelve.open(self.enKeyPath)
        keyFile['keyE'] = self.keyE
        keyFile['publicNumberN'] = self.publicNumberN
        keyFile.close()
        #私钥
        try:
            os.remove(self.deKeyPath)
        except NameError:
            pass
        keyFile = shelve.open(self.deKeyPath)
        keyFile['keyD'] = self.keyD
        keyFile['publicNumberN'] = self.publicNumberN
        keyFile.close()
        pass

    def fileEnCryption(self,plainTextPath = './plainText'):
        try:
            file = open(self.plainTextPath,'r')
            self.plainText = file.read(-1)
            file.close()
        except IOError,m:
            print "IoError",m

        file = shelve.open(self.enKeyPath)
        self.keyE = file['keyE']
        self.publicNumberN = file['publicNumberN']
        file.close()

        # if (self.plainText)&self.keyE&self.publicNumberN :
        #     self.cipherText = self.MultiPowerModule(self.plainText,self.keyE,self.publicNumberN)
        # else :
        #     print 'messege error'
        #     return 0
        self.plainText = int(binascii.b2a_hex(self.plainText),16)
        self.cipherText = self.MultiPowerModule(self.plainText,self.keyE,self.publicNumberN)

        try:
            os.remove(cipherTextPath)
        except NameError:
            pass
        cipherText = open(self.cipherTextPath,'wb+',-1)
        cipherText.write(str(self.cipherText))
        cipherText.close()
        pass

    def fileDeCryption(self,cipherTextPath = './cipherText'):
        try:
            file = open(cipherTextPath,'rb')
            self.cipherText = file.read()
            file.close()
        except IOError,m:
            print "IoError",m

        file = shelve.open(self.deKeyPath)
        self.keyD = file['keyD']
        self.publicNumberN = file['publicNumberN']
        file.close()

        self.cipherText = int(binascii.b2a_hex(self.cipherText),16)
        plainText = self.MultiPowerModule(self.cipherText,self.keyD,self.publicNumberN)
        plainText = hex(self.plainText)
        plainText = plainText[2:-1]
        plainText = binascii.a2b_hex(plainText)
        self.plainText = plainText
        return self.plainText
        pass

def main():
    rsa = Rsa_Cryption()
    # rsa.PrimeNumberInit()
    # rsa.KeyInitial()
    # rsa.SaveKeyFile()
    
    rsa.fileEnCryption()
    print "in encrypto work..."
    plainText = rsa.fileDeCryption()
    print "in decrypto work..."
    print rsa.plainText
    pass


if __name__ == '__main__':
    main()
    # print 'primeNumberP = ',newRsa.primeNumberP
    # print 'primeNumberQ = ',newRsa.primeNumberQ
    # print 'phiN = ',newRsa.phiN
    # print 'keyE = ',newRsa.keyE
    # print 'keyD = ',newRsa.keyD
    # print 'publicNumberN = ',newRsa.publicNumberN