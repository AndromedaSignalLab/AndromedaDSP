/*
DSP class declarations and definitions of DSP Project
Copyright (C) 2020 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once
#include <cmath>
#include <RGB.hpp>

#define REAL 0
#define IMAG 1

namespace DSP {
    template<class T>
    class DSP {
    public:
        static inline T *hanningMultipliers(size_t dataSize);
        static inline T *hanningMultipliersMatlab(int N, short itype = 0);
        static inline T *hammingMultipliers(int windowLength);
        static inline T *blackmanMultipliers(int windowLength);
        static inline T calculateMagnitude(T real, T imaginary);
        static inline T calculateDecibel(T real, T imaginary);
        static inline T magnitudeToDecibel(T magnitude);
        static inline void magnitudeToDecibel(T* magnitudeValues, T* decibelValues, const size_t count);
        static inline T decibelToMagnitude(T decibel);
        static inline T calculateVolumeDbLevel(T* leftBuffer, T* rightBuffer,size_t count);
        //static inline T powerToDecibel(T powerValue);
        //static inline T decibelToPower(T decibelValue);
        static inline T linearToDecibel(T v1, T v2);
        static inline T logarithm(T);
        static inline double calculatePerceptualLightness(const RGB &rgb);
        static inline double calculateContrast(const RGB &rgb1, const RGB &rgb2);
        /**
         * Calculates black or white foreground color that makes maximum contrast for the given background color.
         * @param backgroundColor background color
         * @param tolerence Tolorence value between 0 and 100 for white color. For example, is 10 is chosen for tolerence,
         * then until contrast is 10, this method will return true, otherwise, will return the calculated value.
         * @return false for black, true for white.
         */
        static inline bool calculateBWForegroundColor(const RGB & backgroundColor, const int &tolerence);
    };
}
// Inline Method Definitions

template <class T> inline T DSP::DSP<T>::magnitudeToDecibel(T magnitude) {
    return T(20)*logarithm(magnitude);
}

template <class T> inline T DSP::DSP<T>::decibelToMagnitude(T decibel) {
    return pow<T>(10, decibel/T(20));
}

template <> inline double DSP::DSP<double>::logarithm(double value) {
    return log10(value);
}

template <> inline float DSP::DSP<float>::logarithm(float value) {
    return log10f(value);
}

template <> inline long double DSP::DSP<long double>::logarithm(long double value) {
    return log10l(value);
}

template <class T> inline T DSP::DSP<T>::logarithm(T value) {
    return (T) log10(value);
}

template <class T> inline T DSP::DSP<T>::calculateDecibel(T real, T imaginary) {
    return T(10)*logarithm(real*real + imaginary*imaginary);
}

template <class T> inline T DSP::DSP<T>::calculateMagnitude(T real, T imaginary) {
    return std::sqrt(real*real + imaginary*imaginary);
}

template <class T> inline T * DSP::DSP<T>::hanningMultipliers(size_t dataSize){
    T *multipliers;

    multipliers = new T[dataSize];
    for (unsigned int i = 0; i < dataSize; i++) {
        T multiplier = T(0.5) * (T(1) - cos(T(2)*M_PI*i/(dataSize-1)));
        multipliers[i] = multiplier;
    }
    return multipliers;
}

// Create a hamming window of windowLength samples in buffer
template<class T> inline T* DSP::DSP<T>::hammingMultipliers(int windowLength) {
    T *timeDomain = new T[windowLength];
    for(int i = 0; i < windowLength; i++) {
        timeDomain[i] = T(0.53836) - ( T(0.46164) * cos( T(2) * M_PI * T(i)  / ( T(windowLength) - T(1) ) ) );
        //timeDomain[i] = 0.54 - (0.46 * cos( 2 * M_PI * (i / ((windowLength - 1) * 1.0))));
    }
    return timeDomain;
}

template<class T> inline T* DSP::DSP<T>::blackmanMultipliers(int windowLength) {
    T *timeDomain = new T[windowLength];
    for(int i = 0; i < windowLength; i++) {
        timeDomain[i] = T(0.42) - T(0.5) * std::cos(T(2) * M_PI * i / (T(windowLength) - T(1))) + T(0.08) * std::cos(T(4.0) * M_PI * T(i) / (T(windowLength) - T(1)));
    }
    return timeDomain;
}

