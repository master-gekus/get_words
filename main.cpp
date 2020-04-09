#include <iostream>
#include <string>
#include <list>

#include <cctype>

#ifdef _WIN32
#include <Windows.h>
#endif

::std::string trim(const ::std::string& src) noexcept
{
  size_t start{0};
  size_t len{src.length()};
  size_t end{len};

  while ((start < len) && ::std::isspace(static_cast<uint8_t>(src[start]))) {
    ++start;
  }

  while ((end > (start + 1)) && ::std::isspace(static_cast<uint8_t>(src[end - 1]))) {
    --end;
  }

  if (start < end) {
    return src.substr(start, end - start);
  }
  else {
    return ::std::string{};
  }
}

void trim_tests() noexcept;

int main()
{
#ifdef _WIN32
  ::SetConsoleOutputCP(CP_UTF8);
#endif

  trim_tests();

  return 0;
}

void trim_tests() noexcept
{
#define trim_test(c) \
  ::std::cout << "trim(\"" << c << "\"): \"" << trim(c) << "\"" << ::std::endl;
  trim_test("");
  trim_test(" ");
  trim_test(" \t");
  trim_test("думать");
  trim_test("  думать");
  trim_test("думать  ");
  trim_test("  думать  ");
  trim_test("tú tú");
  trim_test("út út");
  trim_test("  tú tú");
  trim_test("  út út");
  trim_test("tú tú  ");
  trim_test("út út  ");
  trim_test("  tú tú  ");
  trim_test("  út út  ");
#undef trim_test
}
