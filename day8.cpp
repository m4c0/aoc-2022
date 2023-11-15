#pragma leco tool
import bruh;
import jute;
import silog;

jute::view lines[1000];
unsigned lcount{};
unsigned vcount{};

int score(int x, int y) {
  auto h = lines[y][x];

  auto su = y;
  for (auto sc = 1; sc < y; sc++) {
    auto i = y - sc;
    if (lines[i][x] >= h) {
      su = sc;
      break;
    }
  }

  auto sd = lcount - y - 1;
  for (auto sc = 1; sc < lcount - y - 1; sc++) {
    auto i = y + sc;
    if (lines[i][x] >= h) {
      sd = sc;
      break;
    }
  }

  auto sl = x;
  for (auto sc = 1; sc < x; sc++) {
    auto i = x - sc;
    if (lines[y][i] >= h) {
      sl = sc;
      break;
    }
  }

  auto sr = lcount - x - 1;
  for (auto sc = 1; sc < lcount - x - 1; sc++) {
    auto i = x + sc;
    if (lines[y][i] >= h) {
      sr = sc;
      break;
    }
  }

  return su * sd * sl * sr;
}

int main() {
  loop([&](auto line) { lines[lcount++] = line; });
  silog::log(silog::debug, "%d x %d", lcount, (int)lines[0].size());

  int max = 0;
  for (auto y = 0; y < lcount; y++) {
    auto line = lines[y];
    for (auto x = 0; x < line.size(); x++) {
      auto sc = score(x, y);
      if (sc > max)
        max = sc;
    }
  }

  silog::log(silog::info, "%d", max);
}
