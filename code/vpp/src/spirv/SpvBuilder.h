//
//Copyright (C) 2014-2015 LunarG, Inc.
//Copyright (C) 2015-2016 Google, Inc.
//
//All rights reserved.
//
//Redistribution and use in source and binary forms, with or without
//modification, are permitted provided that the following conditions
//are met:
//
//    Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
//    Redistributions in binary form must reproduce the above
//    copyright notice, this list of conditions and the following
//    disclaimer in the documentation and/or other materials provided
//    with the distribution.
//
//    Neither the name of 3Dlabs Inc. Ltd. nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
//THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//POSSIBILITY OF SUCH DAMAGE.

//
// "Builder" is an interface to fully build SPIR-V IR.   Allocate one of
// these to build (a thread safe) internal SPIR-V representation (IR),
// and then dump it as a binary stream according to the SPIR-V specification.
//
// A Builder has a 1:1 relationship with a SPIR-V module.
//

#ifndef SpvBuilder_H
#define SpvBuilder_H

#include "../../include/vppDefines.hpp"

#include "Logger.h"
#include "spirv.hpp"
#include "spvIR.h"

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <sstream>
#include <stack>

// -----------------------------------------------------------------------------
namespace spv {
// -----------------------------------------------------------------------------

class Builder
{
public:
    Builder ( unsigned int userNumber, SpvBuildLogger* logger );
    virtual ~Builder();

    static const int maxMatrixSize = 4;

    void setSource ( spv::SourceLanguage lang, int version );
    void addSourceExtension ( const char* ext );

    Id import ( const char* );

    void setMemoryModel ( spv::AddressingModel addr, spv::MemoryModel mem );
    
    Id getUniqueId();
    Id getUniqueIds ( int numIds );

    // For creating new types (will return old type if the requested one was already made).
    VPP_DLLAPI Id makeVoidType();
    VPP_DLLAPI Id makeBoolType();
    VPP_DLLAPI Id makePointer(StorageClass, Id type);
    VPP_DLLAPI Id makeIntegerType(int width, bool hasSign);   // generic
    VPP_DLLAPI Id makeIntType(int width) { return makeIntegerType(width, true); }
    VPP_DLLAPI Id makeUintType(int width) { return makeIntegerType(width, false); }
    VPP_DLLAPI Id makeFloatType(int width);
    VPP_DLLAPI Id makeStructType(const std::vector<Id>& members, const char*);
    VPP_DLLAPI Id makeStructResultType(Id type0, Id type1);
    VPP_DLLAPI Id makeVectorType(Id component, int size);
    VPP_DLLAPI Id makeMatrixType(Id component, int cols, int rows);
    VPP_DLLAPI Id makeArrayType(Id element, Id sizeId, int stride);  // 0 stride means no stride decoration
    VPP_DLLAPI Id makeRuntimeArray(Id element);
    VPP_DLLAPI Id makeFunctionType(Id returnType, const std::vector<Id>& paramTypes);
    VPP_DLLAPI Id makeImageType(Id sampledType, Dim, bool depth, bool arrayed, bool ms, unsigned sampled, ImageFormat format);
    VPP_DLLAPI Id makeSamplerType();
    VPP_DLLAPI Id makeSampledImageType(Id imageType);

    Id getTypeId ( Id resultId ) const;
    Id getDerefTypeId ( Id resultId ) const;
    Op getOpCode ( Id id ) const;
    Op getTypeClass ( Id typeId ) const;
    Op getMostBasicTypeClass ( Id typeId ) const;
    int getNumComponents ( Id resultId ) const;
    VPP_DLLAPI int getNumTypeConstituents ( Id typeId ) const;
    VPP_DLLAPI int getNumTypeBits ( Id typeId ) const;
    int getNumTypeComponents ( Id typeId ) const;
    VPP_DLLAPI Id getScalarTypeId ( Id typeId ) const;
    Id getContainedTypeId ( Id typeId ) const;
    Id getContainedTypeId ( Id typeId, int ) const;
    StorageClass getTypeStorageClass ( Id typeId ) const;
    ImageFormat getImageTypeFormat ( Id typeId ) const;

