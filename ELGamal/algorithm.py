# !/usr/bin/python
# -*- coding: UTF-8 -*-

'''密码学常用数学运算'''

import binascii
import random

class _Bezout_data(object):
    _numberX = 1
    _numberY = 0

def MultiPowerModule(base, index, module):
    '''幂模除 res = base ^ index mod module'''
    lengthOfIndex = LenOfNum_bin(index)
    res = 1
    multiBase = base
    for x in range(lengthOfIndex):
        multiedNum = (index >> x) & 1
        if multiedNum == 1:
            res = (res * (multiBase)) % module
        multiBase = (multiBase**2) % module
    return res

def Bezout(data,number1, number2):
    '''递归求解欧几里得扩展算法'''
    if number2 == 0:
        data._numberX = 1
        data._numberY = 0
        return number1

    res = Bezout(data,number2, number1 % number2)
    temp = data._numberX
    data._numberX = data._numberY
    data._numberY = temp - number1 / number2 * data._numberY
    return res

def Miller_rabin(n, k):
    '''miller_rabin算法'''
    if n < 2:
            return False
    d = n - 1
    r = 0
    while not (d & 1):
            r += 1
            d >>= 1
    for _ in range(k):
            a = random.randint(120,2**10)        #随机数
            x = MultiPowerModule(a, d, n)
            if x == 1 or x == n - 1:
                    continue
            for _ in range(r - 1):
                    x = MultiPowerModule(x, 2, n)
            if x == 1:
                    return False
            if x == n - 1:
                    break
    else:
            return False
    return True 

def LenOfNum_bin(num):
    '''计算整数的二进制格式长度'''
    res = len(bin(num)) - 2
    return res

def IsAPrimeNumber(number,times = 10):
    '''返回是否是个素数'''
    '''参数：被测试数，测试次数'''
    for _ in range(times):
        if MultiPowerModule(random.randint(2**10,2**20),number - 1,number) != 1 :
            return False
    return True

def InverseOfNumber(number,module):
    '''返回一个number在module下的逆'''
    data = _Bezout_data()
    Bezout(data,module,number)
    return data._numberY