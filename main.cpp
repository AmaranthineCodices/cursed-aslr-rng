#include <dlfcn.h>
#include <fstream>
#include <iostream>
#include <stdint.h>

using RandomPtr = uint64_t (*)();

size_t kCount = 0;

extern "C" __attribute__((visibility("default"))) uint64_t aslr_random() {
  char *r = new char[4097];
  r[21] = 'f';
  uint64_t value = uint64_t(r);
  delete[] r;

  return value;
}

uint64_t aslr_rng(const char *self_name) {
  std::string lib_path = self_name + std::to_string(kCount++);

  {
    std::ifstream src(self_name, std::ios::binary);
    std::ofstream dst(lib_path, std::ios::binary);
    dst << src.rdbuf();
  }

  void *handle = dlopen(lib_path.c_str(), RTLD_LAZY | RTLD_LOCAL);
  if (!handle) {
    std::cerr << "couldn't load library :(\n" << dlerror() << std::endl;
    std::exit(1);
  }

  RandomPtr aslr_random;
  aslr_random = reinterpret_cast<RandomPtr>(dlsym(handle, "aslr_random"));

  if (char *error = dlerror(); error != nullptr) {
    std::cerr << "aslr_random not found :(\n" << error << std::endl;
    std::exit(1);
  }

  uint64_t result = aslr_random();
  // dlclose(handle);
  // std::remove(lib_path.c_str());
  return result;
}

int main(int argc, char **argv) {
  for (size_t i = 0; i < 50; ++i)
    std::cout << aslr_rng(argv[0]) << std::endl;

  return 0;
}
