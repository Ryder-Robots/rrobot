#include <rrobot/ai/greedyai.hpp>

using namespace rrobot;

PSTATE GreedyAi::transverse(msp_delta_xy d) {
    dlib::vector<float, 2> dp(d.get_x(), d.get_y()), c = dp;

    while (c.length() != 0) {
        bool f = false;
        dlib::vector<float, VECTOR_DIM> n;  // next transversal
        float l = c.length();

        // attempt to get best path first,
        for (auto v : res) {
            // n is a point at this stage
            dlib::vector<float, 2> nd = c - v;
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
void GreedyAi::rotate(dlib::vector<float, 2>) {}

// TODO: needs completing
bool GreedyAi::detecto() { return false; }

// TODO: needs completing
void GreedyAi::move(dlib::vector<float, 2>) {}

bool GreedyAi::is_valid(dlib::vector<float, 2> p) {
    for (auto r : _excl) {
        if (r.x() == p.x() && r.y() == p.y()) {
            return false;
        }
    }
    return true;
}