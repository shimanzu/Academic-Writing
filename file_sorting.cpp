#include <bits/stdc++.h>
#include <filesystem>
using namespace std;
using namespace std::chrono;
namespace fs = std::filesystem;


struct Metrics {
    long long comparisons = 0;
    long long swaps = 0;
    long long time = 0;
};




void insertionSort(vector<int>& arr, Metrics& m) {
    for (int i = 1; i < arr.size(); i++) {
        int key = arr[i];
        int j = i - 1;

        while (j >= 0) {
            m.comparisons++;
            if (arr[j] > key) {
                arr[j + 1] = arr[j];
                m.swaps++;
                j--;
            } else break;
        }
        arr[j + 1] = key;
    }
}


int partitionQS(vector<int>& arr, int low, int high, Metrics& m) {
    int pivot = arr[high];
    int i = low - 1;

    for (int j = low; j < high; j++) {
        m.comparisons++;
        if (arr[j] < pivot) {
            i++;
            swap(arr[i], arr[j]);
            m.swaps++;
        }
    }
    swap(arr[i + 1], arr[high]);
    m.swaps++;
    return i + 1;
}

void quickSort(vector<int>& arr, int low, int high, Metrics& m) {
    if (low < high) {
        int pi = partitionQS(arr, low, high, m);
        quickSort(arr, low, pi - 1, m);
        quickSort(arr, pi + 1, high, m);
    }
}


void merge(vector<int>& arr, int l, int m, int r, Metrics& met) {
    int n1 = m - l + 1, n2 = r - m;
    vector<int> L(arr.begin() + l, arr.begin() + m + 1);
    vector<int> R(arr.begin() + m + 1, arr.begin() + r + 1);

    int i = 0, j = 0, k = l;

    while (i < n1 && j < n2) {
        met.comparisons++;
        if (L[i] <= R[j]) arr[k++] = L[i++];
        else arr[k++] = R[j++];
        met.swaps++;
    }

    while (i < n1) arr[k++] = L[i++];
    while (j < n2) arr[k++] = R[j++];
}

void mergeSort(vector<int>& arr, int l, int r, Metrics& met) {
    if (l < r) {
        int m = (l + r) / 2;
        mergeSort(arr, l, m, met);
        mergeSort(arr, m + 1, r, met);
        merge(arr, l, m, r, met);
    }
}



vector<int> readFile(const string& filename) {
    ifstream in(filename);
    vector<int> v;
    int x;
    while (in >> x) v.push_back(x);
    return v;
}

template <typename Func>
long long measure(Func f) {
    auto start = high_resolution_clock::now();
    f();
    auto end = high_resolution_clock::now();
    return duration_cast<microseconds>(end - start).count();
}


int main() {

    vector<string> categories = {"random", "nearly", "reversed", "alternating"};

    ofstream detailed("results_detailed.csv");
    ofstream summary("results_summary.csv");

    detailed << "Category,Size,File,Algorithm,Time(us),Comparisons,Swaps\n";
    summary << "Category,Size,Algorithm,AvgTime(us),AvgComparisons,AvgSwaps\n";

    for (auto& category : categories) {
        if (!fs::exists(category)) continue;

        for (const auto& sizeDir : fs::directory_iterator(category)) {
            if (!sizeDir.is_directory()) continue;

            string sizeName = sizeDir.path().filename().string();
            int n = stoi(sizeName);

            cout << "\n===== " << category << " | size=" << n << " =====\n";

            map<string, Metrics> totals;
            map<string, int> counts;

            for (const auto& entry : fs::directory_iterator(sizeDir.path())) {
                if (!entry.is_regular_file()) continue;

                string filepath = entry.path().string();
                string filename = entry.path().filename().string();

                vector<int> original = readFile(filepath);

                cout << "[" << category << "/" << n << "] " << filename << "\n";

                bool runInsertion = (category == "nearly") || (n < 101000);


                if (runInsertion) {
                    cout << "   -> InsertionSort...\n";

                    auto arr = original;
                    Metrics m;
                    m.time = measure([&]() { insertionSort(arr, m); });

                    detailed << category << "," << n << "," << filename
                             << ",InsertionSort," << m.time << ","
                             << m.comparisons << "," << m.swaps << "\n";

                    totals["InsertionSort"].time += m.time;
                    totals["InsertionSort"].comparisons += m.comparisons;
                    totals["InsertionSort"].swaps += m.swaps;
                    counts["InsertionSort"]++;
                } else {
                    cout << "   -> InsertionSort skipped\n";
                }


                {
                    cout << "   -> QuickSort...\n";

                    auto arr = original;
                    Metrics m;
                    m.time = measure([&]() { quickSort(arr, 0, arr.size()-1, m); });

                    detailed << category << "," << n << "," << filename
                             << ",QuickSort," << m.time << ","
                             << m.comparisons << "," << m.swaps << "\n";

                    totals["QuickSort"].time += m.time;
                    totals["QuickSort"].comparisons += m.comparisons;
                    totals["QuickSort"].swaps += m.swaps;
                    counts["QuickSort"]++;
                }


                {
                    cout << "   -> MergeSort...\n";

                    auto arr = original;
                    Metrics m;
                    m.time = measure([&]() { mergeSort(arr, 0, arr.size()-1, m); });

                    detailed << category << "," << n << "," << filename
                             << ",MergeSort," << m.time << ","
                             << m.comparisons << "," << m.swaps << "\n";

                    totals["MergeSort"].time += m.time;
                    totals["MergeSort"].comparisons += m.comparisons;
                    totals["MergeSort"].swaps += m.swaps;
                    counts["MergeSort"]++;
                }
            }


            for (auto& [algo, m] : totals) {
                int c = counts[algo];
                if (c == 0) continue;

                summary << category << "," << n << "," << algo << ","
                        << (m.time / c) << ","
                        << (m.comparisons / c) << ","
                        << (m.swaps / c) << "\n";
            }
        }
    }

    cout << "\nDone. Results saved to CSV files.\n";
    return 0;
}