#include <rrobot/handlers/aihandler.hpp>

dlib::logger dlog_ah("rr_aihandler");

using namespace rrobot;

bool AiHandler::consume(StateManager& smg, Event* event) {
    _available = false;
    _ai.setStatus(RRP_STATUS::INITILIZING);
    std::stack<Event *> st;
    st.push(event);

    while (_ai.getStatus() != RRP_STATUS::TERMINATED) {
        Event* e = st.top();
        AiFeatures f = createFeatures(e);

        // if event is the same dont add it to the stack,
        e = _ai.eval(e, f);
        if (_ai.getStatus() == RRP_STATUS::ACTIVE) {
            st.pop();
            Event* a = _ai.exec(e, f, smg);

            // send command to micro-processor
            RmMultiWii m = _serializer.serialize(a);
            _external.send_rr(m.encode(_crc).c_str(), m.getSize());
        } else if (_ai.getStatus() != RRP_STATUS::TERMINATED) {
            st.push(e);
        }
    }
    _available = true;
    return false;
}

Event*  AiHandler::produce(StateManager& sm) {

    // return sensors back to UI.
    RmMultiWii m = RmMultiWii::createInstance("", MSPCOMMANDS::MSP_SENSOR);
    _external.send_rr(m.encode(_crc).c_str(), m.getSize());

    while (!_external.available()) {}
    uint8_t buf = '\0';
    std::string data = "";

    while (buf != RmMultiWii::_TERMINATION_CHAR) {
        _external.recv_rr(&buf, 1);
        data += buf;
    }
    m = RmMultiWii::createInstance(data, _crc);
    Event* e = _serializer.deserialize(m);
    return e;
}

/*
 * Creates features for command for use during the evaluation period.
 */
AiFeatures AiHandler::createFeatures(Event *e) {
    AiFeatures f;
    for ( auto cmd : _ai.wantedFeatures(e)){
        RmMultiWii m = RmMultiWii::createInstance("", cmd);
        _external.send_rr(m.encode(_crc).c_str(), m.getSize());

        while (!_external.available()) {}
        uint8_t buf = '\0';
        std::string data = "";

        while (buf != RmMultiWii::_TERMINATION_CHAR) {
            _external.recv_rr(&buf, 1);
            data += buf;
        }
        m = RmMultiWii::createInstance(data, _crc);
        f.addFeature(_serializer.deserialize(m));
    }

    return f;
}