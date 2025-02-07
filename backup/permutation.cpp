#include "permutation.h"
#include <oneapi/tbb.h>

void PermutationProcessor::process_combinations_range(
    std::vector<int> &combination, int pos, int start) {
  if (pos == k) {
    do {
      processor(combination);
    } while (std::next_permutation(combination.begin(), combination.end()));

    return;
  }

  int end = n - (k - pos) + 1;
  for (int i = start; i < end; ++i) {
    combination[pos] = i;
    process_combinations_range(combination, pos + 1, i + 1);
  }
}
void PermutationProcessor::process_from_start(int start) {
  std::vector<int> combination(k);
  combination[0] = start;
  process_combinations_range(combination, 1, start + 1);
}

PermutationProcessor::PermutationProcessor(
    int n_, int k_, std::function<void(std::vector<int>)> func_)
    : n(n_), k(k_), processor(func_) {
  if (k_ > n_)
    throw std::invalid_argument("k cannot be greater than n");
};

template <bool isPermutation> void PermutationProcessor::process_all() {

  tbb::parallel_for(0, n - k + 1, [this](int i) { process_from_start(i); });
}

template void PermutationProcessor::process_all<true>();
template void PermutationProcessor::process_all<false>();