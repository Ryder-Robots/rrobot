#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdint.h>
#include <rrobot/environment/environmentProcessor.hpp>
#include <rrobot/state/state_manager.hpp>

using namespace rrobot;

class TestStateManager : public ::testing::Test {
    protected:
     void SetUp() override {
         // Setup code
     }
 
     void TearDown() override {
         // Teardown code
     }

     StateManager sm;
 };

TEST_F(TestStateManager, TestIsRunning) {
    EXPECT_EQ(true, sm.isRunning());
    sm.setIsRunning(false);
    EXPECT_EQ(false, sm.isRunning());
}

TEST_F(TestStateManager, TestMode) {
    EXPECT_EQ(RR_CMODES::CMODE_NOT_SET, sm.getMode());

    sm.setMode(RR_CMODES::CMODE_MANUAL_FLIGHT);
    EXPECT_EQ(RR_CMODES::CMODE_MANUAL_FLIGHT, sm.getMode());
    sm.setMode(RR_CMODES::CMODE_NOT_SET);
    EXPECT_EQ(RR_CMODES::CMODE_NOT_SET, sm.getMode());
}

TEST_F(TestStateManager, TestStatus) {
    sm.setMode(CMODE_MANUAL_FLIGHT);
    sm.resetStatus();
    sm.setStatus(RRP_STATUS::ACTIVE);
    sm.setStatus(RRP_STATUS::ACTIVE);
    sm.setStatus(RRP_STATUS::ACTIVE);
    EXPECT_EQ(RRP_STATUS::ACTIVE, sm.getStatus());

    sm.resetStatus();
    sm.setStatus(RRP_STATUS::TERMINATED);
    sm.setStatus(RRP_STATUS::ERROR);
    sm.setStatus(RRP_STATUS::ACTIVE);
    EXPECT_EQ(RRP_STATUS::TERMINATED, sm.getStatus());

    sm.resetStatus();
    sm.setStatus(RRP_STATUS::ERROR);
    sm.setStatus(RRP_STATUS::RELOADING);
    sm.setStatus(RRP_STATUS::ACTIVE);
    EXPECT_EQ(RRP_STATUS::ERROR, sm.getStatus());
    EXPECT_EQ(CMODE_MANUAL_FLIGHT, sm.getMode());

    sm.setMode(CMODE_NOT_SET);
    EXPECT_EQ(CMODE_NOT_SET, sm.getMode());
}

TEST_F(TestStateManager, TestBitMaps) {
    EXPECT_EQ(1, INITILIZING);
    EXPECT_EQ(2, ACTIVE);
    EXPECT_EQ(4, ERROR);
    EXPECT_EQ(8, RELOADING);
    EXPECT_EQ(16, SHUTTING_DOWN);
    EXPECT_EQ(32, TERMINATED);

    //256, 512, 1024, 2048, 4096, 8192, 16348, 32768
    EXPECT_EQ(256, CMODE_NOT_SET);
    EXPECT_EQ(512, CMODE_MANUAL_FLIGHT);

    EXPECT_EQ(0, (RRP_STATUS::ACTIVE & CMODE_NOT_SET));
    
    int32_t flags = RRP_STATUS::ACTIVE + CMODE_NOT_SET;
    
    EXPECT_EQ(RRP_STATUS::ACTIVE,  STATUS_BITMASK(flags));

    flags = 0 | RRP_STATUS::ACTIVE  + RRP_STATUS::ERROR + CMODE_NOT_SET;
    EXPECT_EQ(RRP_STATUS::ACTIVE + RRP_STATUS::ERROR,  STATUS_BITMASK(flags));


    EXPECT_EQ(0, MODE_BITMASK(0));
    int32_t test = CMODE_NOT_SET + INITILIZING;
    EXPECT_EQ(CMODE_NOT_SET, MODE_BITMASK(test));
    test = CMODE_MANUAL_FLIGHT + INITILIZING + SHUTTING_DOWN + ACTIVE;
    EXPECT_EQ(CMODE_MANUAL_FLIGHT, MODE_BITMASK(test));
    test = ACTIVE + INITILIZING + CMODE_MANUAL_FLIGHT;
    EXPECT_EQ(ACTIVE + INITILIZING, STATUS_BITMASK(test)); 
}

TEST_F(TestStateManager, TestHeadingsFromRadians) {
    // north
    sm.setHeadingFromRadians2(0, 0);
    float degrees = sm.getHeading();
    EXPECT_FLOAT_EQ(0, degrees);

    // Northeast
    sm.setHeadingFromRadians2(0.7071, 0.7071);
    degrees = sm.getHeading();
    EXPECT_FLOAT_EQ(45, degrees);

    // East
    sm.setHeadingFromRadians2(0, 1);
    degrees = sm.getHeading();
    EXPECT_FLOAT_EQ(90, degrees);

    // South
    sm.setHeadingFromRadians2(-1, 0);
    degrees = sm.getHeading();
    EXPECT_FLOAT_EQ(180, degrees);

    // West (note that this can also be 270, but -90 is more correct)
    sm.setHeadingFromRadians2(0, -1);
    degrees = sm.getHeading();
    EXPECT_FLOAT_EQ(-90, degrees);
}

// TEST_F(TestStateManager, getHeadingRadians) {

//     // North
//     float x, y;
//     sm.setHeadingFromRadians2(0, 0);
//     sm.getHeadingRadians(&x, &y);
//     EXPECT_FLOAT_EQ(0, x);
//     EXPECT_FLOAT_EQ(0, y);

//     // Northeast
//     sm.setHeadingFromRadians2(-1, 0);
//     sm.getHeadingRadians(&x, &y);

//     EXPECT_EQ(round(x), -1);
//     EXPECT_EQ(round(y), 0);
// }

TEST_F(TestStateManager, TestRotate) {
    // set to north first
    sm.setHeadingFromRadians2(0, 0);
    float degrees = sm.getHeading();
    EXPECT_FLOAT_EQ(0, degrees);

    // Rotate to the east.
    float x, y;
    sm.rotate(90, &x, &y);
    EXPECT_FLOAT_EQ(0, round(x));
    EXPECT_FLOAT_EQ(1, round(y));
    sm.setHeadingFromRadians2(x, y);

    // rotate south
    sm.rotate(90, &x, &y);
    EXPECT_FLOAT_EQ(-1, round(x));
    EXPECT_FLOAT_EQ(0, round(y));

    // Change heading by reversing 90 degrees
    sm.setHeadingFromRadians2(0, 0);
    sm.rotate(-90, &x, &y);
    EXPECT_FLOAT_EQ(0, round(x));
    EXPECT_FLOAT_EQ(-1, round(y));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}