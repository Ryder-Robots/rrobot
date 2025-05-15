#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rrobot/ai/greedyai.hpp>
#include <rrobot/environment/environmentProcessor.hpp>
#include <string>

using namespace rrobot;
namespace fs = std::filesystem;

class TestGreedyAi : public ::testing::Test {
   protected:
    void SetUp() override {
        // reset current point back to <0,0,0>
        _sm.setCp(dlib::vector<float, VECTOR_DIM>(0, 0, 0));
    }

    void TearDown() override {
        // Teardown code
    }

    StateManager _sm;
};

// Following tests check for 2D space only.
TEST_F(TestGreedyAi, simpleTransversal) {
    GreedyAi gai(_sm);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(dlib::vector<float, VECTOR_DIM>(0, 4, 0)));
}

TEST_F(TestGreedyAi, simpleTransversalNorthEast) {
    GreedyAi gai(_sm);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(dlib::vector<float, VECTOR_DIM>(3, 3, 0)));
}

TEST_F(TestGreedyAi, simpleTransversalNorthWest) {
    GreedyAi gai(_sm);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(dlib::vector<float, VECTOR_DIM>(-2, 5, 0)));
}

TEST_F(TestGreedyAi, simpleTransversalNeg) {
    GreedyAi gai(_sm);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(dlib::vector<float, VECTOR_DIM>(-2, -3, 0)));
}

TEST_F(TestGreedyAi, simpleTransversalPos1_1) {
    _sm.setCp(dlib::vector<float, VECTOR_DIM>(-6, -5, 0));
    GreedyAi gai(_sm);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(dlib::vector<float, VECTOR_DIM>(-2, -3, 0)));
}


TEST_F(TestGreedyAi, simpleTransversalMulti1) {
    GreedyAi gai(_sm);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(dlib::vector<float, VECTOR_DIM>(0, 4, 0)));
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(dlib::vector<float, VECTOR_DIM>(0, 0, 0)));
}

TEST_F(TestGreedyAi, simpleTransversalMulti2) {
    GreedyAi gai(_sm);
    _sm.setCp(dlib::vector<float, VECTOR_DIM>(0, 4, 0));
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.transverse(dlib::vector<float, VECTOR_DIM>(0, 0, 0)));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}