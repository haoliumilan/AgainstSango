//
//  ZBase64.cpp
//  AgainstWar
//
//  Created by 王 海桅 on 12-12-11.
//  Copyright (c) 2012年 OneClick Co.,Ltd. All rights reserved.
//


#include "ZBase64.h"


ZBase64::ZBase64()
{
    //Base64编码表
    this->m_Base64_Table="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
}
string ZBase64::EncodeBase64(const string strSource)
{
    /*
     * 以下是操作二进制数时用到的
     * 11111100  0xFC
     * 11000000  0x3
     * 11110000  0xF0
     * 00001111  0xF
     * 11000000  0xC0
     * 00111111  0x3F
     */
    string strEncode;
    char cTemp[4];
    //行长,MIME规定Base64编码行长为76字节
    int LineLength=0;
    
    for(int i=0;i<strSource.size();i+=3)
    {
        memset(cTemp,0,4);
        cTemp[0]=strSource[i];
        cTemp[1]=strSource[i+1];
        cTemp[2]=strSource[i+2];
        
        int len=strlen(cTemp);
        if(len==3)
        {
            strEncode+=this->m_Base64_Table[((int)cTemp[0] & 0xFC)>>2];
            strEncode+=this->m_Base64_Table[((int)cTemp[0] & 0x3)<<4 | ((int)cTemp[1] & 0xF0)>>4];
            strEncode+=this->m_Base64_Table[((int)cTemp[1] & 0xF)<<2 | ((int)cTemp[2] & 0xC0)>>6];
            strEncode+=this->m_Base64_Table[(int)cTemp[2] & 0x3F];
            if(LineLength+=4>=76) strEncode+="\r\n";
        }
        else if(len==2)
        {
            strEncode+=this->m_Base64_Table[((int)cTemp[0] & 0xFC)>>2];
            strEncode+=this->m_Base64_Table[((int)cTemp[0] & 0x3 )<<4 | ((int)cTemp[1] & 0xF0 )>>4];
            strEncode+=this->m_Base64_Table[((int)cTemp[1] & 0x0F)<<2];
            if(LineLength+=4>=76) strEncode+="\r\n";
            strEncode+="=";
        }
        else if(len==1)
        {
            strEncode+=this->m_Base64_Table[((int)cTemp[0] & 0xFC)>>2];
            strEncode+=this->m_Base64_Table[((int)cTemp[0] & 0x3 )<<4];
            if(LineLength+=4>=76) strEncode+="\r\n";
            strEncode+="==";
        }
        memset(cTemp,0,4);
    }
    return strEncode;
}

string ZBase64::DecodeBase64(const string strSource)
{
    //返回值
    string strDecode;
    char cTemp[5];
    
    for(int i=0;i<strSource.size();i+=4)
    {
        memset(cTemp,0,5);
        cTemp[0]=strSource[i];
        cTemp[1]=strSource[i+1];
        cTemp[2]=strSource[i+2];
        cTemp[3]=strSource[i+3];
        int asc[4];
        for(int j=0;j<4;j++)
        {
            for(int k=0;k<(int)strlen(this->m_Base64_Table);k++)
            {
                if(cTemp[j]==this->m_Base64_Table[k]) asc[j]=k;
            }
        }
        if('='==cTemp[2] && '='==cTemp[3])
        {
            strDecode+=(char)(int)(asc[0] << 2 | asc[1] << 2 >> 6);
        }
        else if('='==cTemp[3])
        {
            strDecode+=(char)(int)(asc[0] << 2 | asc[1] << 2 >> 6);
            strDecode+=(char)(int)(asc[1] << 4 | asc[2] << 2 >> 4);
        }
        else
        {
            strDecode+=(char)(int)(asc[0] << 2 | asc[1] << 2 >> 6);
            strDecode+=(char)(int)(asc[1] << 4 | asc[2] << 2 >> 4);
            strDecode+=(char)(int)(asc[2] << 6 | asc[3] << 2 >> 2);
        }  
    }
    return strDecode;
}
