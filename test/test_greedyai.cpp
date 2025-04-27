#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <rrobot/ai/greedyai.hpp>
#include <string>

using namespace rrobot;


class MockExt : public External {
    public:
    MOCK_METHOD(std::string, recv_rr, (size_t bufsz), (override));
    MOCK_METHOD(int, accept_rr, (), (override));
    MOCK_METHOD(void, close_rr, (), (override));
    MOCK_METHOD(ssize_t, send_rr, (const void* buf, size_t bufsz), (override));
    MOCK_METHOD(int, shutdown_rr, (), (override));

    void init(StateManager&) override {}

    void init_north_0_0() {
        msp_sensor response;
        response.set_acc_avail(0);
        response.set_acc_x(0);
        response.set_acc_y(0);
        response.set_acc_z(0);

        response.set_gyro_avail(0);
        response.set_gyro_x(0);
        response.set_gyro_y(0);
        response.set_gyro_z(0);

        response.set_mag_avail(1);
        response.set_mag_x(1);
        response.set_mag_y(1);
        RmMspSensorCurator curator;
    
        std::string r = curator.serializem(response);
        RmMultiWii m = RmMultiWii::createInstance(r, MSPCOMMANDS::MSP_SENSOR);
        _response = m.encode(Crc32());
        _count = 0;
    }

    ssize_t recv_rr(void* buffer, size_t bufsz) override {
        uint8_t r[] = {_response.at(_count)};
        memcpy(buffer, r, 1);
        _count++;
        return 1;
    }

    ssize_t available() override {
        return 1;
    }

    private:
    int _count = 0;
    std::string _response = "";
};

class TestGreedyAi : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
        _sm.setIsRunning(true);
        _ext.init_north_0_0();
    }

    void TearDown() override {
        // Teardown code
        _sm.setIsRunning(false);
    }


    StateManager _sm;
    MockExt _ext;
};

TEST_F(TestGreedyAi, absDistance) {
    GreedyAi gai(_sm, _ext);

    EXPECT_FLOAT_EQ(4, gai.absDistance(-2, 2));
    EXPECT_FLOAT_EQ(6, gai.absDistance(4, -2));
    EXPECT_FLOAT_EQ(1, gai.absDistance(3, 2));
    EXPECT_FLOAT_EQ(4, gai.absDistance(4, 0));
    EXPECT_FLOAT_EQ(4, gai.absDistance(-8, -4));
    EXPECT_FLOAT_EQ(4, gai.absDistance(-4, -8));
}

TEST_F(TestGreedyAi, isExcluded) {
    msp_delta_xy cdelta;
    GreedyAi gai(_sm, _ext);

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
    GreedyAi gai(_sm, _ext);

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
    GreedyAi gai(_sm, _ext);
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
    GreedyAi gai(_sm, _ext);
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

    _sm.setOrigHeadingFromRadians2(0, 0);
    _sm.setHeadingFromRadians2(0, 0);

    msp_delta_xy d;
    d.set_x(4);
    d.set_y(-4);

    gai.calcPath(d);
}

TEST_F(TestGreedyAi, calcPenalty) {
    GreedyAi gai(_sm, _ext);

    // facing east
    _sm.setOrigHeadingFromRadians2(0, 1);
    _sm.setHeadingFromRadians2(0, -1);

    // rotate 90 degrees (facing south)
    EXPECT_EQ(0.5, gai.computePenalty(90));

    // rotate 180 degrees (facing west)
    EXPECT_EQ(1, gai.computePenalty(180));

    // rotate -90 facing north
    EXPECT_EQ(0.5, gai.computePenalty(-90));

    // no rotation
    EXPECT_EQ(0, gai.computePenalty(0));
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}