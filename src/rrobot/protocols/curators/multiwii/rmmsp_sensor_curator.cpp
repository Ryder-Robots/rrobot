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
        // encode accelometer
        enc = Encoder::encodeUint32(payload.get_acc_avail());
        enc += Encoder::encodeFloat(payload.get_acc_x());
        enc += Encoder::encodeFloat(payload.get_acc_y());
        enc += Encoder::encodeFloat(payload.get_acc_z());
        //encode gyro
        enc += Encoder::encodeUint32(payload.get_gyro_avail());
        enc += Encoder::encodeFloat(payload.get_gyro_x());
        enc += Encoder::encodeFloat(payload.get_gyro_y());
        enc += Encoder::encodeFloat(payload.get_gyro_z());
        // encode mangnometer
        enc += Encoder::encodeUint32(payload.get_mag_avail());
        enc += Encoder::encodeFloat(payload.get_mag_x());
        enc += Encoder::encodeFloat(payload.get_mag_y());
        enc += Encoder::encodeFloat(payload.get_mag_z());
    }

    return enc;
}

Event* RmMspSensorCurator::deserializePayload(RmMultiWii& in) {
    Event* e = nullptr;

    if (in.getSize() > 0) {
        msp_sensor payload;

        payload.set_acc_avail(Encoder::decodeInt32(in.getPayload().substr(0, 4)));
        payload.set_acc_x(Encoder::decodeFloat(in.getPayload().substr(4, 4)));
        payload.set_acc_y(Encoder::decodeFloat(in.getPayload().substr(8, 4)));
        payload.set_acc_z(Encoder::decodeFloat(in.getPayload().substr(14,4)));

        payload.set_gyro_avail(Encoder::decodeInt32(in.getPayload().substr(18,4)));
        payload.set_gyro_x(Encoder::decodeFloat(in.getPayload().substr(22, 4)));
        payload.set_gyro_y(Encoder::decodeFloat(in.getPayload().substr(26,4)));
        payload.set_gyro_z(Encoder::decodeFloat(in.getPayload().substr(30,4)));

        payload.set_mag_avail(Encoder::decodeInt32(in.getPayload().substr(34, 4)));
        payload.set_mag_x(Encoder::decodeFloat(in.getPayload().substr(38,4)));
        payload.set_mag_y(Encoder::decodeFloat(in.getPayload().substr(44, 4)));
        payload.set_mag_z(Encoder::decodeFloat(in.getPayload().substr(48, 4)));

        void* p = static_cast<void*>(malloc(sizeof(payload)));
        memcpy(p, &payload, sizeof(payload));
        e = new Event(in.getCommand(), MSPDIRECTION::EXTERNAL_IN, p);
    } else 
        e = new Event(in.getCommand(), MSPDIRECTION::EXTERNAL_IN);
    return e;
}
