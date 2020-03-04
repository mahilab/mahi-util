#include <Mahi/Util/Math/Butterworth.hpp>
#include <Mahi/Util/Math/Constants.hpp>
#include <complex>

namespace mahi {
namespace util {

/// returns the poles of an order n Butterworth analog lowpass filter prototype
std::vector<std::complex<double>> butt_poles(std::size_t n) {
    std::complex<double> i(0, 1);
    std::vector<std::complex<double>> poles;
    for (double j = 1.0; j < static_cast<double>(n); j += 2.0) {
        std::complex<double> p = std::exp(i * (PI * j / (2.0 * n) + PI / 2.0));
        poles.push_back(p);
        poles.push_back(std::conj(p));
    }
    if (n % 2 == 1)
        poles.push_back(std::complex<double>(-1, 0));
    return poles;
}

/// returns the coefficients of the polynomial whose roots are the elements of r
template <class T>
std::vector<T> poly(std::vector<T> poles) {
    std::size_t n = poles.size();
    std::vector<T> coef(n + 1);
    coef[0] += 1.0;
    std::vector<T> temp = coef;
    for (std::size_t i = 0; i < n; ++i) {
        for (std::size_t j = 1; j < (i + 2); ++j)
            coef[j] = temp[j] - poles[i] * temp[j - 1];
        temp = coef;
    }
    return coef;
}

/// computes the denominator of the digital Butterworth filter
std::vector<double> compute_a(std::size_t n, double Wn) {
    double V = std::tan(Wn * PI / 2.0);
    std::vector<std::complex<double>> ap = butt_poles(n);  // analog poles
    std::vector<std::complex<double>> dp(ap.size());       // digital poles
    for (std::size_t i = 0; i < ap.size(); ++i) {
        ap[i] *= V;                             // warp
        dp[i] = (1.0 + ap[i]) / (1.0 - ap[i]);  // bilinear transform
    }
    std::vector<std::complex<double>> a_complex = poly(dp);
    std::vector<double> a(a_complex.size());
    for (std::size_t i = 0; i < a_complex.size(); ++i)
        a[i] = a_complex[i].real();
    return a;
}

/// computes the numerator of the digital Butterworth filter
std::vector<double> compute_b(std::size_t n,
                              std::vector<double> a,
                              Butterworth::Type type) {
    std::vector<double> r;
    double w = 0;
    switch (type) {
        case Butterworth::Type::Lowpass:
            r = std::vector<double>(n, -1.0);
            w = 0;
            break;
        case Butterworth::Type::Highpass:
            r = std::vector<double>(n, 1.0);
            w = PI;
            break;
    }
    std::vector<double> b = poly(r);
    // normalize b so |H(w)| == 1
    std::vector<std::complex<double>> kern(b.size());
    std::complex<double> i(0, 1);
    std::complex<double> c(0, 0);
    std::complex<double> d(0, 0);
    for (std::size_t j = 0; j < kern.size(); ++j) {
        kern[j] = std::exp(-i * w * static_cast<double>(j));
        c += kern[j] * a[j];
        d += kern[j] * b[j];
    }
    for (std::size_t j = 0; j < b.size(); ++j)
        b[j] = (b[j] * c / d).real();
    return b;
}

Butterworth::Butterworth() : Filter({ 1,0 }, { 1,0 }) {

}

Butterworth::Butterworth(std::size_t n, double Wn, Type type, unsigned int seeding) : Filter(n, seeding) {
    auto a = compute_a(n, Wn);
    auto b = compute_b(n, a, type);
    set_coefficients(b, a);
}

Butterworth::Butterworth(std::size_t n, Frequency cutoff, Frequency sample, Type type, unsigned int seeding)
    : Butterworth(n,
                  2.0 * static_cast<double>(cutoff.as_hertz()) /
                        static_cast<double>(sample.as_hertz()),
                  type,
                  seeding) 
{}

void Butterworth::configure(std::size_t n, double Wn, Type type, unsigned int seeding) {
    set_seeding(seeding);
    auto a = compute_a(n, Wn);
    auto b = compute_b(n, a, type);
    set_coefficients(b, a);
}

void Butterworth::configure(std::size_t n, Frequency cutoff, Frequency sample, Type type, unsigned int seeding) {
    configure(n, 2.0 * static_cast<double>(cutoff.as_hertz()) / static_cast<double>(sample.as_hertz()), type, seeding);
}

}  // namespace util
}  // namespace mahi
