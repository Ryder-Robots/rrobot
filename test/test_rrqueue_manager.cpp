#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <rrobot/environment/environmentProcessor.hpp>
#include <rrobot/state/rrqueue_manager.hpp>

using namespace rrobot;
namespace fs = std::filesystem;

class TestRrQueueManager : public ::testing::Test {
    protected:

    Environment getEnv() {
        const fs::path filepath = "manifests/virtual.json";
        std::ifstream ifs(filepath);
        json manifest = json::parse(ifs);
        ifs.close();
    
        return EnviromentProcessor::createEnvironment(manifest);
    }

    void SetUp() override {
  
    }
    
    void TearDown() override {
        // Teardown code
    }
};

TEST_F(TestRrQueueManager, shouldAddQueue) {
    Environment env = getEnv();
    RrQueueManager qmng = RrQueueManager(
        env.getQueues().getLimit(), 
        std::chrono::milliseconds(env.getQueues().getThreadWaitTime()), 
        std::chrono::milliseconds(env.getQueues().getThreadProcessTime()));

    qmng.addQueue(RRP_QUEUES::MICROCONTROLLER);
    EXPECT_EQ(std::chrono::milliseconds(env.getQueues().getThreadProcessTime()), qmng.queueProcessTime(RRP_QUEUES::MICROCONTROLLER));
    EXPECT_EQ(true, qmng.isEmpty(RRP_QUEUES::MICROCONTROLLER));
    EXPECT_EQ(0, qmng.size(RRP_QUEUES::MICROCONTROLLER));
}

TEST_F(TestRrQueueManager, shouldAddEvent) {
    Environment env = getEnv();
    RrQueueManager qmng = RrQueueManager(
        env.getQueues().getLimit(), 
        std::chrono::milliseconds(env.getQueues().getThreadWaitTime()), 
        std::chrono::milliseconds(env.getQueues().getThreadProcessTime()));

    qmng.addQueue(RRP_QUEUES::MICROCONTROLLER);
    Event* e = new Event(MSPCOMMANDS::MSP_SENSOR, MSPDIRECTION::EXTERNAL_OUT);
    qmng.enqueue(RRP_QUEUES::MICROCONTROLLER, e);
    EXPECT_EQ(false, qmng.isEmpty(RRP_QUEUES::MICROCONTROLLER));
    EXPECT_EQ(1, qmng.size(RRP_QUEUES::MICROCONTROLLER));

    Event* e2 = qmng.dequeue(RRP_QUEUES::MICROCONTROLLER);
    EXPECT_EQ(true, qmng.isEmpty(RRP_QUEUES::MICROCONTROLLER));
    EXPECT_EQ(0, qmng.size(RRP_QUEUES::MICROCONTROLLER));
    EXPECT_EQ(e, e2);

    delete(e);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
