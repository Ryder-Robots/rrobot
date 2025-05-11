#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <rrobot/ai/greedyai.hpp>
#include <rrobot/environment/environmentProcessor.hpp>
#include <string>

using namespace rrobot;
namespace fs = std::filesystem;

class MockExt : public External {
   public:
    MOCK_METHOD(std::string, recv_rr, (size_t bufsz), (override));
    MOCK_METHOD(int, accept_rr, (), (override));
    MOCK_METHOD(void, close_rr, (), (override));
    MOCK_METHOD(ssize_t, send_rr, (const void* buf, size_t bufsz), (override));
    MOCK_METHOD(int, shutdown_rr, (), (override));

    void init(StateManager&) override {}

    string init_north_1_0() { return init_delta(1, 0); }

    string init_delta(float x, float y) {
        msp_sensor response;
        response.set_acc_avail(0);
        response.set_acc_x(0);
        response.set_acc_y(0);
        response.set_acc_z(0);

        response.set_gyro_avail(0);
        response.set_gyro_x(0);
        response.set_gyro_y(0);
        response.set_gyro_z(0);

        // magnometer indicates that it is facing north during itialization.
        response.set_mag_avail(1);
        response.set_mag_x(x);
        response.set_mag_y(y);
        response.set_mag_z(0);

        std::string r = _curator.serializem(response);
        RmMultiWii m = RmMultiWii::createInstance(r, MSPCOMMANDS::MSP_SENSOR);
        return m.encode(Crc32());
    }

    string init_sonic_clear() {
        msp_sonar_altitude response;
        response.set_distance(100);
        response.set_temperature(24);
        std::string r = _sonic_curator.serializem(response);
        RmMultiWii m = RmMultiWii::createInstance(r, MSPCOMMANDS::MSP_SONAR_ALTITUDE);
        return m.encode(Crc32());
    }

    string init_sonic_obstacle() {
        msp_sonar_altitude response;
        response.set_distance(50);
        response.set_temperature(24);
        std::string r = _sonic_curator.serializem(response);
        RmMultiWii m = RmMultiWii::createInstance(r, MSPCOMMANDS::MSP_SONAR_ALTITUDE);
        return m.encode(Crc32());
    }

    void init() {
        _count = 0;
        _response = init_sonic_clear() + init_north_1_0();
    }

    ssize_t recv_rr(void* buffer, size_t bufsz) override {
        uint8_t r[] = {_response.at(_count)};
        memcpy(buffer, r, bufsz);
        _count++;
        return bufsz;
    }

    ssize_t available() override { return 1; }
    std::string _response = "";

   private:
    int _count = 0;
    RmMspSensorCurator _curator;
    RmMspSonicCurator _sonic_curator;
};

class TestGreedyAi : public ::testing::Test {
   protected:
    void SetUp() override {
        // Setup code
        _sm.setIsRunning(true);
        _ext.init();

        const fs::path filepath = "manifests/virtual.json";
        std::ifstream ifs(filepath);
        _manifest = json::parse(ifs);
        ifs.close();
    }

    void TearDown() override {
        // Teardown code
        _sm.setIsRunning(false);
    }

    StateManager _sm;
    MockExt _ext;
    json _manifest;
};

TEST_F(TestGreedyAi, DISABLED_traversePath1) {
    _ext._response = _ext.init_sonic_clear() + _ext.init_north_1_0() + _ext.init_sonic_clear() + _ext.init_north_1_0() +
                     _ext.init_sonic_obstacle() + _ext.init_north_1_0() + _ext.init_sonic_clear() +
                     _ext.init_delta(0, 1) + _ext.init_sonic_clear() + _ext.init_delta(0, 1) + _ext.init_sonic_clear() +
                     _ext.init_delta(0, 1) + _ext.init_sonic_clear() + _ext.init_delta(0, 1);

    GreedyAi gai(_sm, _ext, EnviromentProcessor::createEnvironment(_manifest));
    _sm.setOrigHeadingFromRadians2(0, 0);
    _sm.setHeadingFromRadians2(0, 0);

    msp_delta_xy d;
    d.set_x(4);
    d.set_y(-4);

    gai.calcPath(d);
}

// Straight line to destination.
TEST_F(TestGreedyAi, traversePath2) {
    _ext._response = _ext.init_sonic_clear() + _ext.init_north_1_0();
    GreedyAi gai(_sm, _ext, EnviromentProcessor::createEnvironment(_manifest));
    _sm.setOrigHeadingFromRadians2(0, 0);
    _sm.setHeadingFromRadians2(0, 0);

    msp_delta_xy d;
    d.set_x(0);
    d.set_y(7);

    // Give ultra sonic clear view ahead
    _ext._response = _ext.init_sonic_clear() + _ext.init_north_1_0() + 
        _ext.init_sonic_clear() + _ext.init_north_1_0() + 
        _ext.init_sonic_clear() + _ext.init_north_1_0() +
        _ext.init_sonic_clear() + _ext.init_north_1_0() +
        _ext.init_sonic_clear() + _ext.init_north_1_0() +
        _ext.init_sonic_clear() + _ext.init_north_1_0() +
        _ext.init_sonic_clear() + _ext.init_north_1_0() +
        _ext.init_sonic_clear() + _ext.init_north_1_0() +
        _ext.init_sonic_clear() + _ext.init_north_1_0();
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.calcPath(d)); 
}

// One turn right to destination
TEST_F(TestGreedyAi, traversePath3) {
    _ext._response = _ext.init_sonic_clear() + _ext.init_north_1_0();
    GreedyAi gai(_sm, _ext, EnviromentProcessor::createEnvironment(_manifest));
    _sm.setOrigHeadingFromRadians2(0, 0);
    _sm.setHeadingFromRadians2(0, 0);

    msp_delta_xy d;
    d.set_x(1);
    d.set_y(7);

    // Give ultra sonic clear view ahead
    _ext._response = _ext.init_sonic_clear() + _ext.init_north_1_0() + _ext.init_sonic_clear() + _ext.init_north_1_0() + 
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 1
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 2
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 3
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 4
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 5
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 6
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 7
        _ext.init_sonic_clear() + _ext.init_north_1_0() + _ext.init_delta(0,1);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.calcPath(d)); 
}


// One turn left to destination
TEST_F(TestGreedyAi, traversePath4) {
    _ext._response = _ext.init_sonic_clear() + _ext.init_north_1_0();
    GreedyAi gai(_sm, _ext, EnviromentProcessor::createEnvironment(_manifest));
    _sm.setOrigHeadingFromRadians2(0, 0);
    _sm.setHeadingFromRadians2(0, 0);

    msp_delta_xy d;
    d.set_x(-1);
    d.set_y(7);

    // Give ultra sonic clear view ahead
    _ext._response = _ext.init_sonic_clear() + _ext.init_north_1_0() + _ext.init_sonic_clear() + _ext.init_north_1_0() + 
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 1
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 2
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 3
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 4
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 5
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 6
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 7
        _ext.init_sonic_clear() + _ext.init_north_1_0() + _ext.init_delta(0,-1);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.calcPath(d)); 
}

// Two right positive, otherwise all negative
TEST_F(TestGreedyAi, traversePath5) {
    _ext._response = _ext.init_sonic_clear() + _ext.init_north_1_0();
    GreedyAi gai(_sm, _ext, EnviromentProcessor::createEnvironment(_manifest));
    _sm.setOrigHeadingFromRadians2(0, 0);
    _sm.setHeadingFromRadians2(0, 0);

    msp_delta_xy d;
    d.set_x(2);
    d.set_y(-7);

    // Give ultra sonic clear view ahead
    _ext._response = _ext.init_sonic_clear() + _ext.init_north_1_0() + _ext.init_sonic_clear() + _ext.init_north_1_0() + 
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 1
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 2
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 3
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 4
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 5
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 6
        _ext.init_sonic_clear() + _ext.init_north_1_0() + // 7
        _ext.init_sonic_clear() + _ext.init_north_1_0() + _ext.init_delta(0,-1);
    EXPECT_EQ(PSTATE::P_AVAILABLE, gai.calcPath(d)); 
}

// --- test below will need to be moved to be kept, ones above can move to imlemntation class.

TEST_F(TestGreedyAi, absDistance) {
    GreedyAi gai(_sm, _ext, EnviromentProcessor::createEnvironment(_manifest));

    EXPECT_FLOAT_EQ(4, gai.absDistance(-2, 2));
    EXPECT_FLOAT_EQ(6, gai.absDistance(4, -2));
    EXPECT_FLOAT_EQ(1, gai.absDistance(3, 2));
    EXPECT_FLOAT_EQ(4, gai.absDistance(4, 0));
    EXPECT_FLOAT_EQ(4, gai.absDistance(-8, -4));
    EXPECT_FLOAT_EQ(4, gai.absDistance(-4, -8));
}

TEST_F(TestGreedyAi, isExcluded) {
    msp_delta_xy cdelta;
    GreedyAi gai(_sm, _ext, EnviromentProcessor::createEnvironment(_manifest));

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
    GreedyAi gai(_sm, _ext, EnviromentProcessor::createEnvironment(_manifest));

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
    GreedyAi gai(_sm, _ext, EnviromentProcessor::createEnvironment(_manifest));
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

TEST_F(TestGreedyAi, calcPenalty) {
    GreedyAi gai(_sm, _ext, EnviromentProcessor::createEnvironment(_manifest));

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

TEST_F(TestGreedyAi, offset) {
    GreedyAi gai(_sm, _ext, EnviromentProcessor::createEnvironment(_manifest));
    _sm.setOrigHeadingFromRadians2(1, 0);

    float x = 0, y = 0;

    // facing North
    _sm.setHeadingFromRadians2(1, 0);
    gai.offset(_sm.getHeading(), &x, &y);
    EXPECT_EQ(0, x);
    EXPECT_EQ(1, y);

    // facing East
    _sm.setHeadingFromRadians2(0, 1);
    gai.offset(_sm.getHeading(), &x, &y);
    EXPECT_EQ(1, x);
    EXPECT_EQ(0, y);

    // facing south so foward should be y - 1
    _sm.setHeadingFromRadians2(-1, 0);
    gai.offset(_sm.getHeading(), &x, &y);
    EXPECT_EQ(0, x);
    EXPECT_EQ(-1, y);

    // facing west
    _sm.setHeadingFromRadians2(0, -1);
    gai.offset(_sm.getHeading(), &x, &y);
    EXPECT_EQ(0, x);
    EXPECT_EQ(-1, y);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}