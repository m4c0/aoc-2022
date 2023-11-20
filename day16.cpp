#pragma leco tool
#include <stdio.h>
import bruh;
import hai;
import jute;
import silog;
import traits;

constexpr const auto max_valves = 27 * 27;

class cost {
  // something greater than 30, but low enought to give space to sum, etc
  int i = 1000;

public:
  constexpr cost() = default;
  cost(int i) : i{i} {}

  operator int &() { return i; }
  cost operator+(const cost &o) const { return i + o.i; }
};

int rates[max_valves]{};
hai::varray<int> openables{max_valves};
hai::varray<int> valves{max_valves};
cost costs[max_valves][max_valves]{};

auto id2n(int id) {
  hai::cstr buf{3};
  (buf.data())[0] = (id / 27) + 'A';
  (buf.data())[1] = (id % 27) + 'A';
  return buf;
}
constexpr int n2id(jute::view v) { return (v[0] - 'A') * 27 + (v[1] - 'A'); }
void read(jute::view line) {
  auto [l, n, r] = line.subview(6, 2);
  auto [le, re] = r.split('=');

  auto vid = n2id(n);
  rates[vid] = atoi(re);

  auto [rest, t] = line.rsplit(',');
  while (rest != "") {
    auto tid = n2id(t.subview(1).after);
    costs[vid][tid] = 1;

    auto [a, b] = rest.rsplit(',');
    rest = a;
    t = b;
  }

  auto id = t.rsplit(' ').after;
  costs[vid][n2id(id)] = 1;

  valves.push_back(vid);
  if (rates[vid] > 0)
    openables.push_back(vid);
}

void calc_costs() {
  // "k" iteration allows incremental "breaking" of paths. Roughtly:
  // k0 starts with all direct paths, adds any "2" paths
  // k1 starts with paths <= 2, adds any "3" paths
  // k2 starts with paths <= 3, adds any "4" paths
  // etc etc.
  // The order of "k" doesn't matter
  for (auto k : valves) {
    for (auto i : valves) {
      for (auto j : valves) {
        mn(costs[i][j], costs[i][k] + costs[k][j]);
      }
    }
  }
}

// this will contain a map of all visited notes to its costs.
// doesn't make sense for part 1, but it drastically improves part 2
hai::array<int> pcosts{};
void reset() {
  const int max_paths = 1 << (openables.size() + 1);
  info("max paths", max_paths);
  pcosts.set_capacity(max_paths);
  for (auto &p : pcosts)
    p = 0;
}

// visited is a bitmask of visited nodes
void visit(int vid, int timer, int visited = 0, int rls = 0) {
  // replaces any visited combo of valves with the best option so far.
  // for part 1, a simple "int" would suffice
  mx(pcosts[visited], rls);
  for (auto i = 0; i < openables.size(); i++) {
    auto id = openables[i];
    auto nt = timer - costs[vid][id] - 1;
    if (nt <= 0)
      continue;

    auto is = 1 << i;
    if (is & visited)
      continue;

    // we pass along the cost if we don't move, so the next recursion step will
    // be better
    auto nr = rls + rates[id] * nt;
    visit(id, nt, is | visited, nr);
  }
}

void find_max() {
  int max{};
  for (auto pc : pcosts) {
    mx(max, pc);
  }
  info("res", max);
}

void find_max_pair() {
  int max{};
  for (auto i = 0; i < pcosts.size(); i++) {
    auto pi = pcosts[i];
    if (!pi)
      continue;

    for (auto j = 0; j < pcosts.size(); j++) {
      if ((i & j) != 0)
        continue;

      auto pj = pcosts[j];
      if (!pj)
        continue;

      mx(max, pi + pj);
    }
  }
  info("pair", max);
}

int main() {
  loop(read);
  calc_costs();

  reset();
  visit(n2id("AA"), 30);
  find_max();

  reset();
  visit(n2id("AA"), 26);
  find_max_pair();
}
