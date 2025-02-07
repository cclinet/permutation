#ifndef COMBINATION_PROCESSOR_H
#define COMBINATION_PROCESSOR_H
#include <functional>
#include <oneapi/tbb.h>
#include <vector>

template <bool isPermutation> class PermutationProcessor {
private:
  const int n, k;
  std::function<void(std::vector<int>)> processor;

  void process_combinations_range(std::vector<int> &combination, int pos,
                                  int start) {

    if (pos == k) {
      if constexpr (isPermutation) {
        do {
          processor(combination);
        } while (std::next_permutation(combination.begin(), combination.end()));
      } else {
        processor(combination);
      }

      return;
    }

    int end = n - (k - pos) + 1;
    for (int i = start; i < end; ++i) {
      combination[pos] = i;
      process_combinations_range(combination, pos + 1, i + 1);
    }
  };

  void process_from_start(int start) {
    std::vector<int> combination(k);
    combination[0] = start;
    process_combinations_range(combination, 1, start + 1);
  };

public:
  PermutationProcessor(int n_, int k_,
                       std::function<void(std::vector<int>)> processor_)
      : n(n_), k(k_), processor(processor_) {
    if (k_ > n_)
      throw std::invalid_argument("k cannot be greater than n");
  };
  ;
  void process_all() {
    tbb::parallel_for(0, n - k + 1, [this](int i) { process_from_start(i); });
  };
};
#endif