#include "base/string/string.h"

#ifdef USE_HALCYON_STRING_VIEW
#include "base/string/string_view.h"
#else
#include <string_view>
#endif

#include "gtest/gtest.h"

#include <string>
#include <forward_list>

using namespace halcyon;

TEST(StringCoreTest, construct)
{
    std::string std_str1;
    std::string std_str2(12, 'c');
    std::string std_str3(256, 'c');
    std::string std_str4(512, 'c');

    base::string_core<char> str0;
    EXPECT_EQ(str0.size(), 0);
    base::string_core<char> str1(std_str1.data(), std_str1.size());
    EXPECT_EQ(str1.size(), 0);
    base::string_core<char> str2(std_str2.data(), std_str2.size());
    EXPECT_EQ(str2.size(), 12);
    base::string_core<char> str3(std_str3.data(), std_str3.size());
    EXPECT_EQ(str3.size(), 256);
    base::string_core<char> str4(std_str4.data(), std_str4.size());
    EXPECT_EQ(str4.size(), 512);

    base::string_core<char> str5(str1);
    EXPECT_EQ(str5.size(), 0);
    base::string_core<char> str6(str2);
    EXPECT_EQ(str6.size(), 12);
    base::string_core<char> str7(str3);
    EXPECT_EQ(str7.size(), 256);
    base::string_core<char> str8(str4);
    EXPECT_EQ(str8.size(), 512);
    EXPECT_EQ(str8.data(), str4.data());

    base::string_core<char> str9(std::move(str5));
    EXPECT_EQ(str5.size(), 0);
    EXPECT_EQ(str9.size(), 0);
    base::string_core<char> str10(std::move(str6));
    EXPECT_EQ(str6.size(), 0);
    EXPECT_EQ(str10.size(), 12);
    base::string_core<char> str11(std::move(str7));
    EXPECT_EQ(str7.size(), 0);
    EXPECT_EQ(str11.size(), 256);
    base::string_core<char> str12(std::move(str8));
    EXPECT_EQ(str8.size(), 0);
    EXPECT_EQ(str12.size(), 512);
    EXPECT_EQ(str12.data(), str4.data());
}

TEST(StringCoreTest, deconstruct)
{
    std::string std_str1(12, 'c');
    for (size_t i = 0; i < 1000000; ++i) {
        base::string_core<char> str1(std_str1.data(), std_str1.size());
        EXPECT_EQ(str1.size(), 12);
    }

    std::string std_str2(256, 'c');
    for (size_t i = 0; i < 1000000; ++i) {
        base::string_core<char> str2(std_str2.data(), std_str2.size());
        EXPECT_EQ(str2.size(), 256);
    }

    std::string std_str3(100000, 'c');
    for (size_t i = 0; i < 1000000; ++i) {
        base::string_core<char> str3(std_str3.data(), std_str3.size());
        EXPECT_EQ(str3.size(), std_str3.size());
    }
}

TEST(StringCoreTest, access)
{
    std::string std_str1;
    std::string std_str2(12, 'c');
    std::string std_str3(256, 'c');
    std::string std_str4(512, 'c');

    {
        base::string_core<char> str0;
        EXPECT_EQ(std_str1.compare(str0.data()), 0);

        base::string_core<char> str1(std_str1.data(), std_str1.size());
        EXPECT_EQ(std_str1.compare(str1.data()), 0);
        EXPECT_EQ(std_str1.compare(str1.mutableData()), 0);

        base::string_core<char> str2(std_str2.data(), std_str2.size());
        EXPECT_EQ(std_str2.compare(str2.data()), 0);
        EXPECT_EQ(std_str2.compare(str2.mutableData()), 0);

        base::string_core<char> str3(std_str3.data(), std_str3.size());
        EXPECT_EQ(std_str3.compare(str3.data()), 0);
        EXPECT_EQ(std_str3.compare(str3.mutableData()), 0);

        base::string_core<char> str4(std_str4.data(), std_str4.size());
        EXPECT_EQ(std_str4.compare(str4.data()), 0);
        EXPECT_EQ(std_str4.compare(str4.mutableData()), 0);
    }

    {
        const base::string_core<char> str0;
        EXPECT_EQ(std_str1.compare(str0.data()), 0);

        const base::string_core<char> str1(std_str1.data(), std_str1.size());
        EXPECT_EQ(std_str1.compare(str1.data()), 0);
        EXPECT_EQ(std_str1.compare(str1.c_str()), 0);

        const base::string_core<char> str2(std_str2.data(), std_str2.size());
        EXPECT_EQ(std_str2.compare(str2.data()), 0);
        EXPECT_EQ(std_str2.compare(str2.c_str()), 0);

        const base::string_core<char> str3(std_str3.data(), std_str3.size());
        EXPECT_EQ(std_str3.compare(str3.data()), 0);
        EXPECT_EQ(std_str3.compare(str3.c_str()), 0);

        const base::string_core<char> str4(std_str4.data(), std_str4.size());
        EXPECT_EQ(std_str4.compare(str4.data()), 0);
        EXPECT_EQ(std_str4.compare(str4.c_str()), 0);
    }
}

TEST(StringCoreTest, modify)
{
    std::string std_str1;
    std::string std_str2(23, 'c');
    std::string std_str3(288, 'c');
    std::string std_str4(512, 'c');
    std::string std_str5(1024, 'a');

    // 短字符串
    base::string_core<char> str1(std_str1.data(), std_str1.size());
    str1.push_back('c');
    EXPECT_EQ(str1.data()[str1.size() - 1], 'c');
    EXPECT_EQ(str1.size(), 1);
    EXPECT_EQ(str1.capacity(), 23);

    // 短 -> 中
    base::string_core<char> str2(std_str2.data(), std_str2.size());
    EXPECT_EQ(str2.size(), 23);
    str2.push_back('c');
    EXPECT_EQ(str2.data()[str2.size() - 1], 'c');
    EXPECT_EQ(str2.size(), 24);
    EXPECT_GT(str2.capacity(), 24); 
    
    // 中 -> 长
    base::string_core<char> str3(std_str3.data(), std_str3.size());
    EXPECT_EQ(str3.size(), 288);
    str3.push_back('c');
    EXPECT_EQ(str3.data()[str3.size() - 1], 'c');
    EXPECT_GT(str3.capacity(), 289);

    base::string_core<char> str4(std_str4.data(), std_str4.size());
    EXPECT_EQ(str4.size(), 512);
    str4.push_back('c');
    EXPECT_EQ(str3.data()[str3.size() - 1], 'c');
    EXPECT_GT(str4.capacity(), 513);

    str1.reserve(100);
    EXPECT_EQ(str1.capacity(), 100);

    EXPECT_EQ(str3.isShared(), false);
    // str3 虽然为长字符串，但没有被共享，shrink 时按中字符串处理，不会重新申请内存
    str3.shrink(200);
    // 289 - 200        (89)
    EXPECT_EQ(str3.size(), 89);
    // 289 * 1.5        (433)
    EXPECT_EQ(str3.capacity(), 433);

    // 将 str4 共享
    base::string_core<char> str5(str4);
    EXPECT_EQ(str4.isShared(), true);
    str4.shrink(300);
    // 513 - 300  (213)
    EXPECT_EQ(str4.size(), 213);
    EXPECT_EQ(str4.capacity(), 213);

    // swap
    base::string_core<char> str6;
    str6.swap(str5);
    EXPECT_EQ(str5.size(), 0);
    EXPECT_EQ(str6.size(), 513);
    EXPECT_EQ(str5.capacity(), 23);
    EXPECT_EQ(str6.capacity(), 769);

    // long string
    base::string_core<char> str7(std_str5.data(), std_str5.size());
    base::string_core<char> str8 = str7;
    base::string_core<char> str9 = str7;
    EXPECT_EQ(str7.data(), str8.data());
    str8.push_back('f');
    EXPECT_NE(str7.data(), str8.data());
    EXPECT_EQ(str7.data(), str9.data());
}

TEST(StringTest, construct)
{
    // construct
    base::string str1;
    EXPECT_EQ(str1.size(), 0);
    
    base::string str2(100, 'r');
    EXPECT_EQ(str2.size(), 100);
    EXPECT_EQ(str2.data()[99], 'r');

    base::string str3(str2, 10);
    EXPECT_EQ(str3.size(), 90);
    EXPECT_EQ(str3.data()[89], 'r');

    base::string str4(str2, 10, 50);
    EXPECT_EQ(str4.size(), 50);
    EXPECT_EQ(str4.data()[49], 'r');

    base::string str5(str2.data());
    EXPECT_EQ(str5.size(), 100);
    EXPECT_EQ(str5.data()[99], 'r');

    base::string str6(str2.data(), 60);
    EXPECT_EQ(str6.size(), 60);
    EXPECT_EQ(str6.data()[59], 'r');

    std::string std_str1(50, 'a');
    base::string str7(std_str1.begin(), std_str1.end());
    EXPECT_EQ(str7.size(), 50);
    EXPECT_EQ(str7.data()[49], 'a');

    char szch[49];
    memset(szch, 'b', 49);
    base::string str8(szch, szch + 19);
    EXPECT_EQ(str8.size(), 19);
    EXPECT_EQ(str8.data()[18], 'b');

    base::string str9({ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'j' });
    EXPECT_EQ(str9.size(), 9);
    EXPECT_EQ(str9.data()[8], 'j');

    //base::string strE = nullptr;  // compile error

    // copy、move construct
    base::string str10(str9);
    EXPECT_EQ(str10.size(), 9);
    EXPECT_EQ(str10.size(), str9.size());

    base::string str11(std::move(str10));
    EXPECT_EQ(str11.size(), str9.size());
    EXPECT_EQ(str10.size(), 0);
    EXPECT_EQ(str11.size(), 9);

    std::string std_str2(60, 'c');
    base::string str12(std_str2);
    EXPECT_EQ(str12.size(), 60);
    EXPECT_EQ(str12.size(), std_str2.size());
    EXPECT_EQ(std_str2.compare(str12.data()), 0);

    // operator =
    base::string str13;
    str13 = str12;
    EXPECT_EQ(str13.size(), str12.size());
    EXPECT_EQ(std_str2.compare(str13.data()), 0);

    str13 = std::move(str12);
    EXPECT_EQ(str13.size(), std_str2.size());
    EXPECT_EQ(str12.size(), 0);
    EXPECT_EQ(std_str2.compare(str13.data()), 0);

    str13 = 'c';
    EXPECT_EQ(str13.size(), 1);
    EXPECT_EQ(str13.data()[0], 'c');

    str13 = "abcedfghi";
    std::string std_str3("abcedfghi");
    EXPECT_EQ(str13.size(), 9);
    EXPECT_EQ(std_str3.compare(str13.data()), 0);

    str13 = { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'j' };
    std_str3 = ("abcdefghj");
    EXPECT_EQ(str13.size(), 9);
    EXPECT_EQ(std_str3.compare(str13.data()), 0);

    str13 = std_str1;
    EXPECT_EQ(str13.size(), std_str1.size());
    EXPECT_EQ(std_str1.compare(str13.data()), 0);

    //base::string strE;
    //strE = nullptr;  // compile error
}

