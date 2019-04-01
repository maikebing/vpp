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

#ifndef INC_VPPSHAREDOBJECT_HPP
#define INC_VPPSHAREDOBJECT_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPCOMMON_HPP
#include "vppCommon.hpp"
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

template< class ObjectT >
class TSharedReference
{
public:
    TSharedReference();
    TSharedReference ( const TSharedReference< ObjectT >& rhs );
    TSharedReference ( ObjectT* pObject );

    template< class OtherObjectT >
    TSharedReference ( const TSharedReference< OtherObjectT >& rhs );

    ~TSharedReference();

    const TSharedReference< ObjectT >& operator= ( const TSharedReference< ObjectT >& rhs );

    ObjectT* operator->() const;
    ObjectT& operator*() const;
    operator bool() const;

    bool operator< ( const TSharedReference< ObjectT >& rhs ) const;
    bool operator== ( const TSharedReference< ObjectT >& rhs ) const;

    ObjectT* get() const;

private:
    ObjectT* d_pObject;
};

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE ObjectT* TSharedReference< ObjectT > :: get() const
{
    return d_pObject;
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE TSharedReference< ObjectT > :: TSharedReference() :
    d_pObject ( 0 )
{
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE TSharedReference< ObjectT > :: TSharedReference ( const TSharedReference< ObjectT >& rhs ) :
    d_pObject ( rhs.d_pObject )
{
    if ( d_pObject )
        d_pObject->addReference();
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE TSharedReference< ObjectT > :: TSharedReference ( ObjectT* pObject ) :
    d_pObject ( pObject )
{
    if ( d_pObject )
        d_pObject->addReference();
}

// -----------------------------------------------------------------------------

template< class ObjectT >
template< class OtherObjectT >
VPP_INLINE TSharedReference< ObjectT > :: TSharedReference ( const TSharedReference< OtherObjectT >& rhs ) :
    d_pObject ( rhs.get() )
{
    if ( d_pObject )
        d_pObject->addReference();
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE TSharedReference< ObjectT > :: ~TSharedReference()
{
    if ( d_pObject )
        d_pObject->releaseReference();
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE const TSharedReference< ObjectT >& TSharedReference< ObjectT > :: operator= (
    const TSharedReference< ObjectT >& rhs )
{
    if ( this != & rhs )
    {
        if ( d_pObject )
            d_pObject->releaseReference();

        d_pObject = rhs.d_pObject;

        if ( d_pObject )
            d_pObject->addReference();
    }
    return *this;
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE ObjectT* TSharedReference< ObjectT > :: operator->() const
{
    return d_pObject;
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE ObjectT& TSharedReference< ObjectT > :: operator*() const
{
    return *d_pObject;
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE TSharedReference< ObjectT > :: operator bool() const
{
    return d_pObject != 0;
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE bool TSharedReference< ObjectT > :: operator< ( const TSharedReference< ObjectT >& rhs ) const
{
    return d_pObject->compareObjects ( rhs.d_pObject );
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE bool TSharedReference< ObjectT > :: operator== ( const TSharedReference< ObjectT >& rhs ) const
{
    return d_pObject == rhs.d_pObject;
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ObjectT >
class TCloneableSharedReference
{
public:
    TCloneableSharedReference();
    TCloneableSharedReference ( const TCloneableSharedReference< ObjectT >& rhs );
    TCloneableSharedReference ( ObjectT* pObject );

    template< class OtherObjectT >
    TCloneableSharedReference ( const TCloneableSharedReference< OtherObjectT >& rhs );

    ~TCloneableSharedReference();

    const TCloneableSharedReference< ObjectT >& operator= ( const TCloneableSharedReference< ObjectT >& rhs );

    ObjectT* operator->() const;
    ObjectT& operator*() const;
    operator bool() const;

    bool operator< ( const TCloneableSharedReference< ObjectT >& rhs ) const;
    bool operator== ( const TCloneableSharedReference< ObjectT >& rhs ) const;

    ObjectT* get() const;

    void copyOnWrite();

private:
    ObjectT* d_pObject;
};

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE ObjectT* TCloneableSharedReference< ObjectT > :: get() const
{
    return d_pObject;
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE TCloneableSharedReference< ObjectT > :: TCloneableSharedReference() :
    d_pObject ( 0 )
{
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE TCloneableSharedReference< ObjectT > :: TCloneableSharedReference ( const TCloneableSharedReference< ObjectT >& rhs ) :
    d_pObject ( rhs.d_pObject )
{
    if ( d_pObject )
        d_pObject->addReference();
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE TCloneableSharedReference< ObjectT > :: TCloneableSharedReference ( ObjectT* pObject ) :
    d_pObject ( pObject )
{
    if ( d_pObject )
        d_pObject->addReference();
}

// -----------------------------------------------------------------------------

template< class ObjectT >
template< class OtherObjectT >
VPP_INLINE TCloneableSharedReference< ObjectT > :: TCloneableSharedReference ( const TCloneableSharedReference< OtherObjectT >& rhs ) :
    d_pObject ( rhs.get() )
{
    if ( d_pObject )
        d_pObject->addReference();
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE TCloneableSharedReference< ObjectT > :: ~TCloneableSharedReference()
{
    if ( d_pObject )
        d_pObject->releaseReference();
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE const TCloneableSharedReference< ObjectT >& TCloneableSharedReference< ObjectT > :: operator= (
    const TCloneableSharedReference< ObjectT >& rhs )
{
    if ( this != & rhs )
    {
        if ( d_pObject )
            d_pObject->releaseReference();

        d_pObject = rhs.d_pObject;

        if ( d_pObject )
            d_pObject->addReference();
    }
    return *this;
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE ObjectT* TCloneableSharedReference< ObjectT > :: operator->() const
{
    return d_pObject;
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE ObjectT& TCloneableSharedReference< ObjectT > :: operator*() const
{
    return *d_pObject;
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE TCloneableSharedReference< ObjectT > :: operator bool() const
{
    return d_pObject != 0;
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE bool TCloneableSharedReference< ObjectT > :: operator< ( const TCloneableSharedReference< ObjectT >& rhs ) const
{
    return d_pObject->compareObjects ( rhs.d_pObject );
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE bool TCloneableSharedReference< ObjectT > :: operator== ( const TCloneableSharedReference< ObjectT >& rhs ) const
{
    return d_pObject == rhs.d_pObject;
}

// -----------------------------------------------------------------------------

template< class ObjectT >
VPP_INLINE void TCloneableSharedReference< ObjectT > :: copyOnWrite()
{
    if ( d_pObject && d_pObject->getReferenceCount() > 1 )
    {
        ObjectT* pOldObject = d_pObject;
        ObjectT* pNewObject = d_pObject->cloneReferencedObject();
        pNewObject->addReference();
        d_pObject = pNewObject;
        pOldObject->releaseReference();
    }
}

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ReferencedT >
class TSharedObject
{
public:
    TSharedObject();

    void addReference();
    void releaseReference();
    unsigned int getReferenceCount() const;
    ReferencedT* cloneReferencedObject();

private:
    std::atomic< int > d_referenceCount;
};

// -----------------------------------------------------------------------------

template< class ReferencedT >
VPP_INLINE TSharedObject< ReferencedT > :: TSharedObject() :
    d_referenceCount ( 0 )
{
}

// -----------------------------------------------------------------------------

template< class ReferencedT >
VPP_INLINE void TSharedObject< ReferencedT > :: addReference()
{
    d_referenceCount.fetch_add ( 1 );
}

// -----------------------------------------------------------------------------

template< class ReferencedT >
VPP_INLINE void TSharedObject< ReferencedT > :: releaseReference()
{
    if ( d_referenceCount.fetch_sub ( 1 ) == 1 )
        delete static_cast< ReferencedT* >( this );
}

// -----------------------------------------------------------------------------

template< class ReferencedT >
VPP_INLINE unsigned int TSharedObject< ReferencedT > :: getReferenceCount() const
{
    return d_referenceCount;
}

// -----------------------------------------------------------------------------

template< class ReferencedT >
VPP_INLINE ReferencedT* TSharedObject< ReferencedT > :: cloneReferencedObject()
{
    ReferencedT* pNewObject = new ReferencedT ( *static_cast< ReferencedT* >( this ) );
    pNewObject->d_referenceCount = 0;
    return pNewObject;
}
    
// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSHAREDOBJECT_HPP
