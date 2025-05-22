#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <climits>

#include <rrobot/ai/ble33iface.hpp>
#include <sstream>

using namespace rrobot;

class MockExternal : public External {
   public:
    MOCK_METHOD(ssize_t, recv_rr, (void* buffer, size_t bufsz), (override));
    MOCK_METHOD(string, recv_rr, (size_t bufsz), (override));
    MOCK_METHOD(int, accept_rr, (), (override));
    MOCK_METHOD(void, close_rr, (), (override));
    MOCK_METHOD(ssize_t, send_rr, (const void* buffer, size_t bufsz), (override));
    MOCK_METHOD(ssize_t, send_rr, (std::string buf), (override));
    MOCK_METHOD(ssize_t, available, (), (override));
    MOCK_METHOD(int, shutdown_rr, (), (override));
    MOCK_METHOD(std::string, get,  (const uint8_t tc, const ssize_t sz), (override));
};

class TestBle33Iface : public ::testing::Test {
   protected:
    void SetUp() override {}

    void TearDown() override {
        // Teardown code
    }
    MockExternal _ext;
    RmMspSonicCurator _curator_sonic;
    Crc32 _crc;
};

TEST_F(TestBle33Iface, TestSonar) {
    msp_sonar_altitude in;
    in.set_distance(20);
    in.set_temperature(24);
    std::string result = _curator_sonic.serializem(in),
                m = RmMultiWii::createInstance(result, MSPCOMMANDS::MSP_SONAR_ALTITUDE).encode(_crc);

    EXPECT_CALL(_ext, available()).WillOnce(::testing::Return(result.size()));
    EXPECT_CALL(_ext, send_rr(RmMultiWii::createInstance("", MSPCOMMANDS::MSP_SONAR_ALTITUDE).encode(_crc)))
        .WillOnce(::testing::Return(result.size()));
    EXPECT_CALL(_ext, get(RmMultiWii::_TERMINATION_CHAR, LLONG_MAX)).WillOnce(::testing::Return(m));

    ble33iface _iface(_ext);
    msp_sonar_altitude sonar = _iface.sen_sonar();

    EXPECT_EQ(20, sonar.get_distance());
    EXPECT_EQ(24, sonar.get_temperature());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}