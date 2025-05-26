#include <rrobot/ai/greedyai.hpp>

using namespace rrobot;

/**
 * TODO:
 * - add stop command.
 * - some logging is desperately needed.
 */

/*
 * sp = starting point
 * c = current vector
 * dp = detination point
 * n = vector of travel, including heading.
 */
PSTATE GreedyAi::transverse(const dlib::vector<float, VECTOR_DIM> dp) {

    // call decto first, this will set the sensors.
    detecto();

    const dlib::vector<float, VECTOR_DIM> sp = _sm.getCp();
    dlib::vector<float, VECTOR_DIM> c = dp - sp;
    // previously transversed points
    std::stack<dlib::vector<float, VECTOR_DIM>> _trans;
    // local exclusions,  these include all paths that have been transversed. 
    std::vector<dlib::vector<float, VECTOR_DIM>> excl = _excl;

    // resolutions need to phased out in future versions, this can be calculated by using SIN and 
    // vector maths for more accurate turning. It assumes that everything is 45 degrees, which is obviously
    // not correct.
    while (c.length() > 0) {
        bool f = false;
        dlib::vector<float, VECTOR_DIM> n(0,0,0);  // next transversal
        float l = c.length();

        // attempt to get best path first,
        for (auto v : res) {
            // n is a point at this stage
            dlib::vector<float, VECTOR_DIM> nd = c - v;
            if (is_valid((dp + nd) + sp, excl)) {
                if (!f || nd.length() < l) {
                    l = nd.length();
                    n = v;
                    f = true;
                } 
            }
        }

        if (f) {
            excl.push_back((dp - c) + sp);
            rotate(n);

            if (!detecto()) {
                move(n);
                c = c - n;
                _sm.setCp(excl.back());
                _trans.push(excl.back());
            } else {
                _excl.push_back(excl.back());
            }
        } else {
            if (_trans.empty()) {
                stop();
                return PSTATE::P_NOT_AVAIL;
            }
            dlib::vector<float, VECTOR_DIM> lp = _trans.top(), n = _trans.top() - c;
            _trans.pop();
            rotate(n);
            move(n);
            c = lp;
        }
    }
    stop();
    return PSTATE::P_AVAILABLE;
}

/*
 * use base algorithm rotate the robot. 
 */
void GreedyAi::rotate(dlib::vector<float, VECTOR_DIM> v) {
    _base.rotate(v);
}

/*
 * use base algorithm to detect object.
 */
bool GreedyAi::detecto() { 
    return _base.detecto(); 
}

/*
 * use base algorithm to move
 */
void GreedyAi::move(dlib::vector<float, VECTOR_DIM> v) {
    _base.move(v);
}

void GreedyAi::stop() {
    _base.stop();
}

/*
 * compare attributes within all excluded points, to ensure that point has not been discovered before.
 */
bool GreedyAi::is_valid(dlib::vector<float, VECTOR_DIM> p, std::vector<dlib::vector<float, VECTOR_DIM>> excl) {
    for (auto r : excl) {
        if (r.x() == p.x() && r.y() == p.y() && r.z() == p.z()) {
            return false;
        }
    }
    return true;
}