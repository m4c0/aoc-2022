#pragma leco tool
import jute;
import silog;
import sires;

int main() {
  auto file = sires::slurp("../../day2.txt").take([](auto e) {
    silog::log(silog::error, "failure: %s", e);
    throw 1;
  });
  silog::log(silog::info, "file size: %d", file.size());

  int points = 0;

  jute::view f{file.begin(), file.size()};
  while (f != "") {
    auto [line, rest] = f.split('\n');
    auto foe = line[0] - 'A';
    auto out = line[2] - 'X';

    auto me = (foe + out - 1 + 3) % 3;

    auto pta = me + 1;

    auto ptb = 0;
    if (me == (foe + 1) % 3) {
      ptb = 6;
    } else if (me == foe) {
      ptb = 3;
    }

    points += pta + ptb;

    silog::log(silog::debug, "%d %d - %d %d - %d", foe, me, pta, ptb, points);

    f = rest;
  }

  silog::log(silog::info, "points: %d", points);
}
