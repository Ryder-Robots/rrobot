#include <rrobot/handlers/aihandler.hpp>

dlib::logger dlog_ah("rr_aihandler");

using namespace rrobot;

bool AiHandler::consume(StateManager& smg, Event* event) {
    return false;
}

Event*  AiHandler::produce(StateManager& sm) {
    return nullptr;
}

AiFeatures createFeatures(Event *e) {
    return AiFeatures();
}