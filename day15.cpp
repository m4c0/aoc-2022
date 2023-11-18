#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

struct sensor {
  point pos;
  int manh;
  int min_x;
  int max_x;
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
  auto d2m = abs(y_check - s.pos.y);
  auto rng2m = s.manh - d2m;
  if (rng2m > 0) {
    s.min_x = s.pos.x - rng2m;
    s.max_x = s.pos.x + rng2m;

    mn(min_x, s.min_x);
    mx(max_x, s.max_x);

    sens.push_back(s);
  }

  if (b.y != y_check)
    return;
  for (auto p : bics) {
    if (p == b)
      return;
  }
  bics.push_back(b);
}

bool check(point p) {
  for (auto b : bics) {
    if (b == p)
      return false;
  }

  for (auto &s : sens) {
    if (p.x >= s.min_x && p.x <= s.max_x)
      return true;
  }

  return false;
}

int main() {
  loop(read);
  info("sens", sens.size());
  info("bics", bics.size());

  point p{.y = y_check};
  int n{};
  for (p.x = min_x; p.x <= max_x; p.x++) {
    if (check(p))
      n++;
  }
  info("res", n);
}
