#include <bits/stdc++.h>
using namespace std;


struct Metrics {
    long long time = 0;
    long long comparisons = 0;
    long long swaps = 0;
    int count = 0;
};


vector<string> split(const string& line) {
    vector<string> result;
    stringstream ss(line);
    string item;

    while (getline(ss, item, ',')) {
        result.push_back(item);
    }
    return result;
}


int main() {
    ifstream in("results_detailed.csv");
    ofstream out("results_summary_from_detailed.csv");

    if (!in.is_open()) {
        cout << "Error: could not open results_detailed.csv\n";
        return 1;
    }


    string line;
    getline(in, line);

    map<string, Metrics> data;

    while (getline(in, line)) {
        if (line.empty()) continue;

        vector<string> fields = split(line);

        string category = fields[0];
        string size = fields[1];
        string algorithm = fields[3];

        long long time = stoll(fields[4]);
        long long comparisons = stoll(fields[5]);
        long long swaps = stoll(fields[6]);

        string key = category + "|" + size + "|" + algorithm;

        data[key].time += time;
        data[key].comparisons += comparisons;
        data[key].swaps += swaps;
        data[key].count++;
    }

    out << "Category,Size,Algorithm,AvgTime(us),AvgComparisons,AvgSwaps,Count\n";

    for (auto& [key, m] : data) {
        stringstream ss(key);
        string category, size, algorithm;

        getline(ss, category, '|');
        getline(ss, size, '|');
        getline(ss, algorithm, '|');

        out << category << ","
            << size << ","
            << algorithm << ","
            << (m.time / m.count) << ","
            << (m.comparisons / m.count) << ","
            << (m.swaps / m.count) << ","
            << m.count << "\n";
    }

    cout << "Summary written to results_summary_from_detailed.csv\n";
    return 0;
}
