#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

// Fungsi untuk menghitung fungsi objektif
int fungsi_objektif(int a, int b, int c, int d) {
    return abs((a + 4*b + 2*c + 3*d) - 30);
}

// Fungsi untuk menghasilkan populasi awal
vector<vector<int> > inisialisasi_populasi(int jumlah_populasi) {
    vector<vector<int> > populasi;
    srand(time(0)); // Untuk menghasilkan angka acak berbeda setiap kali program dijalankan
    for (int i = 0; i < jumlah_populasi; i++) {
        vector<int> chromosome;
        chromosome.push_back(rand() % 31); // Nilai a antara 0-30
        chromosome.push_back(rand() % 11); // Nilai b antara 0-10
        chromosome.push_back(rand() % 11); // Nilai c antara 0-10
        chromosome.push_back(rand() % 11); // Nilai d antara 0-10
        populasi.push_back(chromosome);
    }
    return populasi;
}

// Fungsi untuk melakukan seleksi chromosome
vector<vector<int> > seleksi_chromosome(vector<vector<int> >& populasi) {
    vector<double> fitness;
    int total_fitness = 0;
    for (int i = 0; i < populasi.size(); i++) {
        int objektif = fungsi_objektif(populasi[i][0], populasi[i][1], populasi[i][2], populasi[i][3]);
        double fit = 1.0 / (1.0 + objektif);
        fitness.push_back(fit);
        total_fitness += fit;
    }

    vector<double> probabilitas;
    double kumulatif = 0;
    for (int i = 0; i < populasi.size(); i++) {
        double p = fitness[i] / total_fitness;
        probabilitas.push_back(p);
        kumulatif += p;
    }

    vector<vector<int> > populasi_baru;
    srand(time(0));
    for (int i = 0; i < populasi.size(); i++) {
        double r = (double)rand() / RAND_MAX;
        double kumulatif_sebelumnya = 0;
        for (int j = 0; j < populasi.size(); j++) {
            if (kumulatif_sebelumnya < r && r < kumulatif_sebelumnya + probabilitas[j]) {
                populasi_baru.push_back(populasi[j]);
                break;
            }
            kumulatif_sebelumnya += probabilitas[j];
        }
    }

    return populasi_baru;
}

// Fungsi untuk melakukan crossover
void crossover(vector<vector<int> >& populasi, double crossover_rate) {
    srand(time(0));
    int jumlah_crossover = crossover_rate * populasi.size();
    for (int i = 0; i < jumlah_crossover; i++) {
        int idx1 = rand() % populasi.size();
        int idx2 = rand() % populasi.size();
        int cut_point = rand() % 4; // Pilih posisi pemotongan secara acak

        for (int j = cut_point; j < 4; j++) {
            int temp = populasi[idx1][j];
            populasi[idx1][j] = populasi[idx2][j];
            populasi[idx2][j] = temp;
        }
    }
}

// Fungsi untuk melakukan mutasi
void mutasi(vector<vector<int> >& populasi, double mutation_rate) {
    srand(time(0));
    int total_gen = populasi.size() * 4;
    int jumlah_mutasi = mutation_rate * total_gen;
    for (int i = 0; i < jumlah_mutasi; i++) {
        int idx = rand() % populasi.size();
        int gen_idx = rand() % 4;
        populasi[idx][gen_idx] = rand() % (gen_idx == 0 ? 31 : 11); // Batas nilai gen yang berbeda untuk variabel a dan variabel lainnya
    }
}

int main() {
    int jumlah_populasi = 6;
    double crossover_rate = 0.8;
    double mutation_rate = 0.1;
    int generasi = 100;

    vector<vector<int> > populasi = inisialisasi_populasi(jumlah_populasi);

    for (int i = 0; i < generasi; i++) {
        populasi = seleksi_chromosome(populasi);
        crossover(populasi, crossover_rate);
        mutasi(populasi, mutation_rate);
    }

    // Cari chromosome dengan nilai fungsi objektif terkecil
    int min_objektif = INT_MAX;
    vector<int> solusi;
    for (int i = 0; i < populasi.size(); i++) {
        int objektif = fungsi_objektif(populasi[i][0], populasi[i][1], populasi[i][2], populasi[i][3]);
        if (objektif < min_objektif) {
            min_objektif = objektif;
            solusi = populasi[i];
        }
    }

    // Tampilkan solusi
    cout << "Solusi: a=" << solusi[0] << ", b=" << solusi[1] << ", c=" << solusi[2] << ", d=" << solusi[3] << endl;

    return 0;
}

