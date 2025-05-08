#include <rrobot/protocols/curators/multiwii/rmmsp_orientation_curator.hpp>

using namespace rrobot;

std::string RmMspOrientationCurator::serializePayload(Event* in) {
    string enc = "";
    if (in->hasPayload()) {
        msp_orientation payload = in->getPayload<msp_orientation>();
        enc = serializem(payload);
    }
    return enc;
}

Event* RmMspOrientationCurator::deserializePayload(RmMultiWii& in) {
    Event* e = nullptr;

    if (in.getSize() > 0) {
        msp_orientation payload = deserializem(in);

        void* p = static_cast<void*>(malloc(sizeof(payload)));
        memcpy(p, &payload, sizeof(payload));
        e = new Event(in.getCommand(), MSPDIRECTION::EXTERNAL_IN, p);
    } else
        e = new Event(in.getCommand(), MSPDIRECTION::EXTERNAL_IN);
    return e;
}

msp_orientation RmMspOrientationCurator::deserializem(RmMultiWii& in) {
    msp_orientation payload;

    if (in.getSize() > 0) {
        vector<std::string> tokens = decodeTokens(in.getPayload());

        if (tokens.size() != 2) {
            throw InvalidFormat("sensor had incorrect number of returned parameters");
        }

        payload.set_x(Encoder::decodeFloat(tokens.at(0)));
        payload.set_y(Encoder::decodeFloat(tokens.at(1)));
    }

    return payload;
}

std::string RmMspOrientationCurator::serializem(msp_orientation payload) {

    // encode accelometer
    std::vector<std::string> tokens = {
        Encoder::encodeFloat(payload.get_x()),
        Encoder::encodeFloat(payload.get_y()),
    };

    return encodeTokens(tokens);
}