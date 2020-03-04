namespace mahi {
namespace util {

template <typename T>
inline int sign(T val) {
    return (T(0) < val) - (val < T(0));
}

inline bool isEven(int value) {
    return !(value % 2);
}

inline bool isOdd(int value) {
    return (value % 2);
}

template <template <typename, typename> class Container,  typename T, typename A, typename R>
inline void linspace(R a, R b, Container<T,A>& array) {
    if (array.size() == 0)
        return;
    auto N = array.size();
    T aa = static_cast<T>(a);
    T bb = static_cast<T>(b);
    T delta = (bb - aa) / static_cast<T>(N - 1);
    array[0] = aa;
    for (std::size_t i = 1; i < N - 1; i++)
        array[i] = array[i - 1] + delta;
    array[N - 1] = bb;
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
inline T wrap_toPi(T angle) {
    angle = std::fmod(angle + PI, TWOPI);
    if (angle < 0)
        angle += TWOPI;
    return angle - PI;
}

template <typename T>
inline T wrap_to_2pi(T angle) {
    angle = std::fmod(angle, TWOPI);
    if (angle < 0)
        angle += TWOPI;
    return angle;
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
inline int orderOfMagnitude(T value) {
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
inline T roundUpToNearest(T value, T interval)
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
inline T roundDownToNearest(T value, T interval) {
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
inline T roundToNearest(T value, T interval) {
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

template <typename T, typename TArray>
T min_element(const TArray& values) {
    return *std::min_element(values.begin(), values.end());
}

template <typename T, typename TArray>
T max_element(const TArray& values) {
    return *std::max_element(values.begin(), values.end());
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

//==============================================================================
// STATISTICS
//==============================================================================

template <template <typename, typename> class Container,  typename T, typename A>
inline T sum(const Container<T,A>& data) {
    T s = 0;
    for (std::size_t i = 0; i < data.size(); ++i)
        s += data[i];
    return s;
}

template <template <typename, typename> class Container,  typename T, typename A>
inline T mean(const Container<T, A> & data) {
    T den = static_cast<T>(1) / static_cast<T>(data.size());
    T mean = 0;
    for (std::size_t i = 0; i < data.size(); ++i)
        mean += data[i] * den;
    return mean;
}

template <template <typename, typename> class Container,  typename T, typename A>
inline T stddev_p(const Container<T,A>& data, T* meanOut) {
    if (data.size() > 0) {
        T u = mean(data);
        Container<T,A> diff(data.size());
        std::transform(data.begin(), data.end(), diff.begin(), [u](T x) { return x - u; });
        T sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0);
        if (meanOut)
            *meanOut = u;
        return std::sqrt(sq_sum / data.size());
    }
    else {
        return 0;
    }
}

template <template <typename, typename> class Container,  typename T, typename A>
inline T stddev_s(const Container<T,A>& data, T* meanOut) {
    if (data.size() > 1) {
        T u = mean(data);
        Container<T,A> diff(data.size());
        std::transform(data.begin(), data.end(), diff.begin(), [u](T x) { return x - u; });
        T sq_sum = std::inner_product(diff.begin(), diff.end(), diff.begin(), 0);
        if (meanOut)
            *meanOut = u;
        return std::sqrt(sq_sum / (data.size() - 1));
    }
    else {
        return 0;
    }
}

} // namespace util
} // namespace mahi

