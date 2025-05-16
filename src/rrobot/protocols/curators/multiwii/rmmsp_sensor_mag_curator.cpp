#include <rrobot/protocols/curators/multiwii/rmmsp_sensor_mag_curator.hpp>
/*
 *  encoding order is as follows:
 *
 *  0 available
 *  1 x
 *  2 y
 *  3 z
 */

using namespace rrobot;

std::string RmMspSensorMagCurator::serializePayload(Event* in) {
    return "";
}

Event* RmMspSensorMagCurator::deserializePayload(RmMultiWii& in) {
    return nullptr;
}

msp_sensor_mag RmMspSensorMagCurator::deserializem(RmMultiWii& in) {
    msp_sensor_mag payload;
    std::vector<std::string> tokens = decodeTokens(in.getPayload());

    if (in.getSize() > 0) {
        payload.set_available(Encoder::decodeInt32(tokens.at(0)));
        dlib::vector<float, 3> data(Encoder::decodeFloat(tokens.at(1)), Encoder::decodeFloat(tokens.at(2)),
                                    Encoder::decodeFloat(tokens.at(3)));
        payload.set_data(data);
    }
    return payload;
}

std::string RmMspSensorMagCurator::serializem(const msp_sensor_mag in) {return "";}