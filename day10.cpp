#pragma leco tool
import bruh;
import jute;
import silog;

int cycles{};
int cm{20};
int res{};
int reg{1};
void snap() {
  cycles++;
  if (cycles != cm) {
    return;
  }

  silog::log(silog::debug, "%d %d %d", cm, reg, cycles * reg);
  res += cycles * reg;
  cm += 40;
}
void cycle() {}

int main() {
  loop([](auto line) {
    silog::log(silog::debug, "%.*s", (int)line.size(), line.data());
    if (line == "noop") {
      snap();
    } else {
      auto [cmd, n] = line.split(' ');
      if (cmd != "addx")
        return;

      snap();
      snap();
      reg += atoi(n);
    }
  });

  info("cycles", cycles);
  info("reg", reg);
  info("res", res);
}
