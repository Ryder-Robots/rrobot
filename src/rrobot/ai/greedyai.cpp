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

float GreedyAi::sqr(float n) {
    return n * n;
}

bool GreedyAi::isExp(msp_delta_xy x, vector<Event*> exp) {
    for (auto ex : exp) {
        msp_delta_xy exp = ex->getPayload<msp_delta_xy>();
        if (exp.get_x() == x.get_x() && exp.get_y() == x.get_y()) {
            return true;
        }
    }
    return false;
}

bool GreedyAi::isExplored(msp_delta_xy x) {
    return isExp(x, _explored);
}

bool GreedyAi::isExcluded(msp_delta_xy x) {
    return isExp(x, _excluded);
}

bool GreedyAi::isValid(float x, float y, msp_delta_xy& nc, msp_delta_xy d, float dc) {
    nc.set_x(x);
    nc.set_y(y);
    if (!isExcluded(nc) && !isExplored(nc) && 
        floor(sqrt(sqr(absDistance(d.get_x(), nc.get_x())) + sqr(absDistance(d.get_y(), nc.get_y())))) < dc
    ) {
        return true;
    }
    return false;
}

PSTATE GreedyAi::traveresePath(queue<Event*>& q,  msp_delta_xy c,  msp_delta_xy d) {
    float dc = floor(sqrt(
        sqr(absDistance(d.get_x(), c.get_x())) + sqr(absDistance(d.get_y(), c.get_y()))
    ));

    if (dc == 0) {
        return PSTATE::P_AVAILABLE;
    }

    msp_delta_xy nc;
    float x = 0, y = 0;

    // try add 1 to x first
    if (isValid(c.get_x()+ 1, c.get_y(), nc, d, dc)) {
        x = c.get_x()+ 1;
        y = c.get_y();
    }
    // try adding one to y
    else if (isValid(c.get_x(), c.get_y() + 1, nc, d, dc)) {
        x = c.get_x();
        y = c.get_y() + 1;
    }

    // try reducing one from x 
    if (isValid(c.get_x()- 1, c.get_y(), nc, d, dc)) {
        x = c.get_x()- 1;
        y = c.get_y();
    }

    // try removing one from y
    else if (isValid(c.get_x(), c.get_y() - 1, nc, d, dc)) {
        x = c.get_x();
        y = c.get_y() - 1;
    }


    if (x - y == 0) {
        return PSTATE::P_NOT_AVAIL;
    }
    msp_delta_xy* pl = new  msp_delta_xy();
    pl->set_x(nc.get_x());
    pl->set_y(nc.get_y());
    Event* e = new Event(MSPCOMMANDS::MSP_DELTA_XY, MSPDIRECTION::EXTERNAL_OUT, pl);
    q.push(e);
    return traveresePath(q, nc, d);    
}

queue<Event*>  GreedyAi::calcPath(Event* dest) {
    queue<Event*> q;

    msp_delta_xy c = _smg.getCurrentDelta(), d = dest->getPayload<msp_delta_xy>();


    return q;
}