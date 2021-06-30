#include <bits/stdc++.h>
using namespace std;

int n;
vector<array<int, 3>> wuensche;
vector<int> best_match;
int erfuellt[3] = {};

void read_input()
{
    ifstream fin("wichteln.in");
    fin >> n;
    wuensche = vector<array<int, 3>>(n);
    best_match.assign(n+1, -1);
    for(int i = 0; i < n; ++i)
    {
        fin >> wuensche[i][0] >> wuensche[i][1] >> wuensche[i][2];
    }
    fin.close();
}

void write_output()
{
    ofstream fout("wichteln.out");
    fout << "Maximale erfüllbare Wünsche: " << endl;
    for(int i = 0; i < 3; ++i)
    {
        fout << i+1 << ".\t" << erfuellt[i] << endl;
    }
    fout << endl << "Zuordnung:  (G = Geschenk, P = Person, W = Wunsch)" << endl;
    fout << "G \tP \tW \t|\tP \tG \tW" << endl;
    vector<int> geschenk_von_person(n+1);
    for(int i = 0; i < n; ++i)
    {
        geschenk_von_person[best_match[i+1]] = i+1;
    }
    for(int i = 0; i < n; ++i)
    {
        char wunsch1 = '-';
        char wunsch2 = '-';
        for(int k = 0; k < 3; ++k)
        {
            if(wunsch1 == '-' && i+1 == wuensche[best_match[i+1]][k]) wunsch1 = '1' + k;
            if(wunsch2 == '-' && best_match[wuensche[i][k]] == i) wunsch2 = '1' + k;
        }
        fout << i+1 << "\t" << best_match[i+1]+1 << "\t" << wunsch1 << "\t|\t";
        fout << i+1 << "\t" << geschenk_von_person[i] << "\t" << wunsch2 << endl;
    }
}


bool find_path(int i, int order, vector<bool>& visited)
{
    // falls der order-te Wunsch von Person i erfüllt werden kann
    if(best_match[wuensche[i][order]] == -1)
    {
        erfuellt[order]++;
        best_match[wuensche[i][order]] = i;
        return true;
    }
    for(int j = 0; j <= order; ++j)
    {
        // Person, der wir ein neues Geschenk zuordnen müssen, falls wir Person i den j-ten Wunsch erfüllen
        int next = best_match[wuensche[i][j]];
        // Prüfe, ob dieses Geschenk wirklich j-te Wunsch von Person next ist
        // Es könnte auch sein, dass dies der erste Wunsch von next ist, wir aber i
        // nur den zweiten erfüllen.
        // Die andere Richtung (der Wunsch von i hat eine höhere Priorität) kann nicht eintreten,
        // da wir das Geschenk sonst schon in der vorherigen iteration Person i zugeordnet hätten
        if(best_match[wuensche[next][j]] == next)
        {
            if(!visited[next])
            {
                visited[next] = true;
                if(find_path(next, order, visited))
                {
                    best_match[wuensche[i][j]] = i;
                    return true;
                }
            }
        }
    }
    return false;
}

void solve()
{
    // Betrachte erst Wünsche mit Priorität 1, danach 2 und 3
    for(int i = 0; i < 3; ++i)
    {
        for(int j = 0; j < n; ++j)
        {
            // Falls Person j bereits ein Geschenk zugeordnet wurde, kann sie übersprungen werden
            bool matched = false;
            for(int k = 0; k < i; ++k)
            {
                if(best_match[wuensche[j][k]] == j) matched = true;
            }
            if(!matched)
            {
                vector<bool> visited(n, false);
                visited[j] = true;
                find_path(j, i, visited);
            }
        }
    }
}

void assign_unmatched_gifts()
{
    queue<int> unmatched_gifts;
    for(int i = 1; i <= n; ++i)
    {
        if(best_match[i] == -1) unmatched_gifts.push(i);
    }
    for(int i = 0; i < n; ++i)
    {
        bool matched = false;
        for(int k = 0; k < 3; ++k)
        {
            if(best_match[wuensche[i][k]] == i) matched = true;
        }
        if(!matched)
        {
            best_match[unmatched_gifts.front()] = i;
            unmatched_gifts.pop();
        }
    }
}

int main()
{
    read_input();
    solve();
    assign_unmatched_gifts();
    write_output();
}