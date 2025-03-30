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
    for (const auto& [key, value] : _queues) {
        if (!value.empty()) {
            while(!value.empty()) {
                Event* e = value.front();
                delete(e);
            }
        }
    }
}

void RrQueueManager::addQueue(RRP_QUEUES q) {
    const std::lock_guard<std::mutex> lock(_mtx);
    queue<Event*> qu;
    _queues.emplace(q, qu);
    auto& mtx = _locks[q];
}

/**
 * CAVEAT: without using mutex references, there are two different locks on the queue, this could lead to some undefined behanviour,
 * best to avoid mapQueue() unless absolutely necessary.
 */
void RrQueueManager::mapQueue(RRP_QUEUES q1, RRP_QUEUES q2) {
    const std::lock_guard<std::mutex> lock(_mtx);
    if (_queues.find(q1) == _queues.end()) {
        throw QueueDoesNotExit("could not find queue for direction");
    }
    _queues.emplace(q2, _queues.at(q1));
    auto& mtx = _locks.at(q2);
}

chrono::milliseconds RrQueueManager::queueProcessTime(RRP_QUEUES q) {
    return _queueProcessTime;
}

void  RrQueueManager::enqueue(RRP_QUEUES q, Event* e) {
    if (_queues.find(q) == _queues.end()) {
        throw QueueDoesNotExit("could not find queue for direction");
    }
    const std::lock_guard<std::mutex> lock(_locks.at(q));
    std::this_thread::sleep_for(_queueWaitTime);

    if (isFull(q)) {
        throw new ExceedMaxMessage("queue has too many messages");
    }

    _queues.at(q).emplace(e);
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