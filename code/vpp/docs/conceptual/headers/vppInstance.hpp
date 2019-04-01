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
    \brief Structure holding information about Vulkan version.
*/

struct SVulkanVersion
{
    /** \brief Major version number. */
    unsigned int major;

    /** \brief Minor version number. */
    unsigned int minor;

    /** \brief Patch number. */
    unsigned int patch;

    /** \brief Returns string representation. */
    std::string toString() const;

    /** \brief Returns opaque identifier. */
    unsigned int id() const;

    /** \brief Creates from opaque identifier. */
    static SVulkanVersion fromId ( unsigned int id );
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \brief Represents the instance of Vulkan system.

    The instance is the root object of entire Vulkan system. The main purpose
    is to provide access to physical devices in the system as well as the list
    of available Vulkan extensions.

    There can be only one instance object per program. You can construct
    multiple ones, but all of them will point to the same object internally.
    Do not create many instances from different threads however, this operation
    is not thread safe. Best practice is to create just one, from your main
    thread and keep its life time until the end of the program. You can pass the
    Instance object by value, also to other threads.

    This object is reference-counted and may be passed by value.

    Instance is created by means of createInstance() special function.

    An example:

    \code
        int main()
        {
            #ifdef MY_DEBUG_BUILD
                const bool bValidation = true;
            #else
                const bool bValidation = false;
            #endif

            vpp::Instance myInstance = createInstance().validation ( bValidation );

            if ( ! myInstance.valid() )
            {
                // Can't create instance - maybe this system does not support Vulkan.
                return -1;
            }

            vpp::PhysicalDevices physicalDevices;

            if ( myInstance.enumeratePhysicalDevices ( & physicalDevices ) != VK_SUCCESS
                    || physicalDevices.empty() )
            {
                // No Vulkan-capable devices found.
                return -1;
            }

            // Optionally handle multiple devices. Here we just pick the first one.
            const vpp::PhysicalDevice hPhysicalDevice = physicalDevices [ 0 ];

            // Application main loop routine.
            return runApplication ( myInstance, hPhysicalDevice );
        }

    \endcode
*/

class Instance
{
public:
    /** \brief Destructor. */
    ~Instance();

    /** \brief Retrieves Vulkan handle for the instance. */
    VkInstance handle() const;

    /** \brief Checks whether instance creation has succeeded. */
    bool valid() const;

    /** \brief Retrieves the names of enabled extensions. */
    const std::set< std::string >& enabledExtensions() const;
    
    /** \brief Gets a list of physical devices in the system. */
    VkResult enumeratePhysicalDevices ( PhysicalDevices* pResult ) const;

    /** \brief Gets newest Vulkan version available on the system.
    
        Caution: on systems where there is no Vulkan driver installed, it might
        still return version 1.0. Try creating instance and check \c valid() method
        on it in order to determine if Vulkan is supported at all.
    */
    static SVulkanVersion getLatestAvailableVulkanVersion();

    /** \brief Gets the minimum of two versions: latest available and requested by user.
    */
    static SVulkanVersion supportedVersion();

    /** \brief Gets a list of available Vulkan extensions for the instance. */
    static VkResult enumerateExtensions ( ExtensionProperties* pResult );

    /** \brief Gets currently registered debug reporter, or zero if there is no one. */
    static DebugReporter* getDebugReporter();
};

// -----------------------------------------------------------------------------
// -----------------------------------------------------------------------------

/**
    \brief Instance factory class.

    This class is meant to be used somewhat like a function. The syntax is
    as in the following examples:

    \code
        // Most basic instance with default options.
        Instance inst = createInstance();

        // Instance with default options and validation turned on.
        Instance inst = createInstance().validation ( true );

        // Instance targeting specific version of Vulkan, with validation turned on and some extension enabled.
        Instance inst = createInstance()
            .vulkan ( { 1, 1, 0 } )
            .validation ( true )
            .ext ( "VK_KHR_device_group_creation" );

    \endcode

    The \c createInstance actually construct proxy object that you can configure using chain
    of methods, and finally assign it to Instance variable. The last step creates the actual
    instance.

    The default options (if you do not call any methods on the proxy object) are:
    - Vulkan 1.0.0 target,
    - no validation,
    - no application info,
    - no user-specified extensions (but VPP enables the swapchain and debug extensions by default),
    - no user-specified layers (but VPP enables validation layer automatically if validation is enabled).
*/

class createInstance
{
public:
    /** \brief Constructs instance factory object with default options. */
    createInstance();

    /** \brief Sets the target Vulkan version (default: 1.0.0).
    
        Vulkan installations are backward-compatible. For example, if the system
        supports Vulkan 1.1, then it also supports Vulkan 1.0. However, there might exist
        systems not supporting Vulkan 1.1. It is your decision whether you require
        extended features of Vulkan 1.1 (or any further version). If not, then target earlier
        version. In general, it is reasonable to require minimum version that you actually
        need for features.

        Only \c major and \c minor fields of the version structure are significant when
        matching the version. The \c patch field is ignored.

        Specifying required version during instance construction must be followed
        by checking whether target physical device actually supports that version. There
        might be e.g. a system where new Vulkan driver is installed, but the device
        is an old graphic card with only Vulkan 1.0 support. You must manually check
        the device properties before using any features requiring newer version.
        Otherwise assume that capabilities and semantics are just as in older
        version. 
    */
    createInstance& vulkan ( const SVulkanVersion& v );

    /** \brief Sets the validation status (default: disabled). */
    createInstance& validation ( bool v );

    /** \brief If set, VPP will not add any extensions automatically (default: false).
    
        Only extensions you explicitly mention will be loaded (including surface,
        swapchain and debug extensions).
    */
    createInstance& raw ( bool v );

    /** \brief Sets the application name (default: null).
    
        This is arbitrary name of your choice.

        Providing information about your application in theory might enable Vulkan
        to include optimizations made for specific applications.    
    */
    createInstance& appName ( const char* v );

    /** \brief Sets the engine name (default: null).

        This is arbitrary name of your choice.
    
        Providing information about your application in theory might enable Vulkan
        to include optimizations made for specific applications.    
    */
    createInstance& engineName ( const char* v );

    /** \brief Sets the application version (default: 0).
    
        This is arbitrary value of your choice.

        Providing information about your application in theory might enable Vulkan
        to include optimizations made for specific applications.    
    */
    createInstance& appVersion ( unsigned int v );

    /** \brief Sets the engine version (default: 0).

        This is arbitrary value of your choice.

        Providing information about your application in theory might enable Vulkan
        to include optimizations made for specific applications.    
    */
    createInstance& engineVersion ( unsigned int v );

    /** \brief Adds an instance extension to enable.
    
        Note that VPP enables some extensions automatically. Also, extensions that
        you supply here will be enabled only if supported by the system the application
        is being run on.
    */
    createInstance& ext ( const std::string& v );

    /** \brief Adds a layer to enable. 
    
        Note that VPP enables some layers automatically and the system may also load additional
        layers without knowledge of the application.
    */
    createInstance& layer ( const std::string& v );

    /** \brief Creates actual instance with specified options. */
    operator Instance() const;
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
