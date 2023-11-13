#pragma leco tool
import bruh;
import silog;

constexpr auto prio(char c) {
  if (c >= 'a' && c <= 'z')
    return c - 'a' + 1;
  if (c >= 'A' && c <= 'Z')
    return c - 'A' + 27;

  return 0;
}
static_assert(prio('p') == 16);
static_assert(prio('L') == 38);

int main() {
  constexpr const auto N = prio('Z') + 1;

  unsigned sum;
  unsigned meh[N]{};
  unsigned bit{1};
  loop("../../day3.txt", [&](auto line) {
    for (auto c : line) {
      meh[prio(c)] |= bit;
    }

    if (bit != 1 << 2) {
      bit <<= 1;
      return;
    }

    for (auto i = 0; i < N; i++) {
      if (meh[i] == 0b111)
        sum += i;

      meh[i] = 0;
    }
    bit = 1 << 0;
  });

  silog::log(silog::info, "sum: %d", sum);
}
