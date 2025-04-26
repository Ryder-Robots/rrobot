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
    gai._excluded.push_back(payload1);

    msp_delta_xy payload2;
    payload2.set_x(0);
    payload2.set_y(-1);
    gai._excluded.push_back(payload2);

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
}

TEST_F(TestGreedyAi, isExplored) {
    GreedyAi gai(_sm);

    msp_delta_xy payload1;
    payload1.set_x(0);
    payload1.set_y(1);
    gai._explored.push_back(payload1);

    msp_delta_xy payload2;
    payload2.set_x(0);
    payload2.set_y(-1);
    gai._explored.push_back(payload2);

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

    gai._explored.push_back(ex);
    gai._excluded.push_back(ep1);
    gai._excluded.push_back(ep2);

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

}

TEST_F(TestGreedyAi, traversePath) {
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

    gai._explored.push_back(ex);
    gai._excluded.push_back(ep1);
    gai._excluded.push_back(ep2);

    //gai.calcPath(path);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}