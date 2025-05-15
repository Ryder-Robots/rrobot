#include <rrobot/ai/greedyai.hpp>

using namespace rrobot;

/**
 * TODO:
 * - Transient points are not being calculated correctly at this state, currently the 
 * vector is just getting added to stack, this will need to be something like
 * 
 * (dp - c) + sp
 * 
 * - sp needs to reset to dp when destination is achieved, or P_NOT_AVAIL
 * - currently sp is assumed to be <0,0,0> this wont be true. it will be first the very first
 * request, so is a sensible default, but after subsequent requests it will change to whatever the last
 * dp was.
 * - some logging is desperately needed.
 */

/*
 * sp = starting point
 * c = current vector
 * dp = detination point
 * n = vector of travel, including heading.
 */
PSTATE GreedyAi::transverse(const dlib::vector<float, VECTOR_DIM> dp) {
    dlib::vector<float, VECTOR_DIM> c = dp;

    while (c.length() != 0) {
        bool f = false;
        dlib::vector<float, VECTOR_DIM> n;  // next transversal
        float l = c.length();

        // attempt to get best path first,
        for (auto v : res) {
            // n is a point at this stage
            dlib::vector<float, VECTOR_DIM> nd = c - v;
            if (is_valid(nd)) {
                if (!f || nd.length() < l) {
                    l = nd.length();
                    n = v;
                    f = true;
                } 
            }
        }

        if (f) {
            _excl.push_back(c - n);
            rotate(n);

            if (!detecto()) {
                move(n);
                c = c - n;
                _trans.push(c);
            } 
        } else {
            if (_trans.empty()) {
                return PSTATE::P_NOT_AVAIL;
            }
            dlib::vector<float, VECTOR_DIM> lp = _trans.top(), n = _trans.top() - c;
            _trans.pop();
            rotate(n);
            move(n);
            c = lp;
        }
    }

    return PSTATE::P_AVAILABLE;
}

// TODO: needs completing
void GreedyAi::rotate(dlib::vector<float, VECTOR_DIM>) {}

// TODO: needs completing
bool GreedyAi::detecto() { return false; }

// TODO: needs completing
void GreedyAi::move(dlib::vector<float, VECTOR_DIM>) {}

bool GreedyAi::is_valid(dlib::vector<float, VECTOR_DIM> p) {
    for (auto r : _excl) {
        if (r.x() == p.x() && r.y() == p.y()) {
            return false;
        }
    }
    return true;
}