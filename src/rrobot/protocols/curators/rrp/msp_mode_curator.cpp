#include <rrobot/protocols/curators/rrp/msp_mode_curator.hpp>

using namespace rrobot;

Event* msp_mode_curator::deserialize(json in) {
    msp_mode* payload = new msp_mode();
    Event* event = new Event(MSP_MODE, _direction, payload);
    validate("mode", in);
    VALID_CMODES_INIT;

    if (VALID_CMODES_KEYS.find(in["mode"]) == VALID_CMODES_KEYS.end()) {
        throw UnsupportedAttribute(in["mode"].get<string>() + " is not an CMODE");
    }
    payload->set_mode(VALID_CMODES_KEYS.at(in["mode"]));
    return event;
}

// direction is always the default
Event* msp_mode_curator::deserialize(json in, MSPDIRECTION direction) {
    return  deserialize(in);
}

json msp_mode_curator::serialize(Event *in) {
    string command = getCommandString();
    msp_mode payload = in->getPayload<msp_mode>();
    string cmode = "CMODE_NOT_SET";
    VALID_CMODES_INIT;
    for (const auto& pair : VALID_CMODES_KEYS) {
        if (pair.second ==  payload.get_mode()) {
            cmode = pair.first;
            break;
        }
    }
    json j = {
        {"command", command},
        {"payload", {
            {"mode", cmode}
        }}
    };
    return j;
}