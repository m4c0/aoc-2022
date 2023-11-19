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
    v2v_cost[vi.id][vi.id] = 1;
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

int besty(int from, int mins, int rate) {
  auto &v = vs[from];
  rate += v.rate;

  if (mins <= 0)
    return 0;

  v.visited = true;

  int max{rate * mins};
  for (auto &t : vs) {
    auto tid = t.id;
    if (t.rate == 0)
      continue;
    if (t.visited)
      continue;

    auto cost = v2v_cost[from][tid] + 1;
    if (cost > mins)
      continue;

    auto rls = cost * rate;

    mx(max, rls + besty(tid, mins - cost, rate));
  }

  v.visited = false;
  return max;
}

int main() {
  loop(read);
  calc_costs();

  auto aa = n2id("AA");
  info("res", besty(aa, 30, 0));
}
