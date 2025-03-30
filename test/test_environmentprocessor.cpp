#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <rrobot/environment/environmentProcessor.hpp>

using namespace rrobot;
namespace fs = std::filesystem;

class TestEnvironmentProcessor : public ::testing::Test {
    protected:
    void SetUp() override {
        // Setup code
    }
    
    void TearDown() override {
        // Teardown code
    }
};

TEST(TestEnvironmentProcessor, TestEnvironment) {
    const fs::path filepath = "manifests/virtual.json";
    std::ifstream ifs(filepath);
    json manifest = json::parse(ifs);
    ifs.close();

    Environment environment = EnviromentProcessor::createEnvironment(manifest);

    EXPECT_EQ(MULTITYPE_T::LANDDRONE_4W, environment.getHwModel().getMultiType());
    EXPECT_EQ(MSP_VERSION::VIRTUAL, environment.getHwModel().getMspVersion());
    EXPECT_EQ(0, environment.getHwModel().getCapability());

    EXPECT_EQ("/dev/ttyAMA0", environment.getMc().getPort());
    EXPECT_EQ(LibSerial::BaudRate::BAUD_9600, environment.getMc().getBaud());
    EXPECT_EQ(LibSerial::CharacterSize::CHAR_SIZE_8, environment.getMc().getCharsize());
    EXPECT_EQ(LibSerial::FlowControl::FLOW_CONTROL_NONE, environment.getMc().getFlowControl());
    EXPECT_EQ(LibSerial::StopBits::STOP_BITS_1, environment.getMc().getStopBits());
    EXPECT_EQ(LibSerial::Parity::PARITY_NONE, environment.getMc().getParity());
}


int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}