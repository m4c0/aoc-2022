#pragma leco tool
import bruh;
import jute;
import silog;

char crt[6][40]{};

int cycles{};
int cm{40};
int res{};
int reg{1};
void snap() {
  unsigned row = cycles / 40;
  unsigned col = cycles % 40;
  if (row < 0 || row > 6 || col < 0 || col >= 40)
    throw 0;
  if (reg - 1 <= col && col <= reg + 1) {
    crt[row][col] = '#';
  } else {
    crt[row][col] = '.';
  }

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

  for (auto &row : crt) {
    silog::log(silog::debug, "%.*s", 40, row);
  }
}
