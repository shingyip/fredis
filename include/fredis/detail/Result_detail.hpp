#pragma once
#include "../Result.hpp"
namespace fredis {

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