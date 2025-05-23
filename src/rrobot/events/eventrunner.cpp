#include <rrobot/events/eventrunner.hpp>

using namespace rrobot;
dlib::logger dlog_hnd("rr_robot_eventrunner");

void EventRunner::handleConsumeEvents(EventHandler* handler) {
    dlog_hnd << dlib::LDEBUG << "consuming events for " << handler->name();
    if (!_qmg.isEmpty(_inbound)) {
        dlog_hnd << dlib::LDEBUG << "consume events available:  " << handler->name();
        Event* event = _qmg.dequeue(_inbound);
        _handler->consume(_smg, event);
        delete(event);
    }
}

/* this will alway execute regardless of status, in a single run */
void EventRunner::handleProduceEvents(EventHandler* handler) {
    dlog_hnd << dlib::LDEBUG << "checking for produce events for " << handler->name();

    if (!_qmg.isFull(_outbound) && handler->available()) {
        dlog_hnd << dlib::LDEBUG << "produce events available:  " << handler->name();
        Event* event = handler->produce(_smg);
        _qmg.enqueue(_outbound, event);
    }
}

void EventRunner::timeout(EventHandler* handler, RRP_STATUS wanted) {
    auto start = std::chrono::high_resolution_clock::now();

    // arbitary pick of 3 second here, may be ut should be adjustable.
    while (std::chrono::high_resolution_clock::now() - start < 3s) {
        if (_status == wanted) {
            return;
        }
    }

    dlog_hnd << dlib::LFATAL << " timeout error occrred while waiting for " << handler->name();
    throw new TimeOutException("timeout error occurred in event runner: expected");
}

void EventRunner::run(EventRunner* runner) {
    runner->_status = RRP_STATUS::INITILIZING;
    EventHandler* handler = runner->_handler;
    StateManager& smg = runner->_smg;
    dlog_hnd << dlib::LDEBUG << "initializing:  " << handler->name();
    runner->_smg.setStatus(runner->status());

    handler->setUp();
    // set a timeout to become active
    runner->_status = RRP_STATUS::ACTIVE;

    dlog_hnd << dlib::LDEBUG << handler->name() << " is now active";
    runner->_smg.setStatus(runner->status());

    // isRunning can be considered a kill switch if it is hit, then stop
    while (runner->_smg.isRunning()) {
        auto start = std::chrono::high_resolution_clock::now();
        try {
            if (runner->status() == RRP_STATUS::ACTIVE) {
                // consume event then produce result
                runner->handleConsumeEvents(handler);                
                runner->handleProduceEvents(handler);
            }
        } catch (const std::exception& e) {
            dlog_hnd << dlib::LERROR << "handler: " << handler->name()
                     << "reported :" << "error occured while handling event: " << e.what();
            runner->_status = RRP_STATUS::ERROR;
            runner->_smg.setStatus(runner->status());
            handler->onError(e);
        }
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        std::chrono::microseconds waitTime = std::chrono::duration_cast<std::chrono::microseconds>(
           runner->_qmg.queueProcessTime(runner->_inbound) -  elapsed);

        if (waitTime.count() > 0) {
            dlog_hnd << dlib::LDEBUG << handler->name() << " finished early now waiting for " << waitTime.count()
                     << "msecs";
            this_thread::sleep_for(waitTime);
        }

        if (smg.isRunning() && runner->status() != RRP_STATUS::ACTIVE) {
            dlog_hnd << dlib::LWARN << "handler" << handler->name()
                     << ": handler status was not active attempting to reload";
            handler->reload();
            runner->timeout(handler, RRP_STATUS::ACTIVE);
        }
    }
    try {
        dlog_hnd << dlib::LINFO << handler->name() << " is now shutting down";
        runner->_status = RRP_STATUS::SHUTTING_DOWN;
        handler->tearDown();
        runner->_status = RRP_STATUS::TERMINATED;
        dlog_hnd << dlib::LINFO << handler->name() << " is terminated";
    } catch (const std::exception& e) {
        dlog_hnd << dlib::LERROR << "handler: " << handler->name()
            << "reported :" << "error occured while shutting down: " << e.what();
        runner->_smg.setStatus(RRP_STATUS::ERROR);
        runner->_status = RRP_STATUS::ERROR;
        handler->onError(e);
    }
}
