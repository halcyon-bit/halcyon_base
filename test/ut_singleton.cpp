#include "base/singleton/singleton.h"

#include "gtest/gtest.h"

#include <thread>
#include <vector>
#include <atomic>
#include <type_traits>

using namespace halcyon;

// 用来测试的普通类型
struct Sample
{
    int x{0};
};

// 带 no_destroy 的类型
struct SampleNoDestroy
{
    static void no_destroy() {}  // 只要有这个静态成员即可
    int x{0};
};

// ------- 编译期特性检测（has_no_destroy） -------
TEST(SingletonTest, HasNoDestroyTrait)
{
    using halcyon::base::detail::has_no_destroy;

    static_assert(!has_no_destroy<Sample>::value,
                  "Sample 不应被检测到有 no_destroy");
    static_assert(has_no_destroy<SampleNoDestroy>::value,
                  "SampleNoDestroy 应被检测到有 no_destroy");
}

// ------- 单线程下的唯一实例与读写 -------
TEST(SingletonTest, SameInstanceSingleThread)
{
    auto& s1 = base::Singleton<Sample>::GetInstance();
    auto& s2 = base::Singleton<Sample>::GetInstance();

    // 地址必须相同
    EXPECT_EQ(&s1, &s2);

    s1.x = 42;
    EXPECT_EQ(42, s2.x);

    // 换一个类型也应是单例
    auto& n1 = base::Singleton<SampleNoDestroy>::GetInstance();
    auto& n2 = base::Singleton<SampleNoDestroy>::GetInstance();
    EXPECT_EQ(&n1, &n2);
    n1.x = 100;
    EXPECT_EQ(100, n2.x);
}

// ------- 多线程并发获取实例（验证 call_once 的正确性） -------
TEST(SingletonTest, SameInstanceMultiThread)
{
    constexpr int kThreadCount = 8;

    // 先触发一次构造，确保后面线程里拿到的实例已存在
    auto& mainInst = base::Singleton<Sample>::GetInstance();
    mainInst.x = 0;

    std::atomic<int> counter{0};
    std::vector<std::thread> threads;

    auto worker = [&]() {
        auto& inst = base::Singleton<Sample>::GetInstance();
        // 所有线程拿到的实例地址都必须与主线程相同
        EXPECT_EQ(&inst, &mainInst);
        counter.fetch_add(1, std::memory_order_relaxed);
    };

    threads.reserve(kThreadCount);
    for (int i = 0; i < kThreadCount; ++i) {
        threads.emplace_back(worker);
    }
    for (auto& t : threads) {
        t.join();
    }

    EXPECT_EQ(counter.load(std::memory_order_relaxed), kThreadCount);
}

// ------- Singleton 自身的不可拷贝特性 -------
TEST(SingletonTest, SingletonIsNonCopyable)
{
    using S = base::Singleton<Sample>;

    EXPECT_FALSE(std::is_copy_constructible<S>::value);
    EXPECT_FALSE(std::is_copy_assignable<S>::value);
    EXPECT_FALSE(std::is_move_constructible<S>::value);
    EXPECT_FALSE(std::is_move_assignable<S>::value);
}

int main(int argc, char* argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}