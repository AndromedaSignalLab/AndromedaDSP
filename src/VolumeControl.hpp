/*
VolumeControl class declarations and definitions of DSP Project
Copyright (C) 2020 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once
#include <cmath>

namespace AndromedaDSP {
    template<class T>
    class VolumeControl {
        public:
            static inline T calculateExponetialVolume(const T linearVolume, const size_t dynamicRange = 60);
        private:
            static inline T calculateExponetialVolume(const T linearVolume, const T a, const T b);
            static inline void calculateVariables(const size_t dynamicRange, size_t &approximationOrder, T &a, T &b);
    };
}


template<class T> inline T AndromedaDSP::VolumeControl<T>::calculateExponetialVolume(const T linearVolume, const size_t dynamicRange) {
    T a, b;
    size_t approximationOrder;
    calculateVariables(dynamicRange, approximationOrder, a, b);
    return calculateExponetialVolume(linearVolume, a, b);
}

template<class T> inline T AndromedaDSP::VolumeControl<T>::calculateExponetialVolume(const T linearVolume, const T a, const T b) {
    //This algorithm was taken from https://www.dr-lex.be/info-stuff/volumecontrols.html
    if(linearVolume == 0)
        return 0;
    if(linearVolume == 1)
        return 1;
    //if(x < .1) ampl = x*10*a*exp(0.1*b);
    if(linearVolume < .1)
        return linearVolume * T(10) * a * exp(T(0.1) * b);
    //y = a*exp(b*x)
    return a * exp(b * linearVolume);
}

template<class T> inline void AndromedaDSP::VolumeControl<T>::calculateVariables(const size_t dynamicRange, size_t &approximationOrder, T &a, T &b) {
    switch(dynamicRange) {
        case 50:
            a = 3.1623e-3;
            b = 5.757;
            approximationOrder = 3;
        break;
        case 60:
            a = 1e-3;
            b = 6.908;
            approximationOrder = 4;
        break;
        case 70:
            a = 3.1623e-4;
            b = 8.059;
            approximationOrder = 5;
        break;
        case 80:
            a = 1e-4;
            b = 9.210;
            approximationOrder = 6;
        break;
        case 90:
            a = 3.1623e-5;
            b = 10.36;
            approximationOrder = 6;
        break;
        case 100:
            a = 1e-5;
            b = 11.51;
            approximationOrder = 7;
        break;
    }
}
