#pragma once
#include "../Result.hpp"
namespace fredis {

template <typename T, typename E>
template <typename F>
constexpr Result<T, E> Result<T, E>::then(F&& f) const& {
  if (isOk())
    f(get());

  return *this;
}

template <typename T, typename E>
template <typename F>
constexpr void Result<T, E>::then(F&& f) && {
  if (isOk())
    f(std::move(*this).get());
}

template <typename T, typename E>
constexpr T Result<T, E>::expect(const char* unexpectedMsg) const& {
  if (!isOk())
    throw Unexpected(unexpectedMsg);
  return get();
}

template <typename T, typename E>
constexpr T&& Result<T, E>::expect(const char* unexpectedMsg) && {
  if (!isOk())
    throw Unexpected(unexpectedMsg);
  return std::move(*this).get();
}

template <typename T, typename E>
template <typename F>
constexpr auto Result<T, E>::map(F&& f) const& {
  using result_t =
      Result<std::remove_cv_t<
                 std::remove_reference_t<std::invoke_result_t<F, const T>>>,
             E>;

  return isOk() ? result_t(std::invoke(std::forward<F>(f), get()))
                : result_t(getError());
}

template <typename T, typename E>
template <typename F>
constexpr auto Result<T, E>::map(F&& f) && {
  using result_t =
      Result<std::remove_cv_t<
                 std::remove_reference_t<std::invoke_result_t<F, const T>>>,
             E>;

  return isOk()
             ? result_t(std::invoke(std::forward<F>(f), std::move(this)->get()))
             : result_t(std::move(this)->getError());
}

template <typename T, typename E>
template <typename F>
constexpr auto Result<T, E>::flatMap(F&& f) const& {
  using result_t =
      Result<std::remove_cv_t<std::remove_reference_t<
                 typename std::invoke_result_t<F, const T>::value_t>>,
             E>;
  return isOk() ? f(get()) : result_t(getError());
}

template <typename T, typename E>
template <typename F>
constexpr auto Result<T, E>::flatMap(F&& f) && {
  using result_t =
      Result<std::remove_cv_t<std::remove_reference_t<
                 typename std::invoke_result_t<F, const T>::value_t>>,
             E>;
  return isOk() ? f(std::move(this)->get())
                : result_t(std::move(this)->getError());
}

}  // namespace fredis