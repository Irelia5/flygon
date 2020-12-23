#ifndef FLYGON_BASE_SINGLETON_H
#define FLYGON_BASE_SINGLETON_H
#include "flygon/base/noncopyable.h"
namespace flygon
{
template<typename T>
class Singleton : noncopyable
{
public:
    ~Singleton() {}
    static T& instance()
    {
        static T instance;
        return instance;
    }
private:
    Singleton() {}
};

} // namespace flygon

#endif //FLYGON_BASE_SINGLETON_H