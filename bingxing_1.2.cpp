#include <iostream>
#include <vector>
#include <chrono>
#include <iomanip>
#include <string>

using namespace std;
using namespace std::chrono;

// 递归算法：不断将数组两端对应的元素相加
int calculation(vector<int> &a, int n)
{
  if (n == 1)
    return a[0];
  int newSize = n / 2;
  if (n % 2 == 1) // 如果元素个数为奇数，则将中间的元素直接保留到下一层
    newSize++;
  for (int i = 0; i < n / 2; i++)
  {
    a[i] = a[i] + a[n - i - 1];
  }
  if (n % 2 == 1)
  {
    a[newSize - 1] = a[n / 2];
  }
  return calculation(a, newSize);
}

int main()
{
  vector<int> text_Size = {2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192};
  int repeatCount = 20;

  cout << setw(10) << "Size"
       << setw(15) << "Normal(ms)"
       << setw(15) << "Better(ms)"
       << setw(15) << "Recursive(ms)"
       << setw(15) << "Half(ms)" << endl;

  for (int n : text_Size)
  {
    double totalTime_Normal = 0.0;
    double totalTime_Better = 0.0;
    double totalTime_Recursive = 0.0;
    double totalTime_Half = 0.0;

    for (int rep = 0; rep < repeatCount; rep++)
    {
      // 平凡算法：直接循环求和
      vector<int> vec(n);
      for (int i = 0; i < n; i++)
        vec[i] = i + 1;
      int sum = 0;
      auto start = high_resolution_clock::now();
      for (int i = 0; i < n; i++)
      {
        sum += vec[i];
      }
      auto end = high_resolution_clock::now();
      totalTime_Normal += duration<double, milli>(end - start).count();
      // 优化算法一：两两累加
      vector<int> vec1(n);
      for (int i = 0; i < n; i++)
        vec1[i] = i + 1;
      int sum1 = 0, sum2 = 0;
      start = high_resolution_clock::now();
      // 每次取相邻两个元素相加
      for (int i = 0; i < n; i += 2)
      {
        sum1 += vec1[i];
        if (i + 1 < n)
        {
          sum2 += vec1[i + 1];
        }
      }
      sum = sum1 + sum2;
      end = high_resolution_clock::now();
      totalTime_Better += duration<double, milli>(end - start).count();
      // 优化算法二：递归方式不断合并（递归算法）
      vector<int> vec2(n);
      for (int i = 0; i < n; i++)
        vec2[i] = i + 1;
      start = high_resolution_clock::now();
      sum = calculation(vec2, vec2.size());
      end = high_resolution_clock::now();
      totalTime_Recursive += duration<double, milli>(end - start).count();
      // 算法三：迭代折半合并
      vector<int> vec3(n);
      for (int i = 0; i < n; i++)
        vec3[i] = i + 1;
      start = high_resolution_clock::now();
      int m = n;
      while (m > 1)
      {
        int newSize = m / 2;
        if (m % 2 == 1)
        { // 若 m 为奇数，最后一个元素直接保留
          for (int i = 0; i < m / 2; i++)
          {
            vec3[i] = vec3[i * 2] + vec3[i * 2 + 1];
          }
          vec3[newSize] = vec3[m - 1];
          m = newSize + 1;
        }
        else
        {
          for (int i = 0; i < m / 2; i++)
          {
            vec3[i] = vec3[i * 2] + vec3[i * 2 + 1];
          }
          m = newSize;
        }
      }
      sum = vec3[0];
      end = high_resolution_clock::now();
      totalTime_Half += duration<double, milli>(end - start).count();
    }

    // 计算每种算法的平均时间
    double avgTime_Normal = totalTime_Normal / repeatCount;
    double avgTime_Better = totalTime_Better / repeatCount;
    double avgTime_Recursive = totalTime_Recursive / repeatCount;
    double avgTime_Half = totalTime_Half / repeatCount;

    // 输出结果
    cout << setw(10) << n
         << setw(15) << fixed << setprecision(6) << avgTime_Normal
         << setw(15) << avgTime_Better
         << setw(15) << avgTime_Recursive
         << setw(15) << avgTime_Half << endl;
  }

  return 0;
}
