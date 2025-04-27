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

float  GreedyAi::computePenalty(float heading) {
    return 0;
}

PSTATE GreedyAi::calcPath(msp_delta_xy d) {
    msp_delta_xy c = _smg.getCurrentDelta();
    float dc = DELTA_DISTANCE(c.get_x(), d.get_x(), c.get_y(), d.get_y());
    
    while (dc != 0) {
        float offset = _smg.getHeading() - _smg.getOHeading();
        // ^: y up, 90 x: left ->, 180 v: y down, 270 x left <-

        // add penalties.
        dc = 0;

    }
    return PSTATE::P_AVAILABLE;

}

void GreedyAi::init() {

}

void GreedyAi::teardown() {
    _excluded.clear();
    _explored.clear();
}