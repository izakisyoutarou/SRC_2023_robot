#pragma once
/*******************************************
 * velocity_planner ver1.0 2020/6/29
 * This program is base class for velocity generator
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.1 2020/11/25 delete extra include
 * ver1.0 2020/ 6/29 The first version
 ******************************************/

class VelocityPlanner {
public:
    virtual ~VelocityPlanner() {}
    virtual void cycle() = 0;

    virtual void current(float pos, float vel, float acc) = 0;
    virtual void limit(float pos, float vel, float acc, float dec) = 0;
    virtual void limit(float pos, float vel, float acc) { limit(pos, vel, acc, -acc); }

    virtual void pos(float pos, float vel) = 0;
    virtual void pos(float pos) { this->pos(pos, 0.f); }
    virtual void vel(float vel) = 0;

    virtual float pos() = 0;
    virtual float vel() = 0;
    virtual float acc() = 0;

    virtual bool hasAchievedTarget() = 0;
};
