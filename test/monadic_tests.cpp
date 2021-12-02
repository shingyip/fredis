#include <catch2/catch_test_macros.hpp>
#include "../include/fredis/Maybe.hpp"
#include "../include/fredis/Result.hpp"

using namespace fredis;

TEST_CASE("Maybe", "[fredis]") {
  REQUIRE(1 + 1 == 2);
  Maybe<int> n(1);

  auto m1 = n.map([](auto v) { return v * 10; });
  CHECK(m1.value() == 10);

  auto m2 = n.map([](auto v) { return v * 10; }).map([](auto v) {
    return "v=" + std::to_string(v);
  });
  CHECK(m2 == "v=10");
  CHECK("v=10" == m2);

  auto m3 = Maybe<int>();
  auto m4 = nothing;
  CHECK(m3 == nothing);
  CHECK(m3 == m4);

  CHECK(some(123) < 124);
  CHECK(some(123) <= 124);
  CHECK(some(123) > 122);
  CHECK(some(123) >= 122);

  CHECK(123 < some(124));
  CHECK(123 <= some(124));
  CHECK(123 > some(122));
  CHECK(123 >= some(122));

  auto m5 = some(2.0).map([](auto v) { return v * 2.0; }).flatMap([](auto v) {
    return (v == 0.0) ? none<double>() : some(1.0 / v);
  });
  CHECK(m5 == 1.0 / 4.0);
}

TEST_CASE("Result", "[fredis]") {
  Result<int> r1 = ok(4);
  REQUIRE(r1.isOk() == true);
  CHECK(!r1.isError());
  CHECK(r1.get() == 4);

  Result<int> r2 = ErrorMsg("bad number");
  REQUIRE(r2.isOk() == false);
  CHECK(r2.isError());
  CHECK(r2.getError() == ErrorMsg("bad number"));

  auto r3 = ok(42).map([](int n) { return "answer = " + std::to_string(n); });
  auto r4 = r2.map([](int n) { return "answer = " + std::to_string(n); });
  CHECK(r3.get() == "answer = 42");
  CHECK(r4.getError() == ErrorMsg("bad number"));

  struct NoCpy {
    NoCpy(NoCpy&&) = default;
    NoCpy& operator=(NoCpy&&) = default;
    NoCpy(const NoCpy&) = delete;
    NoCpy& operator=(const NoCpy&) = delete;
    std::string val;
  };

  Result<NoCpy> r5 = ok(NoCpy{"s1"});

  std::string r5Val;
  std::move(r5).then([&r5Val](const NoCpy& nc) { r5Val = nc.val; });
  CHECK(r5Val == "s1");

  Result<int> r6 = ErrorMsg("bad number"), r7 = ok(7);
  CHECK_THROWS_AS(r6.expect("number not good"), Result<int>::Unexpected);
  CHECK_NOTHROW(r7.expect("number not good"));

  Result<int> r8 = ErrorMsg("bad number"), r9 = ok(9);
  CHECK_THROWS_AS(r8.unwrap(), Result<int>::Unexpected);
  CHECK(r9.unwrap() == 9);

  Result<int, std::string> r10 = std::string("bad number"), r11 = 11;
  using ToThrow = Result<int, std::string>::Unexpected;
  CHECK_THROWS_AS(r10.unwrap(), ToThrow);
  CHECK(r11.unwrap() == 11);
}