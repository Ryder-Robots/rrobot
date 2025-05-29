#include <rrobot/ai/greedyai.hpp>

using namespace rrobot;

/**
 * TODO:
 * - some logging is desperately needed.
 * - c should indicate vector.
 * - cp needs to be added to simply equations,  this is just cp = cp + n after movement, then round to get actual point.
 * - need to round off very small numbers by using a wrapper for cos, and sin
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
    dlib::vector<float, VECTOR_DIM> c = dp - _sm.getFeatures().get_sensor_mag().get_data(), cp = sp;
    // previously transversed points
    std::stack<dlib::vector<float, VECTOR_DIM>> _trans;
    // local exclusions,  these include all paths that have been transversed.
    std::vector<dlib::vector<float, VECTOR_DIM>> excl = _excl;

    while (c.length() > 0) {
        bool f = false;
        float r = R(atan2(c.y(), c.x())), l = c.length();
        dlib::vector<float, VECTOR_DIM> n(R(cos(r)), R(sin(r)), 0);

        if (!is_valid(cp + n, excl)) {
            // attempt to get best path first,
            for (auto v : res) {
                // n is a point at this stage
                dlib::vector<float, VECTOR_DIM> nd = cp + v;
                if (is_valid(nd, excl)) {
                    if (!f || nd.length() < l) {
                        l = nd.length();
                        n = v;
                        f = true;
                    }
                }
            }
        } else {
            f = true;
        }

        if (f) {
            excl.push_back(cp);
            rotate(n);
            if (!detecto()) {
                move(n);
                c = RR(c - n);
                cp = RR(cp + n);
                _sm.setCp(cp);
                _trans.push(cp);
            } else {
                _excl.push_back(cp + n);
            }
        } else {
            if (_trans.empty()) {
                stop();
                return PSTATE::P_NOT_AVAIL;
            }
            dlib::vector<float, VECTOR_DIM> lp = _trans.top(), n = _trans.top() - c;
            _trans.pop();
            rotate(n);
            cp = RR(cp + n);
            move(n);
            c = lp;
        }
    }
    stop();
    return PSTATE::P_AVAILABLE;
}

// round to 4 decimal place, by converting to an integer then dividing the number by 10000
float GreedyAi::R(float x) { return std::round(x * 10000.f) / 10000.0f; }

dlib::vector<float, VECTOR_DIM> GreedyAi::RR(dlib::vector<float, VECTOR_DIM> vi) {
    dlib::vector<float, VECTOR_DIM> vo(std::round(vi.x()), std::round(vi.y()), std::round(vi.z()));
    return vo;
}

/*
 * use base algorithm rotate the robot.
 */
void GreedyAi::rotate(dlib::vector<float, VECTOR_DIM> v) { _base.rotate(v); }

/*
 * use base algorithm to detect object.
 */
bool GreedyAi::detecto() { return _base.detecto(); }

/*
 * use base algorithm to move
 */
void GreedyAi::move(dlib::vector<float, VECTOR_DIM> v) { _base.move(v); }

void GreedyAi::stop() { _base.stop(); }

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