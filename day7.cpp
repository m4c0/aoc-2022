#pragma leco tool
import bruh;
import jute;
import silog;

unsigned sizes[1000];
unsigned ssz{};
unsigned total{};

class {
  jute::view parts[100];
  unsigned sidx[100];
  unsigned size{};

public:
  void push(jute::view part) {
    sidx[size] = ssz++;
    parts[size++] = part;
  }
  void pop() { size--; }

  void inc(int s) {
    for (auto i = 0; i < size; i++) {
      sizes[sidx[i]] += s;
    }
    total += s;
  }

  auto get() {
    jute::heap p{};
    for (auto i = 0; i < size; i++) {
      p = p + "/" + parts[i];
    }
    return p;
  }
} path;

void line(jute::view line) {
  auto [l, r] = line.split(' ');
  auto [cmd, param] = r.split(' ');
  if (l == "$" && cmd == "cd") {
    if (param == "/") {
      path = {};
    } else if (param == "..") {
      path.pop();
    } else {
      path.push(param);
    }
  } else if (l == "$" && cmd == "ls") {
  } else if (l == "dir") {
  } else {
    auto sz = atoi(l);
    path.inc(sz);
  }
}

int main() {
  loop(line);

  auto cap = 70000000;
  auto req = 30000000;
  auto free = cap - total;
  auto rel = req - free;

  silog::log(silog::debug, "total: %d -- free: %d -- to release: %d", total,
             free, rel);

  unsigned min = ~0;
  for (unsigned i = 0; i < ssz; i++) {
    auto sz = sizes[i];
    if (sz > rel && sz < min) {
      min = sz;
    }
  }

  silog::log(silog::debug, "%d", min);
}
