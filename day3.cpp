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
  unsigned sum{};
  loop("../../day3.txt", [&](auto line) {
    constexpr const auto N = 100;
    auto [a, b] = line.subview(line.size() / 2);
    bool meh[N]{};
    for (auto ca : a) {
      for (auto &cb : b) {
        if (ca == cb) {
          meh[prio(ca)] = true;
          break;
        }
      }
    }

    for (auto i = 0; i < N; i++) {
      if (meh[i])
        sum += i;
    }
  });

  silog::log(silog::info, "sum: %d", sum);
}
