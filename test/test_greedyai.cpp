#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rrobot/ai/greedyai.hpp>
#include <rrobot/environment/environmentProcessor.hpp>
#include <string>

using namespace rrobot;
namespace fs = std::filesystem;

class TestEncoder : public ::testing::Test {
    protected:
     void SetUp() override {
         // Setup code
     }
 
     void TearDown() override {
         // Teardown code
     }
};

TEST_F(TestEncoder, simpleTransversal) {
    GreedyAi gai;
    msp_delta_xy d;
    d.set_x(0);
    d.set_y(4);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(d));
}

TEST_F(TestEncoder, simpleTransversalNorthEast) {
    GreedyAi gai;
    msp_delta_xy d;
    d.set_x(3);
    d.set_y(3);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(d));
}

TEST_F(TestEncoder, simpleTransversalNorthWest) {
    GreedyAi gai;
    msp_delta_xy d;
    d.set_x(-2);
    d.set_y(5);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(d));
}

TEST_F(TestEncoder, simpleTransversalNeg) {
    GreedyAi gai;
    msp_delta_xy d;
    d.set_x(-2);
    d.set_y(-3);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(d));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}