#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

constexpr const auto max = 20;

struct blk {
  int vsides{};
  bool exists{};
};
blk map[max][max][max];

void read(jute::view line) {
  auto [l, lr] = line.split(',');
  auto [m, r] = lr.split(',');
  auto x = atoi(l);
  auto y = atoi(m);
  auto z = atoi(r);
  map[x][y][z] = {6, true};
}

void check(blk &m, blk &m1) {
  if (m.exists && m1.exists) {
    m.vsides--;
    m1.vsides--;
  }
}

int main() {
  loop(read);

  for (auto a = 0; a < max - 1; a++) {
    for (auto b = 0; b < max; b++) {
      for (auto c = 0; c < max; c++) {
        check(map[a][b][c], map[a + 1][b][c]);
        check(map[b][a][c], map[b][a + 1][c]);
        check(map[b][c][a], map[b][c][a + 1]);
      }
    }
  }

  int qty{};
  for (auto a = 0; a < max; a++) {
    for (auto b = 0; b < max; b++) {
      for (auto c = 0; c < max; c++) {
        qty += map[a][b][c].vsides;
      }
    }
  }
  info("res", qty);
}
