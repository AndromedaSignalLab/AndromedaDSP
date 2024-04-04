/*
SearchUtil utilization class declarations and definitions of DSP Project
Copyright (C) 4/5/2020 Volkan Orhan

This library is free software; you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation; either version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with this library; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
*/

#pragma once

template <class T>
class SearchUtil {
private:
    inline static T getClosest(T val1, T val2, T target);

    // Returns element closest to target in arr[]
public:
    inline static T findClosest(const T * arr, int n, T target);
};


// Returns element closest to target in arr[]
template<class T> inline T SearchUtil<T>::findClosest(const T * arr, int n, T target) {
// Corner cases
    if (target <= arr[0])
        return arr[0];
    if (target >= arr[n - 1])
        return arr[n - 1];

// Doing binary search
    int i = 0, j = n, mid = 0;
    while (i < j) {
        mid = (i + j) / 2;

        if (arr[mid] == target)
            return arr[mid];

/* If target is less than array element,
    then search in left */
        if (target < arr[mid]) {

// If target is greater than previous
// to mid, return closest of two
            if (mid > 0 && target > arr[mid - 1])
                return getClosest(arr[mid - 1],
                                  arr[mid], target);

/* Repeat for left half */
            j = mid;
        }

// If target is greater than mid
        else {
            if (mid < n - 1 && target < arr[mid + 1])
                return getClosest(arr[mid],
                                  arr[mid + 1], target);
// update i
            i = mid + 1;
        }
    }

// Only single element left after search
    return arr[mid];
}

// Method to compare which one is the more close.
// We find the closest by taking the difference
// between the target and both values. It assumes
// that val2 is greater than val1 and target lies
// between these two.
template<class T> inline T SearchUtil<T>::getClosest(T val1, T val2,
                           T target) {
    if (target - val1 >= val2 - target)
        return val2;
    else
        return val1;
}
