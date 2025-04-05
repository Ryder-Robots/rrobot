#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <rrobot/environment/environmentProcessor.hpp>
#include <rrobot/events/eventrunner.hpp>

using ::testing::_;
using ::testing::Return;
namespace fs = std::filesystem;
using namespace rrobot;

class TestEventRunner : public ::testing::Test {
   protected:
    void SetUp() override {}
    void TearDown() override {}
};

Environment createEnv() {
    const fs::path filepath = "manifests/virtual.json";
    std::ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    return EnviromentProcessor::createEnvironment(manifest);
}

class MockConcreteHandler : public EventHandler {
   public:
    MockConcreteHandler() {}
    ~MockConcreteHandler() {}
    MOCK_METHOD(bool, consume, (StateManager&, Event*), (override));
    MOCK_METHOD(void, tearDown, (), (override));

    string name() override { return "mockhandler"; }
    void init(StateManager& smg, Environment& env) {}

    bool _reload_called = false;
    bool _available = true;

    void reload() override { 
        _reload_called = true;
    }

    Event* produce(StateManager& s) {
        msp_authkey* payload = new msp_authkey();
        payload->set_key("test");
        Event* event = new Event(MSP_AUTHKEY, MSPDIRECTION::EXTERNAL_IN, payload);
        return event;
    }

    bool available() override {
        if (_available == true) {
            _available = false;
            return true;
        }
        return false;
    }
};

TEST_F(TestEventRunner, TestShouldExecute) {
    Environment env = createEnv();
    RrQueueManager qmg(
        env.getQueues().getLimit(), 
        std::chrono::milliseconds(env.getQueues().getThreadWaitTime()), 
        std::chrono::milliseconds(env.getQueues().getThreadProcessTime()));

    StateManager smg;
    MockConcreteHandler handler;
    qmg.addQueue(RRP_QUEUES::AI_ENGINE);
    qmg.addQueue(RRP_QUEUES::USER_INTERFACE);
    EventRunner* runner = new EventRunner(&handler, qmg, smg, RRP_QUEUES::AI_ENGINE, RRP_QUEUES::USER_INTERFACE);
    std::thread t = std::thread(EventRunner::run, runner);
    std::this_thread::sleep_for(std::chrono::milliseconds(env.getQueues().getThreadProcessTime()));
    smg.setIsRunning(false);
    t.join();
    EXPECT_EQ(true, true);
    delete(runner);
}

// TEST_F(TestEventRunner, TestInvalidQueue) {
//     MockConcreteHandler handler = MockConcreteHandler();
//     Environment env = createEnv();
//     RrQueueManager qmg(
//         env.getQueues().getLimit(), 
//         std::chrono::milliseconds(env.getQueues().getThreadWaitTime()), 
//         std::chrono::milliseconds(env.getQueues().getThreadProcessTime()));
//     StateManager smg;
//     handler._reload_called = false;
//     EventRunner* runner = new EventRunner(&handler, qmg, smg, RRP_QUEUES::AI_ENGINE, RRP_QUEUES::USER_INTERFACE);
    
//     try {
//         std::thread t = std::thread(EventRunner::run, runner);
//         t.join();
//     } catch (const std::exception& e) {
//         throw;
//     }
    
//     EXPECT_EQ(true, handler._reload_called);
//     delete(runner);
// }

TEST_F(TestEventRunner, TestConsumeEvent) {
    Environment env = createEnv();
    RrQueueManager qmg(
        env.getQueues().getLimit(), 
        std::chrono::milliseconds(env.getQueues().getThreadWaitTime()), 
        std::chrono::milliseconds(env.getQueues().getThreadProcessTime()));

    StateManager smg;
    MockConcreteHandler handler;
    EXPECT_CALL(handler, consume(_, _)).Times(1).WillOnce(Return(true));

    qmg.addQueue(RRP_QUEUES::AI_ENGINE);
    qmg.addQueue(RRP_QUEUES::USER_INTERFACE);

    msp_authkey* payload = new msp_authkey();
    payload->set_key("test");
    Event* event = new Event(MSP_AUTHKEY, MSPDIRECTION::EXTERNAL_IN, payload);
    qmg.enqueue(RRP_QUEUES::AI_ENGINE, event);
    EXPECT_EQ(false, qmg.isEmpty(RRP_QUEUES::AI_ENGINE));
    EventRunner* runner = new EventRunner(&handler, qmg, smg, RRP_QUEUES::AI_ENGINE, RRP_QUEUES::USER_INTERFACE);
    std::thread t = std::thread(EventRunner::run, runner);
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    smg.setIsRunning(false);
    t.join();

    EXPECT_EQ(true, qmg.isEmpty(RRP_QUEUES::AI_ENGINE));

    delete(runner);
}

TEST_F(TestEventRunner, TestProduceEvent) {
    Environment env = createEnv();
    RrQueueManager qmg(
        env.getQueues().getLimit(), 
        std::chrono::milliseconds(env.getQueues().getThreadWaitTime()), 
        std::chrono::milliseconds(env.getQueues().getThreadProcessTime()));

    StateManager smg;
    MockConcreteHandler handler;

    qmg.addQueue(RRP_QUEUES::AI_ENGINE);
    qmg.addQueue(RRP_QUEUES::USER_INTERFACE);

    smg.setIsRunning(true);
    EventRunner* runner = new EventRunner(&handler, qmg, smg, RRP_QUEUES::AI_ENGINE, RRP_QUEUES::USER_INTERFACE);
    std::thread t = std::thread(EventRunner::run, runner);
    while(qmg.isEmpty(RRP_QUEUES::USER_INTERFACE)) {}
    EXPECT_EQ(1, qmg.size(RRP_QUEUES::USER_INTERFACE));
    smg.setIsRunning(false);
    t.join();
    delete(runner);
}



int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
