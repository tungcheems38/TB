#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>
#include <random>
#include <list>
using namespace std;

int chiphi(const vector<int>& route, const vector<vector<int>>& distance) {
    int total_distance = 0;
    for (int i = 0; i < route.size(); ++i) {
        int current_city = route[i];
        int next_city;
        if (i == route.size() - 1) {
            next_city = route[0];
        } else {
            next_city = route[i + 1];
        }
        total_distance += distance[current_city][next_city];
    }
    return total_distance;
}

vector<int> lotrinhngaunhien(vector<int>& route, const int& n) {
    for (int i = 0; i < n; ++i) {
        route[i] = i;
    }
    /*random_device rd;
    mt19937 gen(rd());
    shuffle(*/
    random_shuffle(route.begin(), route.end());
    return route;
}

vector<int> generate_neighbor_route(const vector<int>& route, const vector<vector<int>>& distance) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(0, route.size() - 1);
    int city1 = dis(gen);
    int city2 = dis(gen);
    while (city1 == city2) {
        city2 = dis(gen);
    }

    vector<int> neighbor_route = route;
    swap(neighbor_route[city1], neighbor_route[city2]);
    int new_distance = chiphi(neighbor_route, distance);
    if (new_distance < chiphi(route, distance)) {
        return neighbor_route;
    } else {
        return route;
    }
}

bool is_tabu(const vector<int>& route, list<vector<int>>& tabu_list, int tabu_size) {
    for (const vector<int>& tabu_route : tabu_list) {
        if (route == tabu_route) {
            return true;
        }
    }
    return false;
}

void add_to_tabu_list(const vector<int>& route, list<vector<int>>& tabu_list, int tabu_size) {
    tabu_list.push_front(route);
    while (tabu_list.size() > tabu_size) {
        tabu_list.pop_back();
    }
}

int main() {
    int n;
    cin >> n;
    vector<vector<int>> distance(n, vector<int>(n));
    vector<int> route(n);
    ifstream fin("D:\\C++\\Tabusearch.txt");
    if (!fin) {
        cerr << "Error opening file" << endl;
        return 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fin >> distance[i][j];
            cout << distance[i][j] << " ";
        }
        cout << endl;
    }
    fin.close();

    lotrinhngaunhien(route, n);
    int chiphihientai = chiphi(route, distance);

    list<vector<int>> tabu_list;
    const int tabu_size = 10;
    int max_iterations = 10;
    while (max_iterations-- > 0) {
        vector<int> neighbor_route = generate_neighbor_route(route, distance);
        lotrinhngaunhien(route, n);
        chiphihientai = chiphi(route, distance);
        if (!is_tabu(neighbor_route, tabu_list, tabu_size)) {
            int chiphimoi = chiphi(neighbor_route, distance);
            if (chiphimoi < chiphihientai) {
                route = neighbor_route;
                chiphihientai = chiphimoi;
                add_to_tabu_list(route, tabu_list, tabu_size);
            }
        }
    }

    cout << "Lo trinh lan can tot nhat: ";
    for (int city : route) {
        cout << city << " -> ";
    }
    cout << route[0] << endl; // to close the loop in output
    cout << "Tong chieu dai: " << chiphihientai << endl;

    return 0;
}
