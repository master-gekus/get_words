#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <set>
#include <random>

#include <cctype>
#include <cstring>

#ifdef _WIN32
#include <Windows.h>
#endif

const ::std::regex seps1{"((\\s\\-)|(\\:\\s))"};
const ::std::regex seps2{"\\,"};

using list_type = ::std::vector<::std::string>;
list_type split(const char *src, const ::std::regex& sep);

class dic_entry {
public:
  explicit dic_entry(const list_type& list) noexcept :
    first_{split(list[0].c_str(), seps2)},
    second_{split(list[1].c_str(), seps2)},
    rest_{list.begin() + 2, list.end()}
  {
  }
  dic_entry(dic_entry&&) = default;

public:
  const list_type& first() const noexcept
  {
    return first_;
  }

  const list_type& second() const noexcept
  {
    return second_;
  }

  const list_type& rest() const noexcept
  {
    return rest_;
  }

private:
  list_type first_;
  list_type second_;
  list_type rest_;
};

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

::std::vector<dic_entry> known;
::std::vector<dic_entry> unknown;

int main()
{
#ifdef _WIN32
  ::SetConsoleOutputCP(CP_UTF8);
#endif
//  trim_tests();
//  split_tests();

  ::std::string words_file_name{"words.txt"};
  size_t num_of_word{30};

  ::std::ifstream infile{words_file_name};
  if (!infile.good()) {
    ::std::cout << "Can not open words file \"" << words_file_name << "\"." << ::std::endl;
    return 1;
  }

  for( std::string line; getline(infile, line); ) {
    auto splitted{split(line.c_str(), seps1)};
    if (2 > splitted.size()) {
      continue;
    }
    if ('#' == splitted[0][0]) {
      continue;
    }
    bool is_known{false};
    const ::std::string last{splitted.back()};
    if ('+' == last.back()) {
      is_known = true;
      if ("+" == last) {
        if (3 > splitted.size()) {
          continue;
        }
        splitted.pop_back();
      }
      else {
        splitted.back() = trim(last.substr(0, last.size() - 1));
      }
    }

    if (is_known) {
      known.push_back(dic_entry{splitted});
    }
    else {
      unknown.push_back(dic_entry{splitted});
    }
  }

//  ::std::cout << "Found " << unknown.size() << " unknown entries:" << ::std::endl;
//  for (const auto& e : unknown) {
//    ::std::cout << e.first() << " ==> " << e.second();
//    if (!e.rest().empty()) {
//      ::std::cout << " (" << e.rest() << ")";
//    }
//    ::std::cout << ::std::endl;
//  }

//  ::std::cout << "Found " << known.size() << " known entries:" << ::std::endl;
//  for (const auto& e : known) {
//    ::std::cout << e.first() << " ==> " << e.second();
//    if (!e.rest().empty()) {
//      ::std::cout << " (" << e.rest() << ")";
//    }
//    ::std::cout << ::std::endl;
//  }

  ::std::cout << "Found " << unknown.size() << " unknown and " << known.size() << " known entries." << ::std::endl;
  if (num_of_word > unknown.size()) {
    num_of_word = unknown.size();
  }

  ::std::random_device rd;
  ::std::mt19937 gen(rd());
  ::std::uniform_int_distribution<size_t> dis;
  ::std::set<size_t> used;
  for (size_t i = 0; i < num_of_word; ++i) {
    size_t index{0};
    do {
      index = (dis(gen) % unknown.size());
    } while (!used.insert(index).second);
    const dic_entry& entry{unknown[index]};
    const list_type& l{entry.first()};
    if (1 < l.size()) {
      ::std::cout << l[dis(gen) % l.size()] << ::std::endl;
    }
    else {
      ::std::cout << l.front() << ::std::endl;
    }
  }

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
