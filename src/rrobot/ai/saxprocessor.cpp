#include <rrobot/ai/saxprocessor.hpp>

/*
 & TODO: - Add railguards around setup, to ensure that file is open.
 *       - Add features
 *       - create global function that adds vectors
 */

using namespace rrobot;

void saxprocessor::setup(string fname) {
    _fout = std::ofstream(fname, std::ios::binary);
}

void saxprocessor::teardown() {
    _fout.close();
}

void saxprocessor::serialize(const sax_el el) {
    dlib::serialize(el.tid, _fout);
    dlib::serialize(el.v, _fout);
    dlib::serialize(el.cp, _fout);
    dlib::serialize(el.dp, _fout);
    dlib::serialize(_sm.getFeatures().get_sensor_acc().get_data(), _fout);
}

sax_el saxprocessor::deserialize() {
    sax_el el;
    
    return el;
}