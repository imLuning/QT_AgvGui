#ifndef STRINGTOHEX_H
#define STRINGTOHEX_H

#include <QString>
#include <QByteArray>

char ConvertCharToHex(char ch);
void ConvertStringToHex(QString str,QByteArray &bytearray);

#endif // STRINGTOHEX_H
