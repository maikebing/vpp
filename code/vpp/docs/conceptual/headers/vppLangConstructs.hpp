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

/** \file */

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

/**
    \brief Creates the \b if clause of the conditional construct.
    
    Place in the shader code to create a conditional construct. Use a
    conditional expression as the argument of If(). At the end of conditional
    block, place the closing clause Fi(). All instructions between If() and Fi()
    will be executed only if the expression evaluates to \b true. 

    Note that If() and Fi() are actually function calls (as C++ instructions
    cannot be overloaded). You must put semicolon after them. Also pay attention
    to the fact that the code block between If() and Fi() does not create C++ scope.
    If you want to have C++ scope there, you must put the braces explicitly around
    the code. It is recommended if you create new identifiers inside the block,
    although not required.

    \code
        // without scope
        If ( booleanExpression );
            // instructions...
            // instructions...
        Fi();

        // with scope
        If ( booleanExpression );
        {
            // instructions...
            // instructions...
        }
        Fi();

    \endcode
*/
void If ( Bool v );

/**
    \brief Creates the \b else clause of the conditional construct.
    
    Place in the shader code between If() and Fi() clauses to create a conditional
    construct with \b else branch. All instructions between If() and Else()
    will be executed only if the expression evaluates to \b true. Instructions
    between Else() and Fi() will be executed only if the expression evaluates
    to \b false.

    \code
        // without scope
        If ( booleanExpression );
            // instructions...
            // instructions...
        Else();
            // instructions...
            // instructions...
        Fi();

        // with scope
        If ( booleanExpression );
        {
            // instructions...
            // instructions...
        }
        Else();
        {
            // instructions...
            // instructions...
        }
        Fi();

    \endcode
*/
void Else();

/**
    \brief Creates the ending clause of the conditional construct.
    
    Place in the shader code to end the conditional construct: If() ... Fi()
    or If() ... Else() ... Fi().
*/
void Fi();

// -----------------------------------------------------------------------------
/**
    \brief Creates the \b do clause of the loop construct.
    
    Place in the shader code to create a loop construct. Always use with While()
    clause. At the end of the looped block, place the closing clause Od(). All
    instructions between While() and Od() will be executed as long as the expression
    in While() evaluates to \b true.

    Usually a loop control variable is used to limit iterations. You must declare
    the variable explicitly, somewhere before the Do() clause. Also do not forget
    to increment the variable, or make the loop finish by other means. Infinite loop
    will cause unpredictable behavior, most likely the GPU driver will restart.
    In case you program hangs due to infinite loop on GPU, kill it explicitly
    (e.g. with Task Manager).

    Note that Do(), While() and Od() are actually function calls (as C++ instructions
    cannot be overloaded). You must put semicolon after them. Also pay attention
    to the fact that the code block between While() and Od() does not create C++ scope.
    If you want to have C++ scope there, you must put the braces explicitly around
    the code. It is recommended if you create new identifiers inside the block,
    although not required.

    \code
        VInt i;

        // without scope
        i = 0; Do(); While ( i < nr_iterations );
            // instructions...
            // instructions...
            ++i;
        Od();

        // with scope
        i = 0; Do(); While ( i < nr_iterations );
        {
            // instructions...
            // instructions...
            ++i;
        }
        Od();

    \endcode
*/

void Do();

/**
    \brief Creates the \b while clause of the loop construct.
    
    Place in the shader code to create a loop construct. Always use with Do()
    clause. At the end of the looped block, place the closing clause Od(). All
    instructions between While() and Od() will be executed as long as the expression
    in While() evaluates to \b true.

    See Do() documentation for more details.

*/

void While ( Bool v );

/**
    \brief Creates the closing clause of the loop construct.
    
    Place in the shader code to create a loop construct. Always use with Do() .. While()
    clauses. Place Od() at the end of the looped block. All
    instructions between While() and Od() will be executed as long as the expression
    in While() evaluates to \b true.

    See Do() documentation for more details.
*/

void Od();

// -----------------------------------------------------------------------------
/**
    \brief Creates the \b for clause of the 'for' loop construct.
    
    Place in the shader code to create a 'for' loop construct.
    At the end of the looped block, always place the closing clause Rof().
    
    The For() loop is simplified, specialized version of While(). It takes
    a single integer variable reference to the loop control variable. The
    variable must be declared earlier. The loop will initialize the variable
    to \c begin value and before executing each iteration it will check whether
    the variable is less than \c end. At the end of each iteration, the \c step
    value will be added to the control variable.

    This kind of 'for' loop is more similar to languages like Fortran,
    but is still useful in many scenarios. For C++ like 'for' (with arbitrary
    condition), use the While() loop.

    Note that For(), and Rof() are actually function calls (as C++ instructions
    cannot be overloaded). You must put semicolon after them. Also pay attention
    to the fact that the code block between For() and Rof() does not create C++ scope.
    If you want to have C++ scope there, you must put the braces explicitly around
    the code. It is recommended if you create new identifiers inside the block,
    although not required.

    \code
        VInt i;

        // without scope
        For ( i, 0, nr_iterations );
            // instructions...
            // instructions...
        Rof();

        // with scope
        For ( i, 0, nr_iterations );
        {
            // instructions...
            // instructions...
        }
        Rof();

    \endcode

    Caution: pay attention to the fact that For() automatically increments
    the control variable, unlike While(). This is important thing
    to be aware of when converting between For() and While() constructs.
*/

void For ( VInt& variable, Int begin, Int end, Int step = 1 );

/** \overload */
void For ( VUInt& variable, UInt begin, UInt end, UInt step = 1 );

/**
    \brief Creates the closing clause of the 'for' loop construct.
    
    See For() documentation for more details.
*/
void Rof();

// -----------------------------------------------------------------------------

/**
    \brief Creates the \b switch construct.
    
    Place in the shader code to create a switch ... case construct. Provide switch
    expression as the argument. Always use with Case() and EndSwitch(). Optionally
    use also Default() and Break().
    
    The semantics of the Switch() instruction is identical to the C++ counterpart.
    Particluarly, the fall-through behavior is the same, so you need Break() clause
    to break control flow just as in C++. Also Default() works the same as in C++.
*/

void Switch ( Int expr );

/**
    \brief Creates the \b case clause of the switch construct.
*/

void Case ( int value );

/**
    \brief Creates the \b default clause of the switch construct.
*/

void Default();

/**
    \brief Creates the \b break clause of the switch construct.
*/

void Break();

/**
    \brief Creates the closing clause of the switch construct.
*/

void EndSwitch();

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

template< class ReturnType, typename... Args >
struct Function : public detail::KFunction
{
    Function ( const char* pName = "unnamedFunction" );

    ReturnType operator()( typename Args::rvalue_type ... args );
};

template< class ParamType >
struct Par
{
    Par();

    inline operator rvalue_type() const
    {
        ParamType par ( d_variableId );
        return rvalue_type ( par );
    }

    template< typename IndexT >
    inline auto operator[]( const IndexT& idx ) const
    {
        const rvalue_type rValue = *this;
        return rValue [ idx ];
    }

    inline int size() const
    {
        const rvalue_type rValue = *this;
        return rValue.size();
    }

    inline int Size() const
    {
        const rvalue_type rValue = *this;
        return rValue.Size();
    }

    VPP_DEFINE_PARAM_OPERATORS
};

// -----------------------------------------------------------------------------

void Begin();

void End();

void Return();

template< typename ValueT >
void Return ( const ValueT& value );

// -----------------------------------------------------------------------------

/**
    \brief A conditional expression.

    Since the operator <tt>?:</tt> in C++ can not be overloaded, VPP offers
    a counterpart as a function.

    Select() takes three arguments.

    The first argument should be the condition to check. It must be one
    of two following cases:
    - a scalar expression which evaluates to Bool,
    - a vector expression which evaluates to BVec2, BVec3 or BVec4 and the
      vector size is the same as operand vector size.

    The second and third argument can be any two expressions, but evaluating
    to common type. If the condition is \c true, the expression given as
    second argument will be the result. Otherwise, the third expression
    will be the result. It matches semantics of the <tt>?:</tt> C++ operator.

    In case if you specify a vector as the condition, you can select vector
    components individually. This feature does not exist in C++.
*/

template< class CondT, class Arg1T, class Arg2T >
auto Select ( const CondT& cond, const Arg1T& argIfTrue, const Arg2T& argIfFalse );

// -----------------------------------------------------------------------------

/**
    \brief Tags the next declared variable to be allocated in shared memory.

*/

void Shared();

/**
    \brief Tags the next declared variable to be visible across functions.

*/

void Static();

// -----------------------------------------------------------------------------

enum EMemorySemantics
{
    MSM_NONE,
    MSM_ACQ,
    MSM_REL,
    MSM_ACQREL,
    MSM_SEQCONS,
    MSM_UNIFORM,
    MSM_SUBGROUP,
    MSM_WORKGROUP,
    MSM_CROSSWG,
    MSM_ATOMIC,
    MSM_IMAGE
};

// -----------------------------------------------------------------------------

/**
    \brief Creates control and memory barrier affecting threads in workgroup.
*/

void WorkgroupBarrier (
    EMemorySemantics msClass = MSM_WORKGROUP,
    EMemorySemantics msSem = MSM_ACQREL );

/**
    \brief Creates control and memory barrier affecting threads in all workgroups.
*/
void DeviceBarrier ( 
    EMemorySemantics msClass = MSM_CROSSWG,
    EMemorySemantics msSem = MSM_ACQREL );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
