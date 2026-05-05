#include <bits/stdc++.h>
#include <filesystem>
using namespace std;
namespace fs = std::filesystem;


vector<int> generateRandom(int n) {
    vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = rand();
    return v;
}

vector<int> generateNearlySorted(int n) {
    vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = i;

    for (int i = 0; i < max(1, n / 100); i++) {
        int a = rand() % n;
        int b = rand() % n;
        swap(v[a], v[b]);
    }
    return v;
}

vector<int> generateReversed(int n) {
    vector<int> v(n);
    for (int i = 0; i < n; i++) v[i] = n - i;
    return v;
}

vector<int> generateAlternating(int n) {
    vector<int> v(n);
    int low = 0, high = n - 1;
    for (int i = 0; i < n; i++) {
        v[i] = (i % 2 == 0) ? low++ : high--;
    }
    return v;
}


void saveFile(const string& path, const vector<int>& v) {
    ofstream out(path);
    for (int x : v) out << x << " ";
}


void generateCategory(const string& category,
                      int n,
                      int files,
                      const vector<int>& sizes,
                      function<vector<int>(int)> gen) {

    for (int size : sizes) {
        string folder = category + "/" + to_string(size);
        fs::create_directories(folder);

        cout << "\nGenerating " << category << " | size=" << size << "\n";

        for (int i = 0; i < files; i++) {
            vector<int> data = gen(size);
            string filename = folder + "/data_" + to_string(i) + ".txt";
            saveFile(filename, data);
        }
    }
}


int main() {
    srand(time(0));

    int files;
    cout << "Number of files per size/category: ";
    cin >> files;

    vector<int> sizes;
    int sz;

    cout << "Enter sizes (-1 to stop): ";
    while (cin >> sz && sz != -1) {
        sizes.push_back(sz);
    }

    generateCategory("random", 0, files, sizes, generateRandom);
    generateCategory("nearly", 0, files, sizes, generateNearlySorted);
    generateCategory("reversed", 0, files, sizes, generateReversed);
    generateCategory("alternating", 0, files, sizes, generateAlternating);

    cout << "\nDone generating datasets.\n";
    return 0;
}