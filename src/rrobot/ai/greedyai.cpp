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

PSTATE GreedyAi::calcPath(msp_delta_xy d) {
    float dc = floor(sqrt(
        sqr(absDistance(d.get_x(),  _smg.getCurrentDelta().get_x())) + sqr(absDistance(d.get_y(),  _smg.getCurrentDelta().get_y()))
    ));
    
    while (dc != 0) {
        float heading = _smg.getHeading();

        // calulate possible deltas first.
        float paths[4];
        int i = 0;

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