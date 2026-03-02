#ifndef HALCYON_BASE_NONCOPYABLE_H
#define HALCYON_BASE_NONCOPYABLE_H

#include <base/common/base_define.h>

HALCYON_BASE_BEGIN_NAMESPACE

/// 对象语义的基类
class NonCopyable
{
public:
    NonCopyable(const NonCopyable&) = delete;
    void operator=(const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

HALCYON_BASE_END_NAMESPACE

#endif
