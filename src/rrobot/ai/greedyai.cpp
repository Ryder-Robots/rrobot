#include <rrobot/ai/greedyai.hpp>

using namespace rrobot;

PSTATE GreedyAi::transverse(msp_delta_xy d) {
    dlib::vector<float, 2> dp(d.get_x(), d.get_y()), c = dp;

    while (c.x() + c.y() != 0) {
        bool f = false;
        dlib::vector<float, 2> n;  // next transversal
        float least = -1;
        for (auto v : res) {
            // n is a point at this stage
            n = c - v;
            if (is_valid(n)) {
                n = v;
                f = true;
                break;
            }
        }
        
        if (f) {
            c = c - n;
            _excl.push_back(c);
            std::sort(_excl.begin(), _excl.end(), std::less<dlib::vector<float, 2>>());
            rotate(c - n);

            if (!detecto()) {
                move(n);
                _trans.push(c);
            } else {
                c = c + n;
            }
        } else {
            if (_trans.empty()) {
                return PSTATE::P_NOT_AVAIL;
            }
            dlib::vector<float, 2> c = _trans.top() - c;
            _trans.pop();
        }
    }

    return PSTATE::P_AVAILABLE;
}


//TODO: needs completing
void GreedyAi::rotate(dlib::vector<float, 2>) {

}

//TODO: needs completing
bool GreedyAi::detecto() {
    return false;
}

// TODO: needs completing
void GreedyAi::move(dlib::vector<float, 2>) {

}

bool  GreedyAi::is_valid(dlib::vector<float, 2> p) {
    for (auto r : _excl) {
        if (r.x() == p.x() && r.y() == p.y()) {
            return false;
        }
    }
    return true;
}