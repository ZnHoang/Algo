#include <string>

bool DEBUG = false;

void setDebug(bool debug = true)
{
    DEBUG = debug;
}

void MSG(const std::string& msg)
{
#if DEBUG

#endif
}