#pragma leco tool
import bruh;
import jute;
import silog;

constexpr const auto w = 600;
constexpr const auto h = 200;
char map[h][w]{};

void draw(int x0, int y0, jute::view rest) {
  if (rest == "")
    return;

  auto [xs, rx] = rest.subview(1).after.split(',');
  auto [ys, ry] = rx.split('>');

  auto x1 = atoi(xs);
  auto y1 = atoi(ys);

  if (x0 == x1) {
    int s = y1 > y0 ? y0 : y1;
    int e = y1 > y0 ? y1 : y0;
    for (auto y = s; y <= e; y++) {
      map[y][x0] = '#';
    }
  } else if (y0 == y1) {
    int s = x1 > x0 ? x0 : x1;
    int e = x1 > x0 ? x1 : x0;
    for (auto x = s; x <= e; x++) {
      map[y0][x] = '#';
    }
  } else {
    throw 0;
  }

  draw(x1, y1, ry);
}
void run(jute::view line) {
  auto [xs, rx] = line.split(',');
  auto [ys, ry] = rx.split('>');

  auto x = atoi(xs);
  auto y = atoi(ys);
  draw(x, y, ry);
}

void dump() {
  for (auto i = 0; i < h; i++) {
    for (auto j = 0; j < w; j++) {
      if (!map[i][j])
        map[i][j] = ' ';
    }
    silog::log(silog::debug, "[%03d] %.*s", i, 200, map[i] + 400);
  }
}

bool sand() {
  point p{.x = 500};

  while (p.y < h) {
    p.y++;

    if (!map[p.y][p.x])
      continue;

    if (!map[p.y][p.x - 1]) {
      p.x--;
      continue;
    }
    if (!map[p.y][p.x + 1]) {
      p.x++;
      continue;
    }

    map[p.y - 1][p.x] = 'o';
    return false;
  }

  map[p.y - 1][p.x] = 'o';
  return true;
}

int main() {
  loop(run);

  for (auto i = 0; i < 2000; i++) {
    if (sand()) {
      dump();
      info("res", i);
      break;
    }
  }
}
