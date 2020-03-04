#include <Mahi/Util/Logging/Log.hpp>
#include <Mahi/Util/Math/Filter.hpp>
#include <iostream>

namespace mahi {
namespace util {

Filter::Filter(const std::vector<double>& b, const std::vector<double>& a, unsigned int seeding) :
    has_seeding_(seeding > 0),
    first_update_(true),
    will_filter_(true),
    seed_count_(seeding)
{
    set_coefficients(b, a);
}

Filter::Filter(std::size_t n, unsigned int seeding) :
    n_(n),
    s_(n_, 0.0),
    has_seeding_(seeding > 0),
    first_update_(true),
    seed_count_(seeding)
{}

double Filter::update(const double x) {
    if (first_update_) {
        if (has_seeding_) {
            seed(x, seed_count_);
        }
        first_update_ = false;
    }  
    value_ = dir_form_ii_t(x);
    return value_;
}

double Filter::get_value() const {
    return value_;
}

void Filter::set_seeding(unsigned int seeding) {
    has_seeding_ = (seeding > 0);
    seed_count_ = seeding;
}

void Filter::reset() {
    s_ = std::vector<double>(n_, 0.0);
    first_update_ = true;
}

const std::vector<double>& Filter::get_b() const {
    return b_;
}

const std::vector<double>& Filter::get_a() const {
    return a_;
}

void Filter::set_coefficients(const std::vector<double>& b,
                              const std::vector<double>& a) {
    if (a.size() != b.size()) {
        LOG(Error) << "Filter coefficient vector sizes do not match";
    } 
    else if (a.size() < 2) {
        LOG(Error) << "Coefficient vectors must be longer than length 1";
    }
    else {
        b_ = b;
        a_ = a;
        n_ = a_.size() - 1;
        reset();
        if (a_ == std::vector<double>({1,0}) && b_ == std::vector<double>({1,0}))
            will_filter_ = false;
        else
            will_filter_ = true;
    }

}

double Filter::dir_form_ii_t(const double x) {
    if (!will_filter_)
        return x;
    double y = (s_[0] + b_[0] * x) / a_[0];
    for (std::size_t i = 0; i < n_ - 1; ++i) {
        s_[i] = s_[i + 1] + b_[i + 1] * x - a_[i + 1] * y;
    }
    s_[n_ - 1] = b_[n_] * x - a_[n_] * y;
    return y;
}

void Filter::seed(const double x, const unsigned int iterations) {
    for (unsigned int i = 0; i < iterations; ++i) {
        dir_form_ii_t(x);
    }
}

} // namespace util
} // namespace mahi
