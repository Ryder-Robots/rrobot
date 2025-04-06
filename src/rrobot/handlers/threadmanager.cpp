#include <rrobot/handlers/threadmanager.hpp>

using namespace rrobot;

dlib::logger dlog_tm("rr_robot_tm");


void ThreadManager::setup(const string epath) {
    std::ifstream ifs(epath);
    std::string content((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    ifs.close();

    EnviromentProcessor ep;
    json manifest = json::parse(content);
    Environment env = ep.createEnvironment(manifest);
    dlog_tm.set_level(env.getLogging().getLogLevel());

    try {
        dlog_tm << dlib::LINFO << "initializing application";
        _mapper->init(env, _smg, epath);

        dlog_tm << dlib::LINFO << "creating queue manager";
        _qmg = _mapper->queueManager(env);
        _thread_wait_time = env.getQueues().getThreadWaitTime();
        dlog_tm << dlib::LINFO << "created queue manager";

        dlog_tm << dlib::LINFO << "creating queues";
        std::vector<RRP_QUEUES> qnames = _mapper->queueNames();
        for (auto qn : qnames) {
            _qmg->addQueue(qn);
        }
        _qmg->addQueue(RRP_QUEUES::CATEGORIZER);
        dlog_tm << dlib::LINFO << "queues added";
        dlog_tm << dlib::LINFO << "creating threads";
        int i = 0;
        for (auto h : _mapper->createEventHandlers()) {
            EventRunner runner = EventRunner(h, *_qmg, _smg, qnames[i], RRP_QUEUES::CATEGORIZER);
            std::thread* t = new std::thread(EventRunner::run, &runner);
            _eventRunners.push_back(runner);
            _threads.push_back(t);
        }
        dlog_tm << dlib::LINFO << "created threads";

        dlog_tm << dlib::LINFO << "initializing completed";

    } catch (const std::exception& e) {
        dlog_tm << dlib::LFATAL << "cannot start application: " << e.what();
        exit(EXIT_FAILURE);
    }
}

void ThreadManager::teardown() {
    dlog_tm << dlib::LINFO << "waiting for handlers to shutdown";
    std::this_thread::sleep_for(chrono::milliseconds(_thread_wait_time));
    bool isTerminated = false;

    for (int i = 0; i < 5; i++) {
        std::this_thread::sleep_for(chrono::milliseconds(_thread_wait_time));
        if (checkStatus(RRP_STATUS::TERMINATED) || _smg.getStatus() == RRP_STATUS::ERROR) {
            isTerminated = true;
            break;
        }
    }

    if (!isTerminated) {
        dlog_tm << dlib::LERROR << "not all handlers terminated!!!";
    }

    dlog_tm << dlib::LINFO << "terminating and cleaning up threads";
    for (thread* t : _threads) {
        if (t->joinable()) {
            t->join();
            delete (t);
        }
        _threads.clear();
    }

    dlog_tm << dlib::LINFO << "shutting down application";
    if (_qmg != nullptr) {
        delete (_qmg);
    }
}

bool ThreadManager::checkStatus(RRP_STATUS wanted) {
    bool isActive = false;
    for (EventRunner r : _eventRunners) {
        if (r.status() & wanted) {
            isActive = true;
            break;
        }
    }
    return isActive;
}

void ThreadManager::run() {
    bool isActive = true;
    dlog_tm << dlib::LINFO << "starting application";
    while (_smg.isRunning()) {
        std::this_thread::sleep_for(chrono::milliseconds(_thread_wait_time));
        try {
            if (_qmg->isEmpty(RRP_QUEUES::CATEGORIZER)) {
                continue;
            }
            Event* e = _qmg->dequeue(RRP_QUEUES::CATEGORIZER);
            RRP_QUEUES q = _mapper->mapQueue(e);
            if (_qmg->isFull(q)) {
                dlog_tm << dlib::LWARN << "found full queue, ignoring event: " << e->getCommand()
                        << " and attempting to reset handlers";
                throw new ExceedMaxMessage("found full queue, ignoring event");
            }
            _qmg->enqueue(q, e);
            if (!isActive) {
                dlog_tm << dlib::LERROR << "an error has occurred, if this persists, perrform emergancy landing";
                throw new InvalidStatus("incorrect state reseting handlers.");
            }
        } catch (const std::exception& e) {
            dlog_tm << dlib::LERROR << e.what();
            _smg.setStatus(RRP_STATUS::ERROR);
            for (EventRunner r : _eventRunners) {
                if (r.status() != RRP_STATUS::ACTIVE) {
                }
                r.handler()->reload();
            }
            std::this_thread::sleep_for(chrono::milliseconds(_thread_wait_time));
            isActive = checkStatus(RRP_STATUS::ACTIVE);
            if (isActive) {
                _smg.resetStatus();
                _smg.setStatus(RRP_STATUS::ACTIVE);
            }
        }
    }
}
