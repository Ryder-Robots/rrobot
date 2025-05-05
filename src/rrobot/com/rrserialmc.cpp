#include <rrobot/com/rrserialmc.hpp>

using namespace rrobot;

dlib::logger dlog_ser("rr_robot_ser");

RrSerialMc::RrSerialMc(Environment env) {
    dlog_ser.set_level(env.getLogging().getLogLevel());

    dlog_ser << dlib::LINFO << "attempting to configure serial port";
    _serial_port.SetBaudRate(env.getMc().getBaud());
    _serial_port.SetCharacterSize(env.getMc().getCharsize());
    _serial_port.SetFlowControl(env.getMc().getFlowControl());
    _serial_port.SetParity(env.getMc().getParity());
    _serial_port.SetStopBits(env.getMc().getStopBits());

    // attempt to open conncetion
    dlog_ser << dlib::LINFO << "attempting to open connection";
    _serial_port.Open(env.getMc().getPort());
}

void RrSerialMc::close_rr() {
    dlog_ser << dlib::LINFO << "closing serial port";
    if (_serial_port.IsOpen()) {
        _serial_port.Close();
    }
}


std::string RrSerialMc::recv_rr(size_t bufsz) {
    std::string dataString;
    _serial_port.Read(dataString, bufsz, _timeout_read);
    return dataString;
}

ssize_t RrSerialMc::recv_rr(void* buffer, size_t bufsz) {
    std::string dataString = recv_rr(bufsz);
    buffer = reinterpret_cast<uint8_t*>(const_cast<char*>(dataString.c_str()));
    return dataString.size();
}

int  RrSerialMc::accept_rr() {
    return _serial_port.GetFileDescriptor();
}

ssize_t RrSerialMc::send_rr(const void* buf, size_t bufsz) {
    std::string str(reinterpret_cast<const char*>(buf), bufsz);
    return send_rr(str);
}

ssize_t RrSerialMc::send_rr(const std::string str) {
    _serial_port.Write(str);
    return str.size();
}

ssize_t RrSerialMc::available() {
    return static_cast<ssize_t>(_serial_port.IsDataAvailable());
}

int RrSerialMc::shutdown_rr()  {
    close_rr();
    return 0;
}