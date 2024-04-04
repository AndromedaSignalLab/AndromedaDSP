/*
Unused function definitions of DSP Project
Copyright (C) 2020 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once
#include <cmath>
#include <vector>
#include "SearchUtil.hpp"
#include "MathUtil.hpp"
#include "DSP.hpp"

/* This function take Nsamples from the float array *samples_vett, evalutates the
 Fast Fourier Transform of them (by kiss_fftr function) and divided into 10 bands the entire
 audio spectrum getting 10 values into float array *out_filters_vett. kiss_fftr is a very
  smart way to get the TTF of samples.
 You can downlolad the kiss_fft c code from https://sourceforge.net/projects/kissfft/files/kissfft/v1_3_0/.
 Of course, instead of 10 bands you can divide the entire audio spectrum into any value (10,
 15,24,32 ecc). This function, together with kiss_FFT can be wrapped into a pyd module that can be into python.
 can be into python. The road map to do it is showed in the video tutorial */
void samplesTo10BandsFilter(double ** frequencyDomainData, float *filterOutputData, size_t sampleAmount) {
    float band_power=0.0;
    float harmonic_amplitude=0.0;
    size_t i, out_index;

    out_index=0;

    for (i = 0; i < sampleAmount/2+1; i++) {
        //Replaced by or below
        // end_band_index=0;                   // Banda di freq   Frequenza centrale
        // if (i==1) end_band_index=1;         // 22Hz-44Hz            31.5Hz
        // else if (i==2) end_band_index=1;    // 44Hz-88Hz            63Hz
        // else if (i==4) end_band_index=1;    // 88Hz-177Hz           125Hz
        // else if (i==8) end_band_index=1;    // 177Hz-355Hz          250Hz
        // else if (i==17) end_band_index=1;   // 355Hz-710Hz          500Hz
        // else if (i==33) end_band_index=1;   // 710Hz-1420Hz         1000Hz
        // else if (i==66) end_band_index=1;   // 1420Hz-2840Hz        2000Hz
        // else if (i==132) end_band_index=1;  // 2840Hz-5680Hz        4000Hz
        // else if (i==264) end_band_index=1;  // 5680Hz-11360Hz       8000Hz
        // else if (i==511) end_band_index=1;  // 11360Hz-22050Hz      16000Hz
        //if (end_band_index==1)

        if (i==1||i==2||i==4||i==8||i==17||i==33||i==66||i==132||i==264||i==511) {
            filterOutputData[out_index]=band_power; //Memorizzo la potenza di banda
            band_power=0; //Azzero il contatore di potenza della banda
            out_index++;  //Incremento l'indice che indica la barra (va da 0 a 9)
        }
        //Calcolo l'ampiezza dell'armonica (che costituisc una sorta di potenza di armonica)
        harmonic_amplitude = DSP::calculateMagnitude<double>(frequencyDomainData[i][REAL],frequencyDomainData[i][IMAG]);
        //Aggiungo la potenza dell'armonica alla potenza della banda a cui l'armonica appartiene
        band_power+=harmonic_amplitude;

    }

    return;
}

