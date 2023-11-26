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
struct simul {
  unsigned depth{};
  unsigned o_bots{1};
  unsigned c_bots{};
  unsigned b_bots{};
  unsigned g_bots{};
  unsigned os{};
  unsigned cs{};
  unsigned bs{};
  unsigned gs{};
};
constexpr simul bump(simul s) {
  s.depth++;
  s.os += s.o_bots;
  s.cs += s.c_bots;
  s.bs += s.b_bots;
  s.gs += s.g_bots;
  return s;
}

constexpr auto bump_g(const bprint &bp, simul s) {
  auto ss = bump(s);
  ss.g_bots++;
  ss.os -= bp.gro;
  ss.bs -= bp.grob;
  return ss;
}
constexpr auto bump_b(const bprint &bp, simul s) {
  auto ss = bump(s);
  ss.b_bots++;
  ss.os -= bp.obro;
  ss.cs -= bp.obrc;
  return ss;
}
constexpr auto bump_c(const bprint &bp, simul s) {
  auto ss = bump(s);
  ss.c_bots++;
  ss.os -= bp.cro;
  return ss;
}
constexpr auto bump_o(const bprint &bp, simul s) {
  auto ss = bump(s);
  ss.o_bots++;
  ss.os -= bp.oro;
  return ss;
}

constexpr int ceil(int a, int b) { return (a + b - 1) / b; }
constexpr int turns(int req, int stock, int bots) {
  if (bots == 0)
    return 9999999;
  if (stock >= req)
    return 0;
  return ceil(req - stock, bots);
}

constexpr auto gbot_turns(const bprint &bp, const simul &s) {
  return turns(bp.grob, s.bs, s.b_bots) + turns(bp.gro, s.os, s.o_bots);
}
constexpr auto bbot_turns(const bprint &bp, const simul &s) {
  return turns(bp.obrc, s.cs, s.c_bots) + turns(bp.obro, s.os, s.o_bots);
}
constexpr auto cbot_turns(const bprint &bp, const simul &s) {
  return turns(bp.cro, s.os, s.o_bots);
}
constexpr auto obot_turns(const bprint &bp, const simul &s) {
  return turns(bp.oro, s.os, s.o_bots);
}

int step(const bprint &bp, const simul &s, int mins) {
  if (mins == 0) {
    return s.gs;
  }
  // silog::log(silog::debug, "%d %d %d %d - %d", s.o_bots, s.c_bots, s.b_bots,
  // s.g_bots, s.depth);
  mins--;

  // wait?
  int max{};

  // build geode?
  if (s.os >= bp.gro && s.cs >= bp.grob) {
    simul ss = bump(s);
    ss.g_bots++;
    ss.os -= bp.gro;
    ss.bs -= bp.grob;
    mx(max, step(bp, ss, mins));
  }

  // build obsidian?
  if (s.os >= bp.obro && s.cs >= bp.obrc) {
    simul ss = bump(s);
    ss.b_bots++;
    ss.os -= bp.obro;
    ss.cs -= bp.obrc;
    mx(max, step(bp, ss, mins));
  }

  // build clay?
  if (s.os >= bp.cro) {
    simul ss = bump(s);
    ss.c_bots++;
    ss.os -= bp.cro;
    mx(max, step(bp, ss, mins));
  }

  // build ore?
  if (s.os >= bp.oro) {
    simul ss = bump(s);
    ss.o_bots++;
    ss.os -= bp.oro;
    mx(max, step(bp, ss, mins));
  }

  mx(max, step(bp, bump(s), mins));

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

  // o (w) o1
  // o o1 (w) o2
  // o o2 (c,w) +c o1
  // o+c o1 (w) o2 c1
  // o+c o2 c1 (c,w) +c o1 c2
  // o+2c o1 c2 (w) o2 c4
  // o+2c o2 c4 (c,w) +c o1 c6
  // o+3c o1 c6 (w) o2 c9
  // o+3c o2 c9 (c,w) o3 c12
  // o+3c o3 c12 (c,w) o4 c15
}
