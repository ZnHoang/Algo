
#include <chrono>
#include <string>

using timepoint = std::chrono::_V2::steady_clock::time_point;

#define NOW std::chrono::steady_clock::now()
#define USED(end, begin) std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin)
#define PRINT(func, time) printf("\033[33m%-20s%-20.8f\n\033[0m", func, time.count() * 1e-9)

template <class Func>
void timing(const char* funcName, Func func)
{
    auto begin = NOW;
    func();
    auto used = USED(NOW, begin);
    PRINT(funcName, used);
}

template <class Func>
auto timingWithRet(const char* funcName, Func func)
{
    auto begin = NOW;
    auto res = func();
    auto used = USED(NOW, begin);
    PRINT(funcName, used);
    return res;
}