#include <Mahi/Util.hpp>
#include <vector>
#include <memory>

using namespace mahi::util;

template <template <typename, typename> class Container,  typename T, typename A=std::allocator<T> >
T mean2(const Container<T, A> & c)
{
  return T();
}

int main(int argc, char const *argv[])
{
    std::vector<double> x(11);
    linspace(0,20,x);

    auto s = sum(x);
    auto u = mean(x);
    auto sdp = stddev_p(x);
    auto sds = stddev_s(x);

    print("Data {}", x);
    print("Sum {}", s);
    print("Mean {}", u);
    print("Std. Dev. P. {}",sdp);
    print("Std. Dev. S. {}",sds);

    double m, b;
    std::vector<double> y(11);
    linspace(20,100,y);
    linear_regression(x,y,m,b);

    print("y = {} * x + {}",m,b);

    return 0;
}
