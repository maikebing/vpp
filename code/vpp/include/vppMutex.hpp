/*
    Copyright 2016-2018 SOFT-ERG, Przemek Kuczmierczyk (www.softerg.com)
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

#ifndef INC_VPPMUTEX_HPP
#define INC_VPPMUTEX_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPCOMMON_HPP
#include "vppCommon.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

class spinlock_mutex
{
public:
    spinlock_mutex();

    void lock();
    void unlock();

private:
    std::atomic_flag d_acquired;
};

// -----------------------------------------------------------------------------

VPP_INLINE spinlock_mutex :: spinlock_mutex()
{
    d_acquired.clear();
}

// -----------------------------------------------------------------------------

VPP_INLINE void spinlock_mutex :: lock()
{
    while ( d_acquired.test_and_set ( std::memory_order_acquire ) )
        std::this_thread::yield();
}

// -----------------------------------------------------------------------------

VPP_INLINE void spinlock_mutex :: unlock()
{
    d_acquired.clear ( std::memory_order_release );
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

typedef std::lock_guard< std::mutex > mutex_lock;
typedef std::lock_guard< spinlock_mutex > spin_lock;

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPMUTEX_HPP
