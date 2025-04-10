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
    string enc;
    if (in->hasPayload()) {
        msp_sensor payload = in->getPayload<msp_sensor>();
        // encode accelometer
        enc = encodeUint32(payload.get_acc_avail(), enc);
        enc = encodeFloat(payload.get_acc_x(), enc);
        enc = encodeFloat(payload.get_acc_y(), enc);
        enc = encodeFloat(payload.get_acc_z(), enc);
        //encode gyro
        enc = encodeUint32(payload.get_gyro_avail(), enc);
        enc = encodeFloat(payload.get_gyro_x(), enc);
        enc = encodeFloat(payload.get_gyro_y(), enc);
        enc = encodeFloat(payload.get_gyro_z(), enc);
        // encode mangnometer
        enc = encodeUint32(payload.get_mag_avail(), enc);
        enc = encodeFloat(payload.get_mag_x(), enc);
        enc = encodeFloat(payload.get_mag_y(), enc);
        enc = encodeFloat(payload.get_mag_z(), enc);
    }

    return enc;
}

Event* RmMspSensorCurator::deserializePayload(RmMultiWii& in) {
    Event* e = nullptr;

    if (in.getSize() > 0) {
        msp_sensor payload;
        payload.set_acc_avail(decodeUint32(in.getPayload(), 0));
        payload.set_acc_x(decodeFloat(in.getPayload(), 1 * 4));
        payload.set_acc_y(decodeFloat(in.getPayload(), 2 * 4));
        payload.set_acc_z(decodeFloat(in.getPayload(), 3 * 4));
    } else 
        e = new Event(in.getCommand(), MSPDIRECTION::EXTERNAL_IN);
    return e;
}
