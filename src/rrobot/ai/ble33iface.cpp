#include <rrobot/ai/ble33iface.hpp>

using namespace rrobot;

void ble33iface::rotate(dlib::vector<float, VECTOR_DIM>) {}

bool ble33iface::detecto() { return false; }

void ble33iface::move_v(dlib::vector<float, VECTOR_DIM> v) {}

void ble33iface::serialize(const long tid, dlib::vector<float, VECTOR_DIM> v) {}

long ble33iface::gen_tid() { return 0; }

void ble33iface::stop() {}

msp_sonar_altitude ble33iface::sen_sonar() {
    msp_sonar_altitude s;
    return s;
}

/**
 * @brief returns availability, and acc vector.
 */
msp_sensor_acc ble33iface::sen_acc() {
    msp_sensor_acc s;
    return s;
}

/**
 * @brief Gyroscope sensor.
 */
msp_sensor_gyro ble33iface::sen_gyro() {
    msp_sensor_gyro s;
    return s;
}

/**
 * @brief Magnometer sensor.
 */
msp_sensor_mag ble33iface::sen_mag() {
    msp_sensor_mag s;
    return s;
}