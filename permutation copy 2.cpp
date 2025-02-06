#include <atomic>

#include <iostream>
#include <mutex>
#include <oneapi/tbb.h>

#include <vector>
using std::vector;

void print(const vector<int> &vec) {
  for (const auto &i : vec) {
    std::cout << i << ' ';
  }
  std::cout << '\n';
}

vector<vector<int>> combine(int n, int k) {
  vector<int> temp;
  vector<vector<int>> ans;
  // 初始化
  // 将 temp 中 [0, k - 1] 每个位置 i 设置为 i + 1，即 [0, k - 1] 存 [1, k]
  // 末尾加一位 n + 1 作为哨兵
  for (int i = 1; i <= k; ++i) {
    temp.push_back(i);
  }
  temp.push_back(n + 1);

  int j = 0;
  while (j < k) {
    ans.emplace_back(temp.begin(), temp.begin() + k);
    j = 0;
    // 寻找第一个 temp[j] + 1 != temp[j + 1] 的位置 t
    // 我们需要把 [0, t - 1] 区间内的每个位置重置成 [1, t]
    while (j < k && temp[j] + 1 == temp[j + 1]) {
      temp[j] = j + 1;
      ++j;
    }
    // j 是第一个 temp[j] + 1 != temp[j + 1] 的位置
    ++temp[j];
  }
  return ans;
};

void generate_combinations(int start, int n, int k, std::vector<int> &current,

                           std::atomic_int64_t &counter) {
  // 如果组合已经完整，保存它
  if (current.size() == k) {
    counter++;
    return;
  }

  // 从 start 开始遍历，递归生成组合

  for (int i = start; i < n; ++i) {
    current.push_back(i);
    generate_combinations(i + 1, n, k, current, counter);
    current.pop_back();
  }
}

void generate_combinations_parallel(int n, int k) {
  std::mutex result_mutex;
  std::atomic_int64_t counter;

  tbb::parallel_for(0, n, [&](int i) {
    std::vector<int> current{i};
    current.reserve(n);
    generate_combinations(i + 1, n, k, current, counter);
  });

  std::cout << counter << '\n';
}

int main() { generate_combinations_parallel(31, 24); }