TEST(StringTest, iterators)
{
    std::string std_str1(100, 'r');
    base::string str1(100, 'r');
    EXPECT_EQ(std_str1.compare(str1.data()), 0);
    for (auto iter = str1.begin(); iter != str1.end(); ++iter) {
        EXPECT_EQ(*iter, 'r');
    }
    for (auto iter = str1.end() - 1; iter != str1.begin(); --iter) {
        EXPECT_EQ(*iter, 'r');
    }
    EXPECT_EQ(std_str1.compare(str1.data()), 0);
    for (auto iter = str1.cbegin(); iter != str1.cend(); ++iter) {
        EXPECT_EQ(*iter, 'r');
    }
    for (auto iter = str1.cend() - 1; iter != str1.cbegin(); --iter) {
        EXPECT_EQ(*iter, 'r');
    }

    const base::string str2(100, 'r');
    EXPECT_EQ(std_str1.compare(str2.data()), 0);
    for (auto iter = str2.begin(); iter != str2.end(); ++iter) {
        EXPECT_EQ(*iter, 'r');
    }
    for (auto iter = str2.end() - 1; iter != str2.begin(); --iter) {
        EXPECT_EQ(*iter, 'r');
    }
    EXPECT_EQ(std_str1.compare(str2.data()), 0);
    for (auto iter = str2.cbegin(); iter != str2.cend(); ++iter) {
        EXPECT_EQ(*iter, 'r');
    }
    for (auto iter = str2.cend() - 1; iter != str2.cbegin(); --iter) {
        EXPECT_EQ(*iter, 'r');
    }

    std::string std_str2("1234567890abcdefghijklmnopqrstuvwxyz");
    std::string std_str3("zyxwvutsrqponmlkjihgfedcba0987654321");
    base::string str3("1234567890abcdefghijklmnopqrstuvwxyz");
    auto std_iter = std_str3.begin();
    for (auto iter = str3.rbegin(); iter != str3.rend(); ++iter, ++std_iter) {
        EXPECT_EQ(*iter, *std_iter);
    }
    std_iter = std_str2.begin();
    for (auto iter = str3.rend() - 1; iter != str3.rbegin(); --iter, ++std_iter) {
        EXPECT_EQ(*iter, *std_iter);
    }

    std_iter = std_str3.begin();
    for (auto iter = str3.crbegin(); iter != str3.crend(); ++iter, ++std_iter) {
        EXPECT_EQ(*iter, *std_iter);
    }
    std_iter = std_str2.begin();
    for (auto iter = str3.crend() - 1; iter != str3.crbegin(); --iter, ++std_iter) {
        EXPECT_EQ(*iter, *std_iter);
    }

    const base::string str4("1234567890abcdefghijklmnopqrstuvwxyz");
    std_iter = std_str3.begin();
    for (auto iter = str4.rbegin(); iter != str4.rend(); ++iter, ++std_iter) {
        EXPECT_EQ(*iter, *std_iter);
    }
    std_iter = std_str2.begin();
    for (auto iter = str4.rend() - 1; iter != str4.rbegin(); --iter, ++std_iter) {
        EXPECT_EQ(*iter, *std_iter);
    }

    std_iter = std_str3.begin();
    for (auto iter = str4.crbegin(); iter != str4.crend(); ++iter, ++std_iter) {
        EXPECT_EQ(*iter, *std_iter);
    }
    std_iter = std_str2.begin();
    for (auto iter = str4.crend() - 1; iter != str4.crbegin(); --iter, ++std_iter) {
        EXPECT_EQ(*iter, *std_iter);
    }
}

TEST(StringTest, access)
{
    std::string std_str1("1234567890abcdefghijklmnopqrstuvwxyz");
    base::string str1("1234567890abcdefghijklmnopqrstuvwxyz");

    size_t size = str1.size();
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(str1.at(i), std_str1.at(i));
        EXPECT_EQ(str1[i], std_str1[i]);
    }
    
    bool except = false;
    try {
        str1.at(size);
    }
    catch (std::out_of_range e) {
        except = true;
    }
    EXPECT_EQ(except, true);

    const base::string str2("1234567890abcdefghijklmnopqrstuvwxyz");
    size = str2.size();
    for (size_t i = 0; i < size; ++i) {
        EXPECT_EQ(str2.at(i), std_str1.at(i));
        EXPECT_EQ(str2[i], std_str1[i]);
    }

    except = false;
    try {
        str2.at(size);
    }
    catch (std::out_of_range e) {
        except = true;
    }
    EXPECT_EQ(except, true);

    EXPECT_EQ(str1.front(), '1');
    EXPECT_EQ(str2.front(), '1');
    
    EXPECT_EQ(str1.back(), 'z');
    EXPECT_EQ(str2.back(), 'z');

    EXPECT_EQ(std_str1.compare(str1.data()), 0);
    EXPECT_EQ(std_str1.compare(str2.data()), 0);
    EXPECT_EQ(std_str1.compare(str1.c_str()), 0);
    EXPECT_EQ(std_str1.compare(str2.c_str()), 0);
}

TEST(StringTest, std_string)
{
    std::string std_str1("1234567890abcdefghijklmnopqrstuvwxyz");
    base::string str1("1234567890abcdefghijklmnopqrstuvwxyz");

    std::string std_str2 = str1.toStdString();
    EXPECT_EQ(std_str1, std_str2);

#ifdef USE_HALCYON_STRING_VIEW
    base::string_view sv(str1);
#else
    std::string_view sv(str1);
#endif
    EXPECT_EQ(std_str1.compare(sv.data()), 0);
}

TEST(StringTest, capacity)
{
    base::string str1("1234567890abcdefghijklmnopqrstuvwxyz");
    base::string str2;
    std::string std_str1("1234567890abcdefghijklmnopqrstuvwxyz");

    EXPECT_EQ(str1.empty(), false);
    EXPECT_EQ(str2.empty(), true);

    EXPECT_EQ(str1.size(), std_str1.size());
    EXPECT_EQ(str1.length(), std_str1.length());
    EXPECT_EQ(str2.size(), 0);
    EXPECT_EQ(str2.length(), 0);

    str2.max_size();

    EXPECT_EQ(str1.capacity(), str1.size());
    EXPECT_EQ(str2.capacity(), 23);

    str1.reserve(500);
    EXPECT_EQ(str1.capacity(), 500);
    EXPECT_EQ(std_str1.compare(str1.data()), 0);

    str2.reserve(1024);
    EXPECT_EQ(str2.capacity(), 1024);
    EXPECT_EQ(str2.empty(), true);

    str1.shrink_to_fit();
    EXPECT_EQ(std_str1.compare(str1.data()), 0);

    str2.shrink_to_fit();
    EXPECT_EQ(str2.capacity(), 23);
}

TEST(StringTest, operator_1)
{
    base::string str1("1234567890abcdefghijklmnopqrstuvwxyz");

    EXPECT_EQ(str1.empty(), false);
    str1.clear();
    EXPECT_EQ(str1.empty(), true);

    str1.push_back('1');
    EXPECT_EQ(str1.empty(), false);
    EXPECT_EQ(str1.size(), 1);
    str1.push_back('2');
    EXPECT_EQ(str1.data()[1], '2');

    str1.pop_back();
    EXPECT_EQ(str1.size(), 1);

    base::string str2("1234567890abcdefghijklmnopqrstuvwxyz");
    base::string str3;
    str3.swap(str2);
    EXPECT_EQ(str2.empty(), true);
    EXPECT_EQ(str2.size(), 0);
    EXPECT_EQ(str3.empty(), false);
    EXPECT_EQ(str3.size(), strlen("1234567890abcdefghijklmnopqrstuvwxyz"));

    str3.resize(10);
    EXPECT_EQ(str3.size(), 10);
    std::string std_str1("1234567890");
    EXPECT_EQ(std_str1.compare(str3.data()), 0);

    str3.resize(100, 'a');
    EXPECT_EQ(str3.size(), 100);
    EXPECT_EQ(str3.data()[99], 'a');
}

TEST(StringTest, assign)
{
    std::string std_str1("1234567890abcdefghijklmnopqrstuvwxyz");
    base::string str1("1234567890abcdefghijklmnopqrstuvwxyz");
    base::string str2("1234567890abcdefghijklmnopqrstuvwxyz");

    base::string str3;
    str3.assign(str1);
    EXPECT_EQ(std_str1.size(), str3.size());
    EXPECT_EQ(std_str1.compare(str3.data()), 0);
    
    str3.clear();
    str3.assign(std::move(str2));
    EXPECT_EQ(str1.size(), str3.size());
    EXPECT_EQ(str2.size(), 0);
    EXPECT_EQ(std_str1.compare(str3.data()), 0);

    str3.assign(str1, 10);
    EXPECT_EQ(str1.size() - 10, str3.size());
    EXPECT_EQ(std_str1.compare(10, str3.size(), str3.data()), 0);

    str3.assign(str1, 15, 5);
    EXPECT_EQ(str3.size(), 5);
    EXPECT_EQ(std_str1.compare(15, str3.size(), str3.data()), 0);

    str3.assign({ '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', 'a', 'b', 'c', 'd', 'e' });
    EXPECT_EQ(str3.size(), 15);
    EXPECT_EQ(std_str1.compare(0, str3.size(), str3.data()), 0);

    str3.assign("abcdefghijklmnopqrstuvwxyz");
    EXPECT_EQ(str3.size(), 26);
    EXPECT_EQ(std_str1.compare(10, str3.size(), str3.data()), 0);

    str3.assign("abcdefghijklmnopqrstuvwxyz", 10);
    std::string std_str2("abcdefghij");
    EXPECT_EQ(str3.size(), 10);
    EXPECT_EQ(std_str2.compare(str3.data()), 0);

    str3.assign(str1.begin(), str1.end());
    EXPECT_EQ(str3.size(), str1.size());
    EXPECT_EQ(std_str1.compare(str3.data()), 0);

    str3.assign(str1.rbegin(), str1.rend());
    std::reverse(std_str1.begin(), std_str1.end());
    EXPECT_EQ(str3.size(), str1.size());
    EXPECT_EQ(std_str1.compare(str3.data()), 0);

    str3.assign(15, 'c');
    std::string std_str3(15, 'c');
    EXPECT_EQ(str3.size(), 15);
    EXPECT_EQ(std_str3.compare(str3.data()), 0);
}

