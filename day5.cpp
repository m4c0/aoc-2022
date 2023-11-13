#pragma leco tool

import bruh;
import jute;
import silog;

class queue {
  char m_data[100];
  unsigned m_size{};

public:
  constexpr void push(char c) { m_data[m_size++] = c; }
  constexpr char pop() { return m_data[--m_size]; }

  constexpr void push(jute::view s) {
    for (auto c : s) {
      m_data[m_size++] = c;
    }
  }
  constexpr jute::view pop(unsigned n) {
    m_size -= n;
    auto ptr = m_data + m_size;
    return {ptr, n};
  }

  constexpr void shove(char c) {
    if (c >= '0' && c <= '9')
      return;

    for (auto i = m_size; i > 0; i--) {
      m_data[i] = m_data[i - 1];
    }
    m_data[0] = c;
    m_size++;
  }

  constexpr char top() { return m_size == 0 ? ' ' : m_data[m_size - 1]; }

  void dump() { silog::log(silog::debug, "%.*s", m_size, m_data); }
};

static queue qs[9];

void dump() {
  for (auto &q : qs) {
    q.dump();
  }
}

void shove(jute::view line) {
  for (int i = 0; i < 9; i++) {
    char c = line[i * 4 + 1];
    if (c == ' ')
      continue;
    qs[i].shove(c);
  }
}

void run(jute::view line) {
  auto [move, mr] = line.split(' ');
  auto [qtys, qr] = mr.split(' ');
  auto [from, fr] = qr.split(' ');
  auto [as, ar] = fr.split(' ');
  auto [to, tr] = ar.split(' ');
  auto [bs, br] = tr.split(' ');

  auto a = atoi(as) - 1;
  auto b = atoi(bs) - 1;
  auto qty = atoi(qtys);

  silog::log(silog::debug, "---- %d %d %d", qty, a, b);
  dump();
  qs[b].push(qs[a].pop(qty));
}

int main() {
  bool qng = true;
  loop([&](auto line) {
    if (line == "") {
      qng = false;
      return;
    } else if (qng) {
      shove(line);
    } else {
      run(line);
    }
  });

  char buf[9]{};
  char *b = buf;
  for (auto &q : qs) {
    *b++ = q.top();
  }
  silog::log(silog::info, "%.9s", buf);
}
