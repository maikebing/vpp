/*
    Copyright 2016-2019 SOFT-ERG, Przemek Kuczmierczyk (www.softerg.com)
    All rights reserved.

    Redistribution and use in source and binary forms, with or without modification,
    are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice,
       this list of conditions and the following disclaimer.

    2. Redistributions in binary form must reproduce the above copyright notice,
       this list of conditions and the following disclaimer in the documentation
       and/or other materials provided with the distribution.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
    AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
    THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
    ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
    FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
    (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
    ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
    NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
    EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

// -----------------------------------------------------------------------------

#include "ph.hpp"
#include "../include/vppInternalUtils.hpp"

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

const unsigned int s_fastDivisionMultipliers [ 128 ] =
{
    0, 87382, 52429, 37450, 29128, 23832, 20165, 34953, 30841, 55189, 49933, 45591, 10486, 38837,
    18079, 16913, 7944, 7490, 7085, 26887, 51151, 12193, 11651, 44621, 5350, 41121, 19785, 38131,
    36793, 17773, 8595, 33289, 4033, 31301, 15197, 14769, 57457, 55925, 6809, 26547, 25891, 25267,
    24673, 48211, 5891, 11523, 45101, 44151, 43241, 2648, 5191, 20361, 19973, 2450, 2405, 37787,
    18559, 36473, 35849, 35247, 4333, 34101, 33555, 33027, 16257, 16009, 31537, 31069, 3827, 30175,
    29747, 29331, 28927, 28533, 14075, 27777, 13707, 54121, 6679, 6595, 6513, 6433, 50841, 6279,
    24819, 49057, 24245, 1498, 23697, 2929, 23173, 2865, 1417, 11215, 44385, 2745, 43465, 43019,
    21291, 21077, 41735, 10331, 40921, 40525, 20069, 39757, 4923, 39017, 19329, 38305, 18979, 18809,
    37283, 36955, 4579, 36315, 36003, 35697, 8849, 35099, 4351, 17261, 1070, 16981, 16845, 33421,
    33157, 32897
};

// -----------------------------------------------------------------------------

const unsigned int s_fastDivisionShifts [ 128 ] =
{
    0, 18, 18, 18, 18, 18, 18, 19, 19, 20, 20, 20, 18, 20, 19, 19, 18, 18, 18, 20, 
    21, 19, 19, 21, 18, 21, 20, 21, 21, 20, 19, 21, 18, 21, 20, 20, 22, 22, 19, 21, 
    21, 21, 21, 22, 19, 20, 22, 22, 22, 18, 19, 21, 21, 18, 18, 22, 21, 22, 22, 22, 
    19, 22, 22, 22, 21, 21, 22, 22, 19, 22, 22, 22, 22, 22, 21, 22, 21, 23, 20, 20, 
    20, 20, 23, 20, 22, 23, 22, 18, 22, 19, 22, 19, 18, 21, 23, 19, 23, 23, 22, 22, 
    23, 21, 23, 23, 22, 23, 20, 23, 22, 23, 22, 22, 23, 23, 20, 23, 23, 23, 21, 23, 
    20, 22, 18, 22, 22, 23, 23, 23
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
