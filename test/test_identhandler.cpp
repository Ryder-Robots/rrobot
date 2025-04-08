#include <gtest/gtest.h>
#include <rrobot/handlers/identhandler.hpp>
#include <rrobot/environment/environmentProcessor.hpp>
#include <filesystem>
#include <fstream>

using namespace rrobot;
namespace fs = std::filesystem;

Environment createEnv() {
    const fs::path filepath = "manifests/virtual.json";
    std::ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    return EnviromentProcessor::createEnvironment(manifest);
}


class TestIdentHandler : public ::testing::Test {
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

 TEST_F(TestIdentHandler, shouldProduceEvent) {
    IdentHandler handler(createEnv());
    Event* event = new Event(MSPCOMMANDS_HPP::MSP_IDENT, MSPDIRECTION::EXTERNAL_IN);
    EXPECT_EQ(false, event->hasPayload());
    handler.consume(_sm, event);

    EXPECT_EQ(true, handler.available());
    delete(event);
    event = handler.produce(_sm);

    EXPECT_EQ(true, event->hasPayload());
    delete(event);
}

TEST_F(TestIdentHandler, shouldNotProduceEvent) {
    IdentHandler handler(createEnv());
    EXPECT_EQ(false, handler.available());
}

TEST_F(TestIdentHandler, shouldReflectUpdates) {
    IdentHandler handler(createEnv());

    Event* event = new Event(MSPCOMMANDS_HPP::MSP_IDENT, MSPDIRECTION::EXTERNAL_IN);
    EXPECT_EQ(false, event->hasPayload());
    handler.consume(_sm, event);

    EXPECT_EQ(true, handler.available());
    delete(event);
    event = handler.produce(_sm);

    EXPECT_EQ(true, event->hasPayload());
    msp_ident payload = event->getPayload<msp_ident>();
    EXPECT_EQ(0,payload.get_capability());
    EXPECT_EQ(MULTITYPE_T::LANDDRONE_4W, payload.get_multitype());
    EXPECT_EQ(MSP_VERSION::VIRTUAL, payload.get_msp_version());
    EXPECT_EQ(MSPCOMMANDS::MSP_IDENT, event->getCommand());
    delete(event);
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
