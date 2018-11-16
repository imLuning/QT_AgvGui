#include "stringtohex.h"

//将字符型数据转换为其字面意义的数值
char ConvertCharToHex(char ch)
{
    if((ch >= '0')&&(ch <= '9'))
        return (ch - '0');
    else if((ch >= 'A')&&(ch <= 'F'))
        return (ch - 'A' +10);
    else if((ch >= 'a')&&(ch <= 'f'))
        return (ch -'a' +10);
    else
        return 0;
}

//将字符串转换为字面意义对应的HEX值
void ConvertStringToHex(QString str, QByteArray &bytearray)
{
    char lchar,hchar;
    int hhex,lhex;
    int hexdata;
    int bytearraylen = 0;
    int strlen = str.length();
    bytearray.resize(strlen/2);

    for(int i=0;i<strlen;)
    {
        hchar = str[i].toLatin1();
        if(hchar == ' ')
        {
            i++;
            continue;
        }
        i++;
        if(i >= strlen)
            break;
        lchar = str[i].toLatin1();
        hhex = ConvertCharToHex(hchar);
        lhex = ConvertCharToHex(lchar);
        hexdata = hhex*16+lhex;
        i++;
        bytearray[bytearraylen] = (char)hexdata;
        bytearraylen++;
    }
    bytearray.resize(bytearraylen);
}
