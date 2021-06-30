#include <bits/stdc++.h>
using namespace std;

mt19937 mt(time(0));

bool wins(int a, int b)
{
    // Kugel element [1 ; a+b]
    uniform_int_distribution<int> dist(1, a + b);
    // Die ersten a Kugeln gehören Spieler 1
    return dist(mt) <= a;
}

// Fisher-Yates Shuffle
template<typename T>
void shuffle(vector<T>& v)
{
    for(int i = 0; i < v.size()-1; ++i)
    {
        int pos = uniform_int_distribution<int>(i, v.size()-1)(mt);
        swap(v[i], v[pos]);
    }
}

int n;
vector<pair<int, int>> spieler;

void read_input()
{
    ifstream fin("turnier.in");
    fin >> n;
    spieler.resize(n);
    for(int i = 0; i < n; ++i) fin >> spieler[i].first, spieler[i].second = i;
    fin.close();
}

int ko(int spiele)
{
    vector<pair<int, int>> sp = spieler;
    shuffle(sp);

    // Wenn nur noch ein Spieler übrig ist, hat dieser gewonnen
    while(sp.size() > 1)
    {
        vector<pair<int, int>> sieger;
        for(int i = 0; i < sp.size(); i += 2)
        {
            // Spiele mehrere Spiele (1 oder 5)
            int a = 0;
            for(int j = 0; j < spiele; ++j)
            {
                a += wins(sp[i].first, sp[i+1].first);
            }
            // Der Sieger kommt in die nächste Runde
            if(a > spiele / 2) sieger.push_back(sp[i]);
            else sieger.push_back(sp[i+1]);
        }
        sp = sieger;
    }

    return sp[0].second;
}

int liga(bool zufalls_nummern)
{
    vector<pair<int, int>> sp = spieler;
    if(zufalls_nummern) shuffle(sp);

    vector<pair<int, int>> siege(n);
    for(int i = 0; i < n; ++i)
    {
        siege[i].second = i;
    }

    // iteriere über alle ungeordneten Paare verschiedener Spieler
    for(int i = 0; i < n; ++i)
    {
        for(int j = i+1; j < n; ++j)
        {
            // Der Sieger erhält einen Punkt
            if(wins(sp[i].first, sp[j].first)) siege[i].first++;
            else siege[j].first++;
        }
    }

    int sieger = 0;
    for(auto& p : siege)
    {
        // Da die Spieler ihrer Spielernummer nach betrachtet werdem,
        // hat bei Gleichstand der Spieler mit gerigerer Nummer gewonnen.
        if(p.first > siege[sieger].first) sieger = p.second;
    }

    return sp[sieger].second;
}

int main(int argc, char** argv)
{
    

    read_input();

    int simulationen = 1e6;
    int param = 1;
    bool verbose = false;

    if(argc > 2)
    {
        param = stoi(argv[2]);
    }
    if(argc > 3)
    {
        simulationen = stoi(argv[3]);
    }
    if(argc > 4)
    {
        verbose = (string(argv[4]) == "-v");
    }

    
    if(argc > 1)
    {
        vector<int> siege(n);
        if(string(argv[1]) == "liga")
        {
            for(int i = 0; i < simulationen; ++i)
            {
                siege[liga(param)]++;
            }
        }
        else if(string(argv[1]) == "ko")
        {
            for(int i = 0; i < simulationen; ++i)
            {
                siege[ko(param)]++;
            }
        }
        else
        {
            printf("Ungültiges Argument: '%s'  Muss 'ko' oder 'liga' sein\n", argv[1]);
            exit(1);
        }
        

        FILE* fout = fopen("turnier.out", "w");
        if(verbose)
        {
            fprintf(fout, "Spielstärke  Siege\n");
            for(int i = 0; i < n; ++i)
            {
                fprintf(fout, "%i: \t\t %2.2f%%\n", spieler[i].first, ((double)siege[i])/simulationen * 100);
            }
        }
        else
        {
            int best = 0;
            for(int i = 1; i < n; ++i)
            {
                if(spieler[i].first > spieler[best].first) best = i;
            }
            fprintf(fout, "Siege: %2.2f%%\n", ((double)siege[best])/simulationen * 100);
        }
        fclose(fout);
    }
    else
    {
        printf("Gib den Turniermodus an. Entweder 'ko' oder 'liga'!\n");
        exit(1);
    }
}