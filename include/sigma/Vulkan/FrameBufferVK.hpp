#pragma once

#include <sigma/FrameBuffer.hpp>

#include <vulkan/vulkan.h>

#include <memory>

class DeviceVK;
class RenderPassVK;
class Texture2DVK;

class FrameBufferVK : public FrameBuffer {
public:
	FrameBufferVK(std::shared_ptr<DeviceVK> inDevice);

	FrameBufferVK(std::shared_ptr<DeviceVK> inDevice, VkFramebuffer inFrameBuffer, std::shared_ptr<RenderPassVK> inRenderPass, const glm::uvec2& inSize);

    virtual ~FrameBufferVK();

    virtual std::shared_ptr<RenderPass> renderPass() const override;

    virtual glm::uvec2 size() const override;

    VkFramebuffer handle() const { return mFrameBuffer; }

	bool initialize(const FrameBufferCreateParams &inParams);

private:
    std::shared_ptr<DeviceVK> mDevice = nullptr;
    std::shared_ptr<RenderPassVK> mRenderPass = nullptr;
	std::vector<std::shared_ptr<Texture2DVK>> mAttachments;
    VkFramebuffer mFrameBuffer = nullptr;
    glm::uvec2 mSize;
};
