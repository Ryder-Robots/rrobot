#include <gtest/gtest.h>
#include <rrobot/ai/greedyai.hpp>

using namespace rrobot;

class TestGreedyAi : public ::testing::Test {
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

 TEST_F(TestGreedyAi, absDistance) {
    GreedyAi gai(_sm);

    EXPECT_FLOAT_EQ(4, gai.absDistance(-2, 2));
    EXPECT_FLOAT_EQ(6, gai.absDistance(4, -2));
    EXPECT_FLOAT_EQ(1, gai.absDistance(3, 2));
    EXPECT_FLOAT_EQ(4, gai.absDistance(4, 0));
    EXPECT_FLOAT_EQ(4, gai.absDistance(-8, -4));
    EXPECT_FLOAT_EQ(4, gai.absDistance(-4, -8));
 }

 TEST_F(TestGreedyAi, isExcluded) {
    msp_delta_xy cdelta;
    GreedyAi gai(_sm);

    msp_delta_xy payload1;
    payload1.set_x(0);
    payload1.set_y(1);
    Event* e1 = new Event(MSPCOMMANDS::MSP_DELTA_XY, MSPDIRECTION::EXTERNAL_OUT, &payload1);
    gai._excluded.push_back(e1);

    msp_delta_xy payload2;
    payload2.set_x(0);
    payload2.set_y(-1);
    Event* e2 = new Event(MSPCOMMANDS::MSP_DELTA_XY, MSPDIRECTION::EXTERNAL_OUT, &payload2);
    gai._excluded.push_back(e2);

    msp_delta_xy t;
    t.set_x(0);
    t.set_y(1);
    EXPECT_TRUE(gai.isExcluded(t));

    t.set_x(0);
    t.set_y(-1);
    EXPECT_TRUE(gai.isExcluded(t));

    t.set_x(-1);
    t.set_y(0);
    EXPECT_FALSE(gai.isExcluded(t));

    e1->setHasPaylod(false);
    e2->setHasPaylod(false);
    delete(e1);
    delete(e2);
 }

 TEST_F(TestGreedyAi, isExplored) {
    GreedyAi gai(_sm);

    msp_delta_xy payload1;
    payload1.set_x(0);
    payload1.set_y(1);
    Event* e1 = new Event(MSPCOMMANDS::MSP_DELTA_XY, MSPDIRECTION::EXTERNAL_OUT, &payload1);
    gai._explored.push_back(e1);

    msp_delta_xy payload2;
    payload2.set_x(0);
    payload2.set_y(-1);
    Event* e2 = new Event(MSPCOMMANDS::MSP_DELTA_XY, MSPDIRECTION::EXTERNAL_OUT, &payload2);
    gai._explored.push_back(e2);

    msp_delta_xy t;
    t.set_x(0);
    t.set_y(1);
    EXPECT_TRUE(gai.isExplored(t));

    t.set_x(0);
    t.set_y(-1);
    EXPECT_TRUE(gai.isExplored(t));

    t.set_x(-1);
    t.set_y(0);
    EXPECT_FALSE(gai.isExplored(t));

    e1->setHasPaylod(false);
    e2->setHasPaylod(false);
    delete(e1);
    delete(e2);
 }

 TEST_F(TestGreedyAi, isValid) {
    GreedyAi gai(_sm);
    msp_delta_xy ex, ep1, ep2;
    // up excluded
    ex.set_x(0);
    ex.set_y(1);

    // left
    ep1.set_x(-1);
    ep1.set_y(0);

    // right
    ep2.set_x(1);
    ep2.set_y(0);

    // valid = 0, -1
    Event *evx  = new Event(MSPCOMMANDS::MSP_DELTA_XY, MSPDIRECTION::EXTERNAL_OUT, &ex), 
          *evp1 = new Event(MSPCOMMANDS::MSP_DELTA_XY, MSPDIRECTION::EXTERNAL_OUT, &ep1), 
          *evp2 = new Event(MSPCOMMANDS::MSP_DELTA_XY, MSPDIRECTION::EXTERNAL_OUT, &ep2);
    
    gai._explored.push_back(evx);
    gai._excluded.push_back(evp1);
    gai._excluded.push_back(evp2);

    msp_delta_xy c = _sm.getCurrentDelta();
    msp_delta_xy nc;

    // move up
    EXPECT_FALSE(gai.isValid(c.get_x(), c.get_y() + 1));

    // move right
    EXPECT_FALSE(gai.isValid(c.get_x() + 1, c.get_y()));

    // move down
    EXPECT_TRUE(gai.isValid(c.get_x(), c.get_y() - 1));

    // move right
    EXPECT_FALSE(gai.isValid(c.get_x() - 1, c.get_y()));


    evx->setHasPaylod(false);
    evp1->setHasPaylod(false);
    evp2->setHasPaylod(false);

    delete(evx);
    delete(evp1);
    delete(evp2);
 }

 

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}