/*
%% Calculate Third Octave Bands (base 2) in Matlab
fcentre  = 10^3 * (2 .^ ([-18:13]/3))
fd = 2^(1/6);
fupper = fcentre * fd
flower = fcentre / fd

Base 10 calculation

%% Calculate Third Octave Bands (base 10) in Matlab
fcentre = 10.^(0.1.*[12:43])
fd = 10^0.05;
fupper = fcentre * fd
flower = fcentre / fd
*/
template <class T> std::vector<OctaveBand<T>> calculateOctaveBandsWikipedia(OctaveBandBase base, size_t nthOctave) {
    std::vector<OctaveBand<T>> octaveBands;
    T fd;
    T i = 0;
    switch (base) {
        case OctaveBandBase::Base2:
            fd = pow(T(2),T(1)/T(2*nthOctave));
            i=T(-6);
            while(true) {
                OctaveBand<T> octaveBand;
                octaveBand.midBandFrequency = pow(T(2), T(i)) * T(1000);
                octaveBand.upperEdgeBandFrequency = octaveBand.midBandFrequency * fd;
                octaveBand.lowerEdgeBandFrequency = octaveBand.midBandFrequency / fd;
                if(octaveBand.midBandFrequency > T(23000))
                    break;
                octaveBands.push_back(octaveBand);
                i += T(1)/nthOctave;
            }
            break;
        case OctaveBandBase::Base10:
            fd = pow(T(10),T(0.05));
            while(true) {
                OctaveBand<T> octaveBand;
                octaveBand.midBandFrequency = pow(T(10), T(0.1) * i);
                octaveBand.upperEdgeBandFrequency = octaveBand.midBandFrequency * fd;
                octaveBand.lowerEdgeBandFrequency = octaveBand.midBandFrequency / fd;
                if(octaveBand.midBandFrequency > T(23000))
                    break;
                octaveBands.push_back(octaveBand);
                i = i+1;
            }
            break;
    }
    return octaveBands;
}


/* This function take Nsamples from the float array *samples_vett, evalutates the
 Fast Fourier Transform of them (by kiss_fftr function) and divided into 10 bands the entire
 audio spectrum getting 10 values into float array *out_filters_vett. kiss_fftr is a very
  smart way to get the TTF of samples.
 You can downlolad the kiss_fft c code from https://sourceforge.net/projects/kissfft/files/kissfft/v1_3_0/.
 Of course, instead of 10 bands you can divide the entire audio spectrum into any value (10,
 15,24,32 ecc). This function, together with kiss_FFT can be wrapped into a pyd module that can be into python.
 can be into python. The road map to do it is showed in the video tutorial */

#define max_N 2048
void samples_to_10_bands_filterx(float *samples_vett, float *out_filters_vett, int Nsamples) {
    unsigned int end_band_index = 0;
    float band_power = 0.0;
    float harmonic_amplitude = 0.0;
    size_t i, out_index;

    out_index = 0;
    //kiss_fft_cpx ttfr_out_vett[max_N / 2 + 1];
    //kiss_fftr_cfg cfg;

    //if ((cfg = kiss_fftr_alloc(Nsamples, 0/*is_inverse_fft*/, NULL, NULL)) != NULL)
    {
        //kiss_fftr(cfg, samples_vett, ttfr_out_vett);  //real fft of samples_vett into ttfr_out_vett

        //free(cfg);
        /*For each harmonic get the amplitude from complex
          value and divide entire spectrum into band*/
        for (i = 0; i < Nsamples / 2 + 1; i++)
        {
            end_band_index = 0;                     // Band               Center Freq
            if (i == 1) end_band_index = 1;         // 22Hz-44Hz            31.5Hz
            else if (i == 2) end_band_index = 1;    // 44Hz-88Hz            63Hz
            else if (i == 4) end_band_index = 1;    // 88Hz-177Hz           125Hz
            else if (i == 8) end_band_index = 1;    // 177Hz-355Hz          250Hz
            else if (i == 17) end_band_index = 1;   // 355Hz-710Hz          500Hz
            else if (i == 33) end_band_index = 1;   // 710Hz-1420Hz         1000Hz
            else if (i == 66) end_band_index = 1;   // 1420Hz-2840Hz        2000Hz
            else if (i == 132) end_band_index = 1;  // 2840Hz-5680Hz        4000Hz
            else if (i == 264) end_band_index = 1;  // 5680Hz-11360Hz       8000Hz
            else if (i == 511) end_band_index = 1;  // 11360Hz-22050Hz      16000Hz

            if (end_band_index == 1)
            {
                out_filters_vett[out_index] = band_power;
                band_power = 0;
                out_index++;
            }
            //harmonic_amplitude = sqrt(pow(ttfr_out_vett[i].r, 2) + pow(ttfr_out_vett[i].i, 2));
            //band_power += harmonic_amplitude;

        }


        return;
    }
}
