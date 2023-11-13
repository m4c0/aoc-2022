#pragma leco tool
import bruh;
import jute;
import silog;

void run(jute::view line) {
  for (auto i = 0; i < line.size() - 14; i++) {
    bool mask[256]{};
    bool ok{true};
    for (auto j = 0; j < 14; j++) {
      unsigned c = line[i + j];
      if (mask[c]) {
        ok = false;
        break;
      }
      mask[c] = true;
    }
    if (ok)
      throw i + 14;
  }
  throw -1;
}
int main() {
  try {
    loop(run);
  } catch (int i) {
    silog::log(silog::debug, "%d", i);
  }
}
