#include <rrobot/ai/greedyai.hpp>

using namespace rrobot;

dlib::logger dlog_ai("rr_robot_ai");

PSTATE GreedyAi::calcPath(msp_delta_xy d) {
    msp_delta_xy c = _smg.getCurrentDelta();
    float dc = DELTA_DISTANCE(c.get_x(), d.get_x(), c.get_y(), d.get_y());
    float xl = _smg.getOHeading() - 90, xr = _smg.getOHeading() + 90, yf = _smg.getOHeading(),
          yb = _smg.getOHeading() - 180;

    while (dc != 0) {
        // Look in the direction the robot is facing and see if there is any obstacles.
        msp_sonar_altitude sonar = requestSonar();
        if (OBJ_AVOID_DIST - sonar.get_distance() <= OBJ_AVOID_DIST) {
            // must turn to avoid collision.
        }
        // if moving forward is an option, then move forward, before doing anything else.
        else if (DELTA_DISTANCE(c.get_x(), d.get_x(), c.get_y() + 1, d.get_y()) <= dc) {
            // move forward
            moveForward();

            dc = DELTA_DISTANCE(c.get_x(), d.get_x(), c.get_y() + 1, d.get_y());
            continue;
        }

        dc = 0;
    }
    return PSTATE::P_AVAILABLE;
}

void GreedyAi::init(Environment env) {
    dlog_ai.set_level(env.getLogging().getLogLevel());
    dlog_ai << dlib::LINFO << "configurating greedyAi algorithm";

    // Set inital othogonal view.
    msp_sensor sensor = requestSensor();
    msp_delta_xy cdelta, odelta;

    cdelta.set_x(0);
    cdelta.set_y(0);
    odelta.set_x(0);
    odelta.set_y(0);

    _smg.setHeadingFromRadians2(sensor.get_mag_x(), sensor.get_mag_y());
    _smg.setOrigHeadingFromRadians2(sensor.get_mag_x(), sensor.get_mag_y());
}

void GreedyAi::teardown() {
    dlog_ai << dlib::LINFO << "tearing down algorithm";
    _excluded.clear();
    _explored.clear();
}

// potentially all methods below will be moved to a common class.
void GreedyAi::moveForward() {
    msp_motor motor;
    motor.set_roll(0);
    motor.set_pitch(1);
    motor.set_yaw(0);
    motor.set_throttle(SPEED_CRUISE);
    motor.set_aux1(0);
    motor.set_aux2(0);
    motor.set_aux3(0);
    motor.set_aux4(0);
}

float GreedyAi::absDistance(float d, float c) {
    if (d < 0 && c > 0) {
        return abs(d) + c;
    } else if (d > 0 && c < 0) {
        return d + abs(c);
    }

    return abs(d - c);
}

float GreedyAi::sqr(float n) { return n * n; }

bool GreedyAi::isExp(msp_delta_xy x, vector<msp_delta_xy> exp) {
    for (auto ex : exp) {
        if (ex.get_x() == x.get_x() && ex.get_y() == x.get_y()) {
            return true;
        }
    }
    return false;
}

bool GreedyAi::isExplored(msp_delta_xy x) { return isExp(x, _explored); }

bool GreedyAi::isExcluded(msp_delta_xy x) { return isExp(x, _excluded); }

bool GreedyAi::isValid(float x, float y) {
    msp_delta_xy nc;
    nc.set_x(x);
    nc.set_y(y);
    if (isExcluded(nc) || isExplored(nc)) {
        return false;
    }
    return true;
}

float GreedyAi::computePenalty(float heading) {
    float x = (abs(heading) + abs(_smg.getHeading())) - abs(_smg.getHeading());
    x = x / 180;
    return x;
}

msp_sonar_altitude GreedyAi::requestSonar() {
    dlog_ai << dlib::LINFO << "updating sonar and barometer from micro-processor";
    RmMultiWii m = requestFeature(MSPCOMMANDS::MSP_SONAR_ALTITUDE);
    msp_sonar_altitude sonar = _curatorSonar.deserializem(m);
    AiFeatures features;
    features.setSonar(sonar);
    _smg.setFeatures(features);
    return sonar;
}

msp_sensor GreedyAi::requestSensor() {
    dlog_ai << dlib::LINFO << "updating sensors (gyro, mag, and accelometer) from micro-processor";
    RmMultiWii m = requestFeature(MSPCOMMANDS::MSP_SENSOR);
    msp_sensor sensors = _curatorSensor.deserializem(m);
    AiFeatures features;
    features.setSensors(sensors);
    _smg.setFeatures(features);
    return sensors;
}

RmMultiWii GreedyAi::requestFeature(MSPCOMMANDS cmd) {
    RmMultiWii m = RmMultiWii::createInstance("", cmd);
    _ext.send_rr(m.encode(_crc).c_str(), m.getSize());

    while (!_ext.available()) {
    }
    uint8_t buf = '\0';
    std::string data = "";

    while (buf != RmMultiWii::_TERMINATION_CHAR) {
        _ext.recv_rr(&buf, 1);
        data += buf;
    }
    m = RmMultiWii::createInstance(data, Crc32());
    return m;
}

void GreedyAi::sendCommand(RmMultiWii m) {
    if (_ext.send_rr(m.encode(_crc)) == -1) {
        //TODO: throw exception here.
    }
}