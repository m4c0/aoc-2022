#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

constexpr const auto max = 22;

struct blk {
  int vsides{};
  bool exists{};
  bool visited{};
};
blk map[max][max][max];

void read(jute::view line) {
  auto [l, lr] = line.split(',');
  auto [m, r] = lr.split(',');
  auto x = atoi(l);
  auto y = atoi(m);
  auto z = atoi(r);
  map[x + 1][y + 1][z + 1] = {0, true};
}

void check(blk &m, blk &m1) {
  if (m.exists && !m1.exists) {
    m1.vsides++;
  }
  if (!m.exists && m1.exists) {
    m.vsides++;
  }
}

void visit(int x, int y, int z) {
  if (x < 0 || y < 0 || z < 0)
    return;
  if (x >= max || y >= max || z >= max)
    return;

  auto &m = map[x][y][z];

  if (m.exists || m.visited)
    return;

  m.visited = true;
  visit(x - 1, y, z);
  visit(x + 1, y, z);
  visit(x, y - 1, z);
  visit(x, y + 1, z);
  visit(x, y, z - 1);
  visit(x, y, z + 1);
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

  visit(0, 0, 0);

  int vis{};
  int qty{};
  for (auto a = 0; a < max; a++) {
    for (auto b = 0; b < max; b++) {
      for (auto c = 0; c < max; c++) {
        auto &m = map[a][b][c];
        if (m.visited) {
          qty += m.vsides;
          vis++;
        }
      }
    }
  }
  info("vis", vis);
  info("total", max * max * max);
  info("res", qty);
}