    bool isPointer(Id resultId)      const { return isPointerType(getTypeId(resultId)); }
    bool isScalar(Id resultId)       const { return isScalarType(getTypeId(resultId)); }
    bool isVector(Id resultId)       const { return isVectorType(getTypeId(resultId)); }
    bool isMatrix(Id resultId)       const { return isMatrixType(getTypeId(resultId)); }
    bool isAggregate(Id resultId)    const { return isAggregateType(getTypeId(resultId)); }
    bool isSampledImage(Id resultId) const { return isSampledImageType(getTypeId(resultId)); }

    bool isBoolType(Id typeId)         const { return groupedTypes[OpTypeBool].size() > 0 && typeId == groupedTypes[OpTypeBool].back()->getResultId(); }
    bool isPointerType(Id typeId)      const { return getTypeClass(typeId) == OpTypePointer; }
    bool isScalarType(Id typeId)       const { return getTypeClass(typeId) == OpTypeFloat  || getTypeClass(typeId) == OpTypeInt || getTypeClass(typeId) == OpTypeBool; }
    bool isVectorType(Id typeId)       const { return getTypeClass(typeId) == OpTypeVector; }
    bool isMatrixType(Id typeId)       const { return getTypeClass(typeId) == OpTypeMatrix; }
    bool isStructType(Id typeId)       const { return getTypeClass(typeId) == OpTypeStruct; }
    bool isArrayType(Id typeId)        const { return getTypeClass(typeId) == OpTypeArray; }
    bool isAggregateType(Id typeId)    const { return isArrayType(typeId) || isStructType(typeId); }
    bool isImageType(Id typeId)        const { return getTypeClass(typeId) == OpTypeImage; }
    bool isSamplerType(Id typeId)      const { return getTypeClass(typeId) == OpTypeSampler; }
    bool isSampledImageType(Id typeId) const { return getTypeClass(typeId) == OpTypeSampledImage; }

    bool isConstantOpCode(Op opcode) const;
    bool isSpecConstantOpCode(Op opcode) const;
    bool isConstant(Id resultId) const { return isConstantOpCode(getOpCode(resultId)); }
    bool isConstantScalar(Id resultId) const { return getOpCode(resultId) == OpConstant; }
    bool isSpecConstant(Id resultId) const { return isSpecConstantOpCode(getOpCode(resultId)); }
    unsigned int getConstantScalar(Id resultId) const { return module.getInstruction(resultId)->getImmediateOperand(0); }
    StorageClass getStorageClass(Id resultId) const { return getTypeStorageClass(getTypeId(resultId)); }

    int getTypeNumColumns(Id typeId) const
    {
        assert(isMatrixType(typeId));
        return getNumTypeConstituents(typeId);
    }

    int getNumColumns(Id resultId) const
    {
        return getTypeNumColumns(getTypeId(resultId));
    }
    
    int getTypeNumRows(Id typeId) const
    {
        assert(isMatrixType(typeId));
        return getNumTypeComponents(getContainedTypeId(typeId));
    }

    int getNumRows(Id resultId) const
    {
        return getTypeNumRows(getTypeId(resultId));
    }

    Dim getTypeDimensionality(Id typeId) const
    {
        assert(isImageType(typeId));
        return (Dim)module.getInstruction(typeId)->getImmediateOperand(1);
    }

    Id getImageType(Id resultId) const
    {
        Id typeId = getTypeId(resultId);
        assert(isImageType(typeId) || isSampledImageType(typeId));
        return isSampledImageType(typeId) ? module.getInstruction(typeId)->getIdOperand(0) : typeId;
    }

    bool isArrayedImageType(Id typeId) const
    {
        assert(isImageType(typeId));
        return module.getInstruction(typeId)->getImmediateOperand(3) != 0;
    }

    // For making new constants (will return old constant if the requested one was already made).
    VPP_DLLAPI Id makeBoolConstant(bool b, bool specConstant = false);
    VPP_DLLAPI Id makeIntConstant(int i, bool specConstant = false)         { return makeIntConstant(makeIntType(32),  (unsigned)i, specConstant); }
    VPP_DLLAPI Id makeUintConstant(unsigned u, bool specConstant = false)   { return makeIntConstant(makeUintType(32),           u, specConstant); }
    VPP_DLLAPI Id makeInt64Constant(long long i, bool specConstant = false)            { return makeInt64Constant(makeIntType(64),  (unsigned long long)i, specConstant); }
    VPP_DLLAPI Id makeUint64Constant(unsigned long long u, bool specConstant = false)  { return makeInt64Constant(makeUintType(64),                     u, specConstant); }
    VPP_DLLAPI Id makeFloatConstant(float f, bool specConstant = false);
    VPP_DLLAPI Id makeDoubleConstant(double d, bool specConstant = false);
    VPP_DLLAPI Id makeFloat16Constant(float f, bool specConstant = false);

