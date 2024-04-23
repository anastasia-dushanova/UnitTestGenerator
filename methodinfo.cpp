#include "methodinfo.h"

MethodInfo::MethodInfo() { }

MethodInfo::MethodInfo(QString method, size_t args, int type, bool returnValue)
    :method(method), args(args), type(type), returnValue(returnValue) { }