TEST(StringTest, insert)
{
    {
        base::string str("1234567890");
        std::string res("abcedf1234567890");
        str.insert(0, "abcedf");
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(10, "hello");
        res.insert(10, "hello");
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(str.size(), "world");
        res.insert(res.size(), "world");
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("1234567890");
        std::string res("1234567890");

        str.insert(0, "abcedf", 3);
        res.insert(0, "abcedf", 3);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(10, "hello", 5);
        res.insert(10, "hello", 5);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(str.size(), "world", 0);
        res.insert(res.size(), "world", 0);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string tmp("1234567890");

        base::string str("1234567890abcdefghijklmnopqrstuvwxyz");
        std::string res("1234567890abcdefghijklmnopqrstuvwxyz");

        str.insert(0, tmp);
        res.insert(0, tmp.data());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(10, tmp);
        res.insert(10, tmp.data());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(str.size(), tmp);
        res.insert(res.size(), tmp.data());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string tmp("1234567890");

        base::string str("1234567890abcdefghijklmnopqrstuvwxyz");
        std::string res("1234567890abcdefghijklmnopqrstuvwxyz");

        str.insert(0, tmp, 0);
        res.insert(0, tmp.data());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(0, tmp, 5);
        res.insert(0, tmp.data() + 5);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(0, tmp, tmp.size());
        res.insert(0, tmp.data() + tmp.size());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(0, tmp, 5, 3);
        res.insert(0, tmp.data() + 5, 3);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string tmp("1234567890");

        base::string str("1234567890abcdefghijklmnopqrstuvwxyz");
        std::string res("1234567890abcdefghijklmnopqrstuvwxyz");

        str.insert(10, tmp, 0);
        res.insert(10, tmp.data());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(10, tmp, 5);
        res.insert(10, tmp.data() + 5);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(10, tmp, tmp.size());
        res.insert(10, tmp.data() + tmp.size());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(10, tmp, 4, 5);
        res.insert(10, tmp.data() + 4, 5);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string tmp("1234567890");

        base::string str("1234567890abcdefghij");
        std::string res("1234567890abcdefghij");

        str.insert(str.size(), tmp, 0);
        res.insert(res.size(), tmp.data());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(str.size(), tmp, 5);
        res.insert(res.size(), tmp.data() + 5);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(str.size(), tmp, tmp.size());
        res.insert(res.size(), tmp.data() + tmp.size());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(str.size(), tmp, 0, 10);
        res.insert(res.size(), tmp.data(), 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("1234567890abcdefghij");
        std::string res("1234567890abcdefghij");

        str.insert(0, 10, 'a');
        res.insert(0, 10, 'a');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(5, 20, 'a');
        res.insert(5, 20, 'a');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.insert(str.size(), 30, 'a');
        res.insert(res.size(), 30, 'a');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("1234567890abcdefghij");
        std::string res("1234567890abcdefghij");

        auto ret = str.insert(str.begin(), { '1', '2', '3', '4' });
        res.insert(res.begin(), { '1', '2', '3', '4' });
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin());

        ret = str.insert(str.begin() + 10, { '1', '2', '3', '4' });
        res.insert(res.begin() + 10, { '1', '2', '3', '4' });
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin() + 10);

        size_t old_size = str.size();
        ret = str.insert(str.end(), { '1', '2', '3', '4' });
        res.insert(res.end(), { '1', '2', '3', '4' });
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin() + old_size);
    }

    {
        base::string str("1234567890abcdefghij");
        std::string res("1234567890abcdefghij");

        auto ret = str.insert(str.begin(), '1');
        res.insert(res.begin(), '1');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin());

        ret = str.insert(str.begin() + 10, '1');
        res.insert(res.begin() + 10, '1');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin() + 10);

        size_t old_size = str.size();
        ret = str.insert(str.end(), '1');
        res.insert(res.end(), '1');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin() + old_size);
    }

    {
        base::string str("1234567890abcdefghij");
        std::string res("1234567890abcdefghij");

        std::string tmp("hello");
        auto ret = str.insert(str.begin(), tmp.begin(), tmp.end());
        res.insert(res.begin(), tmp.begin(), tmp.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin());

        ret = str.insert(str.begin() + 20, tmp.begin(), tmp.begin() + 4);
        res.insert(res.begin() + 20, tmp.begin(), tmp.begin() + 4);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin() + 20);

        size_t old_size = str.size();
        ret = str.insert(str.end(), tmp.cbegin(), tmp.cend());
        res.insert(res.end(), tmp.begin(), tmp.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin() + old_size);
    }

    {
        base::string str("1234567890abcdefghij");
        std::string res("1234567890abcdefghij");

        base::string tmp("hello");
        auto ret = str.insert(str.begin(), tmp.begin(), tmp.end());
        res.insert(res.begin(), tmp.begin(), tmp.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin());

        ret = str.insert(str.begin() + 20, tmp.begin(), tmp.begin() + 4);
        res.insert(res.begin() + 20, tmp.begin(), tmp.begin() + 4);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin() + 20);

        size_t old_size = str.size();
        ret = str.insert(str.end(), tmp.cbegin(), tmp.cend());
        res.insert(res.end(), tmp.begin(), tmp.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin() + old_size);
    }

    {
        base::string str("1234567890abcdefghij");
        std::string res("1234567890abcdefghij");

        // 函数的最佳匹配的问题, 需要指明第二个参数为 size_t
        auto ret = str.insert(str.begin(), (size_t)10, 'b');
        res.insert(res.begin(), 10, 'b');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin());

        ret = str.insert(str.begin() + 20, size_t(5), 'c');
        res.insert(res.begin() + 20, size_t(5), 'c');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin() + 20);

        size_t old_size = str.size();
        ret = str.insert(str.end(), size_t(5), 'd');
        res.insert(res.end(), size_t(5), 'd');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        EXPECT_EQ(ret, str.begin() + old_size);
    }
}

