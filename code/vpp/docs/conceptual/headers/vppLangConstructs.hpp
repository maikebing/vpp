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

/**
    \brief Creates shader-level function.

    Vulkan allows you to define functions inside shaders. VPP has special syntax
    for it. Note that this is different from defining a C++ function containing
    shader code. The latter will cause the function code to be inlined into the caller
    code. The notation described here allows to define a true function on GPU level.
    The purpose is to avoid GPU code bloat.

    Four major components of each function definition are Function and Par templates,
    as well as Begin(), End() and Return() constructs. Function defines the function,
    Par defines a parameter of the function, Begin() and End() delimit the function body.
    Return() allows to return a value.

    The VPP code which defines a function is meant to be executed as the shader definition
    code, before the function is used. The Function template actually creates a callable
    \b functor in local scope, which in turn may be called from the shader. The simplest
    way to use function looks like this:

    \code

        void MyPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
        {
            using namespace vpp;

            // ...

            Function< Int, Int > factorial ( "factorial" );
                Par< Int > factX;
            Begin();
            {
                VInt t = 0;
                VInt r = 1;

                For ( t, 2, factX+1 );
                    r *= t;
                Rof();

                Return ( r );
            }
            End();

            const Int arg = ...; // compute some arg for factorial
            const Int fact = factorial ( arg );

            // ...
        }

    \endcode

    Here the shader first defines a function named \c factorial and calls it. You may call
    the function as many times you want from anywhere in the shader, assuming the functor still exists. 

    The first template argument of Function template defines the return type. Zero or more
    types which may come later determine parameter types. These must match with Par declarations
    that come later.

    The name of the function should be passed to its constructor. This name will be emited into
    final SPIR-V code and may be helpful during diagnostics.

    The drawback of the notation shown above is leakage of \c factX paramter name to the
    shader code scope. It might be a problem if you want to use another variable with this name.
    To prevent this, use slighly modified version, like in the \c binomial function below:

    \code
            // ...

            Function< Int, Int, Int > binomial ( "binomial" );
            {
                Par< Int > n;
                Par< Int > k;
                Begin();
                {
                    Return ( factorial ( n ) / ( factorial ( k )*factorial ( n-k ) ) );
                }
                End();
            }

            // ...
    \endcode

    Here we have entire definition enclosed in additional C++ scope which hides parameter
    names from the outside.
    
    In all cases the scope between Begin() and End() is required, especially if the function
    defines any mutable variables of its own. It is an error to omit this scope. C++ compiler
    can't detect this error, but VPP will do and an exception will be thrown in such case during
    shader compilation.

    What if we want a reusable function definition, e.g. to be placed in some library? The solution
    is simple - we can just convert the definition above into C++ class, like in this example:

    \code
        class GFunGCD : public vpp::Function< vpp::Int, vpp::Int, vpp::Int >
        {
        public:
            GFunGCD();

        private:
            vpp::Par< vpp::Int > _n;
            vpp::Par< vpp::Int > _k;
        };

        GFunGCD :: GFunGCD() :
            vpp::Function< vpp::Int, vpp::Int, vpp::Int >( "gcd" )
        {
            using namespace vpp;

            Begin();
            {
                VInt x = _n;
                VInt y = _k;
                VInt s = 0;
                VInt t = 0;

                If ( x < y );
                    t = x;
                    x = y;
                    y = t;
                Fi();

                Do(); While ( ( ( x & 1 ) | ( y & 1 ) ) == 0 );
                    x >>= 1u;
                    y >>= 1u;
                    ++s;
                Od();

                Do(); While ( x != 0 );
                {
                    Do(); While ( ( x & 1 ) == 0 );
                        x >>= 1u;
                    Od();

                    Do(); While ( ( y & 1 ) == 0 );
                        y >>= 1u;
                    Od();

                    If ( x >= y );
                        x = ( x - y ) >> 1u;
                    Else();
                        y = ( y - x ) >> 1u;
                    Fi();
                }
                Od();

                Return ( y << s );
            }
            End();
        }

        void MyPipeline :: fComputeShader ( vpp::ComputeShader* pShader )
        {
            using namespace vpp;

            // ...

            GFunGCD gcd;

            const Int x = gcd ( 17*19, 17*23 );

            // ...
        }

    \endcode

    The rules are now as follows:
    - Inherit the class from Function template.
    - Specify parameters as private fields in the class.
    - In the constructor initialization list, provide the function name.
    - In the constructor body define the function. Usual Begin() and End() rules apply
        here as well.
    - Create object instance (like \c gcd above) in the calling shader to "import" the
        function. This operation will trigger generation function code to SPIR-V.
    - Call the functor to make a function call.

    Also it should be noted that you can pass additional parameters on the CPU
    level to the function object in order to parameterize the function. It can also be
    a C++ template, use virtual functions and generally utilise any C++ programming technique.
*/

template< class ReturnType, typename... Args >
struct Function : public detail::KFunction
{
    /** \brief Starts the definition of shader-level function.
    
        Specify the function name as the parameter. The name will be visible
        in generated code.
    */

    Function ( const char* pName = "unnamedFunction" );

    /**
        \brief Calls the defined function from inside shader code on the GPU.
    */

    ReturnType operator()( typename Args::rvalue_type ... args );
};

/**
    \brief Defines a function parameter.
*/

template< class ParamType >
struct Par
{
};

/**
    \brief Marks the start of function body.

    Caution: C++ scope is required between Begin() and End().
*/

void Begin();

/**
    \brief Marks the end of function body.

    Caution: C++ scope is required between Begin() and End().
*/

void End();

/**
    \brief Immediately returns from the function.

    Use when the erturn type is Void.
*/

void Return();

/**
    \brief Returns the value from the function.
*/

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

enum EMemorySemantics
{
    MSM_NONE,
    MSM_ACQ,
    MSM_REL,
    MSM_ACQREL,
    MSM_UNIFORM,
    MSM_SUBGROUP,
    MSM_WORKGROUP,
    MSM_IMAGE
};

// -----------------------------------------------------------------------------

/**
    \brief Creates control and memory barrier affecting threads in a workgroup.
*/

void WorkgroupBarrier (
    EMemorySemantics msClass = MSM_WORKGROUP,
    EMemorySemantics msSem = MSM_ACQREL );

/**
    \brief Creates control and memory barrier affecting threads in a subgroup.
*/

void SubgroupBarrier (
    EMemorySemantics msClass = MSM_SUBGROUP,
    EMemorySemantics msSem = MSM_ACQREL );

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
