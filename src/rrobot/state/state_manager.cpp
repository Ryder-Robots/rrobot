#include <rrobot/state/state_manager.hpp>

using namespace rrobot;

int32_t StateManager::getFlags() {
    const std::lock_guard<std::mutex> lock(_lock);
    return _state.getFlags();
}

RR_CMODES StateManager::getMode() {
    const std::lock_guard<std::mutex> lock(_lock);
    int32_t flags = _state.getFlags();
    flags -= STATUS_BITMASK(flags);
    return static_cast<RR_CMODES>(flags);
}

RRP_STATUS StateManager::getStatus() {
    const std::lock_guard<std::mutex> lock(_lock);
    int32_t flags_modes = MODE_BITMASK(_state.getFlags());
    RRP_STATUS status = RRP_STATUS::INITILIZING;
    std::vector<RRP_STATUS> statusArray = STATUS_ARRAY_INIT;

    // This is the a precise order on what global status will be.
    for (auto s : statusArray) {
        if ((_state.getFlags() & s) == s) {
            status = s;
            break;
        }
    }
    return status;
}

void StateManager::resetStatus() {
    _state.setFlags(getMode());
}

void StateManager::setMode(RR_CMODES mode) {
    const std::lock_guard<std::mutex> lock(_lock);
    int32_t flags = _state.getFlags();
    int32_t statuses = STATUS_BITMASK(flags);    
    _state.setFlags(0 | statuses | mode);
}

void StateManager::setStatus(int32_t status) {
    const std::lock_guard<std::mutex> lock(_lock);
    _state.setFlags(0 | _state.getFlags() | status);
}

bool StateManager::isRunning() {
    const std::lock_guard<std::mutex> lock(_lock);
    return _state.isRunning();
}

void StateManager::setIsRunning(bool r) {
    const std::lock_guard<std::mutex> lock(_lock);
    return _state.setIsRunning(r);
}

void StateManager::setCycleTime(uint16_t v) {
    const std::lock_guard<std::mutex> lock(_lock);
    _state.setCycleTime(v);
}

uint16_t StateManager::getCycleTime() {
    const std::lock_guard<std::mutex> lock(_lock);
    return _state.getCycleTime();
}

void StateManager::setErrorCount(uint16_t v) {
    const std::lock_guard<std::mutex> lock(_lock);
    _state.setErrorCount(v);
}

uint16_t StateManager::getErrorCount() {
    const std::lock_guard<std::mutex> lock(_lock);
    return _state.getErrorCount();
}

void  StateManager::setSensors(uint16_t v) {
    const std::lock_guard<std::mutex> lock(_lock);
    _state.setSensors(v);
}

uint16_t StateManager::getSensors() {
    const std::lock_guard<std::mutex> lock(_lock);
    return _state.getSensors();
}

void StateManager::setFeatures(AiFeatures features) {
    const std::lock_guard<std::mutex> lock(_lock);
    _state.setFeatures(features);
}

AiFeatures StateManager::getFeatures() {
    const std::lock_guard<std::mutex> lock(_lock);
    return _state.getFeatures();
}
