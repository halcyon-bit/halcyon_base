﻿#include <base/common/base_define.h>

export module noncopyable;

HALCYON_BASE_BEGIN_NAMESPACE

/**
 * @brief   对象语义的基类
 */
export class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    void operator=(const noncopyable&) = delete;

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

HALCYON_BASE_END_NAMESPACE