#include <rrobot/handlers/identhandler.hpp>

using namespace rrobot;

IdentHandler::IdentHandler(Environment env) {
    _version = env.getVersion().getVersion();
    _multitype = env.getHwModel().getMultiType();
    _msp_version = env.getHwModel().getMspVersion();
    _capability = env.getHwModel().getCapability();
}

bool IdentHandler::consume(StateManager& smg, Event* event) {
    _available = true;
}

Event* IdentHandler::produce(StateManager& sm) {
    msp_ident* payload = new msp_ident();

    payload->set_version(_version);

    Event* event = new Event(MSPCOMMANDS::MSP_IDENT, MSPDIRECTION::EXTERNAL_OUT);
    return event;
}