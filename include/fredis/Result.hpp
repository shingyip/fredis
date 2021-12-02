#pragma once
#include <cstring>
#include <functional>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <variant>
namespace fredis {

struct ResultError {};

struct ErrorMsg : ResultError {
  ErrorMsg() : msg("error") {}
  ErrorMsg(const std::string& desc) : msg(desc) {}
  ErrorMsg(const char* desc) : msg(desc) {}
  std::string msg;
};

inline bool operator==(const ErrorMsg& lhs, const ErrorMsg& rhs) {
  return lhs.msg == rhs.msg;
}

struct UnexpectedResult : std::runtime_error {
  UnexpectedResult(const char* desc) : std::runtime_error(desc) {}
};

template <typename T, typename E = ErrorMsg>
class Result {
 public:
  using ok_t = T;
  using error_t = E;
  struct Unexpected : UnexpectedResult {
    Unexpected(const char* desc) : UnexpectedResult(desc) {}
  };

  constexpr Result(ok_t&& v) noexcept : var_(std::move(v)) {}
  constexpr Result(const ok_t& v) noexcept : var_(v) {}
  constexpr Result(error_t&& e) noexcept : var_(std::move(e)) {}
  constexpr Result(const error_t& e) noexcept : var_(e) {}

  constexpr bool isOk() const noexcept {
    return std::holds_alternative<ok_t>(var_);
  }

  constexpr bool isError() const noexcept { return !isOk(); }

  constexpr const ok_t& get() const& { return std::get<ok_t>(var_); }
  constexpr ok_t&& get() && { return std::get<ok_t>(std::move(var_)); }

  constexpr const error_t& getError() const& { return std::get<error_t>(var_); }
  constexpr error_t&& getError() & {
    return std::get<error_t>(std::move(var_));
  }

  template <typename F>
  constexpr Result then(F&& f) const&;

  template <typename F>
  constexpr void then(F&& f) &&;

  constexpr ok_t expect(const char* unexpectedMsg) const&;
  constexpr ok_t&& expect(const char* unexpectedMsg) &&;

  template <typename U = error_t,
            std::enable_if_t<std::is_same_v<ErrorMsg, U>, int> = 1>
  constexpr ok_t unwrap() const& {
    if (!isOk())
      throw Unexpected(getError().msg.c_str());
    return get();
  }

  template <typename U = error_t,
            std::enable_if_t<std::is_same_v<ErrorMsg, U>, int> = 1>
  constexpr ok_t&& unwrap() && {
    if (!isOk())
      throw Unexpected(getError().msg.c_str());
    return std::move(*this).get();
  }

  template <typename U = error_t,
            std::enable_if_t<!std::is_same_v<ErrorMsg, U>, int> = 1>
  constexpr ok_t unwrap() const& {
    if (!isOk())
      throw Unexpected("unexpected error");
    return get();
  }

  template <typename U = error_t,
            std::enable_if_t<!std::is_same_v<ErrorMsg, U>, int> = 1>
  constexpr ok_t&& unwrap() && {
    if (!isOk())
      throw Unexpected("unexpected error");
    return std::move(*this).get();
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
  std::variant<ok_t, error_t> var_;
};

template <typename T, typename E = ErrorMsg>
Result<T, E> ok(T&& v) {
  return Result<T, E>(std::forward<T>(v));
}

}  // namespace fredis

#include "detail/Result_detail.hpp"
