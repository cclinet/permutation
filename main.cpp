#include "permutation.hpp"
#include <atomic>
#include <iostream>
#include <vector>
// 使用示例
int main() {
  try {
    std::atomic_uint64_t count{0};
    const auto processor_func = [&count](std::vector<int>) {
      count.fetch_add(1, std::memory_order_release);
    };
    int n = 24, k = 6;
    PermutationProcessor<true> processor(n, k, processor_func);

    auto start = std::chrono::high_resolution_clock::now();
    processor.process_all();
    auto end = std::chrono::high_resolution_clock::now();

    std::cout << "Processed combinations: " << count << std::endl;
    std::cout << "Time taken: "
              << std::chrono::duration_cast<std::chrono::milliseconds>(end -
                                                                       start)
                     .count()
              << "ms" << std::endl;
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}