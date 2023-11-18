#pragma leco tool
import bruh;
import hai;
import jute;
import silog;
import traits;

struct sensor {
  point pos;
  int manh;
};
hai::varray<sensor> sens{10240};
hai::varray<point> bics{10240};

constexpr const int y_check = 2000000;
int min_x = 0x7fffffff;
int max_x = 0;
void read(jute::view line) {
  sensor s{};
  point b{};

  auto [p0, p1] = line.split('=');
  s.pos.x = atoi(p1);
  auto [p2, p3] = p1.split('=');
  s.pos.y = atoi(p3);
  auto [p4, p5] = p3.split('=');
  b.x = atoi(p5);
  auto [p6, p7] = p5.split('=');
  b.y = atoi(p7);

  auto d = abs(b - s.pos);
  s.manh = d.x + d.y;
  sens.push_back(s);

  if (b.y != y_check)
    return;
  for (auto p : bics) {
    if (p == b)
      return;
  }
  bics.push_back(b);
}

struct ival {
  int min{0};
  int max{4000000};
};
void enqueue(hai::varray<ival> &q, ival i) {
  for (auto &ii : q)
    if (ii.min == i.min && ii.max == i.max)
      return;

  q.push_back(i);
}

hai::varray<ival> newq{102400};
hai::varray<ival> q{102400};
long check(int y) {
  newq.truncate(0);
  q.truncate(0);
  q.push_back(ival{});

  for (const auto &s : sens) {
    auto d2m = abs(y - s.pos.y);
    auto rng2m = s.manh - d2m;
    if (y == 2908372) {
      silog::log(silog::debug, "s %d %d %d", d2m, s.manh, rng2m);
    }
    if (rng2m < 0)
      continue;

    auto min_x = s.pos.x - rng2m;
    auto max_x = s.pos.x + rng2m;
    if (y == 2908372) {
      silog::log(silog::debug, "mm %d %d", min_x, max_x);
    }
    for (const auto &i : q) {
      if (max_x <= i.min || min_x >= i.max) {
        newq.push_back(i);
      } else {
        if (i.min < min_x && min_x <= i.max)
          newq.push_back(ival{i.min, min_x - 1});
        if (i.min <= max_x && max_x < i.max)
          newq.push_back(ival{max_x + 1, i.max});
      }
    }

    if (y == 2908372) {
      for (auto i : newq) {
        silog::log(silog::debug, "%d %d", i.min, i.max);
      }
    }
    q.truncate(0);

    auto t = traits::move(q);
    q = traits::move(newq);
    newq = traits::move(t);
  }
  if (q.size() > 0) {
    for (auto qq : q) {
      silog::log(silog::debug, "%d %d", qq.min, qq.max);
    }
    info("qq", q.size());
    info("y", y);
    return q[0].min;
  }
  return -1;
}

int main() {
  loop(read);
  info("sens", sens.size());
  info("bics", bics.size());

  for (long y = 0; y < 4000000; y++) {
    long x = check(y);
    if (x >= 0) {
      silog::log(silog::info, "%ld --- %ld", y, x * 4000000 + y);
    }
  }
}
