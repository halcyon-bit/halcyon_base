export module noncopyable;

namespace halcyon::base {

/**
 * @brief   对象语义的基类
 */
export class NonCopyable
{
public:
    NonCopyable(const NonCopyable&) = delete;
    void operator=(const NonCopyable&) = delete;

protected:
    NonCopyable() = default;
    ~NonCopyable() = default;
};

}