TEST(StringTest, append)
{
    {
        base::string str1("1234567890");
        std::string res1("1234567890");

        base::string str2;
        std::string res2;

        str2.append(str1);
        res2.append(res1);
        EXPECT_EQ(str2.size(), res2.size());
        EXPECT_EQ(res2.compare(str2.data()), 0);

        str2.append(str1);
        res2.append(res1);
        EXPECT_EQ(str2.size(), res2.size());
        EXPECT_EQ(res2.compare(str2.data()), 0);
    }

    {
        base::string str1("1234567890");
        std::string res1("1234567890");

        base::string str2;
        std::string res2;

        str2.append(str1, 4);
        res2.append(res1, 4);
        EXPECT_EQ(str2.size(), res2.size());
        EXPECT_EQ(res2.compare(str2.data()), 0);

        str2.append(str1, str1.size());
        res2.append(res1, res1.size());
        EXPECT_EQ(str2.size(), res2.size());
        EXPECT_EQ(res2.compare(str2.data()), 0);

        str2.append(str1, 0, 5);
        res2.append(res1, 0, 5);
        EXPECT_EQ(str2.size(), res2.size());
        EXPECT_EQ(res2.compare(str2.data()), 0);

        str2.append(str1, 4, 5);
        res2.append(res1, 4, 5);
        EXPECT_EQ(str2.size(), res2.size());
        EXPECT_EQ(res2.compare(str2.data()), 0);

        str2.append(str1, str1.size(), 10);
        res2.append(res1, res1.size(), 10);
        EXPECT_EQ(str2.size(), res2.size());
        EXPECT_EQ(res2.compare(str2.data()), 0);
    }

    {
        base::string str;
        std::string res;

        str.append("hello");
        res.append("hello");
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.append("hello", 4);
        res.append("hello", 4);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str;
        std::string res;

        str.append(10, 'a');
        res.append(10, 'a');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        std::string tmp("1234567890");

        base::string str;
        std::string res;

        str.append(tmp.begin(), tmp.end());
        res.append(tmp.begin(), tmp.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.append(tmp.rbegin(), tmp.rend());
        res.append(tmp.rbegin(), tmp.rend());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string tmp("1234567890");

        base::string str;
        std::string res;

        str.append(tmp.begin(), tmp.end());
        res.append(tmp.begin(), tmp.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.append(tmp.rbegin(), tmp.rend());
        res.append(tmp.rbegin(), tmp.rend());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str;
        std::string res;

        str.append({ 'a', 'b', 'c', 'd', 'f', 'g' });
        res.append({ 'a', 'b', 'c', 'd', 'f', 'g' });
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }
}

TEST(StringTest, operator_add)
{
    base::string str;
    std::string res;

    base::string tmp1("1234567890");
    std::string tmp2("1234567890");

    str += tmp1;
    res += tmp2;
    EXPECT_EQ(str.size(), res.size());
    EXPECT_EQ(res.compare(str.data()), 0);

    str += "hello";
    res += "hello";
    EXPECT_EQ(str.size(), res.size());
    EXPECT_EQ(res.compare(str.data()), 0);

    str += 'h';
    res += 'h';
    EXPECT_EQ(str.size(), res.size());
    EXPECT_EQ(res.compare(str.data()), 0);

    str += {'w', 'o', 'r', 'l', 'd'};
    res += {'w', 'o', 'r', 'l', 'd'};
    EXPECT_EQ(str.size(), res.size());
    EXPECT_EQ(res.compare(str.data()), 0);

    str += tmp2;
    res += tmp2;
    EXPECT_EQ(str.size(), res.size());
    EXPECT_EQ(res.compare(str.data()), 0);

#ifdef USE_HALCYON_STRING_VIEW
    base::string_view sv(str);
#else
    std::string_view sv(str);
#endif
    str += sv;
    res += res;
    EXPECT_EQ(str.size(), res.size());
    EXPECT_EQ(res.compare(str.data()), 0);

    str += str;
    res += res;
    EXPECT_EQ(str.size(), res.size());
    EXPECT_EQ(res.compare(str.data()), 0);
}

TEST(StringTest, erase)
{
    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        str.erase(str.end());
        res.erase(res.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.erase(str.begin() + 10);
        res.erase(res.begin() + 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.erase(str.begin());
        res.erase(res.begin());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(str.empty(), res.empty());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        str.erase(str.begin() + 5, str.begin() + 10);
        res.erase(res.begin() + 5, res.begin() + 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.erase(str.begin(), str.end());
        res.erase(res.begin(), res.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(str.empty(), res.empty());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        str.erase(2, 4);
        res.erase(2, 4);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.erase(2, 0);
        res.erase(2, 0);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.erase(0, str.size());
        res.erase(0, res.size());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(str.empty(), res.empty());
        EXPECT_EQ(res.compare(str.data()), 0);

        str = "12345678901234567890";
        res = "12345678901234567890";
        str.erase();
        res.erase();
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(str.empty(), res.empty());
        EXPECT_EQ(res.compare(str.data()), 0);
    }
}

TEST(StringTest, replace)
{
    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        base::string tmp1("abcdefghijklmnopqrstuvwxyz");
        std::string tmp2("abcdefghijklmnopqrstuvwxyz");

        str.replace(0, 5, tmp1);
        res.replace(0, 5, tmp2);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
        
        str.replace(0, str.size(), tmp1);
        res.replace(0, res.size(), tmp2);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 5, tmp1);
        res.replace(10, 5, tmp2);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 0, tmp1);
        res.replace(10, 0, tmp2);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 0, tmp1);
        res.replace(res.size(), 0, tmp2);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 10, tmp1);
        res.replace(res.size(), 10, tmp2);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }
    
    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        base::string tmp1("abcdefghijklmnopqrstuvwxyz");
        std::string tmp2("abcdefghijklmnopqrstuvwxyz");

        str.replace(0, 5, tmp1, 0);
        res.replace(0, 5, tmp2, 0);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(0, str.size(), tmp1, 0);
        res.replace(0, res.size(), tmp2, 0);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 5, tmp1, 10);
        res.replace(10, 5, tmp2, 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 0, tmp1, tmp1.size());
        res.replace(10, 0, tmp2, tmp2.size());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 0, tmp1, tmp1.size());
        res.replace(res.size(), 0, tmp2, tmp2.size());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 10, tmp1, tmp1.size());
        res.replace(res.size(), 10, tmp2, tmp2.size());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        base::string tmp1("abcdefghijklmnopqrstuvwxyz");
        std::string tmp2("abcdefghijklmnopqrstuvwxyz");

        str.replace(0, 5, tmp1, 0, 0);
        res.replace(0, 5, tmp2, 0, 0);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(0, str.size(), tmp1, 0, 10);
        res.replace(0, res.size(), tmp2, 0, 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 5, tmp1, 10, 5);
        res.replace(10, 5, tmp2, 10, 5);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 5, tmp1, 10, tmp1.size());
        res.replace(10, 5, tmp2, 10, tmp2.size());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 0, tmp1, tmp1.size(), 10);
        res.replace(10, 0, tmp2, tmp2.size(), 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 0, tmp1, tmp1.size(), 10);
        res.replace(res.size(), 0, tmp2, tmp2.size(), 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 10, tmp1, tmp1.size(), 10);
        res.replace(res.size(), 10, tmp2, tmp2.size(), 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        const char* tmp("abcdefghijklmnopqrstuvwxyz");

        str.replace(0, 5, tmp);
        res.replace(0, 5, tmp);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(0, str.size(), tmp);
        res.replace(0, res.size(), tmp);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 5, tmp);
        res.replace(10, 5, tmp);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 0, tmp);
        res.replace(10, 0, tmp);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 0, tmp);
        res.replace(res.size(), 0, tmp);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 10, tmp);
        res.replace(res.size(), 10, tmp);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        const char* tmp("abcdefghijklmnopqrstuvwxyz");

        str.replace(0, 5, tmp, 0);
        res.replace(0, 5, tmp, 0);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(0, str.size(), tmp, 10);
        res.replace(0, res.size(), tmp, 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 5, tmp, strlen(tmp));
        res.replace(10, 5, tmp, strlen(tmp));
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 0, tmp, 10);
        res.replace(10, 0, tmp, 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 0, tmp, 10);
        res.replace(res.size(), 0, tmp, 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 10, tmp, 10);
        res.replace(res.size(), 10, tmp, 10);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        base::string tmp1("abcdefghijklmnopqrstuvwxyz");
        std::string tmp2("abcdefghijklmnopqrstuvwxyz");

        str.replace(0, 5, 10, 'a');
        res.replace(0, 5, 10, 'a');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(0, str.size(), 10, 'b');
        res.replace(0, res.size(), 10, 'b');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 5, 1000, 'c');
        res.replace(10, 5, 1000, 'c');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(10, 0, 100, 'c');
        res.replace(10, 0, 100, 'c');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 0, 10, 'd');
        res.replace(res.size(), 0, 10, 'd');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.size(), 10, 20, 'e');
        res.replace(res.size(), 10, 20, 'e');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        base::string tmp1("abcdefghijklmnopqrstuvwxyz");
        std::string tmp2("abcdefghijklmnopqrstuvwxyz");

        str.replace(str.begin(), str.begin() + 10, tmp1);
        res.replace(res.begin(), res.begin() + 10, tmp2);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.begin(), tmp1);
        res.replace(res.begin(), res.begin(), tmp2);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.end(), tmp1);
        res.replace(res.begin(), res.end(), tmp2);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        const char* tmp("abcdefghijklmnopqrstuvwxyz");

        str.replace(str.begin(), str.begin() + 10, tmp);
        res.replace(res.begin(), res.begin() + 10, tmp);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.begin(), tmp);
        res.replace(res.begin(), res.begin(), tmp);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.end(), tmp);
        res.replace(res.begin(), res.end(), tmp);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        str.replace(str.begin(), str.begin() + 10, { 'a', 'b', 'c', 'd', 'e', 'f', 'g' });
        res.replace(res.begin(), res.begin() + 10, { 'a', 'b', 'c', 'd', 'e', 'f', 'g' });
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.begin(), { 'a', 'b', 'c', 'd', 'e', 'f', 'g' });
        res.replace(res.begin(), res.begin(), { 'a', 'b', 'c', 'd', 'e', 'f', 'g' });
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.end(), { 'a', 'b', 'c', 'd', 'e', 'f', 'g' });
        res.replace(res.begin(), res.end(), { 'a', 'b', 'c', 'd', 'e', 'f', 'g' });
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }

    {
        base::string str("12345678901234567890");
        std::string res("12345678901234567890");

        base::string tmp1("abcdefghijklmnopqrstuvwxyz");
        std::string tmp2("abcdefghijklmnopqrstuvwxyz");
        char tmp[]("abcdefghijklmnopqrstuvwxyz");

        str.replace(str.begin(), str.begin() + 10, tmp1.begin(), tmp1.end());
        res.replace(res.begin(), res.begin() + 10, tmp2.begin(), tmp2.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.begin(), tmp1.begin(), tmp1.end());
        res.replace(res.begin(), res.begin(), tmp2.begin(), tmp2.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.end(), tmp1.begin(), tmp1.end());
        res.replace(res.begin(), res.end(), tmp2.begin(), tmp2.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.begin() + 10, tmp2.begin(), tmp2.end());
        res.replace(res.begin(), res.begin() + 10, tmp2.begin(), tmp2.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.begin(), tmp2.begin(), tmp2.end());
        res.replace(res.begin(), res.begin(), tmp2.begin(), tmp2.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.end(), tmp2.begin(), tmp2.end());
        res.replace(res.begin(), res.end(), tmp2.begin(), tmp2.end());
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        // std::end(tmp) 会计算 '/0'
        str.replace(str.begin(), str.begin() + 10, std::begin(tmp), std::end(tmp) - 1);
        res.replace(res.begin(), res.begin() + 10, std::begin(tmp), std::end(tmp) - 1);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.begin(), std::begin(tmp), std::end(tmp) - 1);
        res.replace(res.begin(), res.begin(), std::begin(tmp), std::end(tmp) - 1);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.end(), std::begin(tmp), std::end(tmp) - 1);
        res.replace(res.begin(), res.end(), std::begin(tmp), std::end(tmp) - 1);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.begin() + 10, tmp, 20);
        res.replace(res.begin(), res.begin() + 10, tmp, 20);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.begin(), tmp, 20);
        res.replace(res.begin(), res.begin(), tmp, 20);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.end(), tmp, 20);
        res.replace(res.begin(), res.end(), tmp, 20);
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.begin() + 10, 20, 'a');
        res.replace(res.begin(), res.begin() + 10, 20, 'a');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.begin(), 20, 'b');
        res.replace(res.begin(), res.begin(), 20, 'b');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);

        str.replace(str.begin(), str.end(), 5, 'c');
        res.replace(res.begin(), res.end(), 5, 'c');
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }
}

TEST(StringTest, compare)
{
    {
        base::string str("456789");
        std::string res("456789");

        base::string tmp1;
        std::string tmp2;
        EXPECT_EQ(str.compare(tmp1), res.compare(tmp2));

        tmp1 = "1"; tmp2 = "1";
        EXPECT_EQ(str.compare(tmp1), res.compare(tmp2));
        tmp1 = "123456"; tmp2 = "123456";
        EXPECT_EQ(str.compare(tmp1), res.compare(tmp2));
        tmp1 = "12345678"; tmp2 = "12345678";
        EXPECT_EQ(str.compare(tmp1), res.compare(tmp2));

        tmp1 = "987"; tmp2 = "987";
        EXPECT_EQ(str.compare(tmp1), res.compare(tmp2));
        tmp1 = "987654"; tmp2 = "987654";
        EXPECT_EQ(str.compare(tmp1), res.compare(tmp2));
        tmp1 = "98765432"; tmp2 = "98765432";
        EXPECT_EQ(str.compare(tmp1), res.compare(tmp2));

        tmp1 = "456789"; tmp2 = "456789";
        EXPECT_EQ(str.compare(tmp1), res.compare(tmp2));
    }

    {
        base::string str("456789");
        std::string res("456789");

        base::string tmp1;
        std::string tmp2;
        EXPECT_EQ(str.compare(0, 0, tmp1), res.compare(0, 0, tmp2));
        EXPECT_EQ(str.compare(1, 5, tmp1), res.compare(1, 5, tmp2));
        EXPECT_EQ(str.compare(0, str.size(), tmp1), res.compare(0, str.size(), tmp2));
        EXPECT_EQ(str.compare(str.size(), str.size(), tmp1), res.compare(str.size(), str.size(), tmp2));

        tmp1 = "1"; tmp2 = "1";
        EXPECT_EQ(str.compare(0, 0, tmp1), res.compare(0, 0, tmp2));
        EXPECT_EQ(str.compare(1, 5, tmp1), res.compare(1, 5, tmp2));
        EXPECT_EQ(str.compare(0, str.size(), tmp1), res.compare(0, str.size(), tmp2));
        EXPECT_EQ(str.compare(str.size(), str.size(), tmp1), res.compare(str.size(), str.size(), tmp2));

        tmp1 = "123456"; tmp2 = "123456";
        EXPECT_EQ(str.compare(0, 0, tmp1), res.compare(0, 0, tmp2));
        EXPECT_EQ(str.compare(1, 5, tmp1), res.compare(1, 5, tmp2));
        EXPECT_EQ(str.compare(0, str.size(), tmp1), res.compare(0, str.size(), tmp2));
        EXPECT_EQ(str.compare(str.size(), str.size(), tmp1), res.compare(str.size(), str.size(), tmp2));

        tmp1 = "12345678"; tmp2 = "12345678";
        EXPECT_EQ(str.compare(0, 0, tmp1), res.compare(0, 0, tmp2));
        EXPECT_EQ(str.compare(1, 5, tmp1), res.compare(1, 5, tmp2));
        EXPECT_EQ(str.compare(0, str.size(), tmp1), res.compare(0, str.size(), tmp2));
        EXPECT_EQ(str.compare(str.size(), str.size(), tmp1), res.compare(str.size(), str.size(), tmp2));

        tmp1 = "987"; tmp2 = "987";
        EXPECT_EQ(str.compare(0, 0, tmp1), res.compare(0, 0, tmp2));
        EXPECT_EQ(str.compare(1, 5, tmp1), res.compare(1, 5, tmp2));
        EXPECT_EQ(str.compare(0, str.size(), tmp1), res.compare(0, str.size(), tmp2));
        EXPECT_EQ(str.compare(str.size(), str.size(), tmp1), res.compare(str.size(), str.size(), tmp2));

        tmp1 = "987654"; tmp2 = "987654";
        EXPECT_EQ(str.compare(0, 0, tmp1), res.compare(0, 0, tmp2));
        EXPECT_EQ(str.compare(1, 5, tmp1), res.compare(1, 5, tmp2));
        EXPECT_EQ(str.compare(0, str.size(), tmp1), res.compare(0, str.size(), tmp2));
        EXPECT_EQ(str.compare(str.size(), str.size(), tmp1), res.compare(str.size(), str.size(), tmp2));

        tmp1 = "98765432"; tmp2 = "98765432";
        EXPECT_EQ(str.compare(0, 0, tmp1), res.compare(0, 0, tmp2));
        EXPECT_EQ(str.compare(1, 5, tmp1), res.compare(1, 5, tmp2));
        EXPECT_EQ(str.compare(0, str.size(), tmp1), res.compare(0, str.size(), tmp2));
        EXPECT_EQ(str.compare(str.size(), str.size(), tmp1), res.compare(str.size(), str.size(), tmp2));

        tmp1 = "456789"; tmp2 = "456789";
        EXPECT_EQ(str.compare(0, 0, tmp1), res.compare(0, 0, tmp2));
        EXPECT_EQ(str.compare(1, 5, tmp1), res.compare(1, 5, tmp2));
        EXPECT_EQ(str.compare(0, str.size(), tmp1), res.compare(0, str.size(), tmp2));
        EXPECT_EQ(str.compare(str.size(), str.size(), tmp1), res.compare(str.size(), str.size(), tmp2));
    }

    {
        base::string str("456789");
        std::string res("456789");

        base::string tmp1("12345678909876543210");
        std::string tmp2("12345678909876543210");
        int ret1 = str.compare(0, str.size(), tmp1, 0);
        int ret2 = res.compare(0, res.size(), tmp2, 0);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), tmp1, 5);
        ret2 = res.compare(0, res.size(), tmp2, 5);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), tmp1, tmp1.size());
        ret2 = res.compare(0, res.size(), tmp2, tmp2.size());
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), tmp1, 0, str.size());
        ret2 = res.compare(0, res.size(), tmp2, 0, res.size());
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), tmp1, 3, str.size());
        ret2 = res.compare(0, res.size(), tmp2, 3, res.size());
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), tmp1, 10, str.size());
        ret2 = res.compare(0, res.size(), tmp2, 10, res.size());
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), tmp1, 0, str.size() - 2);
        ret2 = res.compare(0, res.size(), tmp2, 0, res.size() - 2);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), tmp1, 3, str.size() - 2);
        ret2 = res.compare(0, res.size(), tmp2, 3, res.size() - 2);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), tmp1, 10, str.size() - 2);
        ret2 = res.compare(0, res.size(), tmp2, 10, res.size() - 2);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), tmp1, 0, str.size() + 2);
        ret2 = res.compare(0, res.size(), tmp2, 0, res.size() + 2);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(1, 3, tmp1, 4, 3);
        ret2 = res.compare(1, 3, tmp2, 4, 3);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(res.size(), 3, tmp1, 4, 3);
        ret2 = res.compare(res.size(), 3, tmp2, 4, 3);
        EXPECT_EQ(ret1, ret2);
    }

    {
        base::string str("456789");
        std::string res("456789");

        int ret1 = str.compare(0, str.size(), "456789");
        int ret2 = res.compare(0, res.size(), "456789");
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), "1");
        ret2 = res.compare(0, res.size(), "1");
        EXPECT_EQ(ret1, ret2);
        
        ret1 = str.compare(0, str.size(), "9");
        ret2 = res.compare(0, res.size(), "9");
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), "4567890");
        ret2 = res.compare(0, res.size(), "4567890");
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), "9876543");
        ret2 = res.compare(0, res.size(), "9876543");
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(1, 2, "56");
        ret2 = res.compare(1, 2, "56");
        EXPECT_EQ(ret1, ret2);
    }

    {
        base::string str("456789");
        std::string res("456789");

        int ret1 = str.compare(0, str.size(), "456789", strlen("456789"));
        int ret2 = res.compare(0, res.size(), "456789", strlen("456789"));
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), "1", 1);
        ret2 = res.compare(0, res.size(), "1", 1);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), "9", 1);
        ret2 = res.compare(0, res.size(), "9", 1);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), "4567890", 7);
        ret2 = res.compare(0, res.size(), "4567890", 7);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, str.size(), "9876543", 7);
        ret2 = res.compare(0, res.size(), "9876543", 7);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(0, 4, "4567890123", 4);
        ret2 = res.compare(0, 4, "4567890123", 4);
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare(2, 4, "6789", 4);
        ret2 = res.compare(2, 4, "6789", 4);
        EXPECT_EQ(ret1, ret2);
    }

    {
        base::string str("456789");
        std::string res("456789");

        int ret1 = str.compare("456789");
        int ret2 = res.compare("456789");
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare("1");
        ret2 = res.compare("1");
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare("9");
        ret2 = res.compare("9");
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare("4567890");
        ret2 = res.compare("4567890");
        EXPECT_EQ(ret1, ret2);

        ret1 = str.compare("9876543");
        ret2 = res.compare("9876543");
        EXPECT_EQ(ret1, ret2);
    }
}

