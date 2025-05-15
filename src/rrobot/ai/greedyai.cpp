#include <rrobot/ai/greedyai.hpp>

using namespace rrobot;

/**
 * TODO:
 * 
 * - points are not getting contained properly
 * - some logging is desperately needed.
 */

/*
 * sp = starting point
 * c = current vector
 * dp = detination point
 * n = vector of travel, including heading.
 */
PSTATE GreedyAi::transverse(const dlib::vector<float, VECTOR_DIM> dp) {
    const dlib::vector<float, VECTOR_DIM> sp = _sm.getCp();
    dlib::vector<float, VECTOR_DIM> c = dp - sp;
    // previously transversed points
    std::stack<dlib::vector<float, VECTOR_DIM>> _trans;
    // local exclusions,  these include all paths that have been transversed. 
    std::vector<dlib::vector<float, VECTOR_DIM>> excl = _excl;

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

bool GreedyAi::is_valid(dlib::vector<float, VECTOR_DIM> p, std::vector<dlib::vector<float, VECTOR_DIM>> excl) {
    return true;
    for (auto r : excl) {
        if (r.x() == p.x() && r.y() == p.y() && r.z() == p.z()) {
            return false;
        }
    }
    return true;
}