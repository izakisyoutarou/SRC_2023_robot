#pragma once
/*******************************************
 * variance ver1.0 2022/02/18
 * This program calculate the moving variance.
 *
 * [Dependency]
 *
 * [Note]
 *
 * [Author]
 * Yuta Uehara
 *
 * [Change history]
 * ver1.0 2022/02/18 The first version
 ******************************************/

template <class T>
class Variance {
public:
	Variance() {}
	void add(T value) {
		const auto old_mean = mean_;
		size_++;
		mean_ += (value - mean_) / size_;
		M2 += (value - old_mean) * (value - mean_);
		variance_ = M2 / size_;
	}
	void change(T new_value, T old_value) {
		const auto old_mean = mean_;
		const auto diff = new_value - old_value;
		mean_ += diff / size_;
		M2 += (new_value - mean_ + old_value - old_mean) * diff;
		variance_ = M2 / size_;
	}
	T variance() const { return variance_; }
	T mean() { return mean_; }
	unsigned int size() { return size_; }
	void reset() {
		mean_ = 0;
		M2 = 0;
		size_ = 0;
		variance_ = 0;
	}
private:
	T mean_ = 0;
	T M2 = 0;
	unsigned int size_ = 0;
	T variance_ = 0;
};