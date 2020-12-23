#ifndef FLYGON_BASE_NONCOPYABLE_H
#define FLYGON_BASE_NONCOPYABLE_H

namespace flygon
{

class noncopyable
{
  public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;
    
  protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

}   // namespace flygon

#endif  // FLYGON_BASE_NONCOPYABLE_H