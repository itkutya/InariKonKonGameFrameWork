// module;
//
// #include <unordered_map>
// #include <string_view>
// #include <algorithm>
// #include <stdexcept>
// #include <optional>
// #include <cstdint>
// #include <utility>
// #include <vector>
// #include <chrono>
// #include <ranges>
// #include <array>
// #include <span>
//
// #define GLFW_INCLUDE_VULKAN
// #include "GLFW/glfw3.h"
//
// #define VULKAN_HPP_NO_EXCEPTIONS
// #define VULKAN_HPP_NO_SMART_HANDLE
// #define VULKAN_HPP_NO_CONSTRUCTORS
// #define VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS
// #define VULKAN_HPP_USE_STD_EXPECTED
// #include <vulkan/vulkan_raii.hpp>
//
// #define VMA_IMPLEMENTATION
// #include <vk_mem_alloc.h>
//
export module Vulkan;
//
// import ProjectVersion;
// import NumericCasts;
// import DebugUtils;
//
// import Renderer;
// import Vector;
// import Vertex;
// import Color;
// import Mesh;
//
// //
// import Shader;
// //
//
// import Error;
// import Log;
//
// export namespace ikk
// {
//     class Vulkan final : public Renderer::Type
//     {
//     public:
//         explicit Vulkan(GLFWwindow* window) noexcept;
//
//         Vulkan(const Vulkan&) noexcept = delete;
//         Vulkan(Vulkan&&) noexcept = default;
//
//         Vulkan& operator=(const Vulkan&) noexcept = delete;
//         Vulkan& operator=(Vulkan&&) noexcept = default;
//
//         ~Vulkan() noexcept override;
//
//         void init() noexcept override;
//         void destroy() noexcept override;
//
//         [[nodiscard]] bool isValid() const noexcept override;
//
//         [[nodiscard]] bool beginRendering(const Color& clearColor) noexcept override;
//         void endRendering() noexcept override;
//
//         void draw(const Mesh& mesh) noexcept override;
//     private:
//         struct GPU
//         {
//             vk::raii::PhysicalDevice physicalDevice{nullptr};
//             vk::raii::Device device{nullptr};
//
//             vk::raii::Queue graphicsQueue{nullptr};
//             vk::raii::Queue presentQueue{nullptr};
//
//             std::uint32_t graphicsFamily{};
//             std::uint32_t presentFamily{};
//
//             [[nodiscard]] bool setUpFamilies(const vk::raii::SurfaceKHR& surface) noexcept;
//         };
//
//         struct SwapChainProperties
//         {
//             std::vector<vk::Image> images{};
//             std::vector<vk::raii::ImageView> imageViews{};
//
//             vk::Format imageFormat = vk::Format::eUndefined;
//             vk::Extent2D extent{};
//
//             struct SupportDetails
//             {
//                 vk::SurfaceCapabilitiesKHR capabilities{};
//                 std::vector<vk::SurfaceFormatKHR> formats{};
//                 std::vector<vk::PresentModeKHR> presentModes{};
//
//                 [[nodiscard]] vk::SurfaceFormatKHR getSurfaceFormat() const noexcept;
//                 [[nodiscard]] vk::PresentModeKHR getPresentMode() const noexcept;
//                 [[nodiscard]] vk::Extent2D getExtent(Vec2u size) const noexcept;
//                 [[nodiscard]] std::uint32_t getImageCount() const noexcept;
//             };
//
//             [[nodiscard]] static SupportDetails querySwapChainSupport(const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface) noexcept;
//         };
//
//         struct FrameObjects
//         {
//             vk::raii::CommandBuffer commandBuffer{nullptr};
//             vk::raii::Semaphore imageAvailableSemaphore{nullptr};
//             vk::raii::Fence inFlightFence{nullptr};
//         };
//
//         //TODO:
//         struct GPUMesh
//         {
//             VmaAllocator allocator{nullptr};
//
//             vk::Buffer vertexBuffer{nullptr};
//             VmaAllocation vertexAllocation{nullptr};
//
//             vk::Buffer indexBuffer{nullptr};
//             VmaAllocation indexAllocation{nullptr};
//
//             std::uint32_t indexCount{};
//
//             inline explicit GPUMesh(VmaAllocator allocator) noexcept
//                 : allocator(allocator)
//             {
//             }
//
//             GPUMesh(const GPUMesh&) = delete;
//             inline GPUMesh(GPUMesh&& other) noexcept
//                 :   allocator(std::exchange(other.allocator, nullptr)),
//                     vertexBuffer(std::exchange(other.vertexBuffer, nullptr)),
//                     vertexAllocation(std::exchange(other.vertexAllocation, nullptr)),
//                     indexBuffer(std::exchange(other.indexBuffer, nullptr)),
//                     indexAllocation(std::exchange(other.indexAllocation, nullptr)),
//                     indexCount(other.indexCount)
//             {
//             }
//
//             GPUMesh& operator=(const GPUMesh&) = delete;
//             inline GPUMesh& operator=(GPUMesh&& other) noexcept
//             {
//                 if (this != &other)
//                 {
//                     this->destroy();
//
//                     this->allocator = std::exchange(other.allocator, nullptr);
//                     this->vertexBuffer = std::exchange(other.vertexBuffer, nullptr);
//                     this->vertexAllocation = std::exchange(other.vertexAllocation, nullptr);
//                     this->indexBuffer = std::exchange(other.indexBuffer, nullptr);
//                     this->indexAllocation = std::exchange(other.indexAllocation, nullptr);
//                     this->indexCount = other.indexCount;
//                 }
//                 return *this;
//             }
//
//             inline ~GPUMesh() noexcept
//             {
//                 this->destroy();
//             }
//
//             inline void destroy() noexcept
//             {
//                 if (this->allocator == nullptr)
//                     return;
//
//                 if (this->vertexBuffer != nullptr)
//                 {
//                     vmaDestroyBuffer(this->allocator, this->vertexBuffer, vertexAllocation);
//                     this->vertexBuffer = nullptr;
//                     this->vertexAllocation = nullptr;
//                 }
//
//                 if (this->indexBuffer != nullptr)
//                 {
//                     vmaDestroyBuffer(this->allocator, this->indexBuffer, this->indexAllocation);
//                     this->indexBuffer = nullptr;
//                     this->indexAllocation = nullptr;
//                 }
//
//                 this->allocator = nullptr;
//             }
//         };
//
//         inline static constexpr bool s_enableValidationLayers = isDebugBuild() == true;
//         inline static constexpr std::size_t s_virtualFrameCount = 2;
//
//         bool m_valid = false;
//         GLFWwindow* m_window = nullptr;
//         std::size_t m_currentFrame = 0;
//         std::uint32_t m_imageIndex = 0;
//
//         vk::raii::Context m_context{};
//         vk::raii::Instance m_instance{nullptr};
//         vk::raii::DebugUtilsMessengerEXT m_debugMessenger{nullptr};
//
//         vk::raii::SurfaceKHR m_surface{nullptr};
//
//         GPU m_gpu{};
//
//         vk::raii::SwapchainKHR m_swapChain{nullptr};
//         SwapChainProperties m_swapChainProperties{};
//
//         vk::raii::CommandPool m_commandPool{nullptr};
//
//         std::array<FrameObjects, s_virtualFrameCount> m_frameObjects{};
//         std::vector<vk::raii::Semaphore> m_renderFinishedSemaphores{};
//
//         //TODO: GPUMaterial struct
//         //
//         vk::raii::ShaderModule m_vertexShader{nullptr};
//         vk::raii::ShaderModule m_fragmentShader{nullptr};
//         //
//
//         //
//         vk::raii::PipelineLayout m_pipelineLayout{nullptr};
//         vk::raii::Pipeline m_graphicsPipeline{nullptr};
//         //
//
//         VmaAllocator m_allocator = nullptr;
//         std::unordered_map<const Mesh*, GPUMesh> m_meshes{};
//
//         [[nodiscard]] std::expected<void, Error> createInstance() noexcept;
//         [[nodiscard]] std::expected<void, Error> createDebugMessenger() noexcept;
//         [[nodiscard]] std::expected<void, Error> createSurface() noexcept;
//         [[nodiscard]] std::expected<void, Error> pickPhysicalDevice() noexcept;
//         [[nodiscard]] std::expected<void, Error> createLogicalDevice() noexcept;
//         [[nodiscard]] std::expected<void, Error> createAllocator() noexcept;
//         [[nodiscard]] std::expected<void, Error> createSwapChain() noexcept;
//         [[nodiscard]] std::expected<void, Error> createImageViews() noexcept;
//         [[nodiscard]] std::expected<void, Error> createGraphicsPipeline() noexcept;
//         [[nodiscard]] std::expected<void, Error> createCommandPool() noexcept;
//         [[nodiscard]] std::expected<void, Error> createCommandBuffers() noexcept;
//         [[nodiscard]] std::expected<void, Error> createBufferedSyncObjects() noexcept;
//         [[nodiscard]] std::expected<void, Error> createRenderFinishedSemaphores() noexcept;
//
//         std::expected<void, Error> recreateSwapChain() noexcept;
//
//         [[nodiscard]] std::expected<std::pair<vk::Buffer, VmaAllocation>, Error> createBuffer(
//             vk::DeviceSize size,
//             vk::BufferUsageFlags usage,
//             VmaMemoryUsage memoryUsage,
//             VmaAllocationCreateFlags allocationFlags = {}) const noexcept;
//         void uploadMesh(const Mesh& mesh) noexcept;
//
//         [[nodiscard]] bool checkLayerSupport(const auto& layers) const noexcept;
//         [[nodiscard]] static std::vector<const char*> getRequiredExtensions() noexcept;
//         [[nodiscard]] static const vk::DebugUtilsMessengerCreateInfoEXT& getDebugMessengerInfo() noexcept;
//     };
// }
//
// namespace ikk
// {
//     Vulkan::Vulkan(GLFWwindow* window) noexcept
//         : m_window(window)
//     {
//     }
//
//     Vulkan::~Vulkan() noexcept
//     {
//         this->destroy();
//     }
//
//     void Vulkan::init() noexcept
//     {
//         using InitFuncType = std::expected<void, Error> (Vulkan::*)() noexcept;
//
//         const std::array initFuncs
//             {
//                 &Vulkan::createInstance,
//                 &Vulkan::createDebugMessenger,
//                 &Vulkan::createSurface,
//                 &Vulkan::pickPhysicalDevice,
//                 &Vulkan::createLogicalDevice,
//                 &Vulkan::createAllocator,
//                 &Vulkan::createSwapChain,
//                 &Vulkan::createImageViews,
//                 &Vulkan::createGraphicsPipeline,
//                 &Vulkan::createCommandPool,
//                 &Vulkan::createCommandBuffers,
//                 &Vulkan::createBufferedSyncObjects,
//                 &Vulkan::createRenderFinishedSemaphores
//             };
//
//         for (const InitFuncType init : initFuncs)
//         {
//             if (const std::expected<void, Error> value = (this->*init)(); value.has_value() == false)
//             {
//                 Log::Error("Vulkan initialization failed: {}", value.error().what());
//                 this->destroy();
//                 return;
//             }
//         }
//         this->m_valid = true;
//     }
//
//     void Vulkan::destroy() noexcept
//     {
//         if (this->m_gpu.device != nullptr)
//             this->m_gpu.device.waitIdle();
//
//         this->m_meshes.clear();
//
//         if (this->m_allocator != nullptr)
//         {
//             vmaDestroyAllocator(this->m_allocator);
//             this->m_allocator = nullptr;
//         }
//
//         this->m_vertexShader = nullptr;
//         this->m_fragmentShader = nullptr;
//
//         this->m_graphicsPipeline = nullptr;
//         this->m_pipelineLayout = nullptr;
//
//         this->m_renderFinishedSemaphores.clear();
//         for (auto& [commandBuffer, imageAvailableSemaphore, inFlightFence] : this->m_frameObjects)
//         {
//             commandBuffer = nullptr;
//             imageAvailableSemaphore = nullptr;
//             inFlightFence = nullptr;
//         }
//         this->m_commandPool = nullptr;
//         this->m_swapChainProperties = {};
//         this->m_swapChain = nullptr;
//         this->m_gpu = {};
//         this->m_surface = nullptr;
//         this->m_debugMessenger = nullptr;
//         this->m_instance = nullptr;
//
//         this->m_valid = false;
//     }
//
//     bool Vulkan::isValid() const noexcept
//     {
//         return this->m_valid;
//     }
//
//     bool Vulkan::beginRendering(const Color& clearColor) noexcept
//     {
//         int width = 0, height = 0;
//         glfwGetFramebufferSize(this->m_window, &width, &height);
//         if (width == 0 || height == 0) return false;
//
//         auto& [commandBuffer, imageAvailableSemaphore, inFlightFence] = this->m_frameObjects[this->m_currentFrame];
//
//         static constexpr std::uint64_t timeout = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::seconds{3}).count();
//         if (this->m_gpu.device.waitForFences(*inFlightFence, vk::True, timeout) != vk::Result::eSuccess)
//             return false;
//
//         vk::AcquireNextImageInfoKHR nextImageInfo{};
//         nextImageInfo.setDeviceMask(1);
//         nextImageInfo.setSemaphore(*imageAvailableSemaphore);
//         nextImageInfo.setSwapchain(*this->m_swapChain);
//         nextImageInfo.setTimeout(std::numeric_limits<std::uint64_t>::max());
//
//         const auto [result, imageIndex] = this->m_gpu.device.acquireNextImage2KHR(nextImageInfo);
//
//         if (result == vk::Result::eErrorOutOfDateKHR)
//         {
//             auto _ = this->recreateSwapChain();
//             return false;
//         }
//
//         if (result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR)
//             return false;
//
//         // Only reset after acquiring an image. Early returns above must leave
//         // the fence signaled so the next frame cannot stall on an unsignaled fence.
//         this->m_gpu.device.resetFences(*inFlightFence);
//
//         this->m_imageIndex = imageIndex;
//
//         commandBuffer.reset();
//         vk::CommandBufferBeginInfo beginInfo{};
//         beginInfo.setFlags(vk::CommandBufferUsageFlagBits::eOneTimeSubmit);
//         commandBuffer.begin(beginInfo);
//
//         vk::ImageSubresourceRange subresourceRange{};
//         subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor)
//           .setLayerCount(1)
//           .setLevelCount(1);
//
//         vk::ImageMemoryBarrier2 barrier{};
//         barrier.setImage(this->m_swapChainProperties.images.at(this->m_imageIndex))
//             .setSubresourceRange(subresourceRange)
//             .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
//             .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
//             .setOldLayout(vk::ImageLayout::eUndefined)
//             .setNewLayout(vk::ImageLayout::eAttachmentOptimal)
//             .setSrcStageMask(vk::PipelineStageFlagBits2::eNone)
//             .setSrcAccessMask(vk::AccessFlagBits2::eNone)
//             .setDstStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput)
//             .setDstAccessMask(vk::AccessFlagBits2::eColorAttachmentWrite);
//
//
//         vk::DependencyInfo dependencyInfo{};
//         dependencyInfo.setImageMemoryBarriers(barrier);
//         commandBuffer.pipelineBarrier2(dependencyInfo);
//
//         const vk::ClearValue color
//             {{std::array{
//                         F32(clearColor.r) / F32(std::numeric_limits<std::uint8_t>::max()),
//                         F32(clearColor.g) / F32(std::numeric_limits<std::uint8_t>::max()),
//                         F32(clearColor.b) / F32(std::numeric_limits<std::uint8_t>::max()),
//                         F32(clearColor.a) / F32(std::numeric_limits<std::uint8_t>::max())
//             }}};
//
//         vk::RenderingAttachmentInfo colorAttachment{};
//         colorAttachment.setImageView(this->m_swapChainProperties.imageViews.at(this->m_imageIndex))
//             .setImageLayout(vk::ImageLayout::eAttachmentOptimal)
//             .setLoadOp(vk::AttachmentLoadOp::eClear)
//             .setStoreOp(vk::AttachmentStoreOp::eStore)
//             .setClearValue(color);
//
//         //vk::RenderingAttachmentInfo depthAttachment{};
//
//         vk::RenderingInfo rendering_info{};
//         const vk::Rect2D render_area
//             {
//                 .offset = vk::Offset2D{ .x = 0, .y = 0 },
//                 .extent = this->m_swapChainProperties.extent
//             };
//         rendering_info.setRenderArea(render_area)
//             .setColorAttachments(colorAttachment)
//             .setLayerCount(1);
//             //.setPDepthAttachment(&depthAttachment);
//
//         commandBuffer.beginRendering(rendering_info);
//
//         commandBuffer.setViewport(
//         0, vk::Viewport
//             {
//                 .x = 0.0f,
//                 .y = 0.0f,
//                 .width = static_cast<float>(this->m_swapChainProperties.extent.width),
//                 .height = static_cast<float>(this->m_swapChainProperties.extent.height),
//                 .minDepth = 0.0f,
//                 .maxDepth = 1.0f
//             });
//
//         commandBuffer.setScissor(
//             0,vk::Rect2D
//             {
//                 .offset = { .x = 0, .y = 0 },
//                 .extent = this->m_swapChainProperties.extent
//             });
//
//         return true;
//     }
//
//     void Vulkan::endRendering() noexcept
//     {
//         auto& [commandBuffer, imageAvailableSemaphore, inFlightFence] = this->m_frameObjects[this->m_currentFrame];
//         const vk::raii::Semaphore& renderFinishedSemaphore = this->m_renderFinishedSemaphores[this->m_imageIndex];
//
//         commandBuffer.endRendering();
//
//         vk::ImageSubresourceRange subresourceRange{};
//         subresourceRange.setAspectMask(vk::ImageAspectFlagBits::eColor)
//           .setLayerCount(1)
//           .setLevelCount(1);
//
//         vk::ImageMemoryBarrier2 barrier{};
//         barrier.setImage(this->m_swapChainProperties.images.at(this->m_imageIndex))
//             .setSubresourceRange(subresourceRange)
//             .setSrcQueueFamilyIndex(vk::QueueFamilyIgnored)
//             .setDstQueueFamilyIndex(vk::QueueFamilyIgnored)
//             .setOldLayout(vk::ImageLayout::eAttachmentOptimal)
//             .setNewLayout(vk::ImageLayout::ePresentSrcKHR)
//             .setSrcStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput)
//             .setSrcAccessMask(vk::AccessFlagBits2::eColorAttachmentWrite)
//             .setDstStageMask(vk::PipelineStageFlagBits2::eNone)
//             .setDstAccessMask(vk::AccessFlagBits2::eNone);
//
//         vk::DependencyInfo dependencyInfo{};
//         dependencyInfo.setImageMemoryBarriers(barrier);
//         commandBuffer.pipelineBarrier2(dependencyInfo);
//
//         commandBuffer.end();
//
//         vk::CommandBufferSubmitInfo commandBufferInfo{};
//         commandBufferInfo.commandBuffer = commandBuffer;
//         vk::SemaphoreSubmitInfo waitSemaphoreInfo{};
//         waitSemaphoreInfo.setSemaphore(imageAvailableSemaphore)
//           .setStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput);
//         vk::SemaphoreSubmitInfo signalSemaphoreInfo{};
//         signalSemaphoreInfo.setSemaphore(renderFinishedSemaphore)
//           .setStageMask(vk::PipelineStageFlagBits2::eColorAttachmentOutput);
//
//         vk::SubmitInfo2 submitInfo{};
//         submitInfo.setCommandBufferInfos(commandBufferInfo)
//           .setWaitSemaphoreInfos(waitSemaphoreInfo)
//           .setSignalSemaphoreInfos(signalSemaphoreInfo);
//
//         this->m_gpu.graphicsQueue.submit2(submitInfo, inFlightFence);
//
//         vk::PresentInfoKHR presentInfo{};
//         presentInfo.setSwapchains(*this->m_swapChain);
//         presentInfo.setImageIndices(this->m_imageIndex);
//         presentInfo.setWaitSemaphores(signalSemaphoreInfo.semaphore);
//
//         int width{}, height{};
//         glfwGetFramebufferSize(this->m_window, &width, &height);
//
//         if (const auto result = this->m_gpu.presentQueue.presentKHR(presentInfo);
//             width > 0 && height > 0 &&
//             ((result == vk::Result::eErrorOutOfDateKHR || result == vk::Result::eSuboptimalKHR) ||
//             (U32(width) != this->m_swapChainProperties.extent.width || U32(height) != this->m_swapChainProperties.extent.height)))
//         {
//             auto _ = this->recreateSwapChain();
//         }
//
//         this->m_currentFrame = (this->m_currentFrame + 1) % s_virtualFrameCount;
//     }
//
//     void Vulkan::draw(const Mesh& mesh) noexcept
//     {
//         auto it = this->m_meshes.find(&mesh);
//
//         if (it == this->m_meshes.end())
//         {
//             this->uploadMesh(mesh);
//
//             it = this->m_meshes.find(&mesh);
//
//             if (it == this->m_meshes.end())
//                 return;
//         }
//
//         const GPUMesh& gpuMesh = it->second;
//
//         const vk::raii::CommandBuffer& commandBuffer = this->m_frameObjects[this->m_currentFrame].commandBuffer;
//
//         const vk::Buffer vertexBuffer = gpuMesh.vertexBuffer;
//
//         commandBuffer.bindPipeline(vk::PipelineBindPoint::eGraphics, *this->m_graphicsPipeline);
//         commandBuffer.bindVertexBuffers(0, vertexBuffer, {0});
//         commandBuffer.bindIndexBuffer(gpuMesh.indexBuffer, 0, vk::IndexType::eUint32);
//         commandBuffer.drawIndexed(gpuMesh.indexCount, 1, 0, 0, 0);
//     }
//
//     std::expected<void, Error> Vulkan::createInstance() noexcept
//     {
//         const std::uint32_t version = this->m_context.enumerateInstanceVersion().value();
//         Log::Info("Vulkan version: v{}.{}.{}",
//             VK_API_VERSION_MAJOR(version),
//             VK_API_VERSION_MINOR(version),
//             VK_API_VERSION_PATCH(version));
//
//         const char* appName = glfwGetWindowTitle(this->m_window);
//         auto [major, minor, patch] = getProjectVersion();
//         const vk::ApplicationInfo appInfo
//             {
//                 .pApplicationName = appName,
//                 .applicationVersion = VK_MAKE_API_VERSION(0, 0, 0, 0), //TODO: App version stuff
//                 .pEngineName = "InariKonKon",
//                 .engineVersion = VK_MAKE_API_VERSION(0, major, minor, patch),
//                 .apiVersion = VK_API_VERSION_1_4
//             };
//
//         constexpr std::array validationLayers{"VK_LAYER_KHRONOS_validation"};
//         if constexpr (s_enableValidationLayers == true)
//         {
//             if (checkLayerSupport(validationLayers) == false)
//                 return std::unexpected(Error{"No validation layers available!"});
//         }
//
//         const std::vector<const char*> extensions = getRequiredExtensions();
//
//         const vk::InstanceCreateInfo createInfo
//             {
//                 .pNext = (s_enableValidationLayers == true) ? &getDebugMessengerInfo() : nullptr,
//                 .pApplicationInfo = &appInfo,
//                 .enabledLayerCount = (s_enableValidationLayers == true) ? U32(validationLayers.size()) : 0,
//                 .ppEnabledLayerNames = (s_enableValidationLayers == true) ? validationLayers.data() : nullptr,
//                 .enabledExtensionCount = U32(extensions.size()),
//                 .ppEnabledExtensionNames = extensions.data()
//             };
//
//
//         std::expected<vk::raii::Instance, vk::Result> instance = this->m_context.createInstance(createInfo);
//         if (instance.has_value() == false)
//             return std::unexpected(Error{"Failed to create Vulkan instance!"});;
//
//         this->m_instance = std::move(instance.value());
//         return std::expected<void, Error>{};
//     }
//
//     std::expected<void, Error> Vulkan::createDebugMessenger() noexcept
//     {
//         if constexpr (isReleaseBuild() == true) return std::expected<void, Error>{};
//
//         std::expected<vk::raii::DebugUtilsMessengerEXT, vk::Result> debugMessenger =
//             this->m_instance.createDebugUtilsMessengerEXT(getDebugMessengerInfo());
//         if (debugMessenger.has_value() == false)
//             return std::unexpected(Error{"Failed to create debug messenger!"});
//
//         this->m_debugMessenger = std::move(debugMessenger.value());
//         return std::expected<void, Error>{};
//     }
//
//     std::expected<void, Error> Vulkan::createSurface() noexcept
//     {
//         if (VkSurfaceKHR surface = VK_NULL_HANDLE;
//             glfwCreateWindowSurface(*this->m_instance, this->m_window, nullptr, &surface) == VK_SUCCESS)
//         {
//             this->m_surface = vk::raii::SurfaceKHR{ this->m_instance, surface };
//             return std::expected<void, Error>{};
//         }
//         return std::unexpected(Error{"Failed to create window surface!"});
//     }
//
//     std::expected<void, Error> Vulkan::pickPhysicalDevice() noexcept
//     {
//         std::expected<std::vector<vk::raii::PhysicalDevice>, vk::Result> devices = this->m_instance.enumeratePhysicalDevices();
//
//         if (devices.has_value() == false)
//             return std::unexpected(Error{"Failed to enumerate physical devices!"});
//
//         if (devices.value().empty() == true)
//             return std::unexpected(Error{"No physical devices found!"});
//
//         constexpr auto getRating = [](const vk::raii::PhysicalDevice& device) noexcept
//             {
//                 const vk::PhysicalDeviceProperties properties = device.getProperties();
//                 const vk::PhysicalDeviceFeatures features = device.getFeatures();
//
//                 using RequirementFunc = vk::Bool32(*)(const vk::PhysicalDeviceFeatures&) noexcept;
//                 constexpr auto requirements = std::array<RequirementFunc, 2>
//                     {
//                         //TODO:
//                         [](const vk::PhysicalDeviceFeatures& f) noexcept
//                         {
//                             return f.tessellationShader;
//                         },
//                         [](const vk::PhysicalDeviceFeatures& f) noexcept
//                         {
//                             return f.geometryShader;
//                         }
//                     };
//
//                 if (std::ranges::all_of(requirements,
//                     [&features](const auto& req)
//                     {
//                         return req(features) == vk::True;
//                     }) == false)
//                 {
//                     return 0u;
//                 }
//
//                 using ScoringFunc = std::uint32_t(*)(const vk::PhysicalDeviceProperties&) noexcept;
//                 constexpr auto scoringRules = std::array<ScoringFunc, 2>
//                     {
//                         //TODO:
//                         [](const vk::PhysicalDeviceProperties& prop) noexcept
//                         {
//                             return prop.deviceType == vk::PhysicalDeviceType::eDiscreteGpu ? 10000u : 0u;
//                         },
//                         [](const vk::PhysicalDeviceProperties& prop) noexcept
//                         {
//                             return prop.limits.maxImageDimension2D;
//                         }
//                     };
//
//                 return std::ranges::fold_left(scoringRules | std::views::transform(
//                     [&properties](const auto& rule) noexcept
//                     {
//                         return rule(properties);
//                     }), 1u, std::plus{});
//             };
//
//         vk::raii::PhysicalDevice* bestDevice = nullptr;
//         std::uint32_t bestScore = 0;
//
//         for (vk::raii::PhysicalDevice& device : devices.value())
//         {
//             if (const std::uint32_t score = getRating(device); score > bestScore)
//             {
//                 bestScore = score;
//                 bestDevice = &device;
//             }
//         }
//
//         if (bestDevice == nullptr && bestScore == 0u) return std::unexpected(Error{"No suitable physical device found!"});
//
//         this->m_gpu.physicalDevice = std::move(*bestDevice);
//         return std::expected<void, Error>{};
//     }
//
//     std::expected<void, Error> Vulkan::createLogicalDevice() noexcept
//     {
//         if (this->m_gpu.setUpFamilies(this->m_surface) == false)
//             return std::unexpected(Error{"Failed to set up device families!"});
//
//         std::vector<std::uint32_t> uniqueFamilies{};
//         uniqueFamilies.emplace_back(this->m_gpu.graphicsFamily);
//         if (this->m_gpu.presentFamily != this->m_gpu.graphicsFamily)
//             uniqueFamilies.emplace_back(this->m_gpu.presentFamily);
//
//         std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos{};
//         queueCreateInfos.reserve(uniqueFamilies.size());
//         std::ranges::transform(uniqueFamilies, std::back_inserter(queueCreateInfos),
//             [](std::uint32_t family) noexcept
//             {
//                 static constexpr float priority = 1.0f;
//                 return vk::DeviceQueueCreateInfo{ .queueFamilyIndex = family, .queueCount = 1, .pQueuePriorities = &priority };
//             });
//
//         //TODO: Sync this up with the requirements in physical device...
//         constexpr vk::PhysicalDeviceFeatures deviceFeatures
//         {
//             .geometryShader = vk::True,
//             .tessellationShader = vk::True
//         };
//
//         vk::PhysicalDeviceDynamicRenderingFeatures dynamicRenderingFeature{ .dynamicRendering = vk::True };
//         vk::PhysicalDeviceSynchronization2Features syncFeature{ .pNext = &dynamicRenderingFeature, .synchronization2 = vk::True };
//
//         constexpr std::array deviceExtensions = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
//
//         vk::DeviceCreateInfo createInfo{};
//         createInfo.setPNext(&syncFeature);
//         createInfo.setQueueCreateInfos(queueCreateInfos);
//         createInfo.setPEnabledExtensionNames(deviceExtensions);
//         createInfo.setPEnabledFeatures(&deviceFeatures);
//
//         std::expected<vk::raii::Device, vk::Result> device = this->m_gpu.physicalDevice.createDevice(createInfo);
//         if (device.has_value() == false)
//             return std::unexpected(Error{"Failed to create logical device!"});
//         this->m_gpu.device = std::move(device.value());
//
//         std::expected<vk::raii::Queue, vk::Result> graphicsQueue = this->m_gpu.device.getQueue(this->m_gpu.graphicsFamily, 0);
//         if (graphicsQueue.has_value() == false)
//             return std::unexpected(Error{"Failed to get graphics queue!"});
//         this->m_gpu.graphicsQueue = std::move(graphicsQueue.value());
//
//         std::expected<vk::raii::Queue, vk::Result> presentQueue = this->m_gpu.device.getQueue(this->m_gpu.presentFamily, 0);
//         if (presentQueue.has_value() == false)
//             return std::unexpected(Error{"Failed to get present queue!"});
//         this->m_gpu.presentQueue = std::move(presentQueue.value());
//
//         return std::expected<void, Error>{};
//     }
//
//     std::expected<void, Error> Vulkan::createAllocator() noexcept
//     {
//         VmaAllocatorCreateInfo createInfo{};
//         createInfo.instance = *this->m_instance;
//         createInfo.physicalDevice = *this->m_gpu.physicalDevice;
//         createInfo.device = *this->m_gpu.device;
//         createInfo.vulkanApiVersion = VK_API_VERSION_1_4;
//
//         if (vmaCreateAllocator(&createInfo, &this->m_allocator) != VK_SUCCESS)
//             return std::unexpected(Error{"Failed to create VMA allocator!"});
//
//         return {};
//     }
//
//     std::expected<void, Error> Vulkan::createSwapChain() noexcept
//     {
//         const SwapChainProperties::SupportDetails swapChainSupportDetails =
//             SwapChainProperties::querySwapChainSupport(this->m_gpu.physicalDevice, this->m_surface);
//
//         if (swapChainSupportDetails.formats.empty() == true || swapChainSupportDetails.presentModes.empty() == true)
//             return std::unexpected(Error{"Failed to query swap chain support!"});
//
//         int width{}, height{};
//         glfwGetFramebufferSize(this->m_window, &width, &height);
//
//         // Minimized Wayland windows report a zero-sized framebuffer. Do not
//         // convert that size to unsigned values or recreate a swapchain for it.
//         if (width <= 0 || height <= 0)
//             return std::unexpected(Error{"Framebuffer has no drawable area!"});
//
//         const auto [format, colorSpace] = swapChainSupportDetails.getSurfaceFormat();
//         const vk::PresentModeKHR presentMode = swapChainSupportDetails.getPresentMode();
//         const vk::Extent2D extent = swapChainSupportDetails.getExtent({ U32(width), U32(height) });
//         const std::uint32_t imageCount = swapChainSupportDetails.getImageCount();
//
//         std::array queueFamilyIndices = { this->m_gpu.graphicsFamily, this->m_gpu.presentFamily };
//
//         vk::SwapchainCreateInfoKHR createInfo
//             {
//                 .surface = this->m_surface,
//                 .minImageCount = imageCount,
//                 .imageFormat = format,
//                 .imageColorSpace = colorSpace,
//                 .imageExtent = extent,
//                 .imageArrayLayers = 1,
//                 .imageUsage = vk::ImageUsageFlagBits::eColorAttachment,
//                 .preTransform = swapChainSupportDetails.capabilities.currentTransform,
//                 .compositeAlpha = vk::CompositeAlphaFlagBitsKHR::eOpaque,
//                 .presentMode = presentMode,
//                 .clipped = true,
//                 .oldSwapchain = this->m_swapChain == nullptr ? nullptr : *this->m_swapChain,
//             };
//
//         if (this->m_gpu.graphicsFamily != this->m_gpu.presentFamily)
//         {
//             createInfo.imageSharingMode = vk::SharingMode::eConcurrent;
//             createInfo.setQueueFamilyIndices(queueFamilyIndices);
//         }
//         else
//             createInfo.imageSharingMode = vk::SharingMode::eExclusive;
//
//         this->m_gpu.device.waitIdle();
//
//         std::expected<vk::raii::SwapchainKHR, vk::Result> swapChain = this->m_gpu.device.createSwapchainKHR(createInfo);
//         if (swapChain.has_value() == false)
//             return std::unexpected(Error{"Failed to create swap chain!"});
//
//         this->m_swapChainProperties.imageViews.clear();
//
//         this->m_swapChain = std::move(swapChain.value());
//
//         this->m_swapChainProperties.images = this->m_swapChain.getImages().value();
//
//         this->m_swapChainProperties.imageFormat = format;
//         this->m_swapChainProperties.extent = extent;
//
//         return std::expected<void, Error>{};
//     }
//
//     std::expected<void, Error> Vulkan::createImageViews() noexcept
//     {
//         this->m_swapChainProperties.imageViews.reserve(this->m_swapChainProperties.images.size());
//
//         vk::ImageViewCreateInfo createInfo
//         {
//             .viewType =  vk::ImageViewType::e2D,
//             .format = this->m_swapChainProperties.imageFormat,
//             .components =
//                 {
//                     .r = vk::ComponentSwizzle::eIdentity,
//                     .g = vk::ComponentSwizzle::eIdentity,
//                     .b = vk::ComponentSwizzle::eIdentity,
//                     .a = vk::ComponentSwizzle::eIdentity
//                 },
//            .subresourceRange = vk::ImageSubresourceRange
//             {
//                 .aspectMask     = vk::ImageAspectFlagBits::eColor,
//                 .baseMipLevel   = 0,
//                 .levelCount     = 1,
//                 .baseArrayLayer = 0,
//                 .layerCount     = 1
//             }
//         };
//
//         for (const vk::Image& image : this->m_swapChainProperties.images)
//         {
//             createInfo.image = image;
//             std::expected<vk::raii::ImageView, vk::Result> imageView = this->m_gpu.device.createImageView(createInfo);
//             if (imageView.has_value() == false)
//                 return std::unexpected(Error{"Failed to create image view!"});
//             this->m_swapChainProperties.imageViews.emplace_back(std::move(imageView.value()));
//         }
//         return std::expected<void, Error>{};
//     }
//
//     std::expected<void, Error> Vulkan::createGraphicsPipeline() noexcept
//     {
//         auto vertexShaderCode = R"(
//             #version 450
//
//             layout(location = 0) in vec2 inPosition;
//             layout(location = 1) in vec4 inColor;
//
//             layout(location = 0) out vec4 fragColor;
//
//             void main()
//             {
//                 gl_Position = vec4(inPosition, 0.0, 1.0);
//                 fragColor = inColor;
//             }
//             )";
//         auto fragmentShaderCode = R"(
//             #version 450
//
//             layout(location = 0) in vec4 fragColor;
//
//             layout(location = 0) out vec4 outColor;
//
//             void main()
//             {
//                 outColor = fragColor;
//             }
//             )";
//
//         Shader vertex{Shader::Type::Vertex, std::string_view{vertexShaderCode}};
//         Shader fragment{Shader::Type::Fragment, std::string_view{fragmentShaderCode}};
//
//         const auto createShaderModule =
//             [this](const auto& shader) noexcept -> std::expected<vk::raii::ShaderModule, Error>
//             {
//                 const auto SPIRV = shader.convertToSPIRV();
//
//                 if (SPIRV.has_value() == false)
//                     return std::unexpected(SPIRV.error());
//
//                 vk::ShaderModuleCreateInfo createInfo{};
//                 createInfo.setCode(SPIRV.value());
//
//                 auto shaderModule = this->m_gpu.device.createShaderModule(createInfo);
//
//                 if (!shaderModule)
//                     return std::unexpected(Error{"Failed to create shader module!"});
//
//                 return std::move(shaderModule.value());
//             };
//
//         auto vertexShader = createShaderModule(vertex);
//         if (!vertexShader)
//             return std::unexpected(vertexShader.error());
//
//         this->m_vertexShader = std::move(vertexShader.value());
//
//         auto fragmentShader = createShaderModule(fragment);
//         if (!fragmentShader)
//             return std::unexpected(fragmentShader.error());
//
//         this->m_fragmentShader = std::move(fragmentShader.value());
//
//         //
//
//         vk::PipelineShaderStageCreateInfo vertexStage{};
//         vertexStage
//             .setStage(vk::ShaderStageFlagBits::eVertex)
//             .setModule(*this->m_vertexShader)
//             .setPName("main");
//
//         vk::PipelineShaderStageCreateInfo fragmentStage{};
//         fragmentStage
//             .setStage(vk::ShaderStageFlagBits::eFragment)
//             .setModule(*this->m_fragmentShader)
//             .setPName("main");
//
//         const std::array shaderStages
//             {
//                 vertexStage,
//                 fragmentStage
//             };
//
//         //TODO: get from vertex buffer
//         vk::VertexInputBindingDescription binding{};
//         binding
//             .setBinding(0)
//             .setStride(sizeof(Vec2f) + sizeof(Color))
//             .setInputRate(vk::VertexInputRate::eVertex);
//
//         vk::VertexInputAttributeDescription positionInput{};
//         positionInput
//             .setLocation(0)
//             .setBinding(0)
//             .setFormat(vk::Format::eR32G32Sfloat)
//             .setOffset(0);
//
//         vk::VertexInputAttributeDescription colorInput{};
//         colorInput
//             .setLocation(1)
//             .setBinding(0)
//             .setFormat(vk::Format::eR8G8B8A8Unorm)
//             .setOffset(sizeof(Vec2f));
//
//         const std::array attributes
//             {
//                 positionInput, colorInput
//             };
//
//         vk::PipelineVertexInputStateCreateInfo vertexInput{};
//         vertexInput
//             .setVertexBindingDescriptions(binding)
//             .setVertexAttributeDescriptions(attributes);
//
//         vk::PipelineInputAssemblyStateCreateInfo inputAssembly{};
//         inputAssembly
//             .setTopology(vk::PrimitiveTopology::eTriangleList)
//             .setPrimitiveRestartEnable(vk::False);
//
//         vk::PipelineViewportStateCreateInfo viewportState{};
//         viewportState
//             .setViewportCount(1)
//             .setScissorCount(1);
//
//         constexpr std::array dynamicStates
//             {
//                 vk::DynamicState::eViewport,
//                 vk::DynamicState::eScissor
//             };
//
//         vk::PipelineDynamicStateCreateInfo dynamicState{};
//         dynamicState.setDynamicStates(dynamicStates);
//
//         // vk::PipelineDepthStencilStateCreateInfo depthStencil{};
//         // depthStencil
//         //     .setDepthTestEnable(true)
//         //     .setDepthWriteEnable(true)
//         //     .setDepthCompareOp(vk::CompareOp::eLess);
//
//         vk::PipelineRasterizationStateCreateInfo rasterization{};
//         rasterization
//             .setDepthClampEnable(vk::False)
//             .setRasterizerDiscardEnable(vk::False)
//             .setPolygonMode(vk::PolygonMode::eFill)
//             .setCullMode(vk::CullModeFlagBits::eNone)
//             .setFrontFace(vk::FrontFace::eCounterClockwise)
//             .setDepthBiasEnable(vk::False)
//             .setLineWidth(1.0f);
//
//         vk::PipelineMultisampleStateCreateInfo multisampling{};
//         multisampling
//             .setRasterizationSamples(vk::SampleCountFlagBits::e1)
//             .setSampleShadingEnable(vk::False);
//
//         vk::PipelineColorBlendAttachmentState colorBlendAttachment{};
//         colorBlendAttachment
//             .setBlendEnable(vk::False)
//             .setColorWriteMask(
//                 vk::ColorComponentFlagBits::eR  |
//                 vk::ColorComponentFlagBits::eG  |
//                 vk::ColorComponentFlagBits::eB  |
//                 vk::ColorComponentFlagBits::eA);
//
//         vk::PipelineColorBlendStateCreateInfo colorBlending{};
//         colorBlending
//             .setLogicOpEnable(vk::False)
//             .setAttachments(colorBlendAttachment);
//
//         vk::PipelineLayoutCreateInfo pipelineLayoutInfo{};
//
//         std::expected<vk::raii::PipelineLayout, vk::Result> pipelineLayout = this->m_gpu.device.createPipelineLayout(pipelineLayoutInfo);
//         if (pipelineLayout.has_value() == false)
//             return std::unexpected(Error{"Failed to create pipeline layout!"});
//
//         this->m_pipelineLayout = std::move(pipelineLayout.value());
//
//         vk::PipelineRenderingCreateInfo renderingInfo{};
//         renderingInfo
//             .setColorAttachmentFormats(this->m_swapChainProperties.imageFormat);
//
//         vk::GraphicsPipelineCreateInfo pipelineInfo{};
//         pipelineInfo
//             .setStages(shaderStages)
//             .setPVertexInputState(&vertexInput)
//             .setPInputAssemblyState(&inputAssembly)
//             .setPViewportState(&viewportState)
//             .setPRasterizationState(&rasterization)
//             .setPMultisampleState(&multisampling)
//             .setPColorBlendState(&colorBlending)
//             .setPDynamicState(&dynamicState)
//             .setLayout(*this->m_pipelineLayout)
//             .setPNext(&renderingInfo);
//
//         std::expected<vk::raii::Pipeline, vk::Result> pipeline = this->m_gpu.device.createGraphicsPipeline(nullptr, pipelineInfo);
//         if (pipeline.has_value() == false)
//             return std::unexpected(Error{"Failed to create graphics pipeline!"});
//
//         this->m_graphicsPipeline = std::move(pipeline.value());
//
//         return std::expected<void, Error>{};
//     }
//
//     std::expected<void, Error> Vulkan::createCommandPool() noexcept
//     {
//         const vk::CommandPoolCreateInfo createInfo
//             {
//                 .flags = vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
//                 .queueFamilyIndex = this->m_gpu.graphicsFamily
//             };
//
//         std::expected<vk::raii::CommandPool, vk::Result> commandPool = this->m_gpu.device.createCommandPool(createInfo);
//         if (commandPool.has_value() == false)
//             return std::unexpected(Error{"Failed to create command pool!"});
//         this->m_commandPool = std::move(commandPool.value());
//         return std::expected<void, Error>{};
//     }
//
//     std::expected<void, Error> Vulkan::createCommandBuffers() noexcept
//     {
//         const vk::CommandBufferAllocateInfo createInfo
//             {
//                 .commandPool = this->m_commandPool,
//                 .level = vk::CommandBufferLevel::ePrimary,
//                 .commandBufferCount = s_virtualFrameCount
//             };
//
//         std::expected<std::vector<vk::raii::CommandBuffer>, vk::Result> commandBuffers = this->m_gpu.device.allocateCommandBuffers(createInfo);
//         if (commandBuffers.has_value() == false) return std::unexpected(Error{"Failed to allocate command buffers!"});
//         for (std::size_t i = 0; i < s_virtualFrameCount; ++i)
//             this->m_frameObjects[i].commandBuffer = std::move(commandBuffers.value()[i]);
//         return std::expected<void, Error>{};
//     }
//
//     std::expected<void, Error> Vulkan::createBufferedSyncObjects() noexcept
//     {
//         for (std::size_t i = 0; i < s_virtualFrameCount; ++i)
//         {
//             std::expected<vk::raii::Semaphore, vk::Result> imageAvailableSemaphore = this->m_gpu.device.createSemaphore({});
//             if (imageAvailableSemaphore.has_value() == false) return std::unexpected(Error{"Failed to create image available semaphore!"});
//             this->m_frameObjects[i].imageAvailableSemaphore = std::move(imageAvailableSemaphore.value());
//
//             constexpr vk::FenceCreateInfo fenceCreateInfo{ .flags = vk::FenceCreateFlagBits::eSignaled };
//             std::expected<vk::raii::Fence, vk::Result> inFlightFence = this->m_gpu.device.createFence(fenceCreateInfo);
//             if (inFlightFence.has_value() == false) return std::unexpected(Error{"Failed to create in flight fence!"});
//             this->m_frameObjects[i].inFlightFence = std::move(inFlightFence.value());
//         }
//         return std::expected<void, Error>{};
//     }
//
//     std::expected<void, Error> Vulkan::createRenderFinishedSemaphores() noexcept
//     {
//         this->m_renderFinishedSemaphores.clear();
//         this->m_renderFinishedSemaphores.reserve(this->m_swapChainProperties.images.size());
//         for (std::size_t i = 0; i < this->m_swapChainProperties.images.size(); ++i)
//         {
//             std::expected<vk::raii::Semaphore, vk::Result> renderFinishedSemaphore = this->m_gpu.device.createSemaphore({});
//             if (renderFinishedSemaphore.has_value() == false) return std::unexpected(Error{"Failed to create render finished semaphore!"});
//             this->m_renderFinishedSemaphores.emplace_back(std::move(renderFinishedSemaphore.value()));
//         }
//         return std::expected<void, Error>{};
//     }
//
//     //TODO: return value and other stuff...
//     std::expected<void, Error> Vulkan::recreateSwapChain() noexcept
//     {
//         if (this->m_gpu.device != nullptr)
//             this->m_gpu.device.waitIdle();
//
//         std::expected<void, Error> value = this->createSwapChain();
//         if (value.has_value() == false) return value;
//
//         value = this->createImageViews();
//         if (value.has_value() == false) return value;
//
//         value = this->createGraphicsPipeline();
//         if (value.has_value() == false) return value;
//
//         value = this->createRenderFinishedSemaphores();
//         if (value.has_value() == false) return value;
//
//         return std::expected<void, Error>{};
//     }
//
//     std::expected<std::pair<vk::Buffer, VmaAllocation>, Error> Vulkan::createBuffer(
//         vk::DeviceSize size, vk::BufferUsageFlags usage,
//         VmaMemoryUsage memoryUsage, VmaAllocationCreateFlags allocationFlags) const noexcept
//     {
//         const vk::BufferCreateInfo bufferInfo
//             {
//                 .size = size,
//                 .usage = usage,
//                 .sharingMode = vk::SharingMode::eExclusive
//             };
//
//         VmaAllocationCreateInfo allocationInfo{};
//         allocationInfo.usage = memoryUsage;
//         allocationInfo.flags = allocationFlags;
//
//         VkBuffer buffer = VK_NULL_HANDLE;
//         VmaAllocation allocation = nullptr;
//
//         const VkResult result = vmaCreateBuffer(
//             this->m_allocator, reinterpret_cast<const VkBufferCreateInfo*>(&bufferInfo),
//             &allocationInfo, &buffer, &allocation, nullptr);
//
//         if (result != VK_SUCCESS)
//             return std::unexpected(Error{"Failed to create GPU buffer!"});
//
//         return std::pair
//             {
//                 vk::Buffer{buffer},
//                 allocation
//             };
//     }
//
//     void Vulkan::uploadMesh(const Mesh& mesh) noexcept
//     {
//         GPUMesh gpuMesh{this->m_allocator};
//
//         const vk::DeviceSize vertexBufferSize = mesh.vertexBuffer.size();
//         const vk::DeviceSize indexBufferSize = mesh.indexBuffer.size() * sizeof(std::uint32_t);
//
//         if (vertexBufferSize == 0 || indexBufferSize == 0)
//         {
//             Log::Warning("Attempted to upload an empty mesh!");
//             return;
//         }
//
//         auto vertexBuffer = this->createBuffer(
//             vertexBufferSize, vk::BufferUsageFlagBits::eVertexBuffer, VMA_MEMORY_USAGE_AUTO,
//             VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT);
//
//         if (!vertexBuffer)
//         {
//             Log::Error("{}", vertexBuffer.error().what());
//             return;
//         }
//
//         gpuMesh.vertexBuffer = vertexBuffer->first;
//         gpuMesh.vertexAllocation = vertexBuffer->second;
//
//         auto indexBuffer = this->createBuffer(
//             indexBufferSize, vk::BufferUsageFlagBits::eIndexBuffer, VMA_MEMORY_USAGE_AUTO,
//             VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT | VMA_ALLOCATION_CREATE_MAPPED_BIT);
//
//         if (!indexBuffer)
//         {
//             Log::Error("{}", indexBuffer.error().what());
//             return;
//         }
//
//         gpuMesh.indexBuffer = indexBuffer->first;
//         gpuMesh.indexAllocation = indexBuffer->second;
//
//         gpuMesh.indexCount = U32(mesh.indexBuffer.size());
//
//         void* vertexData = nullptr;
//
//         if (vmaMapMemory(this->m_allocator, gpuMesh.vertexAllocation, &vertexData) != VK_SUCCESS)
//         {
//             Log::Error("Failed to map vertex buffer memory!");
//             return;
//         }
//
//         auto* destination = static_cast<std::byte*>(vertexData);
//
//         for (const Vertex& vertex : mesh.vertexBuffer.vertices())
//         {
//             std::memcpy(
//                 destination,
//                 vertex.data(),
//                 vertex.size());
//
//             destination += vertex.size();
//         }
//
//         vmaUnmapMemory(this->m_allocator, gpuMesh.vertexAllocation);
//
//         void* indexData = nullptr;
//
//         if (vmaMapMemory(this->m_allocator, gpuMesh.indexAllocation, &indexData) != VK_SUCCESS)
//         {
//             Log::Error("Failed to map index buffer memory!");
//             return;
//         }
//
//         std::ranges::copy(mesh.indexBuffer, static_cast<std::uint32_t*>(indexData));
//
//         vmaUnmapMemory(this->m_allocator, gpuMesh.indexAllocation);
//
//         this->m_meshes.emplace(&mesh, std::move(gpuMesh));
//     }
//
//     bool Vulkan::GPU::setUpFamilies(const vk::raii::SurfaceKHR& surface) noexcept
//     {
//         const std::vector<vk::QueueFamilyProperties> queueFamilies = this->physicalDevice.getQueueFamilyProperties();
//
//         std::optional<std::uint32_t> graphics{}, present{};
//         for (std::uint32_t i = 0; i < queueFamilies.size(); ++i)
//         {
//             if (queueFamilies[i].queueFlags & vk::QueueFlagBits::eGraphics)
//                 graphics = i;
//
//             if (this->physicalDevice.getSurfaceSupportKHR(i, surface) == vk::True)
//                 present = i;
//
//             if (graphics.has_value() == true && present.has_value() == true)
//                 break;
//         }
//
//         if (graphics.has_value() == false || present.has_value() == false)
//             return false;
//
//         this->graphicsFamily = graphics.value();
//         this->presentFamily = present.value();
//         return true;
//     }
//
//     vk::SurfaceFormatKHR Vulkan::SwapChainProperties::SupportDetails::getSurfaceFormat() const noexcept
//     {
//         if (this->formats.size() == 1 && this->formats.front().format == vk::Format::eUndefined)
//             return { .format = vk::Format::eB8G8R8A8Srgb, .colorSpace = vk::ColorSpaceKHR::eSrgbNonlinear };
//
//         const auto preferred = std::ranges::find_if(this->formats,
//                 [](const vk::SurfaceFormatKHR& format) noexcept
//                 {
//                     return format.format == vk::Format::eB8G8R8A8Srgb &&
//                            format.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear;
//                 });
//         return preferred != this->formats.end() ? *preferred : this->formats.front();
//     }
//
//     vk::PresentModeKHR Vulkan::SwapChainProperties::SupportDetails::getPresentMode() const noexcept
//     {
//         const auto it = std::ranges::find(this->presentModes, vk::PresentModeKHR::eMailbox);
//         return it != this->presentModes.end() ? *it : vk::PresentModeKHR::eFifo;
//     }
//
//     vk::Extent2D Vulkan::SwapChainProperties::SupportDetails::getExtent(Vec2u size) const noexcept
//     {
//         if (this->capabilities.currentExtent.width != std::numeric_limits<std::uint32_t>::max()) return this->capabilities.currentExtent;
//         return
//         {
//             .width = std::clamp(size.x(), this->capabilities.minImageExtent.width, this->capabilities.maxImageExtent.width),
//             .height = std::clamp(size.y(), this->capabilities.minImageExtent.height, this->capabilities.maxImageExtent.height)
//         };
//     }
//
//     std::uint32_t Vulkan::SwapChainProperties::SupportDetails::getImageCount() const noexcept
//     {
//         std::uint32_t minImageCount = std::max(3u, this->capabilities.minImageCount);
//         if (this->capabilities.maxImageCount > 0 && this->capabilities.maxImageCount < minImageCount)
//             minImageCount = this->capabilities.maxImageCount;
//         return minImageCount;
//     }
//
//     Vulkan::SwapChainProperties::SupportDetails Vulkan::SwapChainProperties::querySwapChainSupport(
//         const vk::PhysicalDevice& device, const vk::SurfaceKHR& surface) noexcept
//     {
//         const auto surfaceCapabilities = device.getSurfaceCapabilitiesKHR(surface);
//         if (surfaceCapabilities.has_value() == false)
//             return {};
//
//         const auto surfaceFormats = device.getSurfaceFormatsKHR(surface);
//         if (surfaceFormats.has_value() == false)
//             return {};
//
//         const auto surfacePresentModes = device.getSurfacePresentModesKHR(surface);
//         if (surfacePresentModes.has_value() == false)
//             return {};
//
//         return SupportDetails
//             {
//                 .capabilities   = surfaceCapabilities.value(),
//                 .formats        = surfaceFormats.value(),
//                 .presentModes   = surfacePresentModes.value()
//             };
//     }
//
//     bool Vulkan::checkLayerSupport(const auto& layers) const noexcept
//     {
//         const std::vector<vk::LayerProperties> availableLayers = this->m_context.enumerateInstanceLayerProperties().value();
//         return std::ranges::all_of(layers,
//             [&availableLayers](std::string_view layer) noexcept
//             {
//                 return std::ranges::any_of(availableLayers,
//                     [layer](const vk::LayerProperties& property) noexcept
//                     {
//                         return layer == property.layerName;
//                     });
//             });
//     }
//
//     std::vector<const char*> Vulkan::getRequiredExtensions() noexcept
//     {
//         std::uint32_t count = 0;
//         const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&count);
//
//         std::vector<const char*> extensions(glfwExtensions, glfwExtensions + count);
//
//         if constexpr (isDebugBuild() == true)
//             extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
//
//         return extensions;
//     }
//
//     const vk::DebugUtilsMessengerCreateInfoEXT& Vulkan::getDebugMessengerInfo() noexcept
//     {
//         static const auto debugCallback = [](vk::DebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
//                                              [[maybe_unused]] vk::DebugUtilsMessageTypeFlagsEXT messageType,
//                                              const vk::DebugUtilsMessengerCallbackDataEXT* pCallbackData,
//                                              [[maybe_unused]] void* pUserData) noexcept
//         {
//             switch (messageSeverity)
//             {
//             case vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose: [[fallthrough]];
//             case vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo:
//                 Log::Info("Vulkan: {}", pCallbackData->pMessage);
//                 break;
//             case vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning:
//                 Log::Warning("Vulkan: {}", pCallbackData->pMessage);
//                 break;
//             case vk::DebugUtilsMessageSeverityFlagBitsEXT::eError:
//                 Log::Error("Vulkan: {}", pCallbackData->pMessage);
//                 break;
//             }
//             return vk::False;
//         };
//
//         static vk::DebugUtilsMessengerCreateInfoEXT createInfo
//             {
//                 .messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose  |
//                     vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo                 |
//                     vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning              |
//                     vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
//                 .messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral   |
//                     vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation        |
//                     vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance,
//                 .pfnUserCallback = debugCallback
//             };
//         return createInfo;
//     }
// }
