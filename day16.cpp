#pragma leco tool
#include <stdio.h>
import bruh;
import hai;
import jute;
import silog;
import traits;

constexpr const auto max_valves = 27 * 27;

using tun = int;
using tuns = hai::varray<tun>;
struct valve {
  unsigned rate{};
  int id{};
  tuns tunnels{};
  bool visited{};
};
valve vs[max_valves]{};

constexpr auto valid(const valve &v) { return v.tunnels.size() > 0; }

constexpr int n2id(jute::view v) { return (v[0] - 'A') * 27 + (v[1] - 'A'); }
auto id2n(int id) {
  hai::cstr buf{3};
  (buf.data())[0] = (id / 27) + 'A';
  (buf.data())[1] = (id % 27) + 'A';
  return buf;
}

void read(jute::view line) {
  auto [l, n, r] = line.subview(6, 2);
  auto [le, re] = r.split('=');

  tuns ts{16};
  auto [rest, t] = line.rsplit(',');
  while (rest != "") {
    auto id = t.subview(1).after;
    ts.push_back(tun{n2id(id)});

    auto [a, b] = rest.rsplit(',');
    rest = a;
    t = b;
  }

  auto id = t.rsplit(' ').after;
  ts.push_back(tun{n2id(id)});

  auto idn = n2id(n);
  vs[idn] = {
      .rate = atoi(re),
      .id = idn,
      .tunnels = traits::move(ts),
  };
}

int v2v_cost[max_valves][max_valves]{};
int calc_cost(valve &i, const valve &j, int dp = 4) {
  auto &c = v2v_cost[i.id][j.id];
  if (c > 0)
    return c;
  if (i.visited) {
    return 1000000;
  }

  auto min = 1000000;
  i.visited = true;
  for (auto tid : i.tunnels) {
    auto cc = calc_cost(vs[tid], j, dp + 2);
    mn(min, 1 + cc);
  }
  i.visited = false;
  if (min != 1000000)
    c = min;
  return min;
}
void calc_costs() {
  for (auto &vi : vs) {
    for (auto tid : vi.tunnels) {
      v2v_cost[vi.id][tid] = 1;
    }
    v2v_cost[vi.id][vi.id] = 0;
  }
  for (auto &vi : vs) {
    if (!valid(vi))
      continue;
    for (auto &vj : vs) {
      if (!valid(vj))
        continue;
      calc_cost(vi, vj);
    }
  }
}

struct move {
  int to;
  int rtaa; // remaining time after arriving
};
int besty(move mm, move me, int rate) {
  indcounter ind{};
  // fprintf(stderr, "%s%s(%d) %s(%d) %d\n", *ind, id2n(mm.to).data(),
  // 26 - mm.rtaa, id2n(me.to).data(), 26 - me.rtaa, rate);

  if (mm.rtaa <= 0) {
    // fprintf(stderr, "%sdoney\n", *ind);
    return 0;
  }

  int max{rate * me.rtaa};
  // int ppp{};
  for (auto &t : vs) {
    auto tid = t.id;
    if (t.rate == 0)
      continue;
    if (t.visited)
      continue;

    auto cost = v2v_cost[mm.to][tid] + 1;
    move nmm{
        .to = tid,
        .rtaa = mm.rtaa - cost,
    };
    if (nmm.rtaa <= 0)
      continue;

    t.visited = true;
    // ppp++;
    auto rls = (me.rtaa - nmm.rtaa) * rate;
    if (nmm.rtaa > me.rtaa) {
      // auto rls = cost * rate;
      mx(max, rls + besty(nmm, me, rate + t.rate));
    } else if (nmm.rtaa < me.rtaa) {
      mx(max, rls + besty(me, nmm, rate + t.rate));
    } else {
      // auto rls = 0;
      mx(max, rls + besty(nmm, me, rate + t.rate));
      mx(max, rls + besty(me, nmm, rate + t.rate));
    }
    t.visited = false;
  }

  // fprintf(stderr, "%smax = %d, vis = %d\n", *ind, max, ppp);
  return max;
}

int main() {
  loop(read);
  calc_costs();

  auto aa = n2id("AA");
  move m{aa, 26};
  info("res", besty(m, m, 0));
}
