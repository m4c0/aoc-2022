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
    silog::log(silog::debug, "%d < %d (ok)", l, r);
    return true;
  } else if (l > r) {
    silog::log(silog::debug, "%d > %d", l, r);
    return false;
  } else {
    silog::log(silog::debug, "%d == %d", l, r);
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
      silog::log(silog::debug, "open both");
      return compare();
    case list_e:
      silog::log(silog::debug, "left has more");
      return false;
    case num:
      wrap(right);
      silog::log(silog::debug, "right wrapping %d", atoi(right));
      return compare();
    }
  case list_e:
    switch (next(right)) {
    case list_s:
      silog::log(silog::debug, "right has more (ok)");
      return true;
    case list_e:
      silog::log(silog::debug, "close both");
      return compare();
    case num:
      silog::log(silog::debug, "right has %d (ok)", atoi(right));
      return true;
    }
  case num:
    switch (next(right)) {
    case list_s:
      wrap(left);
      silog::log(silog::debug, "left wrapping %d", atoi(left));
      return compare();
    case list_e:
      silog::log(silog::debug, "left has %d", atoi(left));
      return false;
    case num:
      return compare_ints();
    }
  }
}

int sum{};
int pqp{};
void run(jute::view line) {
  lnum++;
  if (line == "")
    return;
  if (lnum % 3 == 1) {
    left.data = line;
    left.fakes.truncate(0);
    return;
  }
  pqp++;
  right.data = line;
  right.fakes.truncate(0);
  if (compare()) {
    silog::log(silog::debug, "-=-=-=-=- %d looks fine", lnum);
    sum += pqp;
  } else {
    silog::log(silog::debug, "-=-=-=-=- %d wasn't fine", lnum);
  }
}

int main() {
  loop(run);
  info("sum", sum);
}
