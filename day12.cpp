#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

struct node {
  int h{};
  unsigned g_score{~0U};
  unsigned f_score{~0U};
};

int e{};
int cols{};
int rows{};
hai::varray<node> nodes{10240};
hai::varray<int> open_set{10240};

point c2p(int c) { return point{.x = c % cols, .y = c / cols}; }
int p2c(point p) { return p.x + p.y * cols; }
int heu(int a) {
  auto p = abs(c2p(a) - c2p(e));
  return p.x + p.y;
}

auto find_min() {
  struct {
    int osi{-1};
    int ni{-1};
  } res;
  unsigned min = ~0U;
  unsigned oi = 0;
  for (auto i : open_set) {
    const auto &n = nodes[i];
    if (n.f_score < min) {
      min = n.f_score;
      res.ni = i;
      res.osi = oi;
    }
    oi++;
  }
  if (res.ni == -1)
    throw 0;
  return res;
}
void try_move(int i, int di) {
  auto pi = c2p(i);
  auto ti = pi + c2p(di);
  if (ti.x < 0 || ti.x >= cols || ti.y < 0 || ti.y >= rows)
    return;

  auto t = p2c(ti);

  auto dh = nodes[t].h - nodes[i].h;
  if (dh > 1)
    return;

  auto ten_g = nodes[i].g_score + 1;
  auto &tn = nodes[t];
  if (ten_g >= tn.g_score)
    return;

  tn.g_score = ten_g;
  tn.f_score = ten_g + heu(t);
  open_set.push_back(t);
}

unsigned run(int s);
int main() {
  loop([&](auto line) {
    cols = line.size();
    for (auto c : line) {
      node n{};
      if (c == 'S') {
        n.h = 'a';
      } else if (c == 'E') {
        n.h = 'z';
        e = nodes.size();
      } else {
        n.h = c;
      }
      nodes.push_back(n);
    }
  });
  rows = nodes.size() / cols;
  silog::log(silog::debug, "cols: %d - rows: %d", cols, rows);

  unsigned min{~0U};
  for (auto i = 0; i < nodes.size(); i++) {
    if (nodes[i].h != 'a')
      continue;

    auto ri = run(i);
    if (ri < min) {
      min = ri;
    }
  }
  info("result", min);
}

unsigned run(int s) {
  for (auto i = 0; i < nodes.size(); i++) {
    nodes[i].g_score = ~0U;
    nodes[i].f_score = ~0U;
  }

  nodes[s].g_score = 0;
  nodes[s].f_score = heu(s);
  open_set.truncate(0);
  open_set.push_back(s);

  while (open_set.size() > 0) {
    auto [min, ni] = find_min();
    if (ni == e) {
      silog::log(silog::info, "%d: %d", s, nodes[e].g_score);
      return nodes[e].g_score;
    }

    open_set[min] = open_set.pop_back();
    try_move(ni, +1);
    try_move(ni, -1);
    try_move(ni, +cols);
    try_move(ni, -cols);
  }
  silog::log(silog::error, "no path found");
  return ~0U;
}
