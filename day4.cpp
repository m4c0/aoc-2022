#pragma leco tool
import bruh;
import silog;

int main() {
  int count{};
  loop("../../data.txt", [&](auto line) {
    auto [a, b] = line.split(',');
    auto [a0, a1] = a.split('-');
    auto [b0, b1] = b.split('-');

    auto a0i = atoi(a0);
    auto a1i = atoi(a1);
    auto b0i = atoi(b0);
    auto b1i = atoi(b1);

    bool yeah = (a0i <= b0i && b1i <= a1i) || (a0i >= b0i && b1i >= a1i);
    silog::log(silog::debug, "%d-%d -- %d-%d -- %c", a0i, a1i, b0i, b1i,
               yeah ? '!' : ' ');

    if (yeah)
      count++;
  });
  silog::log(silog::info, "%d", count);
}
