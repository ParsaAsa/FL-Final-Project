#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <limits>

using namespace std;

struct Transition
{
    char write;  // symbol to write
    int move;    // −1 L , 0 S , +1 R
    string next; // next state
};

class TM // Turing machine
{
    map<string, map<char, Transition>> delta;
    string start, accept, reject;

public:
    TM(const string &q0, const string &qacc, const string &qrej)
        : start(q0), accept(qacc), reject(qrej) {}

    void add(const string &q, char read, char write, int move, const string &nxt)
    {
        delta[q][read] = Transition{write, move, nxt};
    }

    string run(vector<char> tape, int head = 0)
    {
        const char B = '_';
        tape.insert(tape.begin(), B);
        tape.push_back(B);
        head++;
        string st = start;

        while (st != accept && st != reject)
        {
            char sym = (head < 0 || head >= (int)tape.size()) ? B : tape[head];
            if (!delta.count(st) || !delta[st].count(sym))
            {
                st = reject;
                break;
            }
            auto t = delta[st][sym];
            tape[head] = t.write;
            head += t.move;
            if (head < 0)
            {
                tape.insert(tape.begin(), B);
                head = 0;
            }
            if (head >= (int)tape.size())
                tape.push_back(B);
            st = t.next;
        }
        cout << "Final state: " << st << "\n";
        cout << "Raw tape   : " << string(tape.begin(), tape.end()) << "\n";
        return string(tape.begin(), tape.end());
    }
};

unsigned long long factorial(unsigned n)
{
    unsigned long long r = 1;
    for (unsigned i = 2; i <= n; ++i)
    {
        if (r > numeric_limits<unsigned long long>::max() / i)
        {
            cerr << "[warning] factorial overflow for " << n << "!; "
                                                                "capping result.\n";
            return numeric_limits<unsigned long long>::max();
        }
        r *= i;
    }
    return r;
}

void addWriteOnes(TM &M, const string &base, unsigned long long count)
{
    if (count == 0)
    {
        M.add(base, '_', '1', 0, "qacc");
        return;
    }
    for (unsigned long long i = 0; i < count; ++i)
    {
        string s = (i == 0) ? base : base + "_" + to_string(i);
        string s2 = (i == count - 1) ? "qacc" : base + "_" + to_string(i + 1);
        int mv = (i == count - 1) ? 0 : 1;
        M.add(s, '_', '1', mv, s2);
    }
}

/* ------------------------ build machine for a given x ---------- */
void loadTransitions(TM &M, unsigned x, unsigned long long fact)
{
    const char X = 'x', B = '_';

    // PARITY TEST
    M.add("q0", '1', X, 1, "q1");
    M.add("q0", B, B, 0, "q3"); // even path (no 1 at start → x=0)

    M.add("q1", '1', X, 1, "q0"); // consume pair
    M.add("q1", B, B, -1, "q2");  // odd (left‑over 1)

    /* === ODD branch === */
    M.add("q2", X, '1', -1, "odd0");
    M.add("odd0", X, B, -1, "odd1");
    M.add("odd1", X, '1', -1, "odd0");
    M.add("odd0", B, B, 1, "odd_done");
    M.add("odd_done", B, B, 0, "qacc");

    // EVEN branch
    // erase all Xs, move left until blank, then jump to writer
    M.add("q3", X, B, -1, "q3");        // keep erasing/moving left
    M.add("q3", B, B, 0, "write_fact"); // left blank reached

    /* writer chain */
    addWriteOnes(M, "write_fact", fact);
}

int main()
{
    unsigned x;
    cout << "Enter a non-negative integer x (factorial written for even x): ";
    if (!(cin >> x))
    {
        cerr << "Bad input.\n";
        return 1;
    }

    unsigned long long fact = (x % 2 == 0) ? factorial(x) : 0ULL;

    TM M("q0", "qacc", "qrej");
    loadTransitions(M, x, fact);

    /* build unary input _111…1_ */
    vector<char> tape = {'_'};
    for (unsigned i = 0; i < x; ++i)
        tape.push_back('1');
    tape.push_back('_');

    string final = M.run(tape, 1);

    unsigned long long ones = 0;
    for (char c : final)
    {
        if (c == '1')
        {
            ++ones;
        }
    }

    cout << "Answer in decimal: " << ones << "\n";
    return 0;
}
