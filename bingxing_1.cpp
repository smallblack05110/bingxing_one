#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;
using namespace std::chrono;

int main()
{
  // 定义测试规模
  vector<int> testSizes = {10, 20, 50, 100, 300, 500, 1000, 3000, 5000, 7000, 10000};

  const int col1Width1 = 20; // 矩阵规模
  const int col2Width1 = 30; // 重复次数
  const int col3Width1 = 40; // 平凡算法执行总时间(ms)
  const int col4Width1 = 50; // 平凡算法执行平均时间(ms)
  const int col5Width1 = 30; // 优化算法执行总时间(ms)
  const int col6Width1 = 30; // 优化算法执行平均时间(ms)

  const int col1Width = 20; // 矩阵规模
  const int col2Width = 25; // 重复次数
  const int col3Width = 35; // 平凡算法执行总时间(ms)
  const int col4Width = 35; // 平凡算法执行平均时间(ms)
  const int col5Width = 35; // 优化算法执行总时间(ms)
  const int col6Width = 35; // 优化算法执行平均时间(ms)
  int totalWidth = col1Width + col2Width + col3Width + col4Width + col5Width + col6Width;

  cout << fixed << setprecision(4);

  // 输出表格上边框
  cout << string(totalWidth, '=') << "\n";

  // 输出表头
  cout << left
       << setw(col1Width1) << "矩阵规模"
       << setw(col2Width1) << "重复次数"
       << setw(col3Width1) << "平凡总时间"
       << setw(col4Width1) << "平凡均时间"
       << setw(col5Width1) << "优化总时间"
       << setw(col6Width1) << "优化均时间"
       << setw(col5Width1) << "展开总时间"
       << setw(col6Width1) << "展开均时间"
       << "\n";

  // 输出表头下分割线
  cout << string(totalWidth, '-') << "\n";

  for (int n : testSizes)
  {
    int repeatCount = 20;
    vector<vector<int>> a(n, vector<int>(n, 0));
    vector<int> x(n, 1);

    // 初始化矩阵：a[i][j] = i + j
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        a[i][j] = i + j;

    double totalTime_Normal = 0.0;
    double totalTime_Better = 0.0;
    double totalTime_unroll = 0.0;

    // 平凡算法
    for (int rep = 0; rep < repeatCount; rep++)
    {
      vector<double> sum(n, 0.0);
      auto start = high_resolution_clock::now();
      for (int i = 0; i < n; i++)
        for (int j = 0; j < n; j++)
          sum[i] += a[j][i] * x[j];
      auto end = high_resolution_clock::now();
      totalTime_Normal += duration<double, milli>(end - start).count();
    }

    // 优化算法
    for (int rep = 0; rep < repeatCount; rep++)
    {
      vector<double> sum(n, 0.0);
      auto start = high_resolution_clock::now();
      for (int j = 0; j < n; j++)
        for (int i = 0; i < n; i++)
          sum[i] += a[j][i] * x[j];
      auto end = high_resolution_clock::now();
      totalTime_Better += duration<double, milli>(end - start).count();
    }

    // 循环展开
    for (int rep = 0; rep < repeatCount; rep++)
    {
      vector<double> sum(n, 0.0);
      auto start = high_resolution_clock::now();
      for (int j = 0; j < n; j++)
      {
        double scalar = x[j]; // 提前加载，减少重复内存访问
        int i = 0;
        // 内层循环展开，每次处理4个元素
        for (; i <= n - 4; i += 4)
        {
          sum[i] += a[j][i] * scalar;
          sum[i + 1] += a[j][i + 1] * scalar;
          sum[i + 2] += a[j][i + 2] * scalar;
          sum[i + 3] += a[j][i + 3] * scalar;
        }
        // 处理剩余不足4个的情况
        for (; i < n; i++)
        {
          sum[i] += a[j][i] * scalar;
        }
      }
      auto end = high_resolution_clock::now();
      totalTime_unroll += duration<double, milli>(end - start).count();
    }
    double avg_Normal = totalTime_Normal / repeatCount;
    double avg_Better = totalTime_Better / repeatCount;
    double avg_Unroll = totalTime_unroll / repeatCount;
    cout << left << setw(col1Width) << n
         << left << setw(col2Width) << repeatCount
         << left << setw(col3Width) << totalTime_Normal
         << left << setw(col4Width) << avg_Normal
         << left << setw(col5Width) << totalTime_Better
         << left << setw(col6Width) << avg_Better
         << left << setw(col5Width) << totalTime_unroll
         << left << setw(col6Width) << avg_Unroll
         << "\n";
  }
  // 输出表格底部边框
  cout << string(totalWidth, '=') << "\n";
  return 0;
}
