#include <rrobot/ai/ble33iface.hpp>

using namespace rrobot;

dlib::logger dlog_if("rr_miface");

void ble33iface::rotate(dlib::vector<float, VECTOR_DIM>) {}

bool ble33iface::detecto() { return false; }

void ble33iface::move_v(dlib::vector<float, VECTOR_DIM> v) {}

void ble33iface::serialize(const long tid, dlib::vector<float, VECTOR_DIM> v) {}

long ble33iface::gen_tid() { return 0; }

void ble33iface::stop() {}

/*
 * internal get that retrieves sensors.
 */
RmMultiWii ble33iface::get_m(MSPCOMMANDS c) {
    const std::lock_guard<std::mutex> lock(_lock);
    if (_ext.available() && _ext.send_rr(RmMultiWii::createInstance("", c).encode(_crc)) > 0) {
        std::string data = _ext.get(RmMultiWii::_TERMINATION_CHAR, LONG_MAX);
        RmMultiWii m = RmMultiWii::createInstance(data, _crc);
        return m;
    }
    dlog_if << dlib::LWARN << "sensor " << c << " unavailable!";
    return RmMultiWii::createInstance("", MSPCOMMANDS::MSP_NONE);
}

msp_sonar_altitude ble33iface::sen_sonar() {
    RmMultiWii m = get_m(MSPCOMMANDS::MSP_SONAR_ALTITUDE);
    if (m.getCommand() == MSPCOMMANDS::MSP_NONE) {
        return _sm.getFeatures().getSonar();
    }
    msp_sonar_altitude s = curator_sonic.deserializem(m);
    AiFeatures features = _sm.getFeatures();
    features.setSonar(s);
    _sm.setFeatures(features);
    return s;
}

/**
 * @brief returns availability, and acc vector.
 */
msp_sensor_acc ble33iface::sen_acc() {
    RmMultiWii m = get_m(MSPCOMMANDS::MSP_SENSOR_ACC);
    if (m.getCommand() == MSPCOMMANDS::MSP_NONE) {
        return _sm.getFeatures().get_sensor_acc();
    }
    msp_sensor_acc s = curator_acc.deserializem(m);
    AiFeatures features = _sm.getFeatures();
    features.set_sensor_acc(s);
    _sm.setFeatures(features);
    return s;
}

/**
 * @brief Gyroscope sensor.
 */
msp_sensor_gyro ble33iface::sen_gyro() {
    RmMultiWii m = get_m(MSPCOMMANDS::MSP_SENSOR_GYRO);
    if (m.getCommand() == MSPCOMMANDS::MSP_NONE) {
        return _sm.getFeatures().get_sensor_gyro();
    }
    msp_sensor_gyro s = curator_gyro.deserializem(m);
    AiFeatures features = _sm.getFeatures();
    features.set_sensor_gyro(s);
    _sm.setFeatures(features);
    return s;
}

/**
 * @brief Magnometer sensor.
 */
msp_sensor_mag ble33iface::sen_mag() {
    RmMultiWii m = get_m(MSPCOMMANDS::MSP_SENSOR_MAG);
    if (m.getCommand() == MSPCOMMANDS::MSP_NONE) {
        return _sm.getFeatures().get_sensor_mag();
    }
    msp_sensor_mag s = curator_mag.deserializem(m);
    AiFeatures features = _sm.getFeatures();
    features.set_sensor_mag(s);
    _sm.setFeatures(features);
    return s;
}