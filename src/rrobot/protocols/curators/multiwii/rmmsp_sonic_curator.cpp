#include <rrobot/protocols/curators/multiwii/rmmsp_sonic_curator.hpp>

using namespace rrobot;

std::string RmMspSonicCurator::serializePayload(Event* in) {
    string enc = "";
    if (in->hasPayload()) {
        msp_sonar_altitude payload = in->getPayload<msp_sonar_altitude>();
        enc = serializem(payload);
    }
    return enc;
}

Event* RmMspSonicCurator::deserializePayload(RmMultiWii& in) {
    Event* e = nullptr;

    if (in.getSize() > 0) {
        msp_sonar_altitude payload = deserializem(in);

        void* p = static_cast<void*>(malloc(sizeof(payload)));
        memcpy(p, &payload, sizeof(payload));
        e = new Event(in.getCommand(), MSPDIRECTION::EXTERNAL_IN, p);
    } else
        e = new Event(in.getCommand(), MSPDIRECTION::EXTERNAL_IN);
    return e;
}

msp_sonar_altitude RmMspSonicCurator::deserializem(RmMultiWii& in) {
    msp_sonar_altitude payload;
    vector<std::string> tokens = decodeTokens(in.getPayload());

    if (in.getSize() > 0) {
        if (tokens.size() != 2) {
            throw InvalidFormat("sonic had incorrect number of returned parameters");
        }

        payload.set_distance(Encoder::decodeInt32(tokens.at(0)));
        payload.set_temperature(Encoder::decodeInt32(tokens.at(1)));
    }
    return payload;
}

std::string RmMspSonicCurator::serializem(msp_sonar_altitude payload) {
    std::vector<std::string> tokens = {
        Encoder::encodeUint32(payload.get_distance()),
        Encoder::encodeUint32(payload.get_temperature()),
    };
    return encodeTokens(tokens);
}
