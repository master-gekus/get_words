#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

#include <cctype>
#include <cstring>

#ifdef _WIN32
#include <Windows.h>
#endif

const ::std::regex seps1{"((\\s\\-)|(\\:\\s))"};
const ::std::regex seps2{"\\,"};

using list_type = ::std::vector<::std::string>;

::std::ostream& operator << (::std::ostream& o, const list_type& l) noexcept
{
  o << "[";
  bool first{true};
  for (const auto& v : l) {
    if (first) {
      first = false;
    }
    else {
      o << ',';
    }
    o << '"' << v << '"';
  }
  o << "]";
  return o;
}

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

list_type split(const char *src, const ::std::regex& sep)
{
  list_type result;

  while (nullptr != src) {
    ::std::string part;
    ::std::cmatch m;
    if (!::std::regex_search(src, m, sep)) {
      part = trim(src);
      src = nullptr;
    }
    else {
      part = trim(::std::string(src, m[0].first - src));
      src = m[0].second;
    }
    if (!part.empty()) {
      result.push_back(::std::move(part));
    }
  };
  return result;
}

void trim_tests() noexcept;
void split_tests() noexcept;

int main()
{
#ifdef _WIN32
  ::SetConsoleOutputCP(CP_UTF8);
#endif
//  trim_tests();
  split_tests();

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

void split_tests() noexcept
{
  ::std::cout << split("", seps1) << ::std::endl;
  ::std::cout << split("чувствовать - - ", seps1) << ::std::endl;
  ::std::cout << split("чувствовать - - , ощущать, слышать - sentir +", seps1) << ::std::endl;
  ::std::cout << split("чувствовать - - +", seps1) << ::std::endl;
  ::std::cout << split("чувствовать - - , ощущать, слышать - sentir", seps1) << ::std::endl;
  ::std::cout << split("чувствовать, ощущать, слышать - sentir", seps1) << ::std::endl;
  ::std::cout << split("предупреждать (кого-то о чём-то) - advertir (a uno de algo)", seps1) << ::std::endl;
  ::std::cout << split("смеяться над, насмехаться - reír, reírse de", seps1) << ::std::endl;
  ::std::cout << split("думать - pensar: yo pienso; tú piensas; él, ella, usted piensa; nosotros pensamos; vosotros pensáis; ellos, ellas, ustedes piensan.", seps1) << ::std::endl;
}
