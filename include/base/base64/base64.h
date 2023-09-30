﻿#ifndef HALCYON_BASE_BASE64_H
#define HALCYON_BASE_BASE64_H

#include <string>

#include <base/common/noncopyable.h>

HALCYON_BASE_BEGIN_NAMESPACE

/// base64 编码与解码
class HALCYON_BASE_API Base64 final : noncopyable
{
public:
    Base64() = default;
    ~Base64() = default;

public:
    /**
     * @brief      base64编码
     * @param[in]  原始字符串
     * @param[in]  原始字符串长度
     * @return     base64字符串
     */
    std::string encode(const uint8_t* str, int32_t length);

    /**
     * @brief       base64解码
     * @param[in]   base64字符串
     * @param[in]   base64字符串长度
     * @return      原始字符串
     */
    std::string decode(const char* str, int32_t length);
};

HALCYON_BASE_END_NAMESPACE

#endif