#pragma once
/*******************************************
 * GyroSensor ver1.3 2020/03/19
 * This is base class for gyro sensor
 * 
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.3 2020/03/19 Change some function to protected
 * ver1.2 2020/03/19 Add virtual destructor.
 * ver1.1 2020/03/17 Add getter/setter. Field change to private.
 * ver1.0 2020/03/10 The first version.
 ******************************************/

class GyroSensor{
public:
    GyroSensor(){}
    virtual ~GyroSensor(){}

    virtual int setup() = 0;
    virtual void cycle() = 0;

    virtual void rev(){rev_flag = !rev_flag;}
    virtual void rev(bool rev_flag){this->rev_flag = rev_flag;}

    float angVelX() const {return rev_flag ? -ang_vel_x : ang_vel_x;}
    float angVelY() const {return rev_flag ? -ang_vel_y : ang_vel_y;}
    float angVelZ() const {return rev_flag ? -ang_vel_z : ang_vel_z;}

    float angleX() const {return rev_flag ? -angle_x : angle_x;}
    float angleY() const {return rev_flag ? -angle_y : angle_y;}
    float angleZ() const {return rev_flag ? -angle_z : angle_z;}

protected:
    void angVelX(float ang_vel){ang_vel_x = ang_vel;}
    void angVelY(float ang_vel){ang_vel_y = ang_vel;}
    void angVelZ(float ang_vel){ang_vel_z = ang_vel;}

    void angleX(float angle){angle_x = angle;}
    void angleY(float angle){angle_y = angle;}
    void angleZ(float angle){angle_z = angle;}

private:
    float ang_vel_x = 0.f;
    float ang_vel_y = 0.f;
    float ang_vel_z = 0.f;
    float angle_x = 0.f;
    float angle_y = 0.f;
    float angle_z = 0.f;

    bool rev_flag = false;
};
