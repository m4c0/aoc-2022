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

  constexpr auto pos() const { return p; }
  constexpr auto size() const { return data.size(); }

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
  pit_t() { reset(); }
  void reset() {
    for (auto &p : pit) {
      p = pit_mask;
    }
  }
  constexpr auto &operator[](long i) { return pit[i % 1024]; }
} pit;

bool fits(long y, int shf, const pat_t &pat) {
  for (auto py = 0; py < pat_h; py++) {
    auto p = pat[3 - py] << shf;
    if (p & pit[y + py]) {
      return false;
    }
  }
  return true;
}
int shoosh(long y, int shf, const pat_t &pat) {
  auto move = j.next();
  auto nsh = shf - move;
  return fits(y, nsh, pat) ? nsh : shf;
}

long limit = 2022;
int magic_jetn = 0;
void run(jute::view line) {
  j = jets{line};
  pit = {};
  pit[0] = 0x1ff;

  long height = 1;
  long oh = height;
  int duh = 5;

  long oi{};
  int magic_row{};
  int cur_pat = 0;
  for (long i = 0; i < limit; i++) {
    const auto &pat = pats[cur_pat];
    auto lp = j.pos();

    auto shf = 3;
    long y;
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

    if (magic_row == 0) {
      magic_row = pit[1];
      silog::log(silog::debug, "magic: %x", magic_row);
    }

    // if (duh > 0 && pit[height - 1] == magic_row && cur_pat == 0) {
    //   silog::log(silog::debug, "%ld - lp %d", height - 1, lp);
    // }
    if (pit[height - 1] == magic_row && cur_pat == 0 && lp == magic_jetn) {
      silog::log(silog::debug, "%ld - lp %d - i %ld", height - 1, lp, i);
    }
    if (duh > 0 && pit[height - 1] == magic_row && cur_pat == 0 &&
        lp == magic_jetn) {
      long dh = height - 1 - oh;
      long di = i - oi;
      // silog::log(silog::debug, "%ld %ld - lp %d - i %ld %ld", height - 1, dh,
      // lp, i, di);

      if (--duh > 0) {
        oi = i;
        oh = height - 1;
      } else {
        long rem = limit - i;
        long cycles = rem / di - 5;

        pit.reset();
        i += di * cycles;
        height += dh * cycles - 1;
        pit[height] = 0x1ff;
        silog::log(silog::debug, "skip - h: %ld - i: %ld - lp: %d", height, i,
                   lp);
        duh = false;
      }
    }

    cur_pat = (cur_pat + 1) % pat_count;
  }
  silog::log(silog::info, "============== height: %ld", height - 1);
}
int main() {
  // found by testing where pattern repeats. could be done automatically, but
  // do we care?
  magic_jetn = 2;
  loop_e(run);
  magic_jetn = 84;
  loop(run);

  limit = 1000000000000;
  magic_jetn = 2;
  loop_e(run);
  magic_jetn = 84;
  loop(run);
}
