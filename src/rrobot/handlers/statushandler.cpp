#include <rrobot/handlers/statushandler.hpp>

using namespace rrobot;

Event* RrStatusHandler::produce(StateManager& sm) {
    // Reset status before processing. In case one of the handlers has malfunctioned for whatever reason.
    msp_status* mspStatus = new msp_status();
    mspStatus->set_cycletime(sm.getCycleTime());
    mspStatus->set_i2c_errors_count(sm.getErrorCount());
    mspStatus->set_sensor(sm.getSensors());
    mspStatus->set_flag(sm.getFlags());

    // The setting involves ACC_1G which is a current setting,  however this is not
    // on the drone hardware at the moment, so at the moment this is just set to '0'
    mspStatus->set_current_set(0);
    void* payload = static_cast<void*>(mspStatus);

    Event* event = new Event(MSPCOMMANDS::MSP_STATUS, MSPDIRECTION::EXTERNAL_OUT, payload);
    _available = true;
    return event;
}

bool RrStatusHandler::consume(StateManager& smg, Event* event) {
    _available = true;
    return false;
}

bool RrStatusHandler::available() { return _available; }