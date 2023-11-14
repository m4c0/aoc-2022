#pragma leco tool
import bruh;
import jute;
import silog;

unsigned sizes[1000];
unsigned ssz{};

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

  int total{};
  for (auto i = 0; i < ssz; i++) {
    auto s = sizes[i];
    if (s <= 100000)
      total += s;
  }
  silog::log(silog::debug, "%d", total);
}
