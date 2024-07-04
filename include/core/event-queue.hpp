#pragma once

#include <memory>
#include <map>
#include <queue>
#include <mutex>

namespace CzaraEngine {
    enum EventDataObject {
        STRING,
        COMPONENT,
        COMPONENT_COLLECTION
    };
    typedef std::pair<EventDataObject, std::shared_ptr<void>> QueueObject;
    class EventDataQueue {
        public:
            static void enqueue(EventDataObject &type, std::shared_ptr<void> &shared);
            template<typename T>
            static std::shared_ptr<T> dequeue();
            static EventDataObject peekType();
            static bool empty();
        private:
            static std::mutex& getMutex();
            static std::queue<QueueObject>& getQueue();
    };
    template<typename T>
    std::shared_ptr<T> EventDataQueue::dequeue() {
        if (empty()) return std::shared_ptr<T>();
        std::lock_guard<std::mutex> guard(getMutex());
        QueueObject obj = getQueue().front();
        getQueue().pop();
        return static_pointer_cast<T>(obj.second);
    }
}
