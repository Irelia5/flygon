#include "flygon/base/Exception.h"
#include "flygon/base/CurrentThread.h"

namespace flygon
{

Exception::Exception(std::string msg)
    : message_(std::move(msg))
    , stack_(CurrentThread::stackTrace(/*demangle=*/false))
{ }


} // namespace flygon