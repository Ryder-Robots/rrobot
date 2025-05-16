#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <rrobot/protocols/curators/multiwii/rmmsp_sensor_mag_curator.hpp>

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
    RmMspSensorMagCurator curator;
    msp_sensor_mag payload;
    payload.set_available(1);
    payload.set_data(dlib::vector<float, 3>(0,0,0));

    Event* e = new Event(MSPCOMMANDS::MSP_SENSOR_MAG, MSPDIRECTION::EXTERNAL_IN, &payload);
    std::string m = curator.serialize(e);

    Event* e2 = curator.deserialize(m);

    ASSERT_EQ(MSPCOMMANDS::MSP_SENSOR_MAG, e2->getCommand());
    ASSERT_EQ(true, e2->hasPayload());
    msp_sensor_mag p2 = e2->getPayload<msp_sensor_mag>();
    ASSERT_EQ(p2.get_available(), 1);

    e->setHasPaylod(false);
    delete(e);
    delete(e2);
 }


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