TEST(StringTest, substr)
{
    {
        base::string str("1234567890");
        std::string res("1234567890");

        base::string ret1 = str.substr();
        std::string ret2 = res.substr();
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(0);
        ret2 = res.substr(0);
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(5);
        ret2 = res.substr(5);
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(str.size());
        ret2 = res.substr(res.size());
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(0, 0);
        ret2 = res.substr(0, 0);
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(0, 5);
        ret2 = res.substr(0, 5);
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(0, str.size());
        ret2 = res.substr(0, res.size());
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(5, 0);
        ret2 = res.substr(5, 0);
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(5, 5);
        ret2 = res.substr(5, 5);
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(5, str.size());
        ret2 = res.substr(5, res.size());
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(str.size(), 0);
        ret2 = res.substr(res.size(), 0);
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(str.size(), 5);
        ret2 = res.substr(res.size(), 5);
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);

        ret1 = str.substr(str.size(), str.size());
        ret2 = res.substr(res.size(), res.size());
        EXPECT_EQ(ret2.size(), ret1.size());
        EXPECT_EQ(ret2.compare(ret1.data()), 0);
    }

    {
        // basic_string substr(size_type pos = 0, size_type n = npos) &&
        base::string str = base::string("1234567890").substr(0, 5);
        std::string res("12345");
        EXPECT_EQ(str.size(), res.size());
        EXPECT_EQ(res.compare(str.data()), 0);
    }
}

TEST(StringTest, starts_ends_with)
{
    {
        base::string str("abcdefghijklmnopqrstuvwxyz");

        EXPECT_EQ(str.starts_with('a'), true);
        EXPECT_EQ(str.starts_with('c'), false);

        EXPECT_EQ(str.starts_with("a"), true);
        EXPECT_EQ(str.starts_with("c"), false);

        EXPECT_EQ(str.starts_with("abc"), true);
        EXPECT_EQ(str.starts_with("def"), false);

        EXPECT_EQ(str.starts_with("abcdefghijklmnopqrstuvwxyz"), true);
        EXPECT_EQ(str.starts_with("abcdefghijklmnopqrstuvwxyzasdf"), false);

#ifdef USE_HALCYON_STRING_VIEW
        base::string_view sv("a");
#else
        std::string_view sv("a");
#endif
        EXPECT_EQ(str.starts_with(sv), true);

        sv = "c";
        EXPECT_EQ(str.starts_with(sv), false);

        sv = "abc";
        EXPECT_EQ(str.starts_with(sv), true);

        sv = "def";
        EXPECT_EQ(str.starts_with(sv), false);

        sv = "abcdefghijklmnopqrstuvwxyz";
        EXPECT_EQ(str.starts_with(sv), true);

        sv = "abcdefghijklmnopqrstuvwxyzasdf";
        EXPECT_EQ(str.starts_with(sv), false);
    }

    {
        base::string str("abcdefghijklmnopqrstuvwxyz");

        EXPECT_EQ(str.ends_with('z'), true);
        EXPECT_EQ(str.ends_with('c'), false);

        EXPECT_EQ(str.ends_with("z"), true);
        EXPECT_EQ(str.ends_with("c"), false);

        EXPECT_EQ(str.ends_with("xyz"), true);
        EXPECT_EQ(str.ends_with("def"), false);

        EXPECT_EQ(str.ends_with("abcdefghijklmnopqrstuvwxyz"), true);
        EXPECT_EQ(str.ends_with("abcdefghijklmnopqrstuvwxyzasdf"), false);

#ifdef USE_HALCYON_STRING_VIEW
        base::string_view sv("z");
#else
        std::string_view sv("z");
#endif
        EXPECT_EQ(str.ends_with(sv), true);

        sv = "c";
        EXPECT_EQ(str.ends_with(sv), false);

        sv = "xyz";
        EXPECT_EQ(str.ends_with(sv), true);

        sv = "def";
        EXPECT_EQ(str.ends_with(sv), false);

        sv = "abcdefghijklmnopqrstuvwxyz";
        EXPECT_EQ(str.ends_with(sv), true);

        sv = "abcdefghijklmnopqrstuvwxyzasdf";
        EXPECT_EQ(str.ends_with(sv), false);
    }
}

TEST(StringTest, contains)
{
    base::string str("12345678901234567890");
    {
        EXPECT_EQ(str.contains('1'), true);
        EXPECT_EQ(str.contains('0'), true);
        EXPECT_EQ(str.contains('a'), false);

        EXPECT_EQ(str.contains("123"), true);
        EXPECT_EQ(str.contains("12345678901234567890"), true);

        EXPECT_EQ(str.contains("321"), false);
        EXPECT_EQ(str.contains("12345678901234567890123"), false);

#ifdef USE_HALCYON_STRING_VIEW
        base::string_view sv("1");
#else
        std::string_view sv("1");
#endif
        EXPECT_EQ(str.contains(sv), true);
        sv = "0";
        EXPECT_EQ(str.contains(sv), true);
        sv = "a";
        EXPECT_EQ(str.contains(sv), false);
        sv = "123";
        EXPECT_EQ(str.contains(sv), true);
        sv = "321";
        EXPECT_EQ(str.contains(sv), false);
        sv = "12345678901234567890";
        EXPECT_EQ(str.contains(sv), true);
        sv = "123456789012345678901";
        EXPECT_EQ(str.contains(sv), false);
    }
}

