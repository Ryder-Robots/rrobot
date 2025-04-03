#include <rrobot/handlers/uihandler.hpp>

using namespace rrobot;
using json = nlohmann::json;

dlib::logger dlog_ui("rr_robot_ui");

void UiHandler::init(Environment env) {
    dlog_ui.set_level(env.getLogging().getLogLevel());
    dlog_ui << dlib::LINFO << "configurating " << name();
    _available = true;
    _status = RRP_STATUS::ACTIVE;
}

void UiHandler::shutdown() {
    dlog_ui << dlib::LINFO << "begining shutdown procedure: " << name();
    _status = RRP_STATUS::SHUTTING_DOWN;
    _status = RRP_STATUS::TERMINATED;
}

bool UiHandler::consume(StateManager& smg, Event* event) {
    _available = false;
    json out = _serializer.serialize(event);
    string output = out.dump() + _delimiter;
    if (_external.send_rr(output.c_str(), output.length() * sizeof(char)) == -1) {
        _status = RRP_STATUS::ERROR;
        dlog_ui << dlib::LFATAL
                << "sommething went wrong when accepting connection: " + to_string(errno) + ": " + strerror(errno);
        throw NetworkIOException("sommething went wrong when accepting connection: " + to_string(errno) + ": " +
                                 strerror(errno));
    }
    _available = true;    
    return true;
}

Event* UiHandler::produce(StateManager& smg) {
    _available = false;
    size_t pos = 0;
    memset(_buffer, 0, BUFSIZ);

    while (pos != BUFSIZ) {
        if (pos == BUFSIZ) {
            _status = RRP_STATUS::ERROR;
            dlog_ui << dlib::LERROR << "excessively large message has been recieved";
            throw ExceedMaxMessage("excessively large message has been recieved");
        }
        char c;
        size_t n = _external.recv_rr(&c, sizeof(char));
        if (n == -1) {
            _status = RRP_STATUS::ERROR;
            dlog_ui << dlib::LERROR << "IO Networking error has occurred";
            throw NetworkIOException("IO Networking error has occurred");
        } else if (n != 1) {
            _status = RRP_STATUS::ERROR;
            dlog_ui << dlib::LERROR << "message sent without deliemeter";
            throw InvalidMessageSent("message sent without deliemeter");
        }
        if (c == _delimiter) {
            break;
        }
        _buffer[pos++] = c;
    }
    string token = _buffer;
    if (!json::accept(token)) {
        _status = RRP_STATUS::ERROR;
        dlog_ui << dlib::LERROR << "inbound message is not valid JSON";
        throw InvalidFormat("inbound message is not valid JSON");
    }
    json j = json::parse(token);
    if (!j.contains("command")) {
        _status = RRP_STATUS::ERROR;
        dlog_ui << dlib::LERROR << "command is a required attribute";
        throw MissingRequiredAttributeException("command is a required attribute");
    }
    Event* event = _serializer.deserialize(j, MSPDIRECTION::EXTERNAL_IN);
    _available = true;
    return event;
}

bool UiHandler::available() {
    bool available = _available;
    if (available) {
        ssize_t availableBytes = _external.available();
        if (availableBytes <= 0) {
            available = false;
        }
    }
    return available;
}

void UiHandler::setUp() {
    _external.init(_smg);
    dlog_ui << dlib::LINFO << "changing to active state to listen to port";
    _status = RRP_STATUS::ACTIVE;
    if (_external.accept_rr() == -1) {
        _status = RRP_STATUS::ERROR;
        dlog_ui << dlib::LFATAL
                << "sommething went wrong when accepting connection: " + to_string(errno) + ": " + strerror(errno);
        throw BadConnectionException("sommething went wrong when accepting connection: " + to_string(errno) + ": " +
                                     strerror(errno));
    }
    dlog_ui << dlib::LINFO << "established successful connection";
}

void UiHandler::tearDown() {
    _available = false;
    _status = RRP_STATUS::SHUTTING_DOWN;
    dlog_ui << dlib::LINFO << "closing connection";
    _external.close_rr();
    _external.shutdown_rr();
    dlog_ui << dlib::LINFO << "connection closed";
    _status = RRP_STATUS::TERMINATED;
}

void UiHandler::reload() {
    dlog_ui << dlib::LERROR << "attempting to stop and re-establish connection";
    
    if (_external.accept_rr() == -1) {
        _status = RRP_STATUS::ERROR;
        dlog_ui << dlib::LFATAL
                << "sommething went wrong when accepting connection: " + to_string(errno) + ": " + strerror(errno);
        throw BadConnectionException("sommething went wrong when accepting connection: " + to_string(errno) + ": " +
                                     strerror(errno));
    }
    _status = RRPSTATUS_HPP::ACTIVE;
    dlog_ui << dlib::LINFO << " established successful connection";
}

void  UiHandler::onError(const std::exception& e) {
    dlog_ui << dlib::LWARN << "known exception has occurred, checking to see if it is fatal";
    if (_external.available() > 0) {
        dlog_ui << dlib::LINFO << "error is not fatal, retaining current connection - resetting to active";
        _status = RRP_STATUS::ACTIVE;
        return;
    }

    dlog_ui << dlib::LFATAL << "connection has been broken, resarting";
    reload();
}

