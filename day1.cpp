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

  unsigned max{};
  unsigned elf{};
  jute::view f{file.begin(), file.size()};
  while (f != "") {
    auto [line, rest] = f.split('\n');
    if (line == "") {
      silog::log(silog::debug, "elf: %d - max: %d", elf, max);
      if (elf > max)
        max = elf;

      elf = 0;
    } else {
      elf += atoi(line);
    }
    f = rest;
  }
}
