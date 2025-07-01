#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <algorithm>
#include <unordered_set>
#include <functional>
#include <random>
#include <ctime>

// in this code I considered 'e' as epsilon so handling it would be much easier
using namespace std;

struct production
{
    char from;
    string to;
};

bool containsProduction(production p, vector<production> v)
{
    for (production pro : v)
    {
        if (pro.to == p.to && p.from == pro.from)
        {
            return true;
        }
    }
    return false;
}

class CF // context free grammar
{
public:
    char start;
    vector<char> alphabets;
    vector<char> variables;
    vector<production> productions;

    CF(char start, vector<char> alphabets,
       vector<char> variables,
       vector<production> productions) : start(start), alphabets(alphabets),
                                         variables(variables), productions(productions) {}
    CF() {}

private:
};

bool vectorContainsChar(vector<char> v, char c)
{
    for (char i : v)
    {
        if (i == c)
        {
            return true;
        }
    }
    return false;
}

bool stringOfChars(string s, vector<char> v) // checks whether all characters of a string is in desired characters or not
{
    for (char c : s)
    {
        if (!vectorContainsChar(v, c))
        {
            return false;
        }
    }

    return true;
}

vector<string> generateNullableVariants(const string &str, const vector<char> &nullable)
{
    unordered_set<char> nullSet(nullable.begin(), nullable.end());
    vector<string> results;
    string current;

    // Define a helper function (as a nested function)
    function<void(int)> dfs = [&](int i)
    {
        if (i == str.size())
        {
            if (current != "")
            {
                results.push_back(current);
            }
            return;
        }

        char ch = str[i];
        if (nullSet.count(ch))
        {
            // Option 1: Keep it
            current.push_back(ch);
            dfs(i + 1);
            current.pop_back();

            // Option 2: Drop it
            dfs(i + 1);
        }
        else
        {
            // Not nullable: must keep
            current.push_back(ch);
            dfs(i + 1);
            current.pop_back();
        }
    };

    dfs(0);
    return results;
}

CF deleteLandaProductions(CF cf)
{
    // first we have to find nullable variables with some sort of BFS
    vector<char> nullables;
    vector<char> visited;

    for (production p : cf.productions)
    {
        if (p.to == "e")
        {
            nullables.push_back(p.from);
            visited.push_back(p.from);
        }
    }

    vector<char> oldNullables;

    while (oldNullables.size() != nullables.size())
    {
        oldNullables = nullables;
        for (production p : cf.productions)
        {
            if (stringOfChars(p.to, oldNullables) && !vectorContainsChar(visited, p.from))
            {
                nullables.push_back(p.from);
                visited.push_back(p.from);
            }
        }
    }

    // now that we have found the nullable variables we can remove the landa productions
    vector<production> productions;
    for (production p : cf.productions)
    {
        if (p.to == "e" && p.from == cf.start)
        {
            productions.push_back(p);
        }
        else if (p.to == "e")
        {
            continue;
        }
        else
        {
            for (string to : generateNullableVariants(p.to, nullables))
            {
                if (!containsProduction({p.from, to}, productions))
                {
                    productions.push_back({p.from, to});
                }
            }
        }
    }
    return CF(cf.start, cf.alphabets, cf.variables, productions);
}

CF deleteUnitProductions(CF cf)
{
    vector<production> productions;
    for (production p : cf.productions)
    {
        if (p.to.size() != 1 || !vectorContainsChar(cf.variables, p.to[0]))
        {
            productions.push_back(p);
        }
    }
    for (production p1 : cf.productions)
    {
        if (vectorContainsChar(cf.variables, p1.to[0]))
        {
            for (production p2 : cf.productions)
            {
                if (p2.from == p1.to[0])
                {
                    if (!containsProduction({p1.from, p2.to}, productions) && p2.to != "e")
                    {
                        productions.push_back({p1.from, p2.to});
                    }
                }
            }
        }
    }
    return CF(cf.start, cf.alphabets, cf.variables, productions);
}

char generateVariable(vector<char> v) // This method is implemented just to generate name for new variables
// That we're going to add in deleteLeftLinearProductions method
{
    char c;
    do
    {
        srand(time(0));
        int random_number = rand() % 26 + 65;
        c = char(random_number);
    } while (vectorContainsChar(v, c));
    return c;
}

CF deleteLeftLinearProductions(CF cf)
{
    vector<production> productions;

    for (production p : cf.productions)
    {
        if (p.to[0] != p.from)
        {
            productions.push_back(p);
        }
        else
        {
        }
    }
}

CF CFGtoGG(CF cf) // takes a context free grammar and return greibach grammar
{
    vector<char> alphabets = cf.alphabets;
    vector<char> variables = cf.variables;
    char start = cf.start;
    vector<production> productions;

    for (production i : cf.productions)
    {
    }
    CF GG = CF();
    return GG;
}

int main()
{
    CF cf = CF('A', {'a', 'b'}, {'A', 'B'}, {{'A', "e"}, {'A', "abcAB"}, {'B', "A"}});
    char start;
    string temp;
    vector<char> alphabets, variables;
    vector<production> productions;
    cout << "Enter start varible: ";
    cin >> start;
    cout << endl
         << "Enter alphabets:(no space between them) ";
    cin >> temp;

    for (int i = 0; i < temp.length(); i++)
    {
        alphabets.insert(alphabets.begin(), temp[i]);
    }
    CF cf2 = deleteUnitProductions(deleteLandaProductions(cf));
    for (production p : cf2.productions)
    {
        cout << p.from << "=>" << p.to << endl;
    }
}