#include "flygon/base/Singleton.h"
#include "flygon/base/noncopyable.h"
#include "flygon/base/CurrentThread.h"
#include "flygon/base/Thread.h"
#include <string>
#include <stdio.h>
using namespace flygon;

//这个Test不是线程安全的
class Test : noncopyable
{
public:
    Test()
    {
        printf("tid=%d, constructing %p\n", flygon::CurrentThread::tid(), this);
    }

    ~Test()
    {
        printf("tid=%d, destructing %p %s\n", flygon::CurrentThread::tid(), this, name_.c_str());
    }

    const std::string& name() const { return name_; }
    void setName(const std::string& n) { name_ = n; }
private:
    std::string name_;
};

void threadFunc()
{
  printf("tid=%d, %p name=%s\n",
         flygon::CurrentThread::tid(),
         &flygon::Singleton<Test>::instance(),
         flygon::Singleton<Test>::instance().name().c_str());
  flygon::Singleton<Test>::instance().setName("only one, changed");
}

int main()
{
    flygon::Singleton<Test>::instance().setName("only one");
    flygon::Thread t1(threadFunc);
    t1.start();
    t1.join();
    printf("tid=%d, %p name=%s\n",
         flygon::CurrentThread::tid(),
         &flygon::Singleton<Test>::instance(),
         flygon::Singleton<Test>::instance().name().c_str());
}