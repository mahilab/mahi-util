#include <Mahi/Util.hpp>
#include <vector>
#include <array>
#include <memory>

using namespace mahi::util;

int main(int argc, char const *argv[])
{
    std::vector<double> x(11);
    linspace(0,20,x);

    double s = sum(x);
    double u = mean(x);
    double sdp = stddev_p(x);
    double sds = stddev_s(x);
    double mn = min_element(x);
    double mx = max_element(x);

    print("Data {}", x);
    print("Sum {}", s);
    print("Mean {}", u);
    print("Std. Dev. P. {}",sdp);
    print("Std. Dev. S. {}",sds);
    print("Min {}",mn);
    print("Max {}",mx);
  
    double m, b;
    std::array<double,11> y; // can use arrays too
    linspace(20,100,y);
    linear_regression(x,y,m,b);

    print("y = {} * x + {}",m,b);

    return 0;
}
