#include <Mahi/Util/Math/Integrator.hpp>

namespace mahi {
namespace util {

Integrator::Integrator(double initial_condition,
                       Integrator::Technique technique)
    : technique_(technique),
      step_count_(0),
      last_last_x_(0),
      last_x_(0),
      last_last_t_(Time::Zero),
      last_t_(Time::Zero),
      integral_(initial_condition)
{}

void Integrator::set_technique(Technique technique) {
    technique_ = technique;
    reset();
}

double Integrator::update(double x, const Time& t) {
    switch (technique_) {
        case Technique::Trapezoidal:
            if (step_count_ > 0)
                integral_ += (t.as_seconds() - last_t_.as_seconds()) *
                             (0.5 * (last_x_ + x));
            break;
        case Technique::Simpsons:
            if (step_count_ > 1)
                integral_ += 0.166666666666666 *
                             (t.as_seconds() - last_t_.as_seconds()) *
                             (last_last_x_ + 4 * last_x_ + x);
            else if (step_count_ > 0)
                integral_ += (t.as_seconds() - last_t_.as_seconds()) *
                             (0.5 * (last_x_ + x));
            break;
    }
    last_last_x_ = last_x_;
    last_x_      = x;
    last_last_t_ = last_t_;
    last_t_      = t;
    step_count_ += 1;
    return integral_;
}

double Integrator::get_value() const {
    return integral_;
}

void Integrator::reset() {
    integral_    = 0.0;
    step_count_  = 0;
    last_last_x_ = 0.0;
    last_x_      = 0.0;
    last_last_t_ = Time::Zero;
    last_t_      = Time::Zero;
}

void Integrator::set_init(double initial_value) {
    integral_ = initial_value;
}

}  // namespace util
}  // namespace mahi