/*  function w = hanning(varargin)
%   HANNING   Hanning window.
%   HANNING(N) returns the N-point symmetric Hanning window in a column
%   vector.  Note that the first and last zero-weighted window samples
%   are not included.
%
%   HANNING(N,'symmetric') returns the same result as HANNING(N).
%
%   HANNING(N,'periodic') returns the N-point periodic Hanning window,
%   and includes the first zero-weighted window sample.
%
%   NOTE: Use the HANN function to get a Hanning window which has the
%          first and last zero-weighted samples.ep
    itype = 1 --> periodic
    itype = 0 --> symmetric
    default itype=0 (symmetric)

    Copyright 1988-2004 The MathWorks, Inc.
%   $Revision: 1.11.4.3 $  $Date: 2007/12/14 15:05:04 $
*/
template <class T> inline T * DSP::DSP<T>::hanningMultipliersMatlab(int N, short itype){
    int half, i, idx, n;
    T *w;

    w = new T[N];
    std::fill(w, w+N, T(0));

    if(itype==1)    //periodic function
        n = N-1;
    else
        n = N;

    if(n%2==0)
    {
        half = n/2;
        for(i=0; i<half; i++) //CALC_HANNING   Calculates Hanning window samples.
            w[i] = 0.5 * (T(1) - cos(T(2)*M_PI*(i+1) / (n+1)));

        idx = half-1;
        for(i=half; i<n; i++) {
            w[i] = w[idx];
            idx--;
        }
    }
    else
    {
        half = (n+1)/2;
        for(i=0; i<half; i++) //CALC_HANNING   Calculates Hanning window samples.
            w[i] = 0.5 * (T(1) - cos(T(2)*M_PI*(i+1) / (n+1)));

        idx = half-2;
        for(i=half; i<n; i++) {
            w[i] = w[idx];
            idx--;
        }
    }

    if(itype==1)    //periodic function
    {
        for(i=N-1; i>=1; i--)
            w[i] = w[i-1];
        w[0] = 0.0;
    }
    return w;
}

template <typename T> inline void DSP::DSP<T>::magnitudeToDecibel(T* magnitudeValues, T* decibelValues, size_t count) {
    for(size_t i = 0; i < count; i++) {
        decibelValues[i] = magnitudeToDecibel(magnitudeValues[i]);
    }
    return;
}

template <typename T> inline T DSP::DSP<T>::calculateVolumeDbLevel(T* leftBuffer, T* rightBuffer,size_t count) {
    T sum = 0;
    T volume = 0;

    for(size_t i = 0; i < count; i++) {
        sum += pow((T(leftBuffer[i]) + T(rightBuffer[i]))/T(2), 2);
    }

    volume = T(20) * logarithm(std::sqrt(sum / T(count)));

    return volume;
}
/*
Using the natural log, ln, log base e:
linear-to-db(x) = ln(x) / (ln(10) / 20)
db-to-linear(x) = e^(x * (ln(10) / 20))

Using the common log, log, log base 10:
linear-to-db(x) = log(x) * 20
db-to-linear(x) = 10^(x / 20)
*/

template<class T> inline T DSP::DSP<T>::linearToDecibel(T v1, T v2) {
    return T(20) * logarithm(v1/v2);
}

template<class T> inline double DSP::DSP<T>::calculatePerceptualLightness(const RGB & rgb) {
    //Convert all sRGB 8 bit integer values to decimal 0.0-1.0
    double vR = double(rgb.red) / 255.0;
    double vG = double(rgb.green) / 255.0;
    double vB = double(rgb.blue) / 255.0;
    auto sRGBtoLin = [](double colorChannel) {
        // Send this function a decimal sRGB gamma encoded color value
        // between 0.0 and 1.0, and it returns a linearized value.

        if ( colorChannel <= 0.04045 ) {
            return colorChannel / 12.92;
        } else {
            return pow((( colorChannel + 0.055)/1.055),2.4);
        }
    };

    double Y = (0.2126 * sRGBtoLin(vR) + 0.7152 * sRGBtoLin(vG) + 0.0722 * sRGBtoLin(vB));

    auto YtoLstar = [](double Y) {
        // Send this function a luminance value between 0.0 and 1.0,
        // and it returns L* which is "perceptual lightness"

        if ( Y <= (216.0/24389.0) ){       // The CIE standard states 0.008856 but 216/24389 is the intent for 0.008856451679036
            return Y * (24389/27);  // The CIE standard states 903.3, but 24389/27 is the intent, making 903.296296296296296
        } else {
            return pow(Y,(1.0/3.0)) * 116.0 - 16.0;
        }
    };

    return YtoLstar(Y);
}

template<class T> inline double DSP::DSP<T>::calculateContrast(const RGB &rgb1, const RGB &rgb2) {
    double lum1 = DSP<double>::calculatePerceptualLightness(rgb1);
    double lum2 = DSP<double>::calculatePerceptualLightness(rgb2);
    double brightest = std::max(lum1, lum2);
    double darkest = std::min(lum1, lum2);
    return brightest - darkest;
}

template<class T> inline bool DSP::DSP<T>::calculateBWForegroundColor(const RGB &backgroundColor, const int &tolerence) {
    RGB black, white;
    black.red = 0;
    black.green = 0;
    black.blue = 0;
    white.red = 255;
    white.green = 255;
    white.blue = 255;
    double contrastWithWhite = DSP<double>::calculateContrast(backgroundColor, white);
    double contrastWithBlack = DSP<double>::calculateContrast(backgroundColor, black);
    if(std::abs(contrastWithWhite - contrastWithBlack) <= tolerence)
        return true;
    if(contrastWithWhite < contrastWithBlack)
        return false;
    else
        return true;
}
