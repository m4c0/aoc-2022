#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

auto next_int(jute::view v) {
  struct res {
    jute::view a;
    jute::view b;
  };
  for (auto i = 0; i < v.size(); i++) {
    if (v[i] >= '0' && v[i] <= '9')
      for (auto j = i; j < v.size(); j++) {
        if (v[j] < '0' || v[j] > '9')
          return res{v.subview(i).after, v.subview(j).after};
      }
  }
  throw 0;
}

struct bprint {
  unsigned id;
  unsigned oro;
  unsigned cro;
  unsigned obro;
  unsigned obrc;
  unsigned gro;
  unsigned grob;
};

constexpr const auto bot_count = 4;
constexpr const auto max_mins = 24;
struct eq {
  int bot_n_m[bot_count][max_mins]{};
  int rhs{};
};

// costs eqs + max eq (missing other constraints)
constexpr const auto eq_count = 1 + 1 + 2 + 2 + 1;
struct mat {
  eq eqs[X]{};
};

int step(const bprint &bp, const simul &s, int mins) {
  if (mins == 0) {
    return s.gs;
  }
  // silog::log(silog::debug, "%d %d %d %d - %d", s.o_bots, s.c_bots, s.b_bots,
  // s.g_bots, s.depth);
  mins--;

  int max{};

  // maximise:
  // g = sum c=0->23 (bot_g_c * (24 - c))
  //
  // given:
  // bot_n_m >= 0
  // bot_or_0 = 1
  // a = b + 1
  // bot_g_a <= (
  //     sum c=0->(a-2)(bot_or_c)
  //   - ((bot_or_b - 1) * 4 + bot_cl_b * 2 + bot_ob_b * 3)
  //   ) / 2
  // bot_g_a <= (sum c=0->(a-2)(bot_ob_c)) / 7
  // bot_b_a <= (
  //     sum c=0->(a-2)(bot_or_c)
  //   - ((bot_or_b - 1) * 4 + bot_cl_b * 2)
  //   ) / 3
  // ...
  // sum bot_n_a = sum bot_n_b + 1
  // bot_n_a >= bot_n_b
  //
  // cost_or_g * bot_g_a <=
  //   sum c bot_or_c - sum n (bot_n_b * cost_or_n) - cost_or_or
  // cost_ob_g * bot_g_a <=
  //   sum c bot_ob_c - sum n (bot_n_b * cost_ob_n)
  // ...
  //
  // cost_or_g * bot_g_a
  //   - sum c bot_or_c - sum n (bot_n_b * cost_or_n)
  //   + slack_or_g_a
  //   = cost_or_or
  // cost_ob_g * bot_g_a
  //   - sum c bot_ob_c - sum n (bot_n_b * cost_ob_n)
  //   + slack_ob_g_a
  //   = 0
  //

  return max;
}

int main() {
  loop_e([&](auto line) {
    auto [bps, bpr] = next_int(line);
    auto [os, orr] = next_int(bpr);
    auto [cs, cr] = next_int(orr);
    auto [bos, bor] = next_int(cr);
    auto [bcs, bcr] = next_int(bor);
    auto [gos, gor] = next_int(bcr);
    auto [gbs, gbr] = next_int(gor);

    bprint b{
        atoi(bps), atoi(os),  atoi(cs),  atoi(bos),
        atoi(bcs), atoi(gos), atoi(gbs),
    };
    info("g", step(b, {}, 24));
  });
}
