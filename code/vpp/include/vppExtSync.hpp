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

#ifndef INC_VPPEXTSYNC_HPP
#define INC_VPPEXTSYNC_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPMUTEX_HPP
#include "vppMutex.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

#define VPP_EXTSYNC_MUTEX vpp::spinlock_mutex
#define VPP_EXTSYNC_SCOPED_LOCK vpp::spin_lock

// -----------------------------------------------------------------------------

#define VPP_MULTITHREADED

// -----------------------------------------------------------------------------

#ifdef VPP_MULTITHREADED
    #define VPP_EXTSYNC_MTX_DECLARE VPP_EXTSYNC_MUTEX d_extsyncMutex
    #define VPP_EXTSYNC_MTX_SLOCK( pObj ) VPP_EXTSYNC_SCOPED_LOCK scopedLock ( pObj->d_extsyncMutex )

    #define VPP_EXTSYNC_METHODS_DECLARE( pObj ) \
        VPP_INLINE void extSyncLock() { pObj->d_extsyncMutex.lock(); } \
        VPP_INLINE void extSyncUnlock() { pObj->d_extsyncMutex.unlock(); } \

    #define VPP_EXTSYNC_MTX_LOCK( pObj ) pObj->extSyncLock()
    #define VPP_EXTSYNC_MTX_UNLOCK( pObj ) pObj->extSyncUnlock()

#else
    #define VPP_EXTSYNC_MTX_DECLARE unsigned char d_extsyncMutexPlaceholder [ sizeof ( VPP_EXTSYNC_MUTEX ) ]
    #define VPP_EXTSYNC_MTX_LOCK( pObj ) ( void ) 0
    #define VPP_EXTSYNC_MTX_UNLOCK( pObj ) ( void ) 0
    #define VPP_EXTSYNC_MTX_SLOCK( pObj ) ( void ) 0

    #define VPP_EXTSYNC_METHODS_DECLARE( pObj ) \
        VPP_INLINE void extSyncLock() {} \
        VPP_INLINE void extSyncUnlock() {} \

#endif

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPEXTSYNC_HPP
