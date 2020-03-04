#include <Mahi/Util/Math/Differentiator.hpp>

namespace mahi {
namespace util {

Differentiator::Differentiator(Technique technique)
    : technique_(technique),
      step_count_(0),
      last_last_x_(0),
      last_x_(0),
      last_last_t_(Time::Zero),
      last_t_(Time::Zero),
      derivative_(0.0)
{}

void Differentiator::set_technique(Technique technique) {
    technique_ = technique;
    reset();
}

double Differentiator::update(double x, const Time& t) {
    switch (technique_) {
        case BackwardDifference:
            if (step_count_ > 0) {
                derivative_ = (x - last_x_) / (t.as_seconds() - last_t_.as_seconds());
            }
        case CentralDifference:
            if (step_count_ > 1) {
                derivative_ = (x - last_last_x_) / (t.as_seconds() - last_last_t_.as_seconds());
            } 
            else if (step_count_ > 0) {
                derivative_ = (x - last_x_) / (t.as_seconds() - last_t_.as_seconds());
            }
    }
    last_last_x_ = last_x_;
    last_x_      = x;
    last_last_t_ = last_t_;
    last_t_      = t;
    ++step_count_;
    return derivative_;
}

double Differentiator::get_value() const {
    return derivative_;
}

void Differentiator::reset() {
    last_last_x_ = 0.0;
    last_x_ = 0.0;
    last_last_t_ = Time::Zero;
    last_t_ = Time::Zero;
}

}  // namespace util
}  // namespace mahi