    // Turn the array of constants into a proper spv constant of the requested type.
    VPP_DLLAPI Id makeCompositeConstant(Id type, std::vector<Id>& comps, bool specConst = false);

    // Methods for adding information outside the CFG.
    Instruction* addEntryPoint(ExecutionModel, Function*, const char* name);
    VPP_DLLAPI void addExecutionMode(Function*, ExecutionMode mode, int value1 = -1, int value2 = -1, int value3 = -1);
    void addName(Id, const char* name);
    void addMemberName(Id, int member, const char* name);
    void addLine(Id target, Id fileName, int line, int column);
    VPP_DLLAPI void addDecoration(Id, Decoration, int num = -1);
    VPP_DLLAPI void addMemberDecoration(Id, unsigned int member, Decoration, int num = -1);

    // At the end of what block do the next create*() instructions go?
    void setBuildPoint(Block* bp)
    {
        buildPoint = bp;
    }

    Block* getBuildPoint() const
    {
        return buildPoint;
    }

    // Make the entry-point function. The returned pointer is only valid
    // for the lifetime of this builder.
    Function* makeEntrypoint(const char*);

    // Make a shader-style function, and create its entry block if entry is non-zero.
    // Return the function, pass back the entry.
    // The returned pointer is only valid for the lifetime of this builder.
    Function* makeFunctionEntry(Decoration precision, Id returnType, const char* name, const std::vector<Id>& paramTypes,
                                const std::vector<Decoration>& precisions, Block **entry = 0);

    // Create a return. An 'implicit' return is one not appearing in the source
    // code.  In the case of an implicit return, no post-return block is inserted.
    VPP_DLLAPI void makeReturn(bool implicit, Id retVal = 0);

    // Generate all the code needed to finish up a function.
    void leaveFunction();

    // Create a discard.
    void makeDiscard();

    // Create a global or function local or IO variable.
    VPP_DLLAPI Id createVariable(StorageClass, Id type, const char* name = 0);

    // Create an intermediate with an undefined value.
    Id createUndefined(Id type);

    // Store into an Id and return the l-value
    VPP_DLLAPI void createStore(Id rValue, Id lValue);

    // Load from an Id and return it
    VPP_DLLAPI Id createLoad(Id lValue);

    // Create an OpAccessChain instruction
    VPP_DLLAPI Id createAccessChain(StorageClass, Id base, std::vector<Id>& offsets);

    // Create an OpArrayLength instruction
    Id createArrayLength(Id base, unsigned int member);

    // Create an OpCompositeExtract instruction
    Id createCompositeExtract(Id composite, Id typeId, unsigned index);
    Id createCompositeExtract(Id composite, Id typeId, std::vector<unsigned>& indexes);
    Id createCompositeInsert(Id object, Id composite, Id typeId, unsigned index);
    Id createCompositeInsert(Id object, Id composite, Id typeId, std::vector<unsigned>& indexes);

    Id createVectorExtractDynamic(Id vector, Id typeId, Id componentIndex);
    Id createVectorInsertDynamic(Id vector, Id typeId, Id component, Id componentIndex);

    VPP_DLLAPI void createNoResultOp(Op);
    VPP_DLLAPI void createNoResultOp(Op, Id operand);
    VPP_DLLAPI void createNoResultOp(Op, const std::vector<Id>& operands);
    VPP_DLLAPI void createControlBarrier(Scope execution, Scope memory, MemorySemanticsMask);
    VPP_DLLAPI void createMemoryBarrier(unsigned executionScope, unsigned memorySemantics);
    VPP_DLLAPI Id createUnaryOp(Op, Id typeId, Id operand);
    VPP_DLLAPI Id createBinOp(Op, Id typeId, Id operand1, Id operand2);
    VPP_DLLAPI Id createTriOp(Op, Id typeId, Id operand1, Id operand2, Id operand3);
    VPP_DLLAPI Id createOp(Op, Id typeId, const std::vector<Id>& operands);
    VPP_DLLAPI Id createFunctionCall(spv::Function*, std::vector<spv::Id>&);
    VPP_DLLAPI Id createSpecConstantOp(Op, Id typeId, const std::vector<spv::Id>& operands, const std::vector<unsigned>& literals);

