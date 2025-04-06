#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rrobot/handlers/threadmanager.hpp>

using namespace rrobot;
using testing::Return;

class TestThreadManager : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
        _sm.setIsRunning(true);
    }

    void TearDown() override {
        // Teardown code
        _sm.setIsRunning(false);
    }

    StateManager _sm;
};

class MockMapper : public RrMapper {
   public:
    MOCK_METHOD(std::vector<RRP_QUEUES>, queueNames, (), (override));
    MOCK_METHOD(RRP_QUEUES, mapQueue, (Event*), (override));
    MOCK_METHOD(std::vector<EventHandler*>, createEventHandlers, (), (override));

    void init(const Environment env, StateManager& smg) override {}
};

class MockHandler : public EventHandler {
   public:

   MOCK_METHOD(Event*, produce, (StateManager&), (override));
    
   bool consume(StateManager& smg, Event* event) override {return true;}
   string name() override {return "MockHandler";}
   bool available() override {return true;}
};

TEST_F(TestThreadManager, ShouldSetup) {
    MockMapper* mapper = new MockMapper();
    MockHandler* hdl = new MockHandler();
    std::vector<RRP_QUEUES> v = {RRP_QUEUES::AI_ENGINE};
    std::vector<EventHandler*> v2 = {hdl};

    EXPECT_CALL(*mapper, queueNames()).Times(1).WillOnce(Return(v));
    EXPECT_CALL(*mapper, createEventHandlers()).Times(1).WillOnce(Return(v2));

    // set isShutdown to true, to avoid threads attempting to execute.
    _sm.setIsRunning(false);

    ThreadManager tmg = ThreadManager(mapper, _sm, "manifests/virtual.json");

    delete (mapper);
    delete(hdl);
}

// TEST_F(TestThreadManager, shouldRun) {
//     MockMapper* mapper = new MockMapper();
//     MockHandler* hdl = new MockHandler();

//     std::vector<RRP_QUEUES> v = {RRP_QUEUES::AI_ENGINE};
//     std::vector<EventHandler*> v2 = {hdl};

//     EXPECT_CALL(*mapper, queueNames()).Times(1).WillOnce(Return(v));
//     EXPECT_CALL(*mapper, createEventHandlers()).Times(1).WillOnce(Return(v2));

//     std::vector<RRP_QUEUES> v = {RRP_QUEUES::AI_ENGINE};
//     std::vector<EventHandler*> v2 = {hdl};
//     ThreadManager tmg = ThreadManager(mapper, _sm, "manifests/virtual.json");
    
//     tmg.run();
    
//     delete (mapper);
//     delete(hdl);    
// }