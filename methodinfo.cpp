#include "methodinfo.h"

GeneralInfo::GeneralInfo() { }

GeneralInfo::GeneralInfo(ClassInfo classInfo, Method method)
    :classInfo(classInfo), method(method) { }
