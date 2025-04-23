#include <rrobot/ai/greedyai.hpp>

using namespace rrobot;

float GreedyAi::absDistance(float n1, float n2) {
    if (n1 < 0 && n2 > 0) {
        return (abs(n1) + n2) - n2;
    } else if (n1 > 0 && n2 < 0) {
        return n1 - abs(n2) - n2;
    }

    return abs(n1 - n2);
}

queue<Event*>  GreedyAi::calcPath(Event* d) {
    msp_delta_xy current = _smg.getCurrentDelta(), destination = d->getPayload<msp_delta_xy>();
    queue<Event*> q;

    return q;
}