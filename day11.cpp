#pragma leco tool
import bruh;
import jute;
import silog;

constexpr const auto MONKEYS = 8;
constexpr const auto MAX = 30;

class item {
  static constexpr const auto max = 20;
  int rems[max]{};

public:
  constexpr item() = default;
  explicit item(unsigned in) {
    for (auto i = 1; i < max; i++) {
      rems[i] = (rems[i] + in) % i;
    }
  }

  item &operator+=(unsigned in) {
    for (auto i = 1; i < max; i++) {
      rems[i] = (rems[i] + in) % i;
    }
    return *this;
  }
  item &operator*=(unsigned in) {
    for (auto i = 1; i < max; i++) {
      auto iii = in % i;
      rems[i] = (rems[i] * iii) % i;
    }
    return *this;
  }

  int &operator%(unsigned in) { return rems[in]; }

  void sq() {
    for (auto i = 1; i < max; i++) {
      rems[i] = (rems[i] * rems[i]) % i;
    }
  }
};
struct {
  item items[MAX]{};
  int icount{};
  int rems[MAX]{};
  int inspects{};

  void push(item i) { items[icount++] = i; }
} monkeys[MONKEYS];
int mindex{};

void read_items(jute::view line) {
  auto [l, r] = line.split(':');
  if (l != "  Starting items")
    return;

  auto &m = monkeys[mindex++];
  while (r != "") {
    auto [n, rest] = r.split(',');
    auto [na, nb] = n.subview(1);
    m.push(item{atoi(nb)});
    r = rest;
  }
}

void run(jute::view line) {
  auto [l, r] = line.split(':');
  auto &m = monkeys[mindex];
  if (l == "  Operation") {
    for (auto i = 0; i < m.icount; i++) {
      auto &old = m.items[i];
      auto [ml, mr] = r.split('*');
      if (mr != "") {
        auto [mrl, mrr] = mr.subview(1);
        if (mrr == "old") {
          old.sq();
          // old *= old;
        } else {
          old *= atoi(mrr);
        }
      } else {
        auto [pl, pr] = r.split('+');
        auto [prl, prr] = pr.subview(1);
        old += atoi(prr);
      }
      // old /= 3;
    }
  }

  if (l == "  Test") {
    auto [l, r] = line.split('y');
    auto n = atoi(r.subview(1).after);
    for (auto i = 0; i < m.icount; i++) {
      m.rems[i] = m.items[i] % n;
    }
    m.inspects += m.icount;
  }

  if (l == "    If true") {
    auto [l, r] = line.split('y');
    auto n = atoi(r.subview(1).after);
    for (auto i = 0; i < m.icount; i++) {
      if (m.rems[i] != 0)
        continue;

      // silog::log(silog::debug, "throw %d to %d", m.items[i], n);
      monkeys[n].push(m.items[i]);
    }
  }

  if (l == "    If false") {
    auto [l, r] = line.split('y');
    auto n = atoi(r.subview(1).after);
    for (auto i = 0; i < m.icount; i++) {
      if (m.rems[i] == 0)
        continue;

      // silog::log(silog::debug, "throw %d to %d", m.items[i], n);
      monkeys[n].push(m.items[i]);
    }

    m.icount = 0;
    mindex++;
  }
}

int main() {
  mindex = 0;
  loop(read_items);

  for (auto i = 0; i < 10000; i++) {
    mindex = 0;
    loop(run);
  }

  int max[2]{};
  for (auto &m : monkeys) {
    info("m", m.inspects);
    auto i = m.inspects;
    if (i > max[0]) {
      max[1] = max[0];
      max[0] = i;
    } else if (i > max[1]) {
      max[1] = i;
    }
  }

  long l = (long)max[0] * (long)max[1];
  silog::log(silog::debug, "%ld", l);
}
