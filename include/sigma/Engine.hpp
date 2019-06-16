#pragma once

#include <memory>
#include <vector>
#include <map>
#include <set>
#include <string>

namespace spdlog
{
    class logger;
}

enum class GraphicsAPI
{
    None,
    OpenGL,
    Vulkan
};

class Window;
class Device;
class EventListener;
class EventEmitter;

class Engine : public std::enable_shared_from_this<Engine>
{
public:
    virtual ~Engine() = default;

    GraphicsAPI graphicsAPI() const;

    virtual bool initialize(GraphicsAPI inGraphicsAPI);

    std::shared_ptr<Window> createWindow(const std::string &inTitle, std::size_t inWidth, std::size_t inHeight);

    void addEmitter(std::weak_ptr<EventEmitter> inEmitter);

    void removeEmitter(std::weak_ptr<EventEmitter> inEmitter);

    void addListener(std::weak_ptr<EventListener> inListener);

    void removeListener(std::weak_ptr<EventListener> inListener);

    std::shared_ptr<Device> graphicsDevice();

    bool process();

    static std::shared_ptr<Engine> create();
private:
    GraphicsAPI mGraphicsAPI = GraphicsAPI::None;
    bool mDeviceInitialized = false;
    std::map<GraphicsAPI, std::set<std::string>> mRequiredExtensions;
    std::shared_ptr<Device> mDevice = nullptr;
    std::shared_ptr<spdlog::logger> mConsole = nullptr;
    std::vector<std::weak_ptr<EventEmitter>> mEventEmitters;
    std::vector<std::weak_ptr<EventListener>> mEventListeners;
};
