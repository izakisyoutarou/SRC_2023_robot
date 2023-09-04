#pragma once
namespace robomaster {

struct Config{
    int number;
    int send_period_ms;
    int count_resolution;
    bool send_count;
    bool send_rpm;
    bool send_torque;
};

class RobomasterESC {
public:
	RobomasterESC() = default;
	virtual ~RobomasterESC() = default;

	const Config config(){return config_;};
    const int current(){return current_;};
    void setData(const int count=0, const int rpm=0.f, const int torque=0){
        this->count = count;
        this->rpm = rpm;
        this->torque = torque;
    }

protected:
    Config config_;
    int current_=0;

    int count=0;
    int rpm=0;
    int torque=0;
};

}
