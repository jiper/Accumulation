# -*- coding: utf-8 -*-
"""
Created on Thu Aug 22 10:01:42 2019

@author: sky
"""

##########################################
#需要设置的参数
fileIn = "SK9042BBCFW_V_2_1_5.bin"
fileOut = "Update.bin"
##########################################

crcNum  = 0 
def CrcCountSingle(num):
    global crcNum
    iterlist = [0x80,0x40,0x20,0x10,0x8,0x4,0x2,0x1]
    for i in iterlist:
        crcNum = crcNum *2 
        if((crcNum & 0x10000) != 0):
            crcNum = crcNum ^ 0x11021
        if ((num&i) != 0):
            crcNum = crcNum ^ (0x10000 ^ 0x11021)
            

#读取原始bin文件
with open(fileIn,"rb") as InputFile:
    dataArray = InputFile.read()

#print(dataBin)    

for element in  dataArray:
#    print (element)
    CrcCountSingle(element)
#print(hex(crcNum))
ContedLen = len(dataArray)
#print (binLen)
            
UpdateData_1 = (b'\x23\x00\x00\x00\x00\x00\x00\x09\x06')
UpdateData_2 =  ContedLen.to_bytes(2,byteorder='big')
UpdateData_3 = CrcRum.to_bytes(2,byteorder='big')
UpdateData_4 = dataArray
UpdateData_5 = (b'\x04')

updateDate = UpdateData_1 + UpdateData_2 + UpdateData_3 +UpdateData_4 +UpdateData_5

with open(fileOut,"wb") as OutFile:
    OutFile.write(updateDate)

        
            
    