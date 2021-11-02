#pragma once
#include <functional>
#include <optional>
#include <type_traits>

namespace fredis {

struct Nothing {
  explicit constexpr Nothing(int) {}
};

inline constexpr Nothing nothing{0};

template <typename T>
class Maybe {
 public:
  using value_t = T;

  constexpr Maybe() noexcept = default;
  constexpr Maybe(Nothing) noexcept;
  template <typename U = T>
  constexpr Maybe(U&& v);

  constexpr explicit operator bool() const noexcept;
  constexpr bool hasValue() const noexcept;

  constexpr const value_t& value() const&;
  constexpr const value_t&& value() const&&;
  constexpr value_t&& value() &&;

  template <typename U>
  constexpr T otherwise(U&& defVal) const&;

  template <typename F>
  constexpr auto map(F&& f) const&;

  template <typename F>
  constexpr auto map(F&& f) &&;

  template <typename F>
  constexpr auto flatMap(F&& f) const&;

  template <typename F>
  constexpr auto flatMap(F&& f) &&;

 private:
  template <typename L, typename R>
  friend constexpr bool operator==(const Maybe<L>&, const R&);
  template <typename L, typename R>
  friend constexpr bool operator==(const L&, const Maybe<R>&);
  template <typename L, typename R>
  friend constexpr bool operator<(const Maybe<L>&, const R&);
  template <typename L, typename R>
  friend constexpr bool operator<(const L&, const Maybe<R>&);
  template <typename L, typename R>
  friend constexpr bool operator>(const Maybe<L>&, const R&);
  template <typename L, typename R>
  friend constexpr bool operator>(const L&, const Maybe<R>&);
  template <typename L, typename R>
  friend constexpr bool operator<=(const Maybe<L>&, const R&);
  template <typename L, typename R>
  friend constexpr bool operator<=(const L&, const Maybe<R>&);
  template <typename L, typename R>
  friend constexpr bool operator>=(const Maybe<L>&, const R&);
  template <typename L, typename R>
  friend constexpr bool operator>=(const L&, const Maybe<R>&);

  std::optional<T> value_;
};

template <typename T>
constexpr Maybe<T> some(T&& v) {
  return Maybe<T>(std::forward<T>(v));
}

template <typename T>
constexpr Maybe<T> none() {
  return Maybe<T>();
}

template <typename T, typename U>
constexpr bool operator==(const Maybe<T>& lhs, const U& rhs) {
  return lhs.value_ == rhs;
}

template <typename L, typename R>
constexpr bool operator==(const L& lhs, const Maybe<R>& rhs) {
  return lhs == rhs.value_;
}

template <typename L>
constexpr bool operator==(const Maybe<L>& lhs, Nothing) noexcept {
  return !lhs;
}

template <typename R>
constexpr bool operator==(Nothing, const Maybe<R>& rhs) noexcept {
  return !rhs;
}

template <typename T, typename U>
constexpr bool operator!=(const Maybe<T>& lhs, const U& rhs) {
  return !(lhs == rhs);
}

template <typename L, typename R>
constexpr bool operator!=(const L& lhs, const Maybe<R>& rhs) {
  return !(lhs == rhs);
}

template <typename L>
constexpr bool operator!=(const Maybe<L>& lhs, Nothing) noexcept {
  return lhs.hasValue();
}

template <typename R>
constexpr bool operator!=(Nothing, const Maybe<R>& rhs) noexcept {
  return rhs.hasValue();
}

template <typename L, typename R>
constexpr bool operator<(const Maybe<L>& lhs, const R& rhs) {
  return lhs.value_ < rhs;
}

template <typename L, typename R>
constexpr bool operator<(const L& lhs, const Maybe<R>& rhs) {
  return lhs < rhs.value_;
}

template <typename L, typename R>
constexpr bool operator>(const L& lhs, const Maybe<R>& rhs) {
  return lhs > rhs.value_;
}

template <typename L, typename R>
constexpr bool operator>(const Maybe<L>& lhs, const R& rhs) {
  return lhs.value_ > rhs;
}

template <typename L, typename R>
constexpr bool operator<=(const Maybe<L>& lhs, const R& rhs) {
  return lhs.value_ <= rhs;
}

template <typename L, typename R>
constexpr bool operator<=(const L& lhs, const Maybe<R>& rhs) {
  return lhs <= rhs.value_;
}

template <typename L, typename R>
constexpr bool operator>=(const Maybe<L>& lhs, const R& rhs) {
  return lhs.value_ >= rhs;
}

template <typename L, typename R>
constexpr bool operator>=(const L& lhs, const Maybe<R>& rhs) {
  return lhs >= rhs.value_;
}

}  // namespace fredis

#include "detail/Maybe_detail.hpp"