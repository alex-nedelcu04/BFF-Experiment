
#include <algorithm>
#include <barrier>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <random>
#include <string>
#include <thread>
#include <vector>
#include <unordered_set>
using namespace std;

#define NR_TESTS 9
#define MAX_STEPS 10000
#define MAX_TAPES 5000000
#define SOUP_SIZE 1000
#define NR_GENERATIONS 5
#define SCORE_THRESHOLD 48

enum class Distribution { UNIFORM, CUST, CUST64 };

ofstream fout("output.out");

static thread_local mt19937 mt(random_device{}());
static thread_local uniform_int_distribution<int> coinflip(0, 1), gen_byte(0, 7), gen_non_op(0, 247), gen_uniform(0, 255);

string current, res_tapes[NR_TESTS];
barrier st_barrier(NR_TESTS + 1), end_barrier(NR_TESTS + 1);
bool stop = false;

string run_program(string tape);
string generate_tape(Distribution dist);
int generate_byte(Distribution dist);
void mutate_tape(string& tape, Distribution dist, double p);
void worker(int id);
int score(const string& cand);
void run_experiment(Distribution dist, double p);
string dist_name(Distribution dist);

int main() {
    vector<thread> threads;

    for (int i = 0; i < NR_TESTS; i++) {
        threads.emplace_back(worker, i);
    }

    Distribution distributions[] = {
        Distribution::UNIFORM,
        Distribution::CUST,
        Distribution::CUST64
    };

    double probabilities[] = { 1.0 / 200, 1.0 / 100, 1.0 / 50, 1.0 };

    for (Distribution dist : distributions) {
        for (double p : probabilities) {
            run_experiment(dist, p);
        }
    }

    stop = true;
    st_barrier.arrive_and_wait();

    for (auto& t : threads) {
        t.join();
    }

    return 0;
}

string dist_name(Distribution dist) {
    switch (dist) {
        case Distribution::UNIFORM: return "UNIFORM";
        case Distribution::CUST: return "CUST";
        case Distribution::CUST64: return "CUST64";
    }
    return "UNKNOWN";
}

int generate_byte(Distribution dist) {
    if (dist == Distribution::UNIFORM) {
        return gen_uniform(mt);
    }

    if (coinflip(mt)) {
        int byte = (dist == Distribution::CUST64)
            ? gen_byte(mt)
            : uniform_int_distribution<int>(0, 6)(mt);

        return (dist == Distribution::CUST64 && byte == 7)
            ? 64 : byte;
    }

    if (dist == Distribution::CUST64) {
        int byte = gen_non_op(mt);
        return byte + ((byte >= 57) ? 8 : 7);
    }

    return uniform_int_distribution<int>(7, 255)(mt);
}

string generate_tape(Distribution dist) {
    string tape;
    tape.reserve(64);

    for (int i = 0; i < 64; i++) {
        tape += (char)generate_byte(dist);
    }

    return tape;
}

void mutate_tape(string& tape, Distribution dist, double p) {
    bernoulli_distribution mutate(p);

    for (char& byte : tape) {
        if (mutate(mt)) {
            byte = (char)generate_byte(dist);
        }
    }
}