    // Take an rvalue (source) and a set of channels to extract from it to
    // make a new rvalue, which is returned.
    Id createRvalueSwizzle(Decoration precision, Id typeId, Id source, std::vector<unsigned>& channels);

    // Take a copy of an lvalue (target) and a source of components, and set the
    // source components into the lvalue where the 'channels' say to put them.
    // An updated version of the target is returned.
    // (No true lvalue or stores are used.)
    Id createLvalueSwizzle(Id typeId, Id target, Id source, std::vector<unsigned>& channels);

    // If both the id and precision are valid, the id
    // gets tagged with the requested precision.
    // The passed in id is always the returned id, to simplify use patterns.
    Id setPrecision(Id id, Decoration precision)
    {
        if (precision != NoPrecision && id != NoResult)
            addDecoration(id, precision);

        return id;
    }

    // Can smear a scalar to a vector for the following forms:
    //   - promoteScalar(scalar, vector)  // smear scalar to width of vector
    //   - promoteScalar(vector, scalar)  // smear scalar to width of vector
    //   - promoteScalar(pointer, scalar) // smear scalar to width of what pointer points to
    //   - promoteScalar(scalar, scalar)  // do nothing
    // Other forms are not allowed.
    //
    // Generally, the type of 'scalar' does not need to be the same type as the components in 'vector'.
    // The type of the created vector is a vector of components of the same type as the scalar.
    //
    // Note: One of the arguments will change, with the result coming back that way rather than 
    // through the return value.
    void promoteScalar(Decoration precision, Id& left, Id& right);

    // Make a value by smearing the scalar to fill the type.
    // vectorType should be the correct type for making a vector of scalarVal.
    // (No conversions are done.)
    Id smearScalar(Decoration precision, Id scalarVal, Id vectorType);

    // Create a call to a built-in function.
    VPP_DLLAPI Id createBuiltinCall(Id resultType, Id builtins, int entryPoint, std::vector<Id>& args);

    // List of parameters used to create a texture operation
    struct TextureParameters {
        Id sampler;
        Id coords;
        Id bias;
        Id lod;
        Id Dref;
        Id offset;
        Id offsets;
        Id gradX;
        Id gradY;
        Id sample;
        Id component;
        Id texelOut;
        Id lodClamp;
    };

    // Select the correct texture operation based on all inputs, and emit the correct instruction
    VPP_DLLAPI Id createTextureCall(Decoration precision, Id resultType, bool sparse, bool fetch, bool proj, bool gather, bool noImplicit, const TextureParameters&);

    // Emit the OpTextureQuery* instruction that was passed in.
    // Figure out the right return value and type, and return it.
    VPP_DLLAPI Id createTextureQueryCall(Op, const TextureParameters&);

    Id createSamplePositionCall(Decoration precision, Id, Id);

    Id createBitFieldExtractCall(Decoration precision, Id, Id, Id, bool isSigned);
    Id createBitFieldInsertCall(Decoration precision, Id, Id, Id, Id);

    // Reduction comparison for composites:  For equal and not-equal resulting in a scalar.
    Id createCompositeCompare(Decoration precision, Id, Id, bool /* true if for equal, false if for not-equal */);

    // OpCompositeConstruct
    VPP_DLLAPI Id createCompositeConstruct(Id typeId, std::vector<Id>& constituents);

    // vector or scalar constructor
    VPP_DLLAPI Id createConstructor(Decoration precision, const std::vector<Id>& sources, Id resultTypeId);

    // matrix constructor
    VPP_DLLAPI Id createMatrixConstructor(Decoration precision, const std::vector<Id>& sources, Id constructee);

    // Helper to use for building nested control flow with if-then-else.
    class If {
    public:
        If(Id condition, Builder& builder);
        ~If() {}

