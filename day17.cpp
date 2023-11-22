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

constexpr const auto pit_mask = 0b100000001;
jets j{};
class pit_t {
  hai::array<int> pit{10240};

public:
  pit_t() {
    for (auto &p : pit) {
      p = pit_mask;
    }
    pit[0] = 0x1ff;
  }
  constexpr auto &operator[](long i) { return pit[i % 1024]; }
} pit;

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

long limit = 2022;
void run(jute::view line) {
  j = jets{line};
  pit = {};

  long height = 1;

  int cur_pat = 0;
  for (long i = 0; i < limit; i++) {
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
    pit[height + 3] = pit_mask;
    pit[height + 4] = pit_mask;
    pit[height + 5] = pit_mask;
    pit[height + 6] = pit_mask;

    cur_pat = (cur_pat + 1) % pat_count;
  }
  silog::log(silog::info, "============== height: %ld", height - 1);
}
int main() {
  loop_e(run);
  loop(run);

  limit = 1000000000000;
  loop_e(run);
  loop(run);
}
