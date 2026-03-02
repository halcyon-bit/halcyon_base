#include "base/any/any.h"

#include "gtest/gtest.h"

#include <map>
#include <string>
#include <vector>

using namespace halcyon;

TEST(AnyTest, DefaultAndBasicTypes)
{
    // 默认构造：无值，类型为 void
    base::Any a0;
    EXPECT_FALSE(a0.HasValue());
    EXPECT_TRUE(a0.IsType<void>());

    // std::string
    std::string s{ "hello" };
    base::Any a1 = s;
    EXPECT_TRUE(a1.HasValue());
    EXPECT_TRUE(a1.IsType<std::string>());
    EXPECT_EQ(s, a1.AnyCast<std::string>());

    // int32_t
    base::Any a2 = int32_t{42};
    EXPECT_TRUE(a2.HasValue());
    EXPECT_TRUE(a2.IsType<int32_t>());
    EXPECT_EQ(42, a2.AnyCast<int32_t>());

    // int64_t 大数
    base::Any a3 = (int64_t)9223372036854775800LL;
    EXPECT_TRUE(a3.HasValue());
    EXPECT_TRUE(a3.IsType<int64_t>());
    EXPECT_EQ(9223372036854775800LL, a3.AnyCast<int64_t>());

    // float / double
    base::Any a4 = 3.14f;
    EXPECT_TRUE(a4.HasValue());
    EXPECT_TRUE(a4.IsType<float>());
    EXPECT_FLOAT_EQ(3.14f, a4.AnyCast<float>());

    base::Any a5 = 2.718281828;
    EXPECT_TRUE(a5.HasValue());
    EXPECT_TRUE(a5.IsType<double>());
    EXPECT_DOUBLE_EQ(2.718281828, a5.AnyCast<double>());

    // C 风格字符串（退化成 char*）
    char ch[] = "chars";
    base::Any a6 = ch;
    EXPECT_TRUE(a6.HasValue());
    EXPECT_TRUE(a6.IsType<char*>());
    EXPECT_STREQ(ch, a6.AnyCast<char*>());

    // STL 容器
    std::vector<int32_t> vec{1, 2, 3, 4, 5};
    base::Any a7 = vec;
    EXPECT_TRUE(a7.HasValue());
    EXPECT_TRUE(a7.IsType<std::vector<int32_t>>());
    EXPECT_EQ(vec, a7.AnyCast<std::vector<int32_t>>());

    std::map<int32_t, std::string> mp{{1, "a"}, {2, "b"}, {3, "c"}};
    base::Any a8 = mp;
    EXPECT_TRUE(a8.HasValue());
    bool is_map = a8.IsType<std::map<int32_t, std::string>>();
    EXPECT_TRUE(is_map);
    auto value = a8.AnyCast<std::map<int32_t, std::string>>();
    EXPECT_EQ(mp, value);
}

TEST(AnyTest, CopyAndMoveSemantics)
{
    std::vector<int32_t> vec{1, 2, 3};
    base::Any src = vec;

    // 拷贝构造
    base::Any c1 = src;
    EXPECT_TRUE(c1.HasValue());
    EXPECT_TRUE(c1.IsType<std::vector<int32_t>>());
    EXPECT_EQ(vec, c1.AnyCast<std::vector<int32_t>>());
    // 源不变
    EXPECT_TRUE(src.HasValue());
    EXPECT_TRUE(src.IsType<std::vector<int32_t>>());
    EXPECT_EQ(vec, src.AnyCast<std::vector<int32_t>>());

    // 拷贝赋值
    base::Any c2;
    c2 = src;
    EXPECT_TRUE(c2.HasValue());
    EXPECT_TRUE(c2.IsType<std::vector<int32_t>>());
    EXPECT_EQ(vec, c2.AnyCast<std::vector<int32_t>>());

    // 移动构造：目标有值，源变成 void 且无值
    base::Any m1 = std::move(src);
    EXPECT_TRUE(m1.HasValue());
    EXPECT_TRUE(m1.IsType<std::vector<int32_t>>());
    EXPECT_EQ(vec, m1.AnyCast<std::vector<int32_t>>());
    EXPECT_TRUE(src.IsType<void>());
    EXPECT_FALSE(src.HasValue());

    // 移动赋值
    base::Any m2;
    m2 = std::move(m1);
    EXPECT_TRUE(m2.HasValue());
    EXPECT_TRUE(m2.IsType<std::vector<int32_t>>());
    EXPECT_EQ(vec, m2.AnyCast<std::vector<int32_t>>());
    EXPECT_TRUE(m1.IsType<void>());
    EXPECT_FALSE(m1.HasValue());

    // 模板赋值（覆盖原值）
    int n = 100;
    m2 = n;
    EXPECT_TRUE(m2.HasValue());
    EXPECT_TRUE(m2.IsType<int>());
    EXPECT_EQ(100, m2.AnyCast<int>());

    // self-assign 安全性（拷贝）
    m2 = m2;
    EXPECT_TRUE(m2.HasValue());
    EXPECT_TRUE(m2.IsType<int>());
    EXPECT_EQ(100, m2.AnyCast<int>());

    // self-assign 安全性（移动）
    m2 = std::move(m2);
    // 这里语义上允许实现自定义，但当前实现中自移动后仍为 int
    EXPECT_TRUE(m2.IsType<int>());
    EXPECT_TRUE(m2.HasValue());
    EXPECT_EQ(100, m2.AnyCast<int>());
}

TEST(AnyTest, AnyCastException)
{
    base::Any a = 1;          // int32_t
    EXPECT_TRUE(a.HasValue());
    EXPECT_TRUE(a.IsType<int32_t>());

    // 错误类型 AnyCast 必须抛 std::bad_cast
    EXPECT_THROW(a.AnyCast<double>(), std::bad_cast);
    EXPECT_THROW(a.AnyCast<std::string>(), std::bad_cast);

    // 空 Any 上 AnyCast 也应抛异常
    base::Any empty;
    EXPECT_FALSE(empty.HasValue());
    EXPECT_TRUE(empty.IsType<void>());
    EXPECT_THROW(empty.AnyCast<int>(), std::bad_cast);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}