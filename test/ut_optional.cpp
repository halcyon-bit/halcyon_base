#include "base/optional/optional.h"

#include "gtest/gtest.h"

#include <string>

using namespace halcyon;

struct TestA
{
    int n;
    double f;
    std::string s;

    TestA(int n, double f, const std::string& s)
        : n(n), f(f), s(s)
    {}

    bool operator==(const TestA& rhs) const
    {
        return n == rhs.n && f == rhs.f && s == rhs.s;
    }
};

TEST(OptionalTest, DefaultAndBasicCtor)
{
    // 默认构造：无值
    base::Optional<int> o0;
    EXPECT_FALSE(o0.HasValue());
    EXPECT_FALSE(static_cast<bool>(o0));

    // 直接用值构造
    base::Optional<int> o1(42);
    EXPECT_TRUE(o1.HasValue());
    EXPECT_TRUE(static_cast<bool>(o1));
    EXPECT_EQ(*o1, 42);
    EXPECT_EQ(o1.Value(), 42);

    std::string s = "hello";
    base::Optional<std::string> o2(s);
    EXPECT_TRUE(o2.HasValue());
    EXPECT_EQ(*o2, s);

    base::Optional<std::string> o3(std::string("world"));
    EXPECT_TRUE(o3.HasValue());
    EXPECT_EQ(*o3, "world");
}

TEST(OptionalTest, CopyAndMove)
{
    base::Optional<int> src(10);
    base::Optional<int> empty;

    // 拷贝构造
    base::Optional<int> c1(src);
    EXPECT_TRUE(c1.HasValue());
    EXPECT_EQ(c1.Value(), 10);
    EXPECT_TRUE(src.HasValue());

    // 拷贝赋值：有值 -> 有值
    base::Optional<int> c2(1);
    c2 = src;
    EXPECT_TRUE(c2.HasValue());
    EXPECT_EQ(c2.Value(), 10);

    // 拷贝赋值：无值 -> 有值
    empty = src;
    EXPECT_TRUE(empty.HasValue());
    EXPECT_EQ(empty.Value(), 10);

    // 移动构造：有值 -> 新对象；源被 Destroy()
    base::Optional<int> m1(std::move(src));
    EXPECT_TRUE(m1.HasValue());
    EXPECT_EQ(m1.Value(), 10);
    EXPECT_FALSE(src.HasValue());

    // 移动赋值：有值 -> 有值
    base::Optional<int> m2(5);
    m2 = std::move(m1);
    EXPECT_TRUE(m2.HasValue());
    EXPECT_EQ(m2.Value(), 10);
    EXPECT_FALSE(m1.HasValue());

    // 移动赋值：无值 -> 有值
    base::Optional<int> m3;
    base::Optional<int> tmp(7);
    m3 = std::move(tmp);
    EXPECT_TRUE(m3.HasValue());
    EXPECT_EQ(m3.Value(), 7);
    EXPECT_FALSE(tmp.HasValue());
}

TEST(OptionalTest, AssignFromValueAndReset)
{
    base::Optional<std::string> o;

    // 从值赋
    o = std::string("hello");
    EXPECT_TRUE(o.HasValue());
    EXPECT_EQ(o.Value(), "hello");

    std::string s = "world";
    o = s;
    EXPECT_TRUE(o.HasValue());
    EXPECT_EQ(o.Value(), "world");

    // Reset 清空
    o.Reset();
    EXPECT_FALSE(o.HasValue());
    EXPECT_FALSE(static_cast<bool>(o));
}

TEST(OptionalTest, EmplaceAndAccess)
{
    base::Optional<std::string> o1;
    o1.Emplace("hello");
    EXPECT_TRUE(o1.HasValue());
    EXPECT_EQ(*o1, "hello");
    EXPECT_EQ(o1->size(), std::string("hello").size());

    // 再次 emplace 覆盖
    o1.Emplace(5, 'x');
    EXPECT_TRUE(o1.HasValue());
    EXPECT_EQ(o1.Value(), "xxxxx");

    base::Optional<TestA> o2;
    o2.Emplace(10, 0.5, "test");
    EXPECT_TRUE(o2.HasValue());
    EXPECT_EQ(o2->n, 10);
    EXPECT_DOUBLE_EQ(o2->f, 0.5);
    EXPECT_EQ(o2->s, "test");
}

TEST(OptionalTest, ValueAndExceptions)
{
    base::Optional<int> o1;
    EXPECT_FALSE(o1.HasValue());

    // 无值调用 Value 必须抛 BadOptionalAccess
    EXPECT_THROW(o1.Value(), base::BadOptionalAccess);

    o1 = 100;
    EXPECT_NO_THROW({
        int v = o1.Value();
        (void)v;
    });
    EXPECT_EQ(o1.Value(), 100);

    const base::Optional<int> o2(200);
    EXPECT_EQ(o2.Value(), 200);
}

TEST(OptionalTest, ValueOr)
{
    base::Optional<int> o1;
    EXPECT_EQ(o1.ValueOr(5), 5);   // 无值，返回默认值

    o1 = 10;
    EXPECT_EQ(o1.ValueOr(5), 10);  // 有值，返回内部值

    base::Optional<std::string> o2;
    EXPECT_EQ(o2.ValueOr("default"), "default");

    o2 = std::string("real");
    EXPECT_EQ(o2.ValueOr("default"), "real");
}

TEST(OptionalTest, ConstAccess)
{
    const base::Optional<std::string> o1(std::string("const"));
    EXPECT_TRUE(o1.HasValue());
    EXPECT_EQ(*o1, "const");
    EXPECT_EQ(o1->size(), std::string("const").size());
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}