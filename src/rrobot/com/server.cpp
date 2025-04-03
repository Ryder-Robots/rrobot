#include <rrobot/com/server.hpp>

using namespace rrobot;

dlib::logger dlog_svr("rr_robot_svr");

RrServer::RrServer(Environment env, StateManager& smg) {
    dlog_svr.set_level(env.getLogging().getLogLevel());
    struct sockaddr_in servaddr;
    if ((_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        dlog_svr << dlib::LERROR << "unable to create connection for UI " << strerror(errno);
        throw BadConnectionException("unable to create connection for UI " + string(strerror(errno)));
    }
    if (bind(_sockfd, (const struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        dlog_svr << dlib::LFATAL << "fatal error occured: " << strerror(errno);
        throw BadConnectionException("could not bind to port " + string(strerror(errno)));
    }
    listen(_sockfd, env.getServer().getMaxConnections());
    dlog_svr << dlib::LINFO << "listening for connections on port " << env.getServer().getPort();
}

void RrServer::init(StateManager& smg) {
}

ssize_t RrServer::recv_rr(void* buffer, size_t bufsz) {
    if (available() <= 0) {
        return -1;
    }
    size_t n = recv(_socket, buffer, bufsz, 0);
    return n;
}

int RrServer::accept_rr() {
    _socket = accept(_sockfd, nullptr, nullptr);
    return _socket;
}

void RrServer::close_rr() {
    if (available() > 0) {
        close(_socket);
    }
}

ssize_t RrServer::send_rr(const void* buf, size_t bufsz) {
    if (available() <= 0) {
        return -1;
    }
    return send(_socket, buf, bufsz, 0);
}

ssize_t RrServer::available() {
    char peek_buf[1];
    ssize_t available = recv(_socket, peek_buf, sizeof(peek_buf), MSG_PEEK);
    return available;
}

int RrServer::shutdown_rr() { return shutdown(_sockfd, SHUT_RDWR); }