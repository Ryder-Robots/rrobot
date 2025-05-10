#include <rrobot/ai/greedyai.hpp>

using namespace rrobot;

dlib::logger dlog_ai("rr_robot_ai");

PSTATE GreedyAi::calcPath(msp_delta_xy d) {
    dlog_ai << dlib::LINFO << "beginning path transveral";

    msp_delta_xy c = _smg.getCurrentDelta();
    msp_sonar_altitude sonar = requestSonar();
    msp_sensor sensors = requestSensor();
    float dc = DELTA_DISTANCE(c.get_x(), c.get_y(), d.get_x(), d.get_y());

    // list of possible rotations
    const float dgs[] = {0, 90, 180, -90};

    while (!(_smg.getCurrentDelta().get_x() == d.get_x() && _smg.getCurrentDelta().get_y() == d.get_y())) {
        c = _smg.getCurrentDelta();
        float x = c.get_x(), y = c.get_y();

        // if there are any obstacles exclude the next path
        sonar = requestSonar();
        if (OBJ_AVOID_DIST - sonar.get_distance() <= OBJ_AVOID_DIST) {
            dlog_ai << dlib::LINFO << "detected object, excluding possible path";
            msp_delta_xy ex;
            offset(_smg.getHeading(), &x, &y);
            ex.set_heading(c.get_heading());
            ex.set_x(x);
            ex.set_y(y);
            _excluded.push_back(ex);
        }

        sensors = requestSensor();
        // detect the best path.
        float f = -1, pd = -1;
        for (auto dg : dgs) {
            float mx = sensors.get_mag_x(), my = sensors.get_mag_y();
            _smg.rotate(dg, &mx, &my);
            c = _smg.setHeadingFromRadian2(_smg.getCurrentDelta(), mx, my);
            x = c.get_x(), y = c.get_y();
            offset(c.get_heading(), &x, &y);
            if (isValid(x, y)) {
                if (f == -1 || DELTA_DISTANCE(x, y, d.get_x(), d.get_y()) + computePenalty(c.get_heading()) < pd) {
                    pd = DELTA_DISTANCE(x, y, d.get_x(), d.get_y());
                    f = dg;
                }
            }
        }
        c = _smg.getCurrentDelta();
        x = c.get_x(), y = c.get_y();
        if (f == 0) {
            // move forward, and reset delta
            offset(c.get_heading(), &x, &y);
        } else {
            float mx = sensors.get_mag_x(), my = sensors.get_mag_y();
            if (f == -1) {
                if (_transvered.empty()) {
                    dlog_ai << dlib::LWARN << "no paths left to transverse";
                    // stop here.
                    sendCommand(RmMultiWii::createInstance("", MSPCOMMANDS::MSP_STOP));
                    return PSTATE::P_NOT_AVAIL;
                }
                msp_delta_xy l = _transvered.top();
                _transvered.pop();
                _smg.rotate(c.get_heading() - l.get_heading(), &mx, &my);
            } else {
                _smg.rotate(f, &mx, &my);
            }

            msp_orientation m;
            m.set_x(mx);
            m.set_y(my);
            sendCommand(RmMultiWii::createInstance(_curatorOrientation.serializem(m), MSPCOMMANDS::MSP_ORIENTATION));
            sensors = requestSensor();
            _smg.setHeadingFromRadian2(c, mx, my);
            offset(c.get_heading(), &x, &y);
        }
        moveForward();
        c.set_x(x);
        c.set_y(y);
        _smg.setCurrentDelta(c);
        _explored.push_back(c);
        _transvered.push(c);
        dc = DELTA_DISTANCE(c.get_x(), c.get_y(), d.get_x(), d.get_y());
    }

    // stop here, reached the destination
    sendCommand(RmMultiWii::createInstance("", MSPCOMMANDS::MSP_STOP));

    // update origin delta here.
    return PSTATE::P_AVAILABLE;
}

void GreedyAi::init(Environment env) {
    dlog_ai.set_level(env.getLogging().getLogLevel());
    dlog_ai << dlib::LINFO << "configurating greedyAi algorithm";

    // Set inital othogonal view. Sonar is also requested, but this is just used to update
    // the features list, so that UI gets a view of if anything is in the path of the drone.
    requestSonar();
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
    RmMultiWii m = RmMultiWii::createInstance("", MSPCOMMANDS::MSP_MOVE_FORWARD);
    sendCommand(m);
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
        // TODO: throw exception here.
    }
}

void GreedyAi::offset(const float h, float *x, float *y) {
    float c = _smg.getOHeading(), xi = _smg.getCurrentDelta().get_x(), yi = _smg.getCurrentDelta().get_y();

    if (h == 0) {
        yi++;
    } else if (h > 0 && h < 180) {
        xi++;
    } else if (h < 0 && h < -180) {
        xi--;
    } else {
        yi--;
    }

    *x = xi;
    *y = yi;
}

//  may be too convoluted
void GreedyAi::offset(const float degrees, float *x, float *y, const float gx, const float gy) {
    float gix = gx, giy = gy;
    _smg.rotate(degrees, &gix, &giy);
    msp_delta_xy delta;
    delta.set_heading(0);
    delta.set_x(*x);
    delta.set_y(*y);
    delta = _smg.setHeadingFromRadian2(delta, gix, giy);
    offset(delta.get_heading(), x, y);
}