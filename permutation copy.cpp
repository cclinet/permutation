#include <atomic>
#include <format>
#include <iostream>
#include <mutex>
#include <thread>
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

void generate_combinations(int start, int n, int k, std::vector<int> current,
                           std::vector<std::vector<int>> &results,
                           std::atomic_int64_t &counter,
                           std::mutex &result_mutex) {
  // 如果组合已经完整，保存它
  if (current.size() == k) {
    counter++;
    const std::lock_guard<std::mutex> lock(result_mutex);

    results.push_back(current);

    return;
  }

  // 从 start 开始遍历，递归生成组合

  for (int i = start; i < n; ++i) {
    current.push_back(i);
    generate_combinations(i + 1, n, k, current, results, counter, result_mutex);
    current.pop_back();
  }
}

vector<vector<int>> generate_combinations_parallel(int n, int k) {
  std::mutex result_mutex;
  std::atomic_int64_t counter;

  std::vector<std::vector<int>> results;
  results.reserve(2629575);

  // 创建多个线程来并行生成组合
  std::vector<std::thread> threads;
  for (int i = 0; i < n; ++i) {
    std::vector<int> current{i};
    current.reserve(n);
    threads.push_back(std::thread(generate_combinations, i + 1, n, k,
                                  std::ref(current), std::ref(results),
                                  std::ref(counter), std::ref(result_mutex)));
  }

  // 等待所有线程完成
  for (auto &t : threads) {
    t.join();
  }
  std::cout << counter << '\n';
  return results;
}

int main() {
  auto s = generate_combinations_parallel(8, 2);
  std::cout << s.size() << '\n';
}