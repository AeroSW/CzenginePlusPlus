#include "event-queue.hpp"
#include <thread>
#include <mutex>

namespace CzaraEngine {

    void EventDataQueue::enqueue(EventDataObject &type, std::shared_ptr<void> &obj) {
        std::lock_guard<std::mutex> guard(getMutex());
        QueueObject queue_obj {type, obj};
        getQueue().push(queue_obj);
    }

    EventDataObject EventDataQueue::peekType() {
        return getQueue().front().first;
    }

    bool EventDataQueue::empty() {
        return getQueue().empty();
    }

    std::mutex& EventDataQueue::getMutex() {
        static std::mutex m_mutex;
        return m_mutex;
    }

    std::queue<QueueObject>& EventDataQueue::getQueue() {
        static std::queue<QueueObject> m_queue;
        return m_queue;
    }
}
