#ifndef LILY_RUNTIME_MUT_HPP
#define LILY_RUNTIME_MUT_HPP

template<typename T>
class Mut
{
    T _;

  public:
    Mut(T _)
      : _(_)
    {
    }

    auto operator=(T v) noexcept -> void { _ = v; }

    auto getValue() const noexcept -> T { return _; }
};

#endif // LILY_RUNTIME_MUT_HPP
