#pragma once

#include "ichigoplus/layer_application/cycle_once_checker.hpp"
#include "ichigoplus/layer_driver/base/serial.hpp"
#include "ichigoplus/layer_driver/base/can.hpp"
#include "ichigoplus/layer_driver/circuit/can_id.hpp"
#include "ichigoplus/layer_driver/base/encoder.hpp"

namespace encoder{
class AMTAbsoluteEncoder :public SerialHandler,public CanHandler,public Encoder{
public:
    enum class Resolution{
    bit12,
    bit14
    };
    AMTAbsoluteEncoder(Serial &serial, Resolution resolution=Resolution::bit14);
    AMTAbsoluteEncoder(Can &can, can::CanId node_id, Resolution resolution=Resolution::bit14);
    int setup() override;
    void cycle() override;
    void rev(bool rev) override {is_reverse=rev;}
    float radian() const override {return radian_;}
    uint16_t count() const {return count_;}

    void setCountResolution(Resolution resolution);
    void print();

    void serialReadChar(char data) override;

    int canRead(int id,int number,unsigned char data[8]) override;
    int canId(int id) override {return can_id==id;};

private:
    cycle_once_checker::CycleOnceChecker cycleOnceChecker;

    enum class RunMode { null, serial, can } run_mode = RunMode::null;

    //定数
    const can::CanId can_id;
    const int address = 0x00;
    const int baudrate = 38400;

    //フィールド
    Resolution resolution;
    int count_resolution = 0;
    bool is_reverse = false;
    uint16_t observed_count = 0;
    uint16_t count_ = 0;

    bool is_received = false;
    bool is_receiving_first_data = true;


    float radian_ = 0.f;
};
}
