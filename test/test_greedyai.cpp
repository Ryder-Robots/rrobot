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

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}