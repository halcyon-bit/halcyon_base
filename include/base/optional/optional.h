#ifndef HALCYON_BASE_OPTIONAL_H
#define HALCYON_BASE_OPTIONAL_H

#include <base/common/base_define.h>

#include <memory>
#include <stdexcept>
#include <type_traits>

HALCYON_BASE_BEGIN_NAMESPACE

class BadOptionalAccess : public std::exception
{
public:
    BadOptionalAccess() = default;
    ~BadOptionalAccess() override = default;
    const char* what() const noexcept override
    {
        return "Optional has no value";
    }
};

/**
 * @brief  Optional 管理一个值，既可以存在也可以不存在的值，只有当 Optional<T>
 *     被初始化之后，这个 Optional<T> 才是有效的。
 *
 * @ps  C++17 中已有 std::optional 类型
 */
template<typename T>
class Optional final
{
    using value_type = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;

public:
    Optional() = default;

    Optional(const T& val)
    {
        Create(val);
    }
    Optional(T&& val)
    {
        Create(std::forward<T>(val));
    }

    Optional(const Optional& rhs)
    {
        if (rhs.HasValue()) {
            Copy(rhs.data_);
        }
    }
    Optional(Optional&& rhs) noexcept
    {
        if (rhs.HasValue()) {
            Move(std::move(rhs.data_));
            rhs.Destroy();
        }
    }

    ~Optional()
    {
        Destroy();
    }

    Optional& operator=(const Optional& rhs)
    {
        if (this == &rhs) {
            return *this;
        }
        Destroy();  // 销毁自身
        if (rhs.HasValue()) {
            Copy(rhs.data_);
        }
        return *this;
    }
    Optional& operator=(Optional&& rhs) noexcept
    {
        if (this == &rhs) {
            return *this;
        }
        Destroy();  // 销毁自身
        if (rhs.HasValue()) {
            Move(std::move(rhs.data_));
            rhs.Destroy();
        }
        return *this;
    }

    Optional& operator=(const T& val)
    {
        Destroy();  // 销毁自身
        Create(val);
        return *this;
    }
    Optional& operator=(T&& val)
    {
        Destroy();  // 销毁自身
        Create(std::forward<T>(val));
        return *this;
    }

public:
    bool HasValue() const noexcept
    {
        return has_value_;
    }

    operator bool() const noexcept
    {
        return has_value_;
    }

public:
    T& operator*() noexcept
    {
        return Get();
    }
    const T& operator*() const noexcept
    {
        return Get();
    }

    T* operator->() noexcept
    {
        return std::addressof(Get());
    }
    const T* operator->() const noexcept
    {
        return std::addressof(Get());
    }

public:
    T& Value()
    {
        if (HasValue()) {
            return Get();
        }
        throw BadOptionalAccess();
    }

    const T& Value() const
    {
        if (HasValue()) {
            return Get();
        }
        throw BadOptionalAccess();
    }

    /**
     * @brief  获取值，若无值则返回默认值
     */
    template<typename U>
    T ValueOr(U&& default_value) const
    {
        return HasValue() ? Get() : static_cast<T>(std::forward<U>(default_value));
    }

    /**
     * @brief  就地构造/重新赋值
     */
    template<typename... Args>
    T& Emplace(Args&&... args)
    {
        Destroy();
        Create(std::forward<Args>(args)...);
        return Get();
    }

    /**
     * @brief  清空 Optional，销毁内部值
     */
     void Reset()
     {
         Destroy();
     }
 
private:
    template<typename... Args>
    void Create(Args&&... args)
    {
        new (&data_) T(std::forward<Args>(args)...);
        has_value_ = true;
    }

    void Destroy()
    {
        if (!has_value_) {
            return;
        }
        reinterpret_cast<T*>(&data_)->~T();
        has_value_ = false;
    }

    void Copy(const value_type& val)
    {
        new (&data_) T(*reinterpret_cast<const T*>(&val));
        has_value_ = true;
    }

    void Move(value_type&& val)
    {
        new (&data_) T(std::move(*reinterpret_cast<T*>(&val)));
        has_value_ = true;
    }

    T& Get() noexcept
    {
        return *reinterpret_cast<T*>(&data_);
    }

    const T& Get() const noexcept
    {
        return *reinterpret_cast<const T*>(&data_);
    }

private:
    bool has_value_{ false };
    value_type data_;
};

HALCYON_BASE_END_NAMESPACE

#endif
