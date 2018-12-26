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

#ifndef INC_VPPSHADERLANGUAGETRANSLATOR_HPP
#define INC_VPPSHADERLANGUAGETRANSLATOR_HPP

// -----------------------------------------------------------------------------

#ifndef INC_VPPSHADERMODULE_HPP
#include "vppShaderModule.hpp"
#endif

#ifndef INC_VPPSPIRVBUILDER_H
#include "../src/spirv/SpvBuilder.h"
#define INC_VPPSPIRVBUILDER_H
#endif

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------
namespace detail {
// -----------------------------------------------------------------------------

class KParameter;
class KFunction;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KScope
{
public:
    typedef std::map< spv::Id, spv::Id > L2RValue;
    L2RValue d_l2rValue;
};

// -----------------------------------------------------------------------------

class KForRange
{
public:
    VPP_INLINE KForRange ( const KId& idVar, const KId& idStep, bool bUnsigned ) :
        d_varId ( idVar ),
        d_stepId ( idStep ),
        d_bUnsigned ( bUnsigned )
    {}

    spv::Id d_varId;
    spv::Id d_stepId;
    bool d_bUnsigned;
};

// -----------------------------------------------------------------------------
} // namespace detail
// -----------------------------------------------------------------------------

class Bool;

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

struct KShaderScopedVariable
{
    unsigned int d_placeholder;
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

class KShaderTranslator :
    public spv::SpvBuildLogger,
    public spv::Builder
{
public:
    KShaderTranslator ( VkShaderStageFlagBits stage, const Device& hDevice );
    ~KShaderTranslator();

    KShaderModule generateModule() const;

public:
    VPP_DLLAPI static KShaderTranslator* get();

    void print ( std::ostream& output ) const;

    const Device& getDevice() const;
    VkShaderStageFlagBits getStage() const;

    VPP_DLLAPI void pushIf ( Bool bcond );
    VPP_DLLAPI void makeElse();
    VPP_DLLAPI void popIf();

    void pushSwitch ( spv::Id cond );
    void makeSwitchCase ( int value );
    void makeSwitchDefault();
    void popSwitch();
    
    VPP_DLLAPI LoopBlocks& pushLoop();
    VPP_DLLAPI LoopBlocks& currentLoop();
    VPP_DLLAPI void popLoop();

    void pushFor ( const KId& varId, const KId& stepId, bool bUnsigned );
    const detail::KForRange& currentFor() const;
    void popFor();

    void registerNewFunction ( detail::KFunction* pFunction, const char* pName );
    void registerParameter ( detail::KParameter* pParameter );
    void startFunctionCode();
    void endFunctionCode();
    
    VPP_DLLAPI void registerInputOutputVariable ( const KId& id );
    void generateInputOutputForwards ( spv::Instruction* pEntryPoint );

    VPP_DLLAPI void setTemporaryVariableStorageClass ( spv::StorageClass eClass );

    VPP_DLLAPI KId registerLocalVariable ( KId type, spv::StorageClass* pClass = 0 );
    VPP_DLLAPI KId registerUniformBuffer ( KId type, std::uint32_t set, std::uint32_t binding, spv::StorageClass eClass );

    VPP_DLLAPI KId registerSpecialInputVariable ( const KShaderScopedVariable* pVariable, KId type, spv::BuiltIn eVariable );
    VPP_DLLAPI KId registerSpecialOutputVariable ( const KShaderScopedVariable* pVariable, KId type, spv::BuiltIn eVariable );
    VPP_DLLAPI KId registerSamplerVariable ( const KShaderScopedVariable* pVariable, int set, int binding );
    VPP_DLLAPI KId registerSamplerVariable ( const KShaderScopedVariable* pVariable, const KId& indexId, int set, int binding, int count );

    struct SMemberInfo
    {
        VPP_INLINE SMemberInfo ( spv::Id typeId ) :
            d_typeId ( typeId ),
            d_offset ( 0 ),
            d_bHasHostMember ( false ),
            d_bIsMatrix ( false ),
            d_bIsColumnMajor ( false ),
            d_matrixStride ( 0 ),
            d_flags ( 0 )
        {
        }

        VPP_INLINE SMemberInfo ( spv::Id typeId, int offset ) :
            d_typeId ( typeId ),
            d_offset ( offset ),
            d_bHasHostMember ( true ),
            d_bIsMatrix ( false ),
            d_bIsColumnMajor ( false ),
            d_matrixStride ( 0 ),
            d_flags ( 0 )
        {
        }

        VPP_INLINE SMemberInfo (
            spv::Id typeId, int offset,
            unsigned int matrixStride, bool bColMajor ) :
                d_typeId ( typeId ),
                d_offset ( offset ),
                d_bHasHostMember ( true ),
                d_bIsMatrix ( true ),
                d_bIsColumnMajor ( bColMajor ),
                d_matrixStride ( matrixStride ),
                d_flags ( 0 )
        {
        }

        spv::Id d_typeId;
        int d_offset;
        bool d_bHasHostMember;
        bool d_bIsMatrix;
        bool d_bIsColumnMajor;
        unsigned int d_matrixStride;
        unsigned int d_flags;
    };

    struct SStructInfo
    {
        VPP_INLINE SStructInfo ( const std::type_index& index, spv::Decoration decoration ) :
            d_index ( index ),
            d_typeId ( 0 ),
            d_decoration ( decoration ),
            d_bDefined ( false ),
            d_currentMember ( 0 ),
            d_pBase ( 0 )
        {}

        std::type_index d_index;
        std::vector< SMemberInfo > d_memberInfos;
        KId d_typeId;
        spv::Decoration d_decoration;
        bool d_bDefined;
        unsigned int d_currentMember;
        const unsigned char* d_pBase;
    };

    struct SVariableInfo
    {
        VPP_INLINE SVariableInfo() :
            d_variableId ( 0 ),
            d_typeId ( 0 ),
            d_pointerId ( 0 )
        {}

        VPP_INLINE SVariableInfo ( KId variableId, KId typeId ) :
            d_variableId ( variableId ),
            d_typeId ( typeId ),
            d_pointerId ( 0 )
        {}

        VPP_INLINE operator bool() const
        {
            return d_variableId != 0;
        }

        KId d_variableId;
        KId d_typeId;
        KId d_pointerId;
    };

    VPP_DLLAPI void pushStructType (
        const std::type_index& index, spv::Decoration decoration );
    VPP_DLLAPI SStructInfo& currentStructType();
    VPP_DLLAPI void popStructType();
    VPP_DLLAPI const SStructInfo* findStructType (
        const std::type_index& index, spv::Decoration decoration );
    
    void setStructDecoration ( spv::Decoration decoration );
    spv::Decoration getStructDecoration() const;

    KId getBuiltinFunctions() const;

    template< class ScalarT >
    KId getArrayIndex ( const ScalarT& v );

    KId createDescriptor ( const KId& typeId, std::uint32_t set, std::uint32_t binding );
    KId createArrayedDescriptor ( const KId& typeId, std::uint32_t set, std::uint32_t binding, std::uint32_t count );
    KId loadDescriptorFromArray ( const KId& arrayId, const KId& indexId, const KId& typeId );

    VPP_DLLAPI KId getArrayIndex ( int v );
    VPP_DLLAPI KId getArrayIndex ( unsigned int v );

    VPP_DLLAPI SVariableInfo& getShaderScopedVariable ( const KShaderScopedVariable* pVariable );

    VPP_DLLAPI KId expandConstant ( KId constant, int vectorSize );

    void setShaderInputSize ( int s );
    int getShaderInputSize() const;

    void setShaderOutputSize ( int s );
    int getShaderOutputSize() const;

    VPP_DLLAPI spv::Decoration getPrecision() const;
    VPP_DLLAPI void addMemoryFlags ( const KId& idVariable, std::uint32_t flags );
    VPP_DLLAPI KId getScopedRValue ( const KId& lValue );
    VPP_DLLAPI spv::ImageFormat validateImageFormat ( spv::ImageFormat fmt );

private:
    Device d_hDevice;
    VkShaderStageFlagBits d_stage;

    typedef std::list< If > IfStack;
    IfStack d_ifStack;

    typedef std::list< Switch > SwitchStack;
    SwitchStack d_switchStack;

    typedef std::list< detail::KScope > ScopeStack;
    ScopeStack d_scopeStack;

    typedef std::list< LoopBlocks > LoopStack;
    LoopStack d_loopStack;

    typedef std::list< detail::KForRange > ForStack;
    ForStack d_forStack;

    detail::KFunction* d_pCurrentFunction;
    const char* d_pCurrentFunctionName;

    typedef std::vector< detail::KParameter* > Parameters;
    Parameters d_currentFunctionParameters;

    typedef std::set< KId > InputOutputVariables;
    InputOutputVariables d_inputOutputVariables;

    typedef std::map< const KShaderScopedVariable*, SVariableInfo > Variable2Info;
    Variable2Info d_variable2info;

    typedef std::pair< std::type_index, spv::Decoration > StructTypeKey;
    typedef std::map< StructTypeKey, SStructInfo > StructType2Info;
    typedef std::list< SStructInfo* > StructTypeStack;
    StructType2Info d_structType2Info;
    StructTypeStack d_structTypeStack;

    spv::Decoration d_structDecoration;

    KId d_builtinFunctions;

    int d_shaderInputSize;
    int d_shaderOutputSize;

    spv::StorageClass d_currentVariableStorageClass;

    static thread_local KShaderTranslator* s_pThis;
};

// -----------------------------------------------------------------------------

VPP_INLINE const Device& KShaderTranslator :: getDevice() const
{
    return d_hDevice;
}

// -----------------------------------------------------------------------------

VPP_INLINE VkShaderStageFlagBits KShaderTranslator :: getStage() const
{
    return d_stage;
}

// -----------------------------------------------------------------------------

VPP_INLINE void KShaderTranslator :: setStructDecoration ( spv::Decoration decoration )
{
    d_structDecoration = decoration;
}

// -----------------------------------------------------------------------------

VPP_INLINE spv::Decoration KShaderTranslator :: getStructDecoration() const
{
    return d_structDecoration;
}

// -----------------------------------------------------------------------------

VPP_INLINE KId KShaderTranslator :: getBuiltinFunctions() const
{
    return d_builtinFunctions;
}

// -----------------------------------------------------------------------------

template< class ScalarT >
VPP_INLINE KId KShaderTranslator :: getArrayIndex ( const ScalarT& v )
{
    return v.id();
}

// -----------------------------------------------------------------------------

VPP_INLINE KId KShaderTranslator :: createDescriptor (
    const KId& typeId,
    std::uint32_t set, std::uint32_t binding )
{
    const KId pointerId = KId ( createVariable (
        spv::StorageClassUniformConstant, typeId ) );

    addDecoration ( pointerId, spv::DecorationDescriptorSet, static_cast< int >( set ) );
    addDecoration ( pointerId, spv::DecorationBinding, static_cast< int >( binding ) );

    return pointerId;
}

// -----------------------------------------------------------------------------

VPP_INLINE KId KShaderTranslator :: createArrayedDescriptor (
    const KId& typeId,
    std::uint32_t set, std::uint32_t binding, std::uint32_t count )
{
    const KId pointerId = KId ( createVariable (
        spv::StorageClassUniformConstant, makeArrayType (
            typeId, getArrayIndex ( count ), 0 ) ) );

    addDecoration ( pointerId, spv::DecorationDescriptorSet, static_cast< int >( set ) );
    addDecoration ( pointerId, spv::DecorationBinding, static_cast< int >( binding ) );

    return pointerId;
}

// -----------------------------------------------------------------------------

VPP_INLINE KId KShaderTranslator :: loadDescriptorFromArray (
    const KId& arrayId, const KId& indexId, const KId& typeId )
{
    clearAccessChain();
    setAccessChainLValue ( arrayId );
    accessChainPush ( indexId );
    return KId ( accessChainLoad ( getPrecision(), typeId ) );
}

// -----------------------------------------------------------------------------

VPP_INLINE void KShaderTranslator :: setShaderInputSize ( int s )
{
    d_shaderInputSize = s;
}

// -----------------------------------------------------------------------------

VPP_INLINE int KShaderTranslator :: getShaderInputSize() const
{
    return d_shaderInputSize;
}

// -----------------------------------------------------------------------------

VPP_INLINE void KShaderTranslator :: setShaderOutputSize ( int s )
{
    d_shaderOutputSize = s;
}

// -----------------------------------------------------------------------------

VPP_INLINE int KShaderTranslator :: getShaderOutputSize() const
{
    return d_shaderOutputSize;
}

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------

#endif // INC_VPPSHADERLANGUAGETRANSLATOR_HPP
