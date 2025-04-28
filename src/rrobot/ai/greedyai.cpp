#include <rrobot/ai/greedyai.hpp>

using namespace rrobot;

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

PSTATE GreedyAi::calcPath(msp_delta_xy d) {
    msp_delta_xy c = _smg.getCurrentDelta();
    float dc = DELTA_DISTANCE(c.get_x(), d.get_x(), c.get_y(), d.get_y());
    float xl = _smg.getOHeading() - 90, xr = _smg.getOHeading() + 90, yf = _smg.getOHeading(),
          yb = _smg.getOHeading() - 180;

    while (dc != 0) {
        // Look in the direction the robot is facing and see if there is any obstacles.

        dc = 0;
    }
    return PSTATE::P_AVAILABLE;
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

void GreedyAi::init() {
    // Set inital othogonal view.
    RmMultiWii m = requestFeature(MSPCOMMANDS::MSP_SENSOR);
    msp_sensor sensor = _curatorSensor.deserializem(m);

    msp_delta_xy cdelta, odelta;

    cdelta.set_x(0);
    cdelta.set_y(0);
    odelta.set_x(0);
    odelta.set_y(0);

    _smg.setHeadingFromRadians2(sensor.get_mag_x(), sensor.get_mag_y());
    _smg.setOrigHeadingFromRadians2(sensor.get_mag_x(), sensor.get_mag_y());
}

void GreedyAi::teardown() {
    _excluded.clear();
    _explored.clear();
}