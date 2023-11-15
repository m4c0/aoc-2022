#pragma leco tool
import bruh;
import jute;
import silog;

jute::view lines[1000];
unsigned lcount{};
unsigned vcount{};

bool check_v(int x, int y, int sy, int ey) {
  auto h = lines[y][x];
  for (auto yy = sy; yy < ey; yy++) {
    if (lines[yy][x] >= h)
      return false;
  }
  return true;
}
bool check_h(int x, int y, int sx, int ex) {
  auto h = lines[y][x];
  for (auto xx = sx; xx < ex; xx++) {
    if (lines[y][xx] >= h)
      return false;
  }
  return true;
}

bool check(int x, int y) {
  if (check_v(x, y, 0, y))
    return true;
  if (check_v(x, y, y + 1, lcount))
    return true;
  if (check_h(x, y, 0, x))
    return true;
  if (check_h(x, y, x + 1, lcount))
    return true;
  return false;
}

int main() {
  loop([&](auto line) { lines[lcount++] = line; });
  silog::log(silog::debug, "%d x %d", lcount, (int)lines[0].size());

  for (auto y = 0; y < lcount; y++) {
    auto line = lines[y];
    for (auto x = 0; x < line.size(); x++) {
      if (check(x, y))
        vcount++;
    }
  }

  silog::log(silog::info, "%d", vcount);
}
