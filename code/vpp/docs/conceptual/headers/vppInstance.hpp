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
*/

class Instance
{
public:
    /**
        \brief Flags for instance creation.
    */
    enum EFlags
    {
        NO_FLAGS,      /**< All options disabled.*/
        VALIDATION     /**< Enable Vulkan validation.*/
    };
    
    /**
        \brief The constructor.

        Takes optional flags.

        An example:

        \code
            int main()
            {
                #ifdef MY_DEBUG_BUILD
                    static const unsigned int vppFlags = vpp::Instance::VALIDATION;
                #else
                    static const unsigned int vppFlags = 0u;
                #endif

                vpp::Instance myInstance ( vppFlags );

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
    explicit Instance ( unsigned int flags = 0 );

    /** \brief Destructor. */
    ~Instance();

    /** \brief Retrieves Vulkan handle for the instance. */
    VkInstance handle() const;

    /** \brief Checks whether instance creation has succeeded. */
    bool valid() const;
    
    /** \brief Gets a list of physical devices in the system. */
    VkResult enumeratePhysicalDevices ( PhysicalDevices* pResult ) const;

    /** \brief Gets a list of available Vulkan extensions. */
    static VkResult enumerateExtensions ( ExtensionProperties* pResult );

    /** \brief Gets currently registered debug reporter, or zero if there is no one. */
    static DebugReporter* getDebugReporter();
};

// -----------------------------------------------------------------------------
} // namespace vpp
// -----------------------------------------------------------------------------
