export module bruh;
import jute;
import silog;
import sires;

export auto slurp(jute::view fn) {
  auto file = sires::slurp(fn).take([](auto e) {
    silog::log(silog::error, "failure: %s", e);
    throw 1;
  });
  silog::log(silog::info, "file size: %d", file.size());

  return jute::view{file.begin(), file.size()};
}

export void loop(jute::view fn, auto &&func) {
  auto f = slurp(fn);
  while (f != "") {
    auto [line, rest] = f.split('\n');
    func(line);
    f = rest;
  }
}
export void loop(auto &&func) { loop("../../data.txt", func); }

export unsigned atoi(jute::view str) {
  unsigned n = 0;
  for (auto c : str) {
    if (c < '0' || c > '9')
      break;
    n = n * 10 + (c - '0');
  }
  return n;
}
