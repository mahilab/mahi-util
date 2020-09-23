namespace mahi {
namespace util {

template <typename T>
inline int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

inline bool is_even(int value) {
    return !(value % 2);
}

inline bool is_odd(int value) {
    return (value % 2);
}

template <typename T>
inline T clamp(T value, T min, T max) {
    return value <= min ? min : value >= max ? max : value;
}

template <typename T>
inline T clamp(T value, T abs_max) {
    abs_max = std::abs(abs_max);
    return clamp(value, -abs_max, abs_max);
}

template <typename T>
inline T clamp01(T value) {
    return value <= 0 ? 0 : value >= 1 ? 1 : value;
}

template <typename T>
inline T wrap_to_pi(T rad) {
    rad = std::fmod(rad + PI, TWOPI);
    if (rad < 0)
        rad += TWOPI;
    return rad - PI;
}

template <typename T>
inline T wrap_to_2pi(T rad) {
    rad = std::fmod(rad, TWOPI);
    if (rad < 0)
        rad += TWOPI;
    return rad;
}

template <typename T>
inline T wrap_to_180(T deg) {
    deg = std::fmod(deg + 180, 360);
    if (deg < 0)
        deg += 360;
    return deg - 180;
}

template <typename T>
inline T wrap_to_360(T deg) {
    deg = std::fmod(deg, 360);
    if (deg < 0)
        deg += 360;
    return deg;
}

template <typename T>
inline bool approximately(T a, T b, T tol) {
    return std::abs(a-b) < tol;
}

template <typename T>
inline T interp(T x, T x0, T x1, T y0, T y1) {
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0);
}

template <typename T>
inline int order_of_mag(T value) {
    if (value == 0)
        return 0;
    else
        return static_cast<int>(floor(log10(std::abs(value))));
}

inline std::size_t precision(int order) {
    std::size_t prec;
    if (order >= 1)
        prec = 0;
    else if (order == 0)
        prec = 1;
    else
        prec = -order + 1;
    return prec;
}

template <typename T>
inline T round_up_nearest(T value, T interval)
{
    if (interval == 0)
        return value;
    T rem = fmod(abs(value), interval);
    if (rem == 0)
        return value;
    if (value < 0)
        return -(abs(value) - rem);
    else
        return value + interval - rem;
}

template <typename T>
inline T round_down_nearest(T value, T interval) {
    if (interval == 0)
        return value;
    T rem = fmod(abs(value), interval);
    if (rem == 0)
        return value;
    if (value < 0)
        return -(abs(value) - rem);
    else
        return value - rem;
}

template <typename T>
inline T round_nearest(T value, T interval) {
    if (value >= 0) {
        T rem = fmod(value, interval);
        value = rem > interval * static_cast<T>(0.5) ? value + interval - rem : value - rem;
    }
    else {
        value = -value;
        T rem = fmod(value, interval);
        value = rem > interval * static_cast<T>(0.5) ? value + interval - rem : value - rem;
        value = -value;

    }
    return value;
}

template <typename T>
T pd_controller(T kp, T kd, T x_ref, T x, T xd_ref, T xd) {
    return kp * (x_ref - x) + kd * (xd_ref - xd);
}

template <typename T>
T sigmoid(T a) {
    T b = std::exp(-a);
    if (std::isinf(b)) {
        return 0;
    }
    else {
        return 1.0 / (1.0 + b);
    }
}

template <typename T>
inline T remap(T x, T x0, T x1, T y0, T y1) { 
    return y0 + (x - x0) * (y1 - y0) / (x1 - x0); 
}

//==============================================================================
// STATISTICS
//==============================================================================

template <class Container, typename R>
inline void linspace(R a, R b, Container& array) {
    if (array.size() == 0)
        return;
    auto N = array.size();
    typename Container::value_type aa = static_cast<typename Container::value_type>(a);
    typename Container::value_type bb = static_cast<typename Container::value_type>(b);
    typename Container::value_type delta = (bb - aa) / static_cast<typename Container::value_type>(N - 1);
    array[0] = aa;
    for (std::size_t i = 1; i < N - 1; i++)
        array[i] = array[i - 1] + delta;
    array[N - 1] = bb;
}

template <typename Container>
inline typename Container::value_type min_element(const Container& values) {
    return *std::min_element(values.begin(), values.end());
}

template <typename Container>
inline typename Container::value_type max_element(const Container& values) {
    return *std::max_element(values.begin(), values.end());
}

template <class Container>
inline typename Container::value_type sum(const Container& data) {
    typename Container::value_type s = 0;
    for (std::size_t i = 0; i < data.size(); ++i)
        s += data[i];
    return s;
}

template <class Container>
inline typename Container::value_type mean(const Container & data) {
    typename Container::value_type den = static_cast<typename Container::value_type>(1) / static_cast<typename Container::value_type>(data.size());
    typename Container::value_type mean = 0;
    for (std::size_t i = 0; i < data.size(); ++i)
        mean += data[i] * den;
    return mean;
}

template <class Container>
inline typename Container::value_type stddev_p(const Container& data) {
    if (data.size() > 0) {
        typename Container::value_type u = mean(data);
        auto diff = data;
        std::transform(data.begin(), data.end(), diff.begin(), [u](typename Container::value_type x) { return x - u; });
        typename Container::value_type sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), typename Container::value_type(0));
        return std::sqrt(sq_sum / data.size());
    }
    else {
        return typename Container::value_type(0);
    }
}

template <class Container>
inline typename Container::value_type stddev_s(const Container& data) {
    if (data.size() > 1) {
        typename Container::value_type u = mean(data);
        auto diff = data;
        std::transform(data.begin(), data.end(), diff.begin(), [u](typename Container::value_type x) { return x - u; });
        typename Container::value_type sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), typename Container::value_type(0));
        return std::sqrt(sq_sum / (data.size() - 1));
    }
    else {
        return typename Container::value_type(0);
    }
}

template <class Container>
inline typename Container::value_type rms(const Container& data) {
    typename Container::value_type square = 0;
    typename Container::value_type size = static_cast<typename Container::value_type>(data.size());
    for (std::size_t i = 0; i < data.size(); ++i)
        square += pow(data[i],2);
    typename Container::value_type root = sqrt(square/size);
    return root;
}

template <class ContainerX, class ContainerY, typename T>
inline void linear_regression(const ContainerX& x, const ContainerY& y, T& mOut, T& bOut) {
    assert(x.size() == y.size());
    auto xbar = mean(x);
    auto ybar = mean(y);
    auto xbar2 = xbar * xbar;
    auto xbarybar = xbar * ybar;
    auto n = x.size();
    auto x2_xbar2 = x;
    auto xy_xbarybar = x;
    for (std::size_t i = 0; i < n; ++i) {
        x2_xbar2[i]    = x[i] * x[i] - xbar2;
        xy_xbarybar[i] = x[i] * y[i] - xbarybar;
    }
    T sigmax2 = static_cast<T>(mean(x2_xbar2));
    T sigmaxy = static_cast<T>(mean(xy_xbarybar));
    mOut = sigmaxy / sigmax2;
    bOut = -xbar * mOut + ybar;
}

} // namespace util
} // namespace mahi

