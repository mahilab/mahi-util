#include <Mahi/Util/Math/Functions.hpp>
#include <Mahi/Util/Math/Constants.hpp>
#include <Mahi/Util/Logging/Log.hpp>
#include <numeric>
#include <algorithm>
#include <cmath>

namespace mahi {
namespace util {

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



