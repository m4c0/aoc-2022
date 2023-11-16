#pragma leco tool
import bruh;
import jute;
import silog;

constexpr int sign(int a) noexcept { return a == 0 ? 0 : (a < 0 ? -1 : 1); }
constexpr point sign(const point &a) noexcept { return {sign(a.x), sign(a.y)}; }

int main() {
  point knots[10];

  constexpr const auto H = 1000;
  bool visited[H][H]{};
  visited[0][0] = true;

  loop([&](auto line) {
    auto [side, qty_s] = line.split(' ');
    auto qty = atoi(qty_s);

    point d{};
    switch (side[0]) {
    case 'R':
      d.x = 1;
      break;
    case 'L':
      d.x = -1;
      break;
    case 'U':
      d.y = -1;
      break;
    case 'D':
      d.y = 1;
      break;
    default:
      throw 1;
    }

    for (auto i = 0; i < qty; i++) {
      auto &hh = knots[0];
      hh = hh + d;
      if (abs(hh).x >= H || abs(hh).y >= H)
        throw 1;

      for (auto k = 1; k < 10; k++) {
        auto &h = knots[k - 1];
        auto &t = knots[k];

        auto ht = h - t;
        auto aht = abs(ht);
        if (aht.x < 2 && aht.y < 2)
          continue;

        t = t + sign(ht);
        if (abs(t).x >= H || abs(t).y >= H)
          throw 1;
      }
      auto t = knots[9];
      visited[(H + t.y) % H][(H + t.x) % H] = true;
    }
  });

  unsigned count{};
  for (auto y = 0; y < H; y++) {
    for (auto x = 0; x < H; x++) {
      count += visited[y][x] ? 1 : 0;
    }
  }

  silog::log(silog::info, "%d", count);
}
