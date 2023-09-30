#ifndef HALCYON_BASE_CODING_H
#define HALCYON_BASE_CODING_H

#include <locale>
#include <codecvt>

#include <base/common/base_define.h>

HALCYON_BASE_BEGIN_NAMESPACE

/**
 * @brief   编码转换
 * @ps      C++20 好像弃用 wstring_convert
 */
inline std::string utf8_to_gbk(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t> > conv;
    std::wstring tmp_wstr = conv.from_bytes(str);

    //GBK locale name in windows
    const char* kGbkLocaleName = ".936";
    std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> convert(new std::codecvt_byname<wchar_t, char, mbstate_t>(kGbkLocaleName));
    return convert.to_bytes(tmp_wstr);
}

inline std::string gbk_to_utf8(const std::string& str)
{
    //GBK locale name in windows
    const char* kGbkLocaleName = ".936";
    std::wstring_convert<std::codecvt_byname<wchar_t, char, mbstate_t>> convert(new std::codecvt_byname<wchar_t, char, mbstate_t>(kGbkLocaleName));
    std::wstring tmp_wstr = convert.from_bytes(str);

    std::wstring_convert<std::codecvt_utf8<wchar_t>> cv2;
    return cv2.to_bytes(tmp_wstr);
}

HALCYON_BASE_END_NAMESPACE

#endif