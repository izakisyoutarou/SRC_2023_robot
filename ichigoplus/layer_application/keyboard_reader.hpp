#pragma once

/*******************************************
 * KeyboardReader ver1.0 2020/02/25
 * This program read input of pc by bluetooth
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver2.0 2021/10/03 Improved processing speed.
 * ver1.0 2020/02/25 The first version
 *
 ******************************************/

#include <array>
#include "ichigoplus/layer_driver/base/serial.hpp"

class KeyboardReader : SerialHandler {
public:
    KeyboardReader(Serial &serial) { serialHandlerSetup(&serial); }
    int setup(int baudrate) { return serialSetup(baudrate) != 0; }
    void cycle();
    bool operator()(unsigned char key) { return (*read_buf)[key]; }
    bool pcbtn(unsigned char key) { return operator()(key); }
private:
    void serialReadChar(char value) override { (*write_buf)[(unsigned char)value] = true; }
    std::array<bool, 256> buf0 = {false};
    std::array<bool, 256> buf1 = {false};
    std::array<bool, 256> *read_buf = &buf0;
    std::array<bool, 256> *write_buf = &buf1;
};