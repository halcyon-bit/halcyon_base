#ifndef HALCYON_BASE_VERSION_H
#define HALCYON_BASE_VERSION_H

#include <base/common/base_define.h>

HALCYON_BASE_BEGIN_NAMESPACE

/**
 * @brief       获取版本号
 * @return      版本号
 */
extern "C" HALCYON_BASE_API const char* version();

HALCYON_BASE_END_NAMESPACE

#endif