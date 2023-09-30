#ifndef BASE_NONCOPYABLE_H
#define BASE_NONCOPYABLE_H

#include <base/common/base_define.h>

HALCYON_BASE_BEGIN_NAMESPACE

/// 对象语义的基类
class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

HALCYON_BASE_END_NAMESPACE

#endif