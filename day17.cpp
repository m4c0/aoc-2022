#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

constexpr const auto pat_h = 4;
using pat_t = int[pat_h];

constexpr const auto pat_count = 5;
const pat_t pats[pat_count] = {{0b0000, 0b0000, 0b0000, 0b1111},
                               {0b0000, 0b0010, 0b0111, 0b0010},
                               {0b0000, 0b0100, 0b0100, 0b0111},
                               {0b0001, 0b0001, 0b0001, 0b0001},
                               {0b0000, 0b0000, 0b0011, 0b0011}};

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
    return data[op] == '>' ? -1 : 1;
  }
};

jets j{};
hai::array<int> pit{10240};

bool fits(int y, int shf, const pat_t &pat) {
  for (auto py = 0; py < pat_h; py++) {
    auto p = pat[3 - py] << shf;
    if (p & pit[y + py]) {
      return false;
    }
  }
  return true;
}
int shoosh(int y, int shf, const pat_t &pat) {
  auto move = j.next();
  auto nsh = shf - move;
  return fits(y, nsh, pat) ? nsh : shf;
}

constexpr const auto pit_mask = 0b100000001;
void run(jute::view line) {
  j = jets{line};

  for (auto &p : pit) {
    p = pit_mask;
  }
  pit[0] = 0x1ff;
  int height = 1;

  int cur_pat = 0;
  for (auto i = 0; i < 2022; i++) {
    const auto &pat = pats[cur_pat];

    auto shf = 3;
    int y;
    for (y = height + 3; y > 0; y--) {
      shf = shoosh(y, shf, pat);

      if (!fits(y - 1, shf, pat)) {
        break;
      }
    }

    for (auto py = 0; py < pat_h; py++) {
      pit[y + py] |= (pat[3 - py] << shf);
    }
    while (pit[height] != pit_mask)
      height++;

    cur_pat = (cur_pat + 1) % pat_count;
  }
  for (auto i = height + 5; i >= 0; i--) {
    char buf[10]{};
    char *p = buf;
    int n = pit[i];
    while (n > 0) {
      *p++ = (n & 1) ? 'X' : '.';
      n >>= 1;
    }

    silog::log(silog::debug, "%6d: %s", i, buf);
  }
  info("height", height - 1);
}
int main() { loop(run); }
