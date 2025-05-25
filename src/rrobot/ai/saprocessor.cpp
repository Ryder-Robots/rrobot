#include <rrobot/ai/saxprocessor.hpp>

/*
 & TODO: Add railguards around setup, to ensure that file is open.
 */

using namespace rrobot;

void saxprocessor::setup(string fname) {
    _fout = std::ofstream(fname, std::ios::binary);
}

void saxprocessor::teardown() {
    _fout.close();
}

void serialize(saxel el) {

}