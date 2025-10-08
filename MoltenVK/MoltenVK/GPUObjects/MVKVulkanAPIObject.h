/*
 * MVKVulkanAPIObject.h
 *
 * Copyright (c) 2015-2025 The Brenwill Workshop Ltd. (http://www.brenwill.com)
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

#include "MVKBaseObject.h"
#include <vulkan/vk_icd.h>

#import <Foundation/NSString.h>
#import <objc/runtime.h>
#import <mach/mach.h>
#import <mach/mach_vm.h>

class MVKInstance;


#pragma mark -
#pragma mark MVKVulkanAPIObject

/**
 * Abstract class that represents an opaque Vulkan API handle object.
 *
 * Vulkan API objects can sometimes be destroyed by the client before the GPU is done with them.
 * To support this, this class inherits from MVKReferenceCountingMixin to allow an instance to
 * live past its destruction by the client, until it is no longer referenced by other objects.
 */
class MVKVulkanAPIObject : public MVKReferenceCountingMixin<MVKBaseObject>, public MVKConfigurableMixin {

public:

    /** Returns the Vulkan API opaque object controlling this object. */
    MVKVulkanAPIObject* getVulkanAPIObject() override { return this; };

    /** Returns a reference to this object suitable for use as a Vulkan API handle. */
    virtual void* getVkHandle() { return this; }

    /** Returns the Vulkan type of this object. */
    virtual VkObjectType getVkObjectType() = 0;

    /** Returns the debug report object type of this object. */
    virtual VkDebugReportObjectTypeEXT getVkDebugReportObjectType() = 0;

    /** Returns the Vulkan instance. */
    virtual MVKInstance* getInstance() = 0;

    /** Gets the debug object name of this instance. */
    NSString* getDebugName() { return _debugName; }

    /** Sets the debug object name of this instance. */
    VkResult setDebugName(const char* pObjectName);

    /** Sets the label of the Metal object. */
    void setMetalObjectLabel(id mtlObj, NSString* label);

    /** Returns the MVKVulkanAPIObject instance referenced by the object of the given type. */
    static MVKVulkanAPIObject* getMVKVulkanAPIObject(VkDebugReportObjectTypeEXT objType, uint64_t object);

    /** Returns the MVKVulkanAPIObject instance referenced by the object of the given type. */
    static MVKVulkanAPIObject* getMVKVulkanAPIObject(VkObjectType objType, uint64_t objectHandle);

    MVKVulkanAPIObject() {}
    MVKVulkanAPIObject(const MVKVulkanAPIObject& other);
    MVKVulkanAPIObject& operator=(const MVKVulkanAPIObject& other);
    ~MVKVulkanAPIObject() override;

protected:
    virtual void propagateDebugName() = 0;

    NSString* _debugName = nil;
};


#pragma mark -
#pragma mark MVKDispatchableVulkanAPIObject

/** Abstract class that represents a dispatchable opaque Vulkan API handle object. */
class MVKDispatchableVulkanAPIObject : public MVKVulkanAPIObject {

    typedef struct {
        VK_LOADER_DATA loaderData;
        MVKDispatchableVulkanAPIObject* mvkObject;
    } MVKDispatchableObjectICDRef;

public:

    /**
     * Returns a reference to this object suitable for use as a dispatchable Vulkan API handle.
     *
     * Establishes the loader magic number every time, in case the loader
     * overwrote it for some reason before passing the object back,
     * particularly in pooled objects that the loader might consider freed.
     *
     * This is the compliment of the getDispatchableObject() function.
     */
    void* getVkHandle() override {
        set_loader_magic_value(&_icdRef);
        return &_icdRef;
    }

    /**
     * Retrieves the MVKDispatchableVulkanAPIObject instance referenced by the dispatchable Vulkan handle.
     *
     * This is the compliment of the getVkHandle() function.
     */
    static MVKDispatchableVulkanAPIObject* getDispatchableObject(void* vkHandle) {
        return vkHandle ? ((MVKDispatchableObjectICDRef*)vkHandle)->mvkObject : nullptr;
    }

protected:
    MVKDispatchableObjectICDRef _icdRef = { 0, this };

};

static inline BOOL TryReadMachVM(mach_vm_address_t address, uint64_t& val) {
    mach_vm_size_t outSize = 0;
    kern_return_t kr = mach_vm_read_overwrite(mach_task_self(),
                                              address,
                                              sizeof(val),
                                              (mach_vm_address_t)&val,
                                              &outSize);
    if (kr != KERN_SUCCESS || outSize != sizeof(val)) {
        val = 0;
        return NO;
    }
    return YES;
}
static inline BOOL IsValidObjcId(id objc_id, uint64_t& val) {
    if (!objc_id || objc_id == [NSNull null]) {
        return NO;
    }
    mach_vm_address_t address = reinterpret_cast<mach_vm_address_t>(objc_id);
    if (!TryReadMachVM(address, val)) {
        return NO;
    }
    if (!val) {
        return NO;
    }
    return YES;
}
static inline BOOL IsValidObjcId(id objc_id) {
    uint64_t val;
    return IsValidObjcId(objc_id, val);
}
static inline BOOL IsValidMTLSampler(id objc_id) {
    uint64_t addr;
    uint64_t val;
    if (!IsValidObjcId(objc_id, addr))
        return NO;
    if (TryReadMachVM(reinterpret_cast<mach_vm_address_t>(addr), val)) {
        if (!val)
            return NO;
        Class cls = object_getClass(objc_id);
        if (cls == objc_getClass("CaptureMTLSamplerState"))
            return YES;
        Protocol* proto = reinterpret_cast<Protocol*>(objc_id);
        if (proto == objc_getProtocol("MTLSamplerState"))
            return YES;
    }
    else {
        if (addr & 0xff00000000000000ull) {
            Class cls = object_getClass(objc_id);
            if (cls == objc_getClass("CaptureMTLSamplerState"))
                return YES;
        }
    }
    return NO;
}
