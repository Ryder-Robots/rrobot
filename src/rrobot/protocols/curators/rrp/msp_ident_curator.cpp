#include <rrobot/protocols/curators/rrp/msp_ident_curator.hpp>

using namespace rrobot;

/**
 * @fn deserialize
 * @brief
 * deserialize inbound json.
 */
Event* msp_ident_curator::deserialize(json in) {
    msp_ident* payload = new msp_ident();
    Event* event = new Event(MSP_IDENT, _direction, payload);
    validate("version", in);
    payload->set_version(in["version"]);

    // MULTITYPE_T
    validate("multitype", in);
    VALID_MULTITYPE_KEYS_INIT;
    if (VALID_MULTITYPE_KEYS.find(in["multitype"]) == VALID_MULTITYPE_KEYS.end()) {
        throw UnsupportedAttribute(in["multitype"].get<string>() + " is not an MULTITYPE");
    }
    payload->set_multitype(VALID_MULTITYPE_KEYS.at(in["multitype"]));

    // MSP_VERSION
    validate("msp_version", in);
    VALID_MSP_VERSION_KEYS_INIT;
    if (VALID_MSP_VERSION_KEYS.find(in["msp_version"]) == VALID_MSP_VERSION_KEYS.end()) {
        throw UnsupportedAttribute(in["msp_version"].get<string>() + " is not an MSP_VERSION");
    }
    payload->set_msp_version(VALID_MSP_VERSION_KEYS.at(in["msp_version"]));

    validate("capability", in);
    payload->set_capability(in["capability"]);

    return event;
}

// direction is always the default
Event* msp_ident_curator::deserialize(json in, MSPDIRECTION direction) { return deserialize(in); }

/**
 * @fn serialize
 * @brief
 * payload is defined as:
 *   command: {}
 *   payload: {}
 */
json msp_ident_curator::serialize(Event* in) {
    string command = getCommandString();
    msp_ident payload = in->getPayload<msp_ident>();

    string multiType = "NOTDEFINED";
    VALID_MULTITYPE_KEYS_INIT;
    for (const auto& pair : VALID_MULTITYPE_KEYS) {
        if (pair.second == payload.get_multitype()) {
            multiType = pair.first;
            break;
        }
    }

    string mspVersion = "VIRTUAL";
    VALID_MSP_VERSION_KEYS_INIT;
    for (const auto& pair : VALID_MSP_VERSION_KEYS) {
        if (pair.second == payload.get_msp_version()) {
            mspVersion = pair.first;
            break;
        }
    }

    json j = {{"command", command},
              {"payload",
               {{"multitype", multiType},
                {"msp_version", mspVersion},
                {"version", payload.get_version()},
                {"capability", payload.get_capability()}}}};

    return j;
}