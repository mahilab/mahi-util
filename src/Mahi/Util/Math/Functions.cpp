#include <Mahi/Util/Math/Functions.hpp>
#include <Mahi/Util/Math/Constants.hpp>
#include <Mahi/Util/Logging/Log.hpp>
#include <numeric>
#include <algorithm>
#include <cmath>

namespace mahi {
namespace util {

//==============================================================================
// GENERIC FUNCTIONS
//==============================================================================

double min(const std::vector<double>& values) {
    return *min_element(values.begin(), values.end());
}

double max(const std::vector<double>& values) {
    return *max_element(values.begin(), values.end());
}

double saturate(double value, double min, double max) {
    if (value > max)
        return max;
    else if (value < min)
        return min;
    else
        return value;
}

double saturate(double value, double abs_max) {
    abs_max = std::abs(abs_max);
    return saturate(value, -abs_max, abs_max);
}

double pd_controller(double kp, double kd, double x_ref, double x, double xd_ref, double xd) {
    return kp * (x_ref - x) + kd * (xd_ref - xd);
}

bool approx_equal(double a, double b, double tolerance) {
    return std::abs(a - b) < tolerance;
}

std::vector<double> linspace(double a, double b, std::size_t n) {
    std::vector<double> out;
    if (n > 1) {
        out.resize(n);
        double delta = (b - a) / (n - 1);
        out[0] = a;
        for (std::size_t i = 1; i < n - 1; i++) {
            out[i] = out[i - 1] + delta;
        }
        out[n - 1] = b;
    }
    return out;
}

double sigmoid(double a) {
    double b = std::exp(-a);
    if (std::isinf(b)) {
        return 0;
    }
    else {
        return 1.0 / (1.0 + b);
    }
}

double auto_diff(std::complex<double>(*f)(std::complex<double>), double x) {
    return f(std::complex<double>(x, 1.0e-22)).imag() / 1.0e-22;
}

double wrap_to_2pi(double radians) {
    bool was_neg = radians < 0;
    radians = fmod(radians, 2.0 * PI);
    if (was_neg)
        radians += 2.0 * PI;
    return radians;
}

double wrap_to_pi(double radians) {
    return wrap_to_2pi(radians + PI) - PI;
}

//==============================================================================
// STATISTICS
//==============================================================================

std::vector<double> abs_vec(const std::vector<double>& data) {
    std::vector<double> abs_data(data.size());
    for (std::size_t i = 0; i < data.size(); ++i) {
        abs_data[i] = std::abs(data[i]);
    }
    return abs_data;
}

double sum(const std::vector<double>& data) {
    return std::accumulate(data.begin(), data.end(), 0.0);
}

double mean(const std::vector<double>& data) {
    return data.size() > 0 ? sum(data) / data.size() : 0.0;
}

double stddev_p(const std::vector<double>& data) {
    if (data.size() > 0) {
        double avg = mean(data);
        std::vector<double> diff(data.size());
        std::transform(data.begin(), data.end(), diff.begin(), [avg](double x) { return x - avg; });
        double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
        return std::sqrt(sq_sum / data.size());
    }
    else {
        return 0;
    }
}

double stddev_s(const std::vector<double>& data) {
    if (data.size() > 1) {
        double avg = mean(data);
        std::vector<double> diff(data.size());
        std::transform(data.begin(), data.end(), diff.begin(), [avg](double x) { return x - avg; });
        double sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0.0);
        return std::sqrt(sq_sum / (data.size() - 1));
    }
    else {
        return 0;
    }
}

extern std::vector<double> linear_regression(const std::vector<double>& x, const std::vector<double>& y) {
    if (x.size() != y.size()) {
        LOG(Error) << "Length of x (" << x.size() << ") not equal to length of y (" << y.size() << ")";
        return std::vector<double>();
    }

    double xbar = mean(x);
    double ybar = mean(y);
    double xbar2 = xbar * xbar;
    double xbarybar = xbar * ybar;

    std::size_t n = x.size();
    std::vector<double> x2_xbar2(n);
    std::vector<double> xy_xbarybar(n);

    for (std::size_t i = 0; i < n; ++i) {
        x2_xbar2[i]    = x[i] * x[i] - xbar2;
        xy_xbarybar[i] = x[i] * y[i] - xbarybar;
    }

    double sigmax2  = mean(x2_xbar2);
    double sigmaxy = mean(xy_xbarybar);

    double m = sigmaxy / sigmax2;
    double b = -xbar * m + ybar;

    return std::vector<double>{m, b};
}

extern void gauss_mlt_params(const std::vector<std::vector<double>>& sample_data, std::vector<double>& sample_mean, std::vector<std::vector<double>>& sample_cov) {
    std::size_t N = sample_data.size();
    std::size_t sample_dim;
    if (N > 0) {
        sample_dim = sample_data[0].size();
        for (std::size_t i = 0; i < N; ++i) {
            if (sample_data[i].size() != sample_dim) {
                LOG(Warning) << "Data given to gauss_mlt_params() contains samples of different sizes. Parameters were not computed.";
                return;
            }
        }
    }
    else {
        LOG(Warning) << "Data given to gauss_mlt_params() was empty. Parameters were not computed.";
        return;
    }

    sample_mean = std::vector<double>(sample_dim, 0.0);
    sample_cov = std::vector<std::vector<double>>(sample_dim, std::vector<double>(sample_dim, 0.0));

    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < sample_dim; ++j) {
            sample_mean[j] += sample_data[i][j];
        }
    }
    for (std::size_t i = 0; i < sample_dim; ++i) {
        sample_mean[i] /= N;
    }

    std::vector<double> sample(sample_dim, 0.0);
    for (std::size_t i = 0; i < N; ++i) {
        for (std::size_t j = 0; j < sample_dim; ++j) {
            sample[j] = sample_data[i][j] - sample_mean[j];
        }
        for (std::size_t j = 0; j < sample_dim; ++j) {
            for (std::size_t k = 0; k < sample_dim; ++k) {
                sample_cov[j][k] += sample[j] * sample[k];
            }
        }
    }
    double sample_cov_scalar = (double)((signed)(N - 1));
    for (std::size_t i = 0; i < sample_dim; ++i) {
        for (std::size_t j = 0; j < sample_dim; ++j) {
            sample_cov[i][j] /= sample_cov_scalar;
        }
    }
}

} // namespace util
} // namespace mahi



