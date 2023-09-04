#pragma once

/*******************************************
 * motor_pos_vel_controller ver1.2 2021/01/16
 * This program is interface for controlling motor.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.2 2021/01/17 add constructor, destructor
 * ver1.1 2020/12/17 add stop(), restart()
 * ver1.0 2020/12/8  The first version
 ******************************************/

class MotorPosVelController {
public:
    MotorPosVelController() {};
    virtual ~MotorPosVelController() {};

    virtual int setup() = 0;
    virtual void cycle() = 0;

    virtual void pos(float pos, float vel) = 0;
    virtual void pos(float pos) = 0;
    virtual float pos() const = 0;
    virtual float currentTargetPos() const = 0;
    virtual void initPos(float pos) = 0;

    virtual void vel(float vel) = 0;
    virtual float vel() const = 0;
    virtual float currentTargetVel() const = 0;

    virtual void duty(float duty) = 0;
    virtual float duty() const = 0;

    virtual void stop() = 0;
    virtual void restart() = 0;
};