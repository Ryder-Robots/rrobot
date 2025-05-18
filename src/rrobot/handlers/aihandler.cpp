#include <rrobot/handlers/aihandler.hpp>

dlib::logger dlog_ah("rr_aihandler");

using namespace rrobot;

bool AiHandler::consume(StateManager& smg, Event* event) {
    dlog_ah << dlib::LINFO << "recieved request for navigation";

    // Need to get the delta.
    if (event->getCommand() == MSPCOMMANDS::MSP_VECTOR) {
        msp_vector dp = event->getPayload<msp_vector>();
        dlog_ah << dlib::LINFO << "navigating to new point: <" << dp.get_x() << "," << dp.get_y() << "," << dp.get_z()
                << ">";
        dlib::vector<float, VECTOR_DIM> dpv(dp.get_x(), dp.get_y(), dp.get_z());
        _ai.transverse(dpv);
    }
    return false;
}

Event* AiHandler::produce(StateManager& sm) { return new Event(MSPCOMMANDS::MSP_NONE, MSPDIRECTION::EXTERNAL_OUT); }