void run_experiment(Distribution dist, double p) {
    vector<string> soup;
    soup.reserve(SOUP_SIZE);
    unordered_set<string> replicators;

    for (int i = 0; i < SOUP_SIZE; i++) {
        soup.push_back(generate_tape(dist));
    }

    long long tested = 0, first_found = -1;
    int found_count = 0;

    string probability = (p == 1.0) ? "1"
        : (p == 1.0 / 50) ? "1/50"
        : (p == 1.0 / 100) ? "1/100" : "1/200";

    fout << "Distribution: " << dist_name(dist) << '\n';
    fout << "Mutation probability: " << probability << '\n';

    auto start = chrono::steady_clock::now();

    while (tested < MAX_TAPES) {
        for (string& tape : soup) {
            if (tested >= MAX_TAPES) break;

            mutate_tape(tape, dist, p);
            current = tape;

            st_barrier.arrive_and_wait();
            end_barrier.arrive_and_wait();

            int result = score(current);
            tested++;

            if (result >= SCORE_THRESHOLD && replicators.insert(current).second) {
                if (first_found == -1) { first_found = tested; }
                found_count++;

                fout << "Self-replicating " << found_count
                     << " (program " << tested << ", score " << result << "): ";

                for (unsigned char byte : current) {
                    fout << hex << (int)byte << ' ';
                }
                fout << dec << '\n';
            }
        }
    }

    auto end = chrono::steady_clock::now();
    double seconds = chrono::duration<double>(end - start).count();

    fout << "Programs tested: " << tested << '\n';
    fout << "First self-replicator found after: ";
    if (first_found == -1) { fout << "Not found\n"; }
    else { fout << first_found << " programs\n"; }
    fout << "Total unique self-replicators: " << found_count << '\n';
    fout << "Execution time: " << fixed << setprecision(3) << seconds << " seconds\n";
    fout << "----------------------------------------\n";
    fout.flush();

    cout << dist_name(dist) << " | p=" << probability << " | tested=" << tested
         << " | first=" << first_found << " | unique=" << found_count 
         << " | time=" << fixed << setprecision(3) << seconds << "s\n";
}

int score(const string& cand) {
    int score_st = 0, score_end = 0;

    for (int i = 0; i < 64; i++) {
        int same = 0, frecv[256] = {};

        for (const string& tape : res_tapes) {
            if (tape[i] == cand[i]) same++;
            frecv[(unsigned char)tape[i + 64]]++;
        }

        if (same >= 3) score_st++;

        for (int b = 0; b < 256; b++) {
            if (frecv[b] >= 3) {
                score_end++;
                break;
            }
        }
    }

    return min(score_st, score_end);
}

void worker(int id) {
    while (true) {
        st_barrier.arrive_and_wait();
        if (stop) break;

        string cur_tape = current, res_tape;

        for (int g = 0; g < NR_GENERATIONS; g++) {
            string rand_tape = generate_tape(Distribution::UNIFORM);
            res_tape = run_program(cur_tape + rand_tape);
            cur_tape = res_tape.substr(64, 64);
        }

        res_tapes[id] = res_tape;
        end_barrier.arrive_and_wait();
    }
}

string run_program(string tape) {
    int tapeLen = tape.length(), depth = 0;
    int head0 = (unsigned char)tape[0] % tapeLen;
    int head1 = (unsigned char)tape[1] % tapeLen;
    unsigned long long steps = 0;

    for (int pc = 2; pc < tapeLen && pc >= 0 && steps < MAX_STEPS; pc++, steps++) {
        switch ((unsigned char)tape[pc]) {
            case 0: { head0 = (head0 + 1) % tapeLen; break; }
            case 1: { head0 = (head0 - 1 + tapeLen) % tapeLen; break; }
            case 2: { tape[head0] = (char)((unsigned char)tape[head0] + 1); break; }
            case 3: { tape[head0] = (char)((unsigned char)tape[head0] - 1); break; }
            case 4: {
                tape[head0] = tape[head1];
                head1 = (head1 + 1) % tapeLen;
                break;
            }
            case 5: {
                if (tape[head0] == 0) {
                    depth = 1;
                    pc++;
                    for (; pc < tapeLen && depth > 0; pc++) {
                        depth = ((unsigned char)tape[pc] == 5) ? depth + 1 : ((unsigned char)tape[pc] == 6)
                                ? depth - 1 : depth;
                    }
                    pc--;
                    if (depth != 0) pc = tapeLen;
                }
                break;
            }
            case 6: {
                if (tape[head0] != 0) {
                    depth = -1;
                    pc--;
                    for (; pc >= 0 && depth < 0; pc--) {
                        depth = ((unsigned char)tape[pc] == 6) ? depth - 1
                            : ((unsigned char)tape[pc] == 5) ? depth + 1 : depth;
                    }
                    pc++;
                    if (depth != 0) pc = -1;
                }
                break;
            }
        }
    }

    return tape;
}
