#include <rrobot/state/rrqueue_manager.hpp>

/*
 * Replaces stateManager, aside from events all objects are controlled within the class
 */

using namespace rrobot;

/*
 * Check for stray events, and remove them.
 */
RrQueueManager::~RrQueueManager() {
    const std::lock_guard<std::mutex> lock(_mtx);
    
    // allow some time for queues to clear
    std::this_thread::sleep_for(_queueWaitTime * _queues.size());

    for (const auto& [key, value] : _queues) {
        if (!isEmpty(key)) {
            Event* e = dequeue(key);
            delete(e);
        }
    }
}

void RrQueueManager::addQueue(RRP_QUEUES q) {
    const std::lock_guard<std::mutex> lock(_mtx);
    queue<Event*> qu;
    _queues.emplace(q, qu);
    auto& mtx = _locks[q];
}


chrono::milliseconds RrQueueManager::queueProcessTime(RRP_QUEUES q) {
    return _queueProcessTime;
}

void  RrQueueManager::enqueue(RRP_QUEUES q, Event* e) {
    if (_queues.find(q) == _queues.end()) {
        throw QueueDoesNotExit("could not find queue for direction");
    }
    const std::lock_guard<std::mutex> lock(_locks.at(q));
    if (isFull(q)) {
        throw new ExceedMaxMessage("queue has too many messages");
    }

    _queues.at(q).emplace(e);
    std::this_thread::sleep_for(_queueWaitTime);
}

Event* RrQueueManager::dequeue(RRP_QUEUES q) {
    if (_queues.find(q) == _queues.end()) {
        throw QueueDoesNotExit("could not find queue for direction");
    }
    const std::lock_guard<std::mutex> lock(_locks.at(q));
    if (isEmpty(q)) {
        throw new AttemptToDequeueToEmptyQueue("no messages available on queue");
    }
    Event* e = peek(q);
    _queues.at(q).pop();
    return e;
}

/*
 * Don't lock for a peak.
 */
Event* RrQueueManager::peek(RRP_QUEUES q) {
    if (_queues.find(q) == _queues.end()) {
        throw QueueDoesNotExit("could not find queue for direction");
    }
    return _queues.at(q).front();
}

bool RrQueueManager::isEmpty(RRP_QUEUES q) {
    if (_queues.find(q) == _queues.end()) {
        throw QueueDoesNotExit("could not find queue for direction");
    }
    return _queues.at(q).empty();
}

bool RrQueueManager::isFull(RRP_QUEUES q) {
    if (_queues.find(q) == _queues.end()) {
        throw QueueDoesNotExit("could not find queue for direction");
    }

    if (size(q) >= _queueLimit) {
        return true;
    }
    return false;
}

size_t RrQueueManager::size(RRP_QUEUES q) {
    if (_queues.find(q) == _queues.end()) {
        throw QueueDoesNotExit("could not find queue for direction");
    }
    return _queues.at(q).size();    
}