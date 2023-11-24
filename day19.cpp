#pragma leco tool
import bruh;
import hai;
import jute;
import silog;

auto next_int(jute::view v) {
  struct res {
    jute::view a;
    jute::view b;
  };
  for (auto i = 0; i < v.size(); i++) {
    if (v[i] >= '0' && v[i] <= '9')
      for (auto j = i; j < v.size(); j++) {
        if (v[j] < '0' || v[j] > '9')
          return res{v.subview(i).after, v.subview(j).after};
      }
  }
  throw 0;
}
int main() {
  loop_e([&](auto line) {
    auto [bps, bpr] = next_int(line);
    auto [os, orr] = next_int(bpr);
    auto [cs, cr] = next_int(orr);
    auto [bos, bor] = next_int(cr);
    auto [bcs, bcr] = next_int(bor);
    auto [gos, gor] = next_int(bcr);
    auto [gbs, gbr] = next_int(gor);

    auto bp = atoi(bps);
    auto o = atoi(os);
    auto c = atoi(cs);
    auto bo = atoi(bos);
    auto bc = atoi(bcs);
    auto go = atoi(gos);
    auto gb = atoi(gbs);

    silog::log(silog::info, "%d %d %d %d %d %d %d", bp, o, c, bo, bc, go, gb);
  });
}
