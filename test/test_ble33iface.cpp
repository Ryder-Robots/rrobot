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
    MOCK_METHOD(std::string, get, (const uint8_t tc, const ssize_t sz), (override));
};

class TestBle33Iface : public ::testing::Test {
   protected:
    void SetUp() override {}

    void TearDown() override {
        // Teardown code
    }
    MockExternal _ext;
    RmMspSonicCurator _curator_sonic;
    RmMspSensorGyroCurator _curator_gyro;
    RmMspSensorAccCurator _curator_acc;
    RmMspSensorMagCurator _curator_mag;
    Crc32 _crc;
    StateManager _sm;
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

    ble33iface _iface(_ext, _sm);
    msp_sonar_altitude sonar = _iface.sen_sonar();

    EXPECT_EQ(20, sonar.get_distance());
    EXPECT_EQ(24, sonar.get_temperature());

    EXPECT_EQ(20, _sm.getFeatures().getSonar().get_distance());
    EXPECT_EQ(24, _sm.getFeatures().getSonar().get_temperature());
}

TEST_F(TestBle33Iface, TestGyro) {
    msp_sensor_gyro in;
    in.set_available(1);
    in.set_data(dlib::vector<float, 3L>(10, 20, 0));

    std::string result = _curator_gyro.serializem(in),
                m = RmMultiWii::createInstance(result, MSPCOMMANDS::MSP_SENSOR_GYRO).encode(_crc);

    EXPECT_CALL(_ext, available()).WillOnce(::testing::Return(result.size()));
    EXPECT_CALL(_ext, send_rr(RmMultiWii::createInstance("", MSPCOMMANDS::MSP_SENSOR_GYRO).encode(_crc)))
        .WillOnce(::testing::Return(result.size()));
    EXPECT_CALL(_ext, get(RmMultiWii::_TERMINATION_CHAR, LLONG_MAX)).WillOnce(::testing::Return(m));

    ble33iface _iface(_ext, _sm);
    msp_sensor_gyro sensor = _iface.sen_gyro();

    EXPECT_EQ(10, sensor.get_data().x());
    EXPECT_EQ(20, sensor.get_data().y());
    EXPECT_EQ(0, sensor.get_data().z());

    EXPECT_EQ(10, _sm.getFeatures().get_sensor_gyro().get_data().x());
    EXPECT_EQ(20, _sm.getFeatures().get_sensor_gyro().get_data().y());
    EXPECT_EQ(0, _sm.getFeatures().get_sensor_gyro().get_data().z());
}

TEST_F(TestBle33Iface, TestAcc) {
    msp_sensor_acc in;
    in.set_available(1);
    in.set_data(dlib::vector<float, 3L>(12, 22, 0));

    std::string result = _curator_acc.serializem(in),
                m = RmMultiWii::createInstance(result, MSPCOMMANDS::MSP_SENSOR_ACC).encode(_crc);

    EXPECT_CALL(_ext, available()).WillOnce(::testing::Return(result.size()));
    EXPECT_CALL(_ext, send_rr(RmMultiWii::createInstance("", MSPCOMMANDS::MSP_SENSOR_ACC).encode(_crc)))
        .WillOnce(::testing::Return(result.size()));
    EXPECT_CALL(_ext, get(RmMultiWii::_TERMINATION_CHAR, LLONG_MAX)).WillOnce(::testing::Return(m));

    ble33iface _iface(_ext, _sm);
    msp_sensor_acc sensor = _iface.sen_acc();

    EXPECT_EQ(12, sensor.get_data().x());
    EXPECT_EQ(22, sensor.get_data().y());
    EXPECT_EQ(0, sensor.get_data().z());

    EXPECT_EQ(12, _sm.getFeatures().get_sensor_acc().get_data().x());
    EXPECT_EQ(22, _sm.getFeatures().get_sensor_acc().get_data().y());
    EXPECT_EQ(0, _sm.getFeatures().get_sensor_acc().get_data().z());
}


TEST_F(TestBle33Iface, TestMag) {
    msp_sensor_mag in;
    in.set_available(1);
    in.set_data(dlib::vector<float, 3L>(12, 25, 0));

    std::string result = _curator_mag.serializem(in),
                m = RmMultiWii::createInstance(result, MSPCOMMANDS::MSP_SENSOR_MAG).encode(_crc);

    EXPECT_CALL(_ext, available()).WillOnce(::testing::Return(result.size()));
    EXPECT_CALL(_ext, send_rr(RmMultiWii::createInstance("", MSPCOMMANDS::MSP_SENSOR_MAG).encode(_crc)))
        .WillOnce(::testing::Return(result.size()));
    EXPECT_CALL(_ext, get(RmMultiWii::_TERMINATION_CHAR, LLONG_MAX)).WillOnce(::testing::Return(m));

    ble33iface _iface(_ext, _sm);
    msp_sensor_mag sensor = _iface.sen_mag();

    EXPECT_EQ(12, sensor.get_data().x());
    EXPECT_EQ(25, sensor.get_data().y());
    EXPECT_EQ(0, sensor.get_data().z());

    EXPECT_EQ(12, _sm.getFeatures().get_sensor_mag().get_data().x());
    EXPECT_EQ(25, _sm.getFeatures().get_sensor_mag().get_data().y());
    EXPECT_EQ(0, _sm.getFeatures().get_sensor_mag().get_data().z());
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}