TEST(StringTest, find)
{
    base::string str("12345678901234567890");
    std::string res("12345678901234567890");

    {
        size_t pos1 = str.find('1');
        size_t pos2 = res.find('1');
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, 0);

        pos1 = str.find('1', pos1 + 1);
        pos2 = res.find('1', pos2 + 1);
        EXPECT_EQ(pos1, pos2);

        pos1 = str.find('1', pos1 + 1);
        pos2 = res.find('1', pos2 + 1);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, base::string::npos);

        pos1 = str.find('0');
        pos2 = res.find('0');
        EXPECT_EQ(pos1, pos2);

        pos1 = str.find('0', pos1 + 1);
        pos2 = res.find('0', pos2 + 1);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, str.size() - 1);

        pos1 = str.find('0', pos1 + 1);
        pos2 = res.find('0', pos2 + 1);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, base::string::npos);

        pos1 = str.find('a');
        pos2 = res.find('a');
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, base::string::npos);
    }

    {
        base::string tmp("123");

        size_t pos1 = str.find(tmp.c_str());
        size_t pos2 = res.find(tmp.c_str());
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, 0);

        pos1 = str.find(tmp.c_str(), pos1 + 1);
        pos2 = res.find(tmp.c_str(), pos2 + 1);
        EXPECT_EQ(pos1, pos2);

        pos1 = str.find(tmp.c_str(), pos1 + 1);
        pos2 = res.find(tmp.c_str(), pos2 + 1);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, base::string::npos);

        tmp = "890";
        pos1 = str.find(tmp.c_str());
        pos2 = res.find(tmp.c_str());
        EXPECT_EQ(pos1, pos2);

        pos1 = str.find(tmp.c_str(), pos1 + 1);
        pos2 = res.find(tmp.c_str(), pos2 + 1);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1 + 3, str.size());

        pos1 = str.find(tmp.c_str(), pos1 + 1);
        pos2 = res.find(tmp.c_str(), pos2 + 1);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, base::string::npos);

        tmp = "abc";
        pos1 = str.find(tmp.c_str());
        pos2 = res.find(tmp.c_str());
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, base::string::npos);
    }

    {
        base::string tmp1("123");
        std::string tmp2("123");

        size_t pos1 = str.find(tmp1);
        size_t pos2 = res.find(tmp2);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, 0);

        pos1 = str.find(tmp1, pos1 + 1);
        pos2 = res.find(tmp2, pos2 + 1);
        EXPECT_EQ(pos1, pos2);

        pos1 = str.find(tmp1, pos1 + 1);
        pos2 = res.find(tmp2, pos2 + 1);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, base::string::npos);

        tmp1 = "890";
        tmp2 = "890";
        pos1 = str.find(tmp1);
        pos2 = res.find(tmp2);
        EXPECT_EQ(pos1, pos2);

        pos1 = str.find(tmp1, pos1 + 1);
        pos2 = res.find(tmp2, pos2 + 1);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1 + 3, str.size());

        pos1 = str.find(tmp1, pos1 + 1);
        pos2 = res.find(tmp2, pos2 + 1);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, base::string::npos);

        tmp1 = "abc";
        tmp2 = "abc";
        pos1 = str.find(tmp1);
        pos2 = res.find(tmp2);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, base::string::npos);
    }

    {
        base::string tmp("123467");

        size_t pos1 = str.find(tmp.c_str(), 10, 0);
        size_t pos2 = res.find(tmp.c_str(), 10, 0);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, 10);

        pos1 = str.find(tmp.c_str(), 0, 4);
        pos2 = res.find(tmp.c_str(), 0, 4);
        EXPECT_EQ(pos1, pos2);

        pos1 = str.find(tmp.c_str(), pos1 + 1, 3);
        pos2 = res.find(tmp.c_str(), pos2 + 1, 3);
        EXPECT_EQ(pos1, pos2);

        pos1 = str.find(tmp.c_str(), pos1 + 1, 3);
        pos2 = res.find(tmp.c_str(), pos2 + 1, 3);
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, base::string::npos);

        pos1 = str.find(tmp.c_str(), 0, tmp.size());
        pos2 = res.find(tmp.c_str(), 0, tmp.size());
        EXPECT_EQ(pos1, pos2);
        EXPECT_EQ(pos1, base::string::npos);
    }
}

TEST(StringTest, rfind)
{
    base::string str("123456");
    std::string res("123456");

    // rfind(const basic_string&, size_type) const
    EXPECT_EQ(str.rfind(base::string()), str.size());
    EXPECT_EQ(str.rfind(base::string()), res.rfind(std::string()));
    EXPECT_EQ(str.rfind(base::string("23")), 1);
    EXPECT_EQ(str.rfind(base::string("6")), 5);
    EXPECT_EQ(str.rfind(base::string("23")), res.rfind(std::string("23")));
    EXPECT_EQ(str.rfind(base::string("6")), res.rfind(std::string("6")));
    EXPECT_EQ(str.rfind(base::string("1234567")), base::string::npos);
    EXPECT_EQ(str.rfind(base::string("1234567")), res.rfind(std::string("1234567")));

    EXPECT_EQ(str.rfind(base::string("23"), 1), 1);
    EXPECT_EQ(str.rfind(base::string("23"), 1), res.rfind(std::string("23"), 1));
    EXPECT_EQ(str.rfind(base::string("45"), 2), base::string::npos);
    EXPECT_EQ(str.rfind(base::string("45"), 2), res.rfind(std::string("45"), 2));
    EXPECT_EQ(str.rfind(base::string("23"), 10), 1);
    EXPECT_EQ(str.rfind(base::string("23"), 10), res.rfind(std::string("23"), 10));

    // rfind(value_type, size_type) const
    EXPECT_EQ(str.rfind('2'), 1);
    EXPECT_EQ(str.rfind('2'), res.rfind('2'));
    EXPECT_EQ(str.rfind('7'), base::string::npos);
    EXPECT_EQ(str.rfind('7'), res.rfind('7'));

    EXPECT_EQ(str.rfind('2', 1), 1);
    EXPECT_EQ(str.rfind('2', 1), res.rfind('2', 1));
    EXPECT_EQ(str.rfind('4', 2), base::string::npos);
    EXPECT_EQ(str.rfind('4', 2), res.rfind('4', 2));
    EXPECT_EQ(str.rfind('2', 10), 1);
    EXPECT_EQ(str.rfind('2', 10), res.rfind('2', 10));

    // rfind(const value_type*, size_type pos) const
    EXPECT_EQ(str.rfind("23"), 1);
    EXPECT_EQ(str.rfind("6"), 5);
    EXPECT_EQ(str.rfind("23"), res.rfind(std::string("23")));
    EXPECT_EQ(str.rfind("6"), res.rfind(std::string("6")));
    EXPECT_EQ(str.rfind("1234567"), base::string::npos);
    EXPECT_EQ(str.rfind("1234567"), res.rfind(std::string("1234567")));

    EXPECT_EQ(str.rfind("23", 1), 1);
    EXPECT_EQ(str.rfind("23", 1), res.rfind(std::string("23"), 1));
    EXPECT_EQ(str.rfind("56", 3), base::string::npos);
    EXPECT_EQ(str.rfind("56", 3), res.rfind(std::string("56"), 3));
    EXPECT_EQ(str.rfind("23", 10), 1);
    EXPECT_EQ(str.rfind("23", 10), res.rfind(std::string("23"), 10));

    // rfind(const value_type*, size_type, size_type) const
    EXPECT_EQ(str.rfind("23", 1, 2), 1);
    EXPECT_EQ(str.rfind("23", 1, 2), res.rfind(std::string("23"), 1));
    EXPECT_EQ(str.rfind("56", 3, 2), base::string::npos);
    EXPECT_EQ(str.rfind("56", 3, 2), res.rfind(std::string("56"), 3));
    EXPECT_EQ(str.rfind("23", 10, 2), 1);
    EXPECT_EQ(str.rfind("23", 10, 2), res.rfind(std::string("23"), 10));
}

TEST(StringTest, find_first_of)
{
    base::string str("123123");
    std::string res("123123");

    EXPECT_EQ(str.find_first_of(base::string()), base::string::npos);
    EXPECT_EQ(str.find_first_of(base::string()), res.find_first_of(std::string()));
    EXPECT_EQ(str.find_first_of(base::string("23")), 1);
    EXPECT_EQ(str.find_first_of(base::string("23")), res.find_first_of(std::string("23")));
    EXPECT_EQ(str.find_first_of(base::string("39")), 2);
    EXPECT_EQ(str.find_first_of(base::string("39")), res.find_first_of(std::string("39")));
    EXPECT_EQ(str.find_first_of(base::string("93")), 2);
    EXPECT_EQ(str.find_first_of(base::string("93")), res.find_first_of(std::string("93")));
    EXPECT_EQ(str.find_first_of(base::string("9")), base::string::npos);
    EXPECT_EQ(str.find_first_of(base::string("9")), res.find_first_of(std::string("9")));

    EXPECT_EQ(str.find_first_of(base::string("23"), 10), base::string::npos);
    EXPECT_EQ(str.find_first_of(base::string("23"), 10), res.find_first_of(std::string("23"), 10));
    EXPECT_EQ(str.find_first_of(base::string("23"), 2), 2);
    EXPECT_EQ(str.find_first_of(base::string("23"), 2), res.find_first_of(std::string("23"), 2));
    EXPECT_EQ(str.find_first_of(base::string("23"), 3), 4);
    EXPECT_EQ(str.find_first_of(base::string("23"), 3), res.find_first_of(std::string("23"), 3));

    EXPECT_EQ(str.find_first_of('2', 1), 1);
    EXPECT_EQ(str.find_first_of('2', 1), res.find_first_of('2', 1));
    EXPECT_EQ(str.find_first_of('2', 2), 4);
    EXPECT_EQ(str.find_first_of('2', 2), res.find_first_of('2', 2));
    EXPECT_EQ(str.find_first_of('2', 5), base::string::npos);
    EXPECT_EQ(str.find_first_of('2', 5), res.find_first_of('2', 5));

    EXPECT_EQ(str.find_first_of("23", 2), 2);
    EXPECT_EQ(str.find_first_of("23", 2), res.find_first_of("23", 2));
    EXPECT_EQ(str.find_first_of("23", 3), 4);
    EXPECT_EQ(str.find_first_of("23", 3), res.find_first_of("23", 3));
    EXPECT_EQ(str.find_first_of("23", 10), base::string::npos);
    EXPECT_EQ(str.find_first_of("23", 10), res.find_first_of("23", 10));

    EXPECT_EQ(str.find_first_of("23", 2, 2), 2);
    EXPECT_EQ(str.find_first_of("23", 2, 2), res.find_first_of("23", 2, 2));
    EXPECT_EQ(str.find_first_of("23", 3, 2), 4);
    EXPECT_EQ(str.find_first_of("23", 3, 2), res.find_first_of("23", 3, 2));
    EXPECT_EQ(str.find_first_of("23", 2, 1), 4);
    EXPECT_EQ(str.find_first_of("23", 2, 1), res.find_first_of("23", 2, 1));
    EXPECT_EQ(str.find_first_of("23", 5, 1), base::string::npos);
    EXPECT_EQ(str.find_first_of("23", 5, 1), res.find_first_of("23", 5, 1));
    EXPECT_EQ(str.find_first_of("23", 10, 2), base::string::npos);
    EXPECT_EQ(str.find_first_of("23", 10, 2), res.find_first_of("23", 10, 2));
}

TEST(StringTest, find_first_not_of)
{
    base::string str("123123");
    std::string res("123123");

    EXPECT_EQ(str.find_first_not_of(base::string()), 0);
    EXPECT_EQ(str.find_first_not_of(base::string()), res.find_first_not_of(std::string()));
    EXPECT_EQ(str.find_first_not_of(base::string("23")), 0);
    EXPECT_EQ(str.find_first_not_of(base::string("23")), res.find_first_not_of(std::string("23")));
    EXPECT_EQ(str.find_first_not_of(base::string("39")), 0);
    EXPECT_EQ(str.find_first_not_of(base::string("39")), res.find_first_not_of(std::string("39")));
    EXPECT_EQ(str.find_first_not_of(base::string("93")), 0);
    EXPECT_EQ(str.find_first_not_of(base::string("93")), res.find_first_not_of(std::string("93")));

    EXPECT_EQ(str.find_first_not_of(base::string("23"), 10), base::string::npos);
    EXPECT_EQ(str.find_first_not_of(base::string("23"), 10), res.find_first_not_of(std::string("23"), 10));
    EXPECT_EQ(str.find_first_not_of(base::string("23"), 3), 3);
    EXPECT_EQ(str.find_first_not_of(base::string("23"), 3), res.find_first_not_of(std::string("23"), 3));
    EXPECT_EQ(str.find_first_not_of(base::string("23"), 1), 3);
    EXPECT_EQ(str.find_first_not_of(base::string("23"), 1), res.find_first_not_of(std::string("23"), 1));
    EXPECT_EQ(str.find_first_not_of(base::string("23"), 4), base::string::npos);
    EXPECT_EQ(str.find_first_not_of(base::string("23"), 4), res.find_first_not_of(std::string("23"), 4));

    EXPECT_EQ(str.find_first_not_of('2', 2), 2);
    EXPECT_EQ(str.find_first_not_of('2', 2), res.find_first_not_of('2', 2));
    EXPECT_EQ(str.find_first_not_of('2', 4), 5);
    EXPECT_EQ(str.find_first_not_of('2', 4), res.find_first_not_of('2', 4));

    EXPECT_EQ(str.find_first_not_of("23", 10), base::string::npos);
    EXPECT_EQ(str.find_first_not_of("23", 10), res.find_first_not_of("23", 10));
    EXPECT_EQ(str.find_first_not_of("23", 3), 3);
    EXPECT_EQ(str.find_first_not_of("23", 3), res.find_first_not_of("23", 3));

    EXPECT_EQ(str.find_first_not_of("23", 10, 2), base::string::npos);
    EXPECT_EQ(str.find_first_not_of("23", 10, 2), res.find_first_not_of("23", 10, 2));
    EXPECT_EQ(str.find_first_not_of("23", 3, 2), 3);
    EXPECT_EQ(str.find_first_not_of("23", 3, 2), res.find_first_not_of("23", 3, 2));
    EXPECT_EQ(str.find_first_not_of("23", 2, 1), 2);
    EXPECT_EQ(str.find_first_not_of("23", 2, 1), res.find_first_not_of("23", 2, 1));
}

TEST(StringTest, find_last_of)
{
    base::string str("123123");
    std::string res("123123");

    EXPECT_EQ(str.find_last_of(base::string()), base::string::npos);
    EXPECT_EQ(str.find_last_of(base::string()), res.find_last_of(std::string()));
    EXPECT_EQ(str.find_last_of(base::string("23")), 5);
    EXPECT_EQ(str.find_last_of(base::string("23")), res.find_last_of(std::string("23")));
    EXPECT_EQ(str.find_last_of(base::string("39")), 5);
    EXPECT_EQ(str.find_last_of(base::string("39")), res.find_last_of(std::string("39")));
    EXPECT_EQ(str.find_last_of(base::string("93")), 5);
    EXPECT_EQ(str.find_last_of(base::string("93")), res.find_last_of(std::string("93")));
    EXPECT_EQ(str.find_last_of(base::string("9")), base::string::npos);
    EXPECT_EQ(str.find_last_of(base::string("9")), res.find_last_of(std::string("9")));

    EXPECT_EQ(str.find_last_of(base::string("23"), 10), 5);
    EXPECT_EQ(str.find_last_of(base::string("23"), 10), res.find_last_of(std::string("23"), 10));
    EXPECT_EQ(str.find_last_of(base::string("23"), 3), 2);
    EXPECT_EQ(str.find_last_of(base::string("23"), 3), res.find_last_of(std::string("23"), 3));
    EXPECT_EQ(str.find_last_of(base::string("23"), 1), 1);
    EXPECT_EQ(str.find_last_of(base::string("23"), 1), res.find_last_of(std::string("23"), 1));
    EXPECT_EQ(str.find_last_of(base::string("23"), 0), base::string::npos);
    EXPECT_EQ(str.find_last_of(base::string("23"), 0), res.find_last_of(std::string("23"), 0));

    EXPECT_EQ(str.find_last_of('2', 2), 1);
    EXPECT_EQ(str.find_last_of('2', 2), res.find_last_of('2', 2));
    EXPECT_EQ(str.find_last_of('2', 4), 4);
    EXPECT_EQ(str.find_last_of('2', 4), res.find_last_of('2', 4));

    EXPECT_EQ(str.find_last_of("23", 10), 5);
    EXPECT_EQ(str.find_last_of("23", 10), res.find_last_of("23", 10));
    EXPECT_EQ(str.find_last_of("23", 3), 2);
    EXPECT_EQ(str.find_last_of("23", 3), res.find_last_of("23", 3));
    EXPECT_EQ(str.find_last_of("23", 0), base::string::npos);
    EXPECT_EQ(str.find_last_of("23", 0), res.find_last_of("23", 0));

    EXPECT_EQ(str.find_last_of("23", 10, 2), 5);
    EXPECT_EQ(str.find_last_of("23", 10, 2), res.find_last_of("23", 10, 2));
    EXPECT_EQ(str.find_last_of("23", 3, 2), 2);
    EXPECT_EQ(str.find_last_of("23", 3, 2), res.find_last_of("23", 3, 2));
    EXPECT_EQ(str.find_last_of("23", 2, 1), 1);
    EXPECT_EQ(str.find_last_of("23", 2, 1), res.find_last_of("23", 2, 1));
}

TEST(StringTest, find_last_not_of)
{
    base::string str("123123");
    std::string res("123123");

    EXPECT_EQ(str.find_last_not_of(base::string()), 5);
    EXPECT_EQ(str.find_last_not_of(base::string()), res.find_last_not_of(std::string()));
    EXPECT_EQ(str.find_last_not_of(base::string("23")), 3);
    EXPECT_EQ(str.find_last_not_of(base::string("23")), res.find_last_not_of(std::string("23")));
    EXPECT_EQ(str.find_last_not_of(base::string("39")), 4);
    EXPECT_EQ(str.find_last_not_of(base::string("39")), res.find_last_not_of(std::string("39")));
    EXPECT_EQ(str.find_last_not_of(base::string("93")), 4);
    EXPECT_EQ(str.find_last_not_of(base::string("93")), res.find_last_not_of(std::string("93")));

    EXPECT_EQ(str.find_last_not_of(base::string("23"), 10), 3);
    EXPECT_EQ(str.find_last_not_of(base::string("23"), 10), res.find_last_not_of(std::string("23"), 10));
    EXPECT_EQ(str.find_last_not_of(base::string("23"), 3), 3);
    EXPECT_EQ(str.find_last_not_of(base::string("23"), 3), res.find_last_not_of(std::string("23"), 3));
    EXPECT_EQ(str.find_last_not_of(base::string("23"), 1), 0);
    EXPECT_EQ(str.find_last_not_of(base::string("23"), 1), res.find_last_not_of(std::string("23"), 1));
    EXPECT_EQ(str.find_last_not_of(base::string("23"), 0), 0);
    EXPECT_EQ(str.find_last_not_of(base::string("23"), 0), res.find_last_not_of(std::string("23"), 0));

    EXPECT_EQ(str.find_last_not_of('2', 2), 2);
    EXPECT_EQ(str.find_last_not_of('2', 2), res.find_last_not_of('2', 2));
    EXPECT_EQ(str.find_last_not_of('2', 4), 3);
    EXPECT_EQ(str.find_last_not_of('2', 4), res.find_last_not_of('2', 4));
    EXPECT_EQ(str.find_last_not_of('1', 0), base::string::npos);
    EXPECT_EQ(str.find_last_not_of('1', 0), res.find_last_not_of('1', 0));

    EXPECT_EQ(str.find_last_not_of("23", 10), 3);
    EXPECT_EQ(str.find_last_not_of("23", 10), res.find_last_not_of("23", 10));
    EXPECT_EQ(str.find_last_not_of("23", 2), 0);
    EXPECT_EQ(str.find_last_not_of("23", 2), res.find_last_not_of("23", 2));

    EXPECT_EQ(str.find_last_not_of("23", 10, 2), 3);
    EXPECT_EQ(str.find_last_not_of("23", 10, 2), res.find_last_not_of("23", 10, 2));
    EXPECT_EQ(str.find_last_not_of("23", 3, 2), 3);
    EXPECT_EQ(str.find_last_not_of("23", 3, 2), res.find_last_not_of("23", 3, 2));
    EXPECT_EQ(str.find_last_not_of("23", 2, 1), 2);
    EXPECT_EQ(str.find_last_not_of("23", 2, 1), res.find_last_not_of("23", 2, 1));
}

TEST(StringTest, operator_2)
{
    {
        base::string str("123456");
        char tmp[56]{};
        size_t ret = str.copy(tmp, 8, 0);
        EXPECT_EQ(strcmp(tmp, "123456"), 0);
        EXPECT_EQ(ret, 6);

        str = "1234567890asdfghzxioch";
        ret = str.copy(tmp, 10, 0);
        EXPECT_EQ(strcmp(tmp, "1234567890"), 0);
        EXPECT_EQ(ret, 10);

        ret = str.copy(tmp, 10, 5);
        EXPECT_EQ(strcmp(tmp, "67890asdfg"), 0);
        EXPECT_EQ(ret, 10);
    }

    {
        base::string str("abcdefg");
        EXPECT_EQ(str.upper().compare("ABCDEFG"), 0);
        
        str = "abDcf3eWH";
        EXPECT_EQ(str.upper().compare("ABDCF3EWH"), 0);

        str = "abDc23feWH";
        EXPECT_EQ(str.lower().compare("abdc23fewh"), 0);

        str = "ABCAWSEF234.09AJSDF@";
        EXPECT_EQ(str.lower().compare("abcawsef234.09ajsdf@"), 0);
    }

    {
        std::unordered_map<base::string, int> test;

        base::string s1("1234567890");
        base::string s2("12345");

        test[s1] = 10;
        test[s2] = 20;
        EXPECT_EQ(test[s1], 10);
        EXPECT_EQ(test[s2], 20);

        test[s1] = 30;
        EXPECT_EQ(test[s1], 30);
    }

    {
        base::string str1("1234567890");
        base::string str2("abcdefghijk");
        base::swap(str1, str2);

        EXPECT_EQ(str1.compare("abcdefghijk"), 0);
        EXPECT_EQ(str2.compare("1234567890"), 0);
    }
}

TEST(StringTest, operator_3)
{
    base::string tmp1{ "hello " };
    base::string tmp2{ "world" };

    base::string res = tmp1 + tmp2;
    EXPECT_EQ(res.compare("hello world"), 0);

    res = std::move(tmp1) + tmp2;
    EXPECT_EQ(res.compare("hello world"), 0);
    EXPECT_EQ(tmp1.size(), 0);

    tmp1 = "hello ";
    res = tmp1 + std::move(tmp2);
    EXPECT_EQ(res.compare("hello world"), 0);
    EXPECT_EQ(tmp2.size(), 0);

    tmp2 = "world";
    res = std::move(tmp1) + std::move(tmp2);
    EXPECT_EQ(res.compare("hello world"), 0);
    EXPECT_EQ(tmp1.size(), 0);

    tmp2 = "world";
    res = "hello " + tmp2;
    EXPECT_EQ(res.compare("hello world"), 0);

    res = "hello " + std::move(tmp2);
    EXPECT_EQ(res.compare("hello world"), 0);
    EXPECT_EQ(tmp2.size(), 0);

    tmp1 = "hello";
    res = tmp1 + " world";
    EXPECT_EQ(res.compare("hello world"), 0);

    res = std::move(tmp1) + " world";
    EXPECT_EQ(res.compare("hello world"), 0);
    EXPECT_EQ(tmp1.size(), 0);

    tmp2 = "orld";
    res = 'w' + tmp2;
    EXPECT_EQ(res.compare("world"), 0);

    res = "w" + std::move(tmp2);
    EXPECT_EQ(res.compare("world"), 0);
    EXPECT_EQ(tmp2.size(), 0);

    tmp1 = "hell";
    res = tmp1 + "o";
    EXPECT_EQ(res.compare("hello"), 0);

    res = std::move(tmp1) + "o";
    EXPECT_EQ(res.compare("hello"), 0);
    EXPECT_EQ(tmp1.size(), 0);
}

