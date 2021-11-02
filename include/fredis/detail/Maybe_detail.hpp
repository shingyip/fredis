#pragma once
#include "../Maybe.hpp"

namespace fredis {
template <typename T>
constexpr Maybe<T>::Maybe(Nothing) noexcept : value_() {}

template <typename T>
template <typename U>
constexpr Maybe<T>::Maybe(U&& v) : value_(std::forward<U>(v)) {}

template <typename T>
constexpr Maybe<T>::operator bool() const noexcept {
  return hasValue();
}

template <typename T>
constexpr bool Maybe<T>::hasValue() const noexcept {
  return value_.has_value();
}

template <typename T>
constexpr const T& Maybe<T>::value() const& {
  return value_.value();
}
template <typename T>
constexpr const T&& Maybe<T>::value() const&& {
  return value_.value();
}
template <typename T>
constexpr T&& Maybe<T>::value() && {
  return std::move(value_).value();
}

template <typename T>
template <typename U>
constexpr T Maybe<T>::otherwise(U&& defVal) const& {
  return hasValue() ? value() : static_cast<T>(std::forward<U>(defVal));
}

template <typename T>
template <typename F>
constexpr auto Maybe<T>::map(F&& f) const& {
  if (hasValue())
    return some(std::invoke(std::forward<F>(f), value_.value()));
  else
    return some(std::remove_cv_t<
                std::remove_reference_t<std::invoke_result_t<F, const T>>>());
}

template <typename T>
template <typename F>
constexpr auto Maybe<T>::map(F&& f) && {
  if (hasValue())
    return some(std::invoke(std::forward<F>(f), std::move(value_.value())));
  else
    return some(std::remove_cv_t<
                std::remove_reference_t<std::invoke_result_t<F, const T>>>());
}

template <typename T>
template <typename F>
constexpr auto Maybe<T>::flatMap(F&& f) const& {
  if (hasValue())
    return std::invoke(std::forward<F>(f), value_.value());
  else
    return std::remove_cv_t<
        std::remove_reference_t<std::invoke_result_t<F, const T>>>();
}

template <typename T>
template <typename F>
constexpr auto Maybe<T>::flatMap(F&& f) && {
  if (hasValue())
    return std::invoke(std::forward<F>(f), std::move(value_.value()));
  else
    return std::remove_cv_t<
        std::remove_reference_t<std::invoke_result_t<F, const T>>>();
}

}  // namespace fredis