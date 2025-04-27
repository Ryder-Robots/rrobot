#include <rrobot/protocols/curators/multiwii/rmmsp_sensor_curator.hpp>

using namespace rrobot;

/*
 * Enocding format is as follows:
 *  _acc_avail uint32_t
 *  _acc_x float
 *  _acc_y float
 *  _acc_z float
 *  _gyro_avail uint32_t
 *  _gyro_x float
 *  _gyro_y float
 *  _gyro_z float
 *  _mag_avail uint32_t
 *  _mag_x float
 *  _mag_y float
 *  _mag_z float
 */

/*
    data += (_sz >> 24) & 0xFF;
   data += (_sz >> 16) & 0xFF;
   data += (_sz >> 8) & 0xFF;
   data += _sz & 0xFF;
 */
std::string RmMspSensorCurator::serializePayload(Event* in) {
    string enc = "";
    if (in->hasPayload()) {
        msp_sensor payload = in->getPayload<msp_sensor>();
        enc = serializem(payload);
    }
    return enc;
}

Event* RmMspSensorCurator::deserializePayload(RmMultiWii& in) {
    Event* e = nullptr;

    if (in.getSize() > 0) {
        msp_sensor payload = deserializem(in);

        void* p = static_cast<void*>(malloc(sizeof(payload)));
        memcpy(p, &payload, sizeof(payload));
        e = new Event(in.getCommand(), MSPDIRECTION::EXTERNAL_IN, p);
    } else
        e = new Event(in.getCommand(), MSPDIRECTION::EXTERNAL_IN);
    return e;
}

msp_sensor RmMspSensorCurator::deserializem(RmMultiWii& in) {
    msp_sensor payload;

    if (in.getSize() > 0) {
        vector<std::string> tokens = decodeTokens(in.getPayload());
        payload.set_acc_avail(Encoder::decodeInt32(tokens.at(0)));
        payload.set_acc_x(Encoder::decodeFloat(tokens.at(1)));
        payload.set_acc_y(Encoder::decodeFloat(tokens.at(2)));
        payload.set_acc_z(Encoder::decodeFloat(tokens.at(3)));

        payload.set_gyro_avail(Encoder::decodeInt32(tokens.at(4)));
        payload.set_gyro_x(Encoder::decodeFloat(tokens.at(5)));
        payload.set_gyro_y(Encoder::decodeFloat(tokens.at(6)));
        payload.set_gyro_z(Encoder::decodeFloat(tokens.at(7)));

        payload.set_mag_avail(Encoder::decodeInt32(tokens.at(8)));
        payload.set_mag_x(Encoder::decodeFloat(tokens.at(9)));
        payload.set_mag_y(Encoder::decodeFloat(tokens.at(10)));
        payload.set_mag_z(Encoder::decodeFloat(tokens.at(11)));
    }

    return payload;
}

std::string RmMspSensorCurator::serializem(msp_sensor payload) {

    // encode accelometer
    std::vector<std::string> tokens = {
        Encoder::encodeUint32(payload.get_acc_avail()),
        Encoder::encodeFloat(payload.get_acc_x()),
        Encoder::encodeFloat(payload.get_acc_y()),
        Encoder::encodeFloat(payload.get_acc_z()),

        Encoder::encodeUint32(payload.get_gyro_avail()),
        Encoder::encodeFloat(payload.get_gyro_x()),
        Encoder::encodeFloat(payload.get_gyro_y()),
        Encoder::encodeFloat(payload.get_gyro_z()),

        Encoder::encodeUint32(payload.get_mag_avail()),
        Encoder::encodeFloat(payload.get_mag_x()),
        Encoder::encodeFloat(payload.get_mag_y()),
        Encoder::encodeFloat(payload.get_mag_z())
    };

    return encodeTokens(tokens);
}
