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

// -----------------------------------------------------------------------------
namespace vpp {
// -----------------------------------------------------------------------------

/**
    \brief Base class for debug report generators.

    This class participates in generation of debug reports from the following
    sources:
    - Vulkan validation layer,
    - shader translation dump.

    This is a general debug reporter class. To generate text reports into
    specified C++ output stream, you might want to use a vpp::StreamDebugReporter
    subclass. vpp::DebugReporter allows custom debug message processing.

    You use both classes (as well as reporters derived from vpp::DebugReporter)
    by defining an object of respective class after your instance object and
    providing the instance reference to the constructor.

    Additionally, to enable Vulkan validation you must append <tt>.validation ( true )</tt>
    call when creating the vpp::Instance object by vpp::createInstance().

    Cosntructors of debug reporters have two variants. The first one takes
    only the instance reference. The second one requires one additional argument,
    which is a bitwise \c or of flags specifying messages to be included.
*/

class DebugReporter
{
public:
    /**
        \brief Enumeration of flags to be used in DebugReporter constructor.
    */
    enum
    {
        ERRORS,         /**< \brief Include error messages. */
        WARNINGS,       /**< \brief Include warning messages. */
        PERF,           /**< \brief Include performance messages. */
        INFO,           /**< \brief Include information messages. */
        DEBUG,          /**< \brief Include debug messages. */
        SHADERS         /**< \brief Include shader translation dumps. */
    };

    /**
        \brief Constructs the debug reporter with default flags.
    */
    DebugReporter ( const Instance& hInstance );

    /**
        \brief Constructs the debug reporter with specified flags.
    */
    DebugReporter ( const Instance& hInstance, VkFlags flags );

    /**
        \brief Override this method for custom message processing.
    */
    virtual VkBool32 debugReport (
        VkDebugReportFlagsEXT flags,
        VkDebugReportObjectTypeEXT objectType,
        uint64_t object,
        size_t location,
        int32_t messageCode,
        const char* pLayerPrefix,
        const char* pMessage );

    /**
        \brief Override this method for custom handling of shader translation dumps.
    */
    virtual void shaderCompilationLog (
        const std::string& shaderCode,
        const char* pShaderType );
};

// -----------------------------------------------------------------------------
/**
    \brief A debug reporter class working with C++ output streams.
*/

class StreamDebugReporter : public DebugReporter
{
public:
    /**
        \brief Constructs the debug reporter with default flags.
    */
    StreamDebugReporter (
        std::ostream& out, const Instance& hInstance );

    /**
        \brief Constructs the debug reporter with specified flags.
    */
    StreamDebugReporter (
        std::ostream& out, const Instance& hInstance, VkFlags flags );
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
