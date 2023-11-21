#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

constexpr const auto pat_count = 5;
const char pats[pat_count][17] = {
    "...."
    "...."
    "...."
    "####",

    "...."
    ".#.."
    "###."
    ".#..",

    "...."
    "..#."
    "..#."
    "###.",

    "#..."
    "#..."
    "#..."
    "#...",

    "...."
    "...."
    "##.."
    "##..",
};

class jets {
  jute::view data;
  unsigned p{};

public:
  jets() = default;
  jets(jute::view v) : data(v) {
    silog::log(silog::debug, "loaded %zu jet pats", v.size());
  }

  int next() {
    auto op = p;
    p = (p + 1) % data.size();
    return data[op] == '>' ? 1 : -1;
  }
};

struct row {
  char cols[5]{};
};

int main() {
  jets j{};
  loop_e([&](auto line) { j = jets{line}; });

  int height = 0;
  hai::varray<row> pit{10240};
  int cur_pat = 0;
  for (auto i = 0; i < 2022; i++) {
    const auto &pat = pats[cur_pat];
    auto move = j.next();

    cur_pat = (cur_pat + 1) % pat_count;
  }
  info("height", height);
}
