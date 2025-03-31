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
    int32_t statuses = 0;
    int32_t flags_modes = MODE_BITMASK(_state.getFlags());
    RRP_STATUS status = RRP_STATUS::INITILIZING;
    std::vector<RRP_STATUS> statusArray = STATUS_ARRAY_INIT;

    // This is the a precise order on what global status will be.
    for (auto s : statusArray) {
        if ((statuses & s) == s) {
            status = s;
            break;
        }
    }
    return status;
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