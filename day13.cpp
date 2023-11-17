#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

struct source {
  jute::view data{};
  hai::varray<jute::view> fakes{10240};
};
source left{};
source right{};

constexpr int atoi(const source &s) {
  return s.fakes.size() == 0 ? atoi(s.data) : atoi(s.fakes.back());
}
static_assert([] {
  auto fail = []() -> bool { throw 1; };
  source s{};

  s.data = "123";
  (atoi(s) == 123) || fail();

  s.fakes.push_back("222");
  (atoi(s) == 222) || fail();

  s.fakes.pop_back();
  (atoi(s) == 123) || fail();
  return true;
}());

enum tok { list_s, list_e, num };
constexpr tok next(jute::view &in) {
  switch (in[0]) {
  case '[':
    in = in.subview(1).after;
    return list_s;
  case ']':
    in = in.subview(1).after;
    return list_e;
  case ',':
    in = in.subview(1).after;
    return next(in);
  }
  return num;
}
constexpr tok next(source &s) {
  if (s.fakes.size() == 0) {
    return next(s.data);
  }
  auto back = s.fakes.back();
  auto res = next(back);
  if (back == "")
    s.fakes.pop_back();
  return res;
}
static_assert([] {
  auto fail = []() -> bool { throw 1; };

  source s{};
  s.data = ",[]]";
  (next(s) == list_s) || fail();

  s.data = "[123]";
  (next(s) == list_s) || fail();
  (next(s) == num) || fail();
  (s.data == "123]") || fail();

  s.fakes.push_back("]");
  s.fakes.push_back("222");
  (next(s) == num) || fail();
  (atoi(s) == 222) || fail();

  s.fakes.pop_back();
  (next(s) == list_e) || fail();
  (next(s) == num) || fail();
  (atoi(s) == 123) || fail();

  return true;
}());

constexpr jute::view consume_int(jute::view &in) {
  unsigned i = 0;
  while (in[i] >= '0' && in[i] <= '9') {
    i++;
  }
  auto [l, r] = in.subview(i);
  in = r;
  return l;
}
constexpr jute::view consume_int(source &s) {
  if (s.fakes.size() == 0) {
    return consume_int(s.data);
  }
  return s.fakes.pop_back();
}
static_assert([] {
  auto fail = []() -> bool { throw 1; };

  source s{};
  s.data = "123,";
  s.fakes.push_back("222");

  (consume_int(s) == "222") || fail();
  (consume_int(s) == "123") || fail();

  return true;
}());

constexpr void wrap(source &s) {
  auto n = consume_int(s);
  s.fakes.push_back("]");
  s.fakes.push_back(n);
}

int lnum{};
bool compare();
bool compare_ints() {
  auto l = atoi(left);
  auto r = atoi(right);
  if (l < r) {
    return true;
  } else if (l > r) {
    return false;
  } else {
    consume_int(left);
    consume_int(right);
    return compare();
  }
}
bool compare() {
  switch (next(left)) {
  case list_s:
    switch (next(right)) {
    case list_s:
      return compare();
    case list_e:
      return false;
    case num:
      wrap(right);
      return compare();
    }
  case list_e:
    switch (next(right)) {
    case list_s:
      return true;
    case list_e:
      return compare();
    case num:
      return true;
    }
  case num:
    switch (next(right)) {
    case list_s:
      wrap(left);
      return compare();
    case list_e:
      return false;
    case num:
      return compare_ints();
    }
  }
}

hai::varray<jute::view> sorted{10240};
void run(jute::view line) {
  if (line == "")
    return;

  sorted.push_back(line);
}

int main() {
  loop(run);

  sorted.push_back("[[2]]");
  sorted.push_back("[[6]]");

  for (auto i = 0; i < sorted.size(); i++) {
    for (auto j = i + 1; j < sorted.size(); j++) {
      left.data = sorted[i];
      left.fakes.truncate(0);
      right.data = sorted[j];
      right.fakes.truncate(0);

      if (compare())
        continue;

      auto temp = sorted[j];
      sorted[j] = sorted[i];
      sorted[i] = temp;
    }
  }

  int m2{};
  int m6{};
  for (auto i = 0; i < sorted.size(); i++) {
    if (sorted[i] == "[[2]]")
      m2 = i + 1;
    if (sorted[i] == "[[6]]")
      m6 = i + 1;
  }

  info("res", m2 * m6);
}
