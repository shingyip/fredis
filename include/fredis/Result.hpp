#pragma once
#include <cstring>
#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
namespace fredis {

struct ErrorMsg : std::runtime_error {
  ErrorMsg() : std::runtime_error("unknown error") {}
  ErrorMsg(const std::string& msg) : std::runtime_error(msg) {}
  ErrorMsg(const char* msg) : std::runtime_error(msg) {}
  ErrorMsg(const ErrorMsg&) noexcept = default;
  ErrorMsg& operator=(const ErrorMsg& other) noexcept {
    return *this = ErrorMsg(other.what());
  };
};

inline bool operator==(const ErrorMsg& lhs, const ErrorMsg& rhs) {
  return std::strcmp(lhs.what(), rhs.what()) == 0;
}

template <typename T, typename E = ErrorMsg>
class Result {
 public:
  using value_t = T;
  using error_t = E;

  constexpr Result(value_t&& v) noexcept : var_(std::move(v)) {}
  constexpr Result(const value_t& v) noexcept : var_(v) {}
  constexpr Result(error_t&& e) noexcept : var_(std::move(e)) {}
  constexpr Result(const error_t& e) noexcept : var_(e) {}

  constexpr bool isOk() const noexcept {
    return std::holds_alternative<value_t>(var_);
  }

  constexpr bool isError() const noexcept { return !isOk(); }

  constexpr const value_t& get() const& { return std::get<value_t>(var_); }
  constexpr value_t&& get() && { return std::get<value_t>(std::move(var_)); }

  constexpr const error_t& getError() const& { return std::get<error_t>(var_); }
  constexpr error_t&& getError() & {
    return std::get<error_t>(std::move(var_));
  }

  template <typename F>
  constexpr auto map(F&& f) const&;

  template <typename F>
  constexpr auto map(F&& f) &&;

  template <typename F>
  constexpr auto flatMap(F&& f) const&;

  template <typename F>
  constexpr auto flatMap(F&& f) &&;

 private:
  std::variant<value_t, error_t> var_;
};

template <typename T, typename E = ErrorMsg>
Result<T, E> ok(T&& v) {
  return Result<T, E>(std::forward<T>(v));
}

}  // namespace fredis

#include "detail/Result_detail.hpp"
