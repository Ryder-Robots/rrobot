#include <rrobot/handlers/currentstatehandler.hpp>

namespace rrobot {


Event* CurrentStateHandler::produce(StateManager& sm) {
    if (_idx == sm.getFeatures().getFeatures().size()) {
        _idx = 0;
    }
    return sm.getFeatures().getFeatures().at(_idx++);
}

bool CurrentStateHandler::consume(StateManager& smg, Event* event) {
    if (smg.getFeatures().getFeatures().size() > 0) {
        _available = true;
    }

    return true;
}

} // namespace rrobot