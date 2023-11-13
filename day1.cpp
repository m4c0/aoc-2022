#pragma leco tool
import jute;
import silog;
import sires;

unsigned atoi(jute::view str) {
  unsigned n = 0;
  for (auto c : str) {
    if (c < '0' || c > '9')
      break;
    n = n * 10 + (c - '0');
  }
  return n;
}
int main() {
  auto file = sires::slurp("../../day1.txt").take([](auto e) {
    silog::log(silog::error, "failure: %s", e);
    throw 1;
  });
  silog::log(silog::info, "file size: %d", file.size());

  unsigned max[3]{};
  unsigned elf{};
  jute::view f{file.begin(), file.size()};
  while (f != "") {
    auto [line, rest] = f.split('\n');
    if (line == "") {
      if (elf > max[0]) {
        max[2] = max[1];
        max[1] = max[0];
        max[0] = elf;
      } else if (elf > max[1]) {
        max[2] = max[1];
        max[1] = elf;
      } else if (elf > max[0]) {
        max[0] = elf;
      }
      silog::log(silog::debug, "%d %d %d -- %d", max[0], max[1], max[2], elf);

      elf = 0;
    } else {
      elf += atoi(line);
    }
    f = rest;
  }

  auto sum = max[0] + max[1] + max[2];
  silog::log(silog::info, "sum: %d", sum);
}
