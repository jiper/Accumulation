# -*- coding: utf-8 -*-
"""
Created on Thu Aug  8 15:09:55 2019

@author: sky
"""

sourceFile = "SK9042BBCFW_V_2_2_0.bin"
desFile = "SK9042BBCFW_V_2_New.bin"
b2 = b'\x23\x00\x00\x00\x00\x00\x00\x09\x06\xCD\xD6\x4A\x04'
#b2[0]=0x23
with open(sourceFile,"rb") as fIn:
    data = fIn.read()

with open(desFile,"wb") as fOut:
    fOut.write(b2)
    fOut.write(data)
    fOut.write(b'\x04')