#pragma leco tool
import bruh;
import jute;
import silog;

void run(jute::view line) {
  for (auto i = 0; i < line.size() - 4; i++) {
    char a = line[i + 0];
    char b = line[i + 1];
    char c = line[i + 2];
    char d = line[i + 3];
    if (a == b || a == c || a == d)
      continue;
    if (b == c || b == d)
      continue;
    if (c == d)
      continue;
    silog::log(silog::debug, "%d", i + 4);
    break;
  }
  throw 1;
}
int main() { loop(run); }
