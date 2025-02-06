#include <atomic>
#include <iostream>
#include <oneapi/tbb.h>
#include <vector>

class CombinationProcessor {
private:
  const int n, k;
  std::atomic<uint64_t> counter{0};

  // 使用迭代方式生成组合，避免递归开销
  void process_combinations_range(std::vector<int> &combination, int pos,
                                  int start) {
    if (pos == k) {
      do {
        counter.fetch_add(1, std::memory_order_relaxed);
      } while (std::next_permutation(combination.begin(), combination.end()));

      return;
    }

    // 优化：提前计算结束位置，避免不必要的迭代
    int end = n - (k - pos) + 1;
    for (int i = start; i < end; ++i) {
      combination[pos] = i;
      process_combinations_range(combination, pos + 1, i + 1);
    }
  }

  // 并行处理一个起始数字的所有组合
  void process_from_start(int start) {
    std::vector<int> combination(k);
    combination[0] = start;
    process_combinations_range(combination, 1, start + 1);
  }

public:
  CombinationProcessor(int n_, int k_) : n(n_), k(k_) {
    if (k_ > n_)
      throw std::invalid_argument("k cannot be greater than n");
  }

  uint64_t process_all() {
    // 并行处理不同的起始数字
    tbb::parallel_for(0, n - k + 1, [this](int i) { process_from_start(i); });

    return counter.load();
  }
};

// 使用示例
int main() {
  try {
    int n = 24, k = 6;
    CombinationProcessor processor(n, k);

    auto start = std::chrono::high_resolution_clock::now();
    uint64_t count = processor.process_all();
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