        void makeBeginElse();
        void makeEndIf();

    private:
        If(const If&);
        If& operator=(If&);

        Builder& builder;
        Id condition;
        Function* function;
        Block* headerBlock;
        Block* thenBlock;
        Block* elseBlock;
        Block* mergeBlock;
    };

    // Switch generator
    struct SwitchCase
    {
        VPP_INLINE SwitchCase ( Block* b ) : value ( 0 ), block ( b ) {}
        VPP_INLINE SwitchCase ( int v, Block* b ) : value ( v ), block ( b ) {}
        VPP_INLINE bool operator< ( const SwitchCase& rhs ) const { return value < rhs.value; }
        int value;
        Block* block;
    };
    
    class Switch
    {
    public:
        Switch ( Id _condition, Builder& _builder );
        ~Switch() {}

        void addCase ( int value );
        void addDefault();
        void createSwitch();

    private:
        Switch(const Switch&);
        Switch& operator=(Switch&);

        Builder& builder;
        Id condition;
        Function* function;
        Block* headerBlock;
        Block* mergeBlock;
        Block* defaultBlock;
        Instruction* switchInst;

        std::vector< SwitchCase > cases;
    };

    // Make a switch statement.  A switch has 'numSegments' of pieces of code, not containing
    // any case/default labels, all separated by one or more case/default labels.  Each possible
    // case value v is a jump to the caseValues[v] segment.  The defaultSegment is also in this
    // number space.  How to compute the value is given by 'condition', as in switch(condition).
    //
    // The SPIR-V Builder will maintain the stack of post-switch merge blocks for nested switches.
    //
    // Use a defaultSegment < 0 if there is no default segment (to branch to post switch).
    //
    // Returns the right set of basic blocks to start each code segment with, so that the caller's
    // recursion stack can hold the memory for it.
    //
    void makeSwitch(Id condition, int numSegments, std::vector<int>& caseValues, std::vector<int>& valueToSegment, int defaultSegment,
                    std::vector<Block*>& segmentBB);  // return argument

    // Add a branch to the innermost switch's merge block.
    VPP_DLLAPI void addSwitchBreak();

    // Move to the next code segment, passing in the return argument in makeSwitch()
    void nextSwitchSegment(std::vector<Block*>& segmentBB, int segment);

    // Finish off the innermost switch.
    void endSwitch(std::vector<Block*>& segmentBB);

    struct LoopBlocks
    {
        Block &head, &body, &merge, &continue_target;
    };

    // Start a new loop and prepare the builder to generate code for it.  Until
    // closeLoop() is called for this loop, createLoopContinue() and
    // createLoopExit() will target its corresponding blocks.
    LoopBlocks& makeNewLoop();

    // Create a new block in the function containing the build point.  Memory is
    // owned by the function object.
    VPP_DLLAPI Block& makeNewBlock();

    // Add a branch to the continue_target of the current (innermost) loop.
    void createLoopContinue();

    // Add an exit (e.g. "break") from the innermost loop that we're currently
    // in.
    void createLoopExit();

    // Close the innermost loop that you're in
    VPP_DLLAPI void closeLoop();

    //
    // Access chain design for an R-Value vs. L-Value:
    //
    // There is a single access chain the builder is building at
    // any particular time.  Such a chain can be used to either to a load or
    // a store, when desired.
    //
    // Expressions can be r-values, l-values, or both, or only r-values:
    //    a[b.c].d = ....  // l-value
    //    ... = a[b.c].d;  // r-value, that also looks like an l-value
    //    ++a[b.c].d;      // r-value and l-value
    //    (x + y)[2];      // r-value only, can't possibly be l-value
    //
    // Computing an r-value means generating code.  Hence,
    // r-values should only be computed when they are needed, not speculatively.
    //
    // Computing an l-value means saving away information for later use in the compiler,
    // no code is generated until the l-value is later dereferenced.  It is okay
    // to speculatively generate an l-value, just not okay to speculatively dereference it.
    //
    // The base of the access chain (the left-most variable or expression
    // from which everything is based) can be set either as an l-value
    // or as an r-value.  Most efficient would be to set an l-value if one
    // is available.  If an expression was evaluated, the resulting r-value
    // can be set as the chain base.
    //
    // The users of this single access chain can save and restore if they
    // want to nest or manage multiple chains.
    //

    struct AccessChain
    {
        Id base;                       // for l-values, pointer to the base object, for r-values, the base object
        std::vector<Id> indexChain;
        Id instr;                      // cache the instruction that generates this access chain
        std::vector<unsigned> swizzle; // each std::vector element selects the next GLSL component number
        Id component;                  // a dynamic component index, can coexist with a swizzle, done after the swizzle, NoResult if not present
        Id preSwizzleBaseType;         // dereferenced type, before swizzle or component is applied; NoType unless a swizzle or component is present
        bool isRValue;                 // true if 'base' is an r-value, otherwise, base is an l-value
    };

    //
    // the SPIR-V builder maintains a single active chain that
    // the following methods operated on
    //

    // for external save and restore
    AccessChain getAccessChain()
    {
        return accessChain;
    }

    void setAccessChain(AccessChain newChain)
    {
        accessChain = newChain;
    }

    // clear accessChain
    VPP_DLLAPI void clearAccessChain();

    // set new base as an l-value base
    void setAccessChainLValue(Id lValue)
    {
        assert(isPointer(lValue));
        accessChain.base = lValue;
    }

    // set new base value as an r-value
    void setAccessChainRValue(Id rValue)
    {
        accessChain.isRValue = true;
        accessChain.base = rValue;
    }

    // push offset onto the end of the chain
    void accessChainPush(Id offset)
    {
        accessChain.indexChain.push_back(offset);
    }

    // push new swizzle onto the end of any existing swizzle, merging into a single swizzle
    VPP_DLLAPI void accessChainPushSwizzle(std::vector<unsigned>& swizzle, Id preSwizzleBaseType);

    // push a variable component selection onto the access chain; supporting only one, so unsided
    void accessChainPushComponent(Id component, Id preSwizzleBaseType)
    {
        accessChain.component = component;
        if (accessChain.preSwizzleBaseType == NoType)
            accessChain.preSwizzleBaseType = preSwizzleBaseType;
    }

    // use accessChain and swizzle to store value
    VPP_DLLAPI void accessChainStore(Id rvalue);

    // use accessChain and swizzle to load an r-value
    VPP_DLLAPI Id accessChainLoad(Decoration precision, Id ResultType);

    // get the direct pointer for an l-value
    VPP_DLLAPI Id accessChainGetLValue();

    // Get the inferred SPIR-V type of the result of the current access chain,
    // based on the type of the base and the chain of dereferences.
    Id accessChainGetInferredType();

    // Remove OpDecorate instructions whose operands are defined in unreachable
    // blocks.
    void eliminateDeadDecorations();
    void dump(std::vector<unsigned int>&) const;

    VPP_DLLAPI void createBranch(Block* block);
    VPP_DLLAPI void createConditionalBranch(Id condition, Block* thenBlock, Block* elseBlock);
    VPP_DLLAPI void createLoopMerge(Block* mergeBlock, Block* continueBlock, unsigned int control);

    // Sets to generate opcode for specialization constants.
    void setToSpecConstCodeGenMode() { generatingOpCodeForSpecConst = true; }
    // Sets to generate opcode for non-specialization constants (normal mode).
    void setToNormalCodeGenMode() { generatingOpCodeForSpecConst = false; }
    // Check if the builder is generating code for spec constants.
    bool isInSpecConstCodeGenMode() { return generatingOpCodeForSpecConst; }

 protected:
    void addCapability ( spv::Capability cap );

    VPP_DLLAPI Id makeIntConstant(Id typeId, unsigned value, bool specConstant);
    VPP_DLLAPI Id makeInt64Constant(Id typeId, unsigned long long value, bool specConstant);
    Id findScalarConstant(Op typeClass, Op opcode, Id typeId, unsigned value) const;
    Id findScalarConstant(Op typeClass, Op opcode, Id typeId, unsigned v1, unsigned v2) const;
    Id findCompositeConstant(Op typeClass, std::vector<Id>& comps) const;
    Id collapseAccessChain();
    void transferAccessChainSwizzle(bool dynamic);
    void simplifyAccessChainSwizzle();
    void createAndSetNoPredecessorBlock(const char*);
    void createSelectionMerge(Block* mergeBlock, unsigned int control);
    void dumpInstructions(std::vector<unsigned int>&, const std::vector<std::unique_ptr<Instruction> >&) const;

protected:
    SourceLanguage source;
    int sourceVersion;
    std::vector<const char*> extensions;
    AddressingModel addressModel;
    MemoryModel memoryModel;
    std::set<spv::Capability> capabilities;
    int builderNumber;
    Module module;
    Block* buildPoint;
    Id uniqueId;
    Function* mainFunction;
    bool generatingOpCodeForSpecConst;
    AccessChain accessChain;

