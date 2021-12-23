/*
RGB class declarations and definitions of DSP Project
Copyright (C) 6/14/2020 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#ifndef RGB_HPP
#define RGB_HPP

#include <iostream>
#include <iomanip>
#include <sstream>

#ifdef QT_CORE_LIB
#include <QDataStream>
#include <QDebug>
#include <QColor>
#include <QMetaType>
#endif

class RGB {
public:
    unsigned char red;
    unsigned char green;
    unsigned char blue;
    inline RGB();
    inline RGB(const unsigned char &red, const unsigned char &green, const unsigned char &blue);
    inline RGB(const std::string &hex);
    inline std::string hex() const;
    inline void parseFromHex(const std::string &hexString);

    inline bool operator ==(const RGB &other) const;
    inline bool operator !=(const RGB &other) const;

    inline friend std::ostream & operator << (std::ostream &out, const RGB &rgb);
    inline friend std::istream & operator >> (std::istream &in,  RGB &rgb);
    #ifdef QT_CORE_LIB
    inline RGB(const QColor &color);
    inline friend QDataStream &operator<<(QDataStream &out, const RGB &rgb);
    inline friend QDataStream &operator>>(QDataStream &in, RGB &rgb);
    #endif
private:
    inline void init();
};


#ifdef QT_CORE_LIB
QDataStream & operator<<(QDataStream &out, const RGB &rgb) {
    out << rgb.hex().c_str();
    qDebug()<<"Operator << "<<rgb.hex().c_str();
    return out;
}
#endif

#ifdef QT_CORE_LIB
QDataStream & operator>>(QDataStream &in, RGB &rgb) {
    //quint8 length = 0;
    //in >> length;

    QByteArray buffer(10, Qt::Uninitialized);


    QString string;
    in >> string;
    //in.readRawData(buffer.data(), length);
    //QString string(buffer);

    rgb.parseFromHex(string.toStdString());
    qDebug()<<"Operator >> "<<rgb.hex().c_str();

    return in;
}
#endif

#ifdef QT_CORE_LIB
Q_DECLARE_METATYPE(RGB);
#endif

inline void RGB::init() {
#ifdef QT_CORE_LIB
   // qRegisterMetaTypeStreamOperators<RGB>("RGB");
#endif
}

inline std::ostream & operator << (std::ostream &out, const RGB &rgb)
{
    out << rgb.hex();
    std::cout<<"Operator std::cout<<"<<rgb.hex();
    return out;
}

inline std::istream & operator >> (std::istream &in,  RGB &rgb)
{
    std::string hexString(std::istreambuf_iterator<char>(in), {});

    rgb.parseFromHex(hexString);

    std::cout<<"Operator std::cin>>"<<rgb.hex();
    return in;
}

inline RGB::RGB() {
    red = 0;
    green = 0;
    blue = 0;
    init();
}

inline RGB::RGB(const unsigned char &red, const unsigned char &green, const unsigned char &blue) {
    this->red = red;
    this->green = green;
    this->blue = blue;
    init();
}

#ifdef QT_CORE_LIB
inline RGB::RGB(const QColor &color){
    int r,g,b;
    color.getRgb(&r, &g, &b);
    red = r;
    green = g;
    blue = b;
}
#endif

inline RGB::RGB(const std::string &hexString) {
    parseFromHex(hexString);
    init();
}

inline void RGB::parseFromHex(const std::string &hexString) {
    int r, g, b;
    if(hexString[0] == '#') {
        std::sscanf(hexString.c_str(), "#%02x%02x%02x", &r, &g, &b);
    }
    else {
        std::sscanf(hexString.c_str(), "%02x%02x%02x", &r, &g, &b);
    }
    red = r;
    green = g;
    blue = b;
}

inline bool RGB::operator ==(const RGB &other) const
{
    return (red == other.red && green == other.green && blue == other.blue);
}

inline bool RGB::operator !=(const RGB &other) const
{
    return !(red == other.red && green == other.green && blue == other.blue);
}

inline std::string RGB::hex() const{
    std::stringstream ss;
    ss << "#";
    ss << std::hex << std::uppercase << std::setfill('0')
       << std::setw(6) << (int(red) << 16 | int(green) << 8 | int(blue));
    return ss.str();
}

#endif //RGB_HPP