TEST(StringTest, operator_4)
{
    base::string str1{ "123456" };
    base::string str2{ "123" };
    base::string str3{ "456" };
    base::string str4;
    // operator ==
    EXPECT_EQ(str1 == base::string("123456"), true);
    EXPECT_EQ(str4 == base::string(), true);
    EXPECT_EQ(str1 == str2, false);
    EXPECT_EQ(str4 == str2, false);

    // operator !=
    EXPECT_EQ(str1 != base::string("123456"), false);
    EXPECT_EQ(str4 != base::string(), false);
    EXPECT_EQ(str1 != str2, true);
    EXPECT_EQ(str4 != str2, true);

    // operator <
    EXPECT_EQ(str1 < base::string("123456"), false);
    EXPECT_EQ(str4 < base::string(), false);
    EXPECT_EQ(str1 < str2, false);
    EXPECT_EQ(str1 < str3, true);
    EXPECT_EQ(str4 < str2, true);
    EXPECT_EQ(str4 < str3, true);

    // operator >
    EXPECT_EQ(str1 > base::string("123456"), false);
    EXPECT_EQ(str4 > base::string(), false);
    EXPECT_EQ(str1 > str2, true);
    EXPECT_EQ(str1 > str3, false);
    EXPECT_EQ(str4 > str2, false);
    EXPECT_EQ(str4 > str3, false);

    // operator <=
    EXPECT_EQ(str1 <= base::string("123456"), true);
    EXPECT_EQ(str4 <= base::string(), true);
    EXPECT_EQ(str1 <= str2, false);
    EXPECT_EQ(str1 <= str3, true);
    EXPECT_EQ(str4 <= str2, true);
    EXPECT_EQ(str4 <= str3, true);

    // operator >=
    EXPECT_EQ(str1 >= base::string("123456"), true);
    EXPECT_EQ(str4 >= base::string(), true);
    EXPECT_EQ(str1 >= str2, true);
    EXPECT_EQ(str1 >= str3, false);
    EXPECT_EQ(str4 >= str2, false);
    EXPECT_EQ(str4 >= str3, false);

    // std::string
    std::string res2{ "123" };
    std::string res3{ "456" };

    // operator ==
    EXPECT_EQ(str1 == std::string("123456"), true);
    EXPECT_EQ(std::string("123456") == str1, true);
    EXPECT_EQ(str4 == std::string(), true);
    EXPECT_EQ(std::string() == str4, true);
    EXPECT_EQ(str1 == res2, false);
    EXPECT_EQ(res2 == str1, false);
    EXPECT_EQ(str1 == res3, false);
    EXPECT_EQ(res3 == str1, false);
    EXPECT_EQ(str4 == res2, false);
    EXPECT_EQ(res2 == str4, false);
    EXPECT_EQ(str4 == res3, false);
    EXPECT_EQ(res3 == str4, false);

    // operator !=
    EXPECT_EQ(str1 != std::string("123456"), false);
    EXPECT_EQ(std::string("123456") != str1, false);
    EXPECT_EQ(str4 != std::string(), false);
    EXPECT_EQ(std::string() != str4, false);
    EXPECT_EQ(str1 != res2, true);
    EXPECT_EQ(res2 != str1, true);
    EXPECT_EQ(str1 != res3, true);
    EXPECT_EQ(res3 != str1, true);
    EXPECT_EQ(str4 != res2, true);
    EXPECT_EQ(res2 != str4, true);
    EXPECT_EQ(str4 != res3, true);
    EXPECT_EQ(res3 != str4, true);

    // operator <
    EXPECT_EQ(str1 < std::string("123456"), false);
    EXPECT_EQ(std::string("123456") < str1, false);
    EXPECT_EQ(str4 < std::string(), false);
    EXPECT_EQ(std::string() < str4, false);
    EXPECT_EQ(str1 < res2, false);
    EXPECT_EQ(res2 < str1, true);
    EXPECT_EQ(str1 < res3, true);
    EXPECT_EQ(res3 < str1, false);
    EXPECT_EQ(str4 < res2, true);
    EXPECT_EQ(res2 < str4, false);
    EXPECT_EQ(str4 < res3, true);
    EXPECT_EQ(res3 < str4, false);

    // operator >
    EXPECT_EQ(str1 > std::string("123456"), false);
    EXPECT_EQ(std::string("123456") > str1, false);
    EXPECT_EQ(str4 > std::string(), false);
    EXPECT_EQ(std::string() > str4, false);
    EXPECT_EQ(str1 > res2, true);
    EXPECT_EQ(res2 > str1, false);
    EXPECT_EQ(str1 > res3, false);
    EXPECT_EQ(res3 > str1, true);
    EXPECT_EQ(str4 > res2, false);
    EXPECT_EQ(res2 > str4, true);
    EXPECT_EQ(str4 > res3, false);
    EXPECT_EQ(res3 > str4, true);

    // operator <=
    EXPECT_EQ(str1 <= std::string("123456"), true);
    EXPECT_EQ(std::string("123456") <= str1, true);
    EXPECT_EQ(str4 <= std::string(), true);
    EXPECT_EQ(std::string() <= str4, true);
    EXPECT_EQ(str1 <= res2, false);
    EXPECT_EQ(res2 <= str1, true);
    EXPECT_EQ(str1 <= res3, true);
    EXPECT_EQ(res3 <= str1, false);
    EXPECT_EQ(str4 <= res2, true);
    EXPECT_EQ(res2 <= str4, false);
    EXPECT_EQ(str4 <= res3, true);
    EXPECT_EQ(res3 <= str4, false);

    // operator >=
    EXPECT_EQ(str1 >= std::string("123456"), true);
    EXPECT_EQ(std::string("123456") >= str1, true);
    EXPECT_EQ(str4 >= std::string(), true);
    EXPECT_EQ(std::string() >= str4, true);
    EXPECT_EQ(str1 >= res2, true);
    EXPECT_EQ(res2 >= str1, false);
    EXPECT_EQ(str1 >= res3, false);
    EXPECT_EQ(res3 >= str1, true);
    EXPECT_EQ(str4 >= res2, false);
    EXPECT_EQ(res2 >= str4, true);
    EXPECT_EQ(str4 >= res3, false);
    EXPECT_EQ(res3 >= str4, true);

    // const char*
    const char* ch2{ "123" };
    const char* ch3{ "456" };

    // operator ==
    EXPECT_EQ(str1 == "123456", true);
    EXPECT_EQ("123456" == str1, true);
    EXPECT_EQ(str4 == "", true);
    EXPECT_EQ("" == str4, true);
    EXPECT_EQ(str1 == ch2, false);
    EXPECT_EQ(ch2 == str1, false);
    EXPECT_EQ(str1 == ch3, false);
    EXPECT_EQ(ch3 == str1, false);
    EXPECT_EQ(str4 == ch2, false);
    EXPECT_EQ(ch2 == str4, false);
    EXPECT_EQ(str4 == ch3, false);
    EXPECT_EQ(ch3 == str4, false);

    // operator !=
    EXPECT_EQ(str1 != std::string("123456"), false);
    EXPECT_EQ(std::string("123456") != str1, false);
    EXPECT_EQ(str4 != std::string(), false);
    EXPECT_EQ(std::string() != str4, false);
    EXPECT_EQ(str1 != ch2, true);
    EXPECT_EQ(ch2 != str1, true);
    EXPECT_EQ(str1 != ch3, true);
    EXPECT_EQ(ch3 != str1, true);
    EXPECT_EQ(str4 != ch2, true);
    EXPECT_EQ(ch2 != str4, true);
    EXPECT_EQ(str4 != ch3, true);
    EXPECT_EQ(ch3 != str4, true);

    // operator <
    EXPECT_EQ(str1 < std::string("123456"), false);
    EXPECT_EQ(std::string("123456") < str1, false);
    EXPECT_EQ(str4 < std::string(), false);
    EXPECT_EQ(std::string() < str4, false);
    EXPECT_EQ(str1 < ch2, false);
    EXPECT_EQ(ch2 < str1, true);
    EXPECT_EQ(str1 < ch3, true);
    EXPECT_EQ(ch3 < str1, false);
    EXPECT_EQ(str4 < ch2, true);
    EXPECT_EQ(ch2 < str4, false);
    EXPECT_EQ(str4 < ch3, true);
    EXPECT_EQ(ch3 < str4, false);

    // operator >
    EXPECT_EQ(str1 > std::string("123456"), false);
    EXPECT_EQ(std::string("123456") > str1, false);
    EXPECT_EQ(str4 > std::string(), false);
    EXPECT_EQ(std::string() > str4, false);
    EXPECT_EQ(str1 > ch2, true);
    EXPECT_EQ(ch2 > str1, false);
    EXPECT_EQ(str1 > ch3, false);
    EXPECT_EQ(ch3 > str1, true);
    EXPECT_EQ(str4 > ch2, false);
    EXPECT_EQ(ch2 > str4, true);
    EXPECT_EQ(str4 > ch3, false);
    EXPECT_EQ(ch3 > str4, true);

    // operator <=
    EXPECT_EQ(str1 <= std::string("123456"), true);
    EXPECT_EQ(std::string("123456") <= str1, true);
    EXPECT_EQ(str4 <= std::string(), true);
    EXPECT_EQ(std::string() <= str4, true);
    EXPECT_EQ(str1 <= ch2, false);
    EXPECT_EQ(ch2 <= str1, true);
    EXPECT_EQ(str1 <= ch3, true);
    EXPECT_EQ(ch3 <= str1, false);
    EXPECT_EQ(str4 <= ch2, true);
    EXPECT_EQ(ch2 <= str4, false);
    EXPECT_EQ(str4 <= ch3, true);
    EXPECT_EQ(ch3 <= str4, false);

    // operator >=
    EXPECT_EQ(str1 >= std::string("123456"), true);
    EXPECT_EQ(std::string("123456") >= str1, true);
    EXPECT_EQ(str4 >= std::string(), true);
    EXPECT_EQ(std::string() >= str4, true);
    EXPECT_EQ(str1 >= ch2, true);
    EXPECT_EQ(ch2 >= str1, false);
    EXPECT_EQ(str1 >= ch3, false);
    EXPECT_EQ(ch3 >= str1, true);
    EXPECT_EQ(str4 >= ch2, false);
    EXPECT_EQ(ch2 >= str4, true);
    EXPECT_EQ(str4 >= ch3, false);
    EXPECT_EQ(ch3 >= str4, true);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}