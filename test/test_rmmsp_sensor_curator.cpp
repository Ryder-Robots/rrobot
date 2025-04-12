#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <rrobot/protocols/curators/multiwii/rmmsp_sensor_curator.hpp>

using namespace rrobot;

class TestRmMspSensorCurator : public ::testing::Test {
    protected:
     void SetUp() override {
         // Setup code
     }
 
     void TearDown() override {
         // Teardown code
     }
 };

 TEST_F(TestRmMspSensorCurator, shouldCurate) {
    RmMspSensorCurator curator;
    msp_sensor payload;
    payload.set_acc_avail(1);
    payload.set_acc_x(1);
    payload.set_acc_y(2);
    payload.set_acc_z(3);
    payload.set_gyro_avail(5);
    payload.set_gyro_x(8);
    payload.set_gyro_y(13);
    payload.set_gyro_z(21);
    payload.set_mag_avail(34);
    payload.set_mag_x(55);
    payload.set_mag_y(89);
    payload.set_acc_z(144);

    Event* e = new Event(MSPCOMMANDS::MSP_SENSOR, MSPDIRECTION::EXTERNAL_IN, &payload);
    std::string m = curator.serialize(e);

    Event* e2 = curator.deserialize(m);

    ASSERT_EQ(MSPCOMMANDS::MSP_SENSOR, e2->getCommand());
    ASSERT_EQ(true, e2->hasPayload());
    msp_sensor p2 = e2->getPayload<msp_sensor>();
    ASSERT_EQ(p2.get_acc_avail(), 1);

    e->setHasPaylod(false);
    delete(e);
    delete(e2);
 }


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
