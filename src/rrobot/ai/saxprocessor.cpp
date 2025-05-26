#include <rrobot/ai/saxprocessor.hpp>

/*
 & TODO: - Add railguards around setup, to ensure that file is open.
 *       - Add features
 *       - create global function that adds vectors
 */

using namespace rrobot;

void saxprocessor::setup(string fname, bool isinput) {
    _fname = fname;
    set_isinput(isinput);
}

void saxprocessor::teardown() {
    if (_fin.is_open()) {
        _fin.close();
    }
    if (_fout.is_open()) {
        _fout.close();
    }
}

void saxprocessor::set_isinput(bool isinput) {
    if (isinput) {
        if (_fout.is_open()) {
            _fout.close();
        }

        _isinput = true;
        _fin = std::ifstream(_fname);
        _buffer << _fin.rdbuf();
        _sin =  std::istringstream(_buffer.str());
    } else {
        if (_fin.is_open()) {
            _fin.close();
        }
        _fout = std::ofstream(_fname, std::ios::binary);
    }
}

void saxprocessor::serialize(const sax_el el) {
    dlib::serialize(el.tid, _fout);
    dlib::serialize(el.v, _fout);
    dlib::serialize(el.cp, _fout);
    dlib::serialize(el.dp, _fout);
    dlib::serialize(el.acc, _fout);
    dlib::serialize(el.mag, _fout);
    dlib::serialize(el.gyro, _fout);
}

sax_el saxprocessor::deserialize() {
    sax_el el;
    dlib::deserialize(el.tid, _sin);
    dlib::deserialize(el.v, _sin);
    dlib::deserialize(el.cp, _sin);
    dlib::deserialize(el.dp, _sin);
    dlib::deserialize(el.acc, _sin);
    dlib::deserialize(el.mag, _sin);
    dlib::deserialize(el.gyro, _sin);
    return el;
}