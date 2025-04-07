#include <gtest/gtest.h>
#include <rrobot/handlers/statushandler.hpp>
#include <rrobot/environment/mspsensors.hpp>

using namespace rrobot;

class TestStatusManager : public ::testing::Test {
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

TEST_F(TestStatusManager, shouldProduceEvent) {
    RrStatusHandler handler;
    Event* event = new Event(MSPCOMMANDS_HPP::MSP_STATUS, MSPDIRECTION::EXTERNAL_IN);
    EXPECT_EQ(false, event->hasPayload());
    handler.consume(_sm, event);

    EXPECT_EQ(true, handler.available());
    delete(event);
    event = handler.produce(_sm);

    EXPECT_EQ(true, event->hasPayload());
    delete(event);
}

TEST_F(TestStatusManager, shouldNotProduceEvent) {
    RrStatusHandler handler;
    EXPECT_EQ(false, handler.available());
}

TEST_F(TestStatusManager, shouldReflectUpdates) {
    RrStatusHandler handler;

    _sm.setCycleTime(20);
    _sm.setErrorCount(30);
    _sm.setSensors(MSPSENSOR_FLAGS::BARO | MSPSENSOR_FLAGS::SONAR);

    Event* event = new Event(MSPCOMMANDS_HPP::MSP_STATUS, MSPDIRECTION::EXTERNAL_IN);
    EXPECT_EQ(false, event->hasPayload());
    handler.consume(_sm, event);

    EXPECT_EQ(true, handler.available());
    delete(event);
    event = handler.produce(_sm);

    EXPECT_EQ(true, event->hasPayload());
    msp_status status = event->getPayload<msp_status>();
    EXPECT_EQ(20, status.get_cycletime());
    EXPECT_EQ(30, status.get_i2c_errors_count());
    EXPECT_EQ(MSPSENSOR_FLAGS::BARO, status.get_sensor() &  MSPSENSOR_FLAGS::BARO);
    EXPECT_EQ(MSPSENSOR_FLAGS::SONAR, status.get_sensor() &  MSPSENSOR_FLAGS::SONAR);
    delete(event);
}

 int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}