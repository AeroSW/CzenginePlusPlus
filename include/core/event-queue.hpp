#pragma once

#include "shared.hpp"
#include <map>
#include <queue>
#include <mutex>

namespace CzaraEngine {
    enum EventDataObject {
        STRING,
        COMPONENT,
        COMPONENT_COLLECTION
    };
    typedef std::pair<EventDataObject, Shared<void>> QueueObject;
    class EventDataQueue {
        public:
            static void enqueue(EventDataObject &type, Shared<void> &shared);
            template<typename T>
            static Shared<T> dequeue();
            static EventDataObject peekType();
            static bool empty();
        private:
            static std::mutex& getMutex();
            static std::queue<QueueObject>& getQueue();
    };
    template<typename T>
    Shared<T> EventDataQueue::dequeue() {
        if (empty()) return Shared<T>();
        std::lock_guard<std::mutex> guard(getMutex());
        QueueObject obj = getQueue().front();
        getQueue().pop();
        return static_pointer_cast<T>(obj.second);
    }
}