    // special blocks of instructions for output
    std::vector< std::unique_ptr<Instruction> > imports;
    std::vector< std::unique_ptr<Instruction> > entryPoints;
    std::vector< std::unique_ptr<Instruction> > executionModes;
    std::vector< std::unique_ptr<Instruction> > names;
    std::vector< std::unique_ptr<Instruction> > lines;
    std::vector< std::unique_ptr<Instruction> > decorations;
    std::vector< std::unique_ptr<Instruction> > constantsTypesGlobals;
    std::vector< std::unique_ptr<Instruction> > externals;
    std::vector< std::unique_ptr<Function> > functions;

     // not output, internally used for quick & dirty canonical (unique) creation
    std::vector<Instruction*> groupedConstants[OpConstant];  // all types appear before OpConstant
    std::vector<Instruction*> groupedTypes[OpConstant];

    // stack of switches
    std::stack<Block*> switchMerges;

    // Our loop stack.
    std::stack<LoopBlocks> loops;

    // The stream for outputing warnings and errors.
    SpvBuildLogger* logger;
};  // end Builder class

// -----------------------------------------------------------------------------

VPP_INLINE void Builder :: setSource ( spv::SourceLanguage lang, int version )
{
    source = lang;
    sourceVersion = version;
}

// -----------------------------------------------------------------------------

VPP_INLINE void Builder :: addSourceExtension ( const char* ext )
{
    extensions.push_back ( ext );
}

// -----------------------------------------------------------------------------

VPP_INLINE void Builder :: setMemoryModel ( spv::AddressingModel addr, spv::MemoryModel mem )
{
    addressModel = addr;
    memoryModel = mem;
}

// -----------------------------------------------------------------------------

VPP_INLINE void Builder :: addCapability ( spv::Capability cap )
{
    capabilities.insert ( cap );
}

// -----------------------------------------------------------------------------

VPP_INLINE Id Builder :: getUniqueId()
{
    // To get a new <id> for anything needing a new one.
    return ++uniqueId;
}

// -----------------------------------------------------------------------------

VPP_INLINE Id Builder :: getUniqueIds ( int numIds )
{
    // To get a set of new <id>s, e.g., for a set of function parameters
    Id id = uniqueId + 1;
    uniqueId += numIds;
    return id;
}

// -----------------------------------------------------------------------------

VPP_INLINE Id Builder :: getTypeId ( Id resultId ) const
{
    return module.getTypeId(resultId);
}

// -----------------------------------------------------------------------------

VPP_INLINE Op Builder :: getOpCode ( Id id ) const
{
    return module.getInstruction(id)->getOpCode();
}

// -----------------------------------------------------------------------------

VPP_INLINE Op Builder :: getTypeClass ( Id typeId ) const
{
    return getOpCode(typeId);
}

// -----------------------------------------------------------------------------

VPP_INLINE int Builder :: getNumComponents ( Id resultId ) const
{
    return getNumTypeComponents(getTypeId(resultId));
}

// -----------------------------------------------------------------------------

VPP_INLINE int Builder :: getNumTypeComponents ( Id typeId ) const
{
    return getNumTypeConstituents(typeId);
}

// -----------------------------------------------------------------------------

VPP_INLINE StorageClass Builder :: getTypeStorageClass ( Id typeId ) const
{
    return module.getStorageClass ( typeId );
}

// -----------------------------------------------------------------------------

VPP_INLINE ImageFormat Builder :: getImageTypeFormat ( Id typeId ) const
{
    return (ImageFormat)module.getInstruction(typeId)->getImmediateOperand(6);
}

// -----------------------------------------------------------------------------
}  // end spv namespace
// -----------------------------------------------------------------------------

#endif // SpvBuilder_H
