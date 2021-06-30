#include <bits/stdc++.h>
using namespace std;

int sign(int x)
{
    return x >= 0 ? 1 : -1;
}

struct PosComponent 
{
    // PosComponent{a, b} = a * 1/2 + sqrt(3)/2 * b
    int a, b;
    PosComponent multiply(int x)
    {
        return {a * x, b * x};
    }
    PosComponent multSqrt3()
    {
        // sqrt(3) * (a * 1/2 + b * sqrt(3) / 2) = 3*b * 1/2 + a * sqrt(3)/2
        return {3 * b, a};
    }
    PosComponent divide2()
    {
        return {a / 2, b / 2};
    }
    PosComponent add(PosComponent x)
    {
        return {a + x.a, b + x.b};
    }
    bool operator < (const PosComponent &x) const
    {
        // 1/2 * a + sqrt(3)/2 * b < 1/2 * x.a + sqrt(3)/2 * x.b
        // a + sqrt(3) * b < x.a + sqrt(3) * x.b
        // (a - x.a) < sqrt(3) * (x.b - b)
        // sign(a - x.a) * (a - x.a)^2 < 3 * (x.b - b)^2 * sign(x.b - b)
        return sign(a - x.a) * (a - x.a) * (a - x.a) < 3 * (x.b - b) * (x.b - b) * sign(x.b - b);
    }
};

struct Vec
{
    PosComponent x, y;
    Vec multiply(int k)
    {
        return {x.multiply(k), y.multiply(k)};
    }
    Vec multSqrt3()
    {
        return {x.multSqrt3(), y.multSqrt3()};
    }
    Vec add(Vec v)
    {
        return {x.add(v.x), y.add(v.y)};
    }
    Vec move(int dir)
    {
        Vec v = *this;
        return v.add(Vec{{2, 0}, {0, 0}}.rotate(dir));
    }
    Vec divide2()
    {
        return {x.divide2(), y.divide2()};
    }
    Vec rotate(int steps)
    {
        Vec v = *this;
        Vec senkrecht = {y.multiply(-1), x};
        if(steps % 3 == 1)
        {
            v = v.multSqrt3().add(senkrecht).divide2();
            steps -= 1;
        }
        if(steps % 3 == 2)
        {
            v = senkrecht.multSqrt3().add(v).divide2();
            steps -= 2;
        }
        senkrecht = {v.y.multiply(-1), v.x};
        if(steps == 0) return v;
        if(steps == 3) return senkrecht;
        if(steps == 6) return v.multiply(-1);
        if(steps == 9) return senkrecht.multiply(-1);
        // ERROR
        assert(false);
        return *this;
    }
    Vec rotate(Vec center, int steps)
    {
        return this->add(center.multiply(-1)).rotate(steps).add(center);
    }

    bool operator < (const Vec& v) const
    {
        if(x < v.x) return true;
        if(v.x < x) return false;
        return y < v.y;
    }

    void print()
    {
        cout << x.a << " " << x.b << "   " << y.a << " " << y.b << endl;
    }
};

set<pair<Vec, int>> start;
set<pair<Vec, int>> ziel;

set<pair<Vec, int>> rem;
set<pair<Vec, int>> add;

void read_input()
{
    ifstream fin("streichholz.in");
    int n;
    fin >> n;
    for(int i = 0; i < n; ++i)
    {
        Vec v; int d;
        fin >> v.x.a >> v.x.b >> v.y.a >> v.y.b >> d;
        // cout << v.x.a << " " << v.x.b << " " << v.y.a << " " << v.y.b << " " << d << endl;
        // cout << start.count({v, d}) << endl;
        start.insert({v, d});
    }
    for(int i = 0; i < n; ++i)
    {
        Vec v; int d;
        fin >> v.x.a >> v.x.b >> v.y.a >> v.y.b >> d;
        ziel.insert({v, d});
    }
    fin.close();
}

void write_output()
{
    ofstream fout("streichholz.out");
    fout << rem.size() << endl;
    for(auto a : rem)
    {
        fout << a.first.x.a << " " << a.first.x.b << " " << a.first.y.a << " "
            << a.first.y.b << " " << a.second << endl;
    }
    fout << endl;
    for(auto a : add)
    {
        fout << a.first.x.a << " " << a.first.x.b << " " << a.first.y.a << " "
            << a.first.y.b << " " << a.second << endl;
    }
}

void solve()
{
    int best = 0;
    // Itereiere über alle Paare (u, v)
    for(auto u : start)
    {
        for(auto v : ziel)
        {
            int match = 0;
            set<pair<Vec, int>> start_match;
            set<pair<Vec, int>> no_match;
            
            // lambda Funktion, die überprüft, ob ein Streichholz enthalten ist
            auto check = [&](pair<Vec, int> holz)
            {
                // Jedes Streichholz kann auf 2 Arten angegeben werden, indem man das
                // andere Ende als Vektor angibt und die Richtung invertiert
                pair<Vec, int> holz2 = {holz.first.move(holz.second), (holz.second + 6) % 12};
                // prüfe daher beide Orientierungen
                if(start.count(holz))
                {
                    match++;
                    start_match.insert(holz);
                }
                else if(start.count(holz2))
                {
                    match++;
                    start_match.insert(holz2);
                }
                else
                {
                    no_match.insert(holz);
                }
            };

            auto update_best = [&]()
            {
                if(match > best)
                {
                    best = match;
                    add = no_match;
                    rem.clear();
                    for(auto a : start)
                    {
                        if(!start_match.count(a)) rem.insert(a);
                    }
                }
            };

            // u - v    (so gewählt, dass v + translate = u)
            Vec translate = u.first.add(v.first.multiply(-1));
            // Winkel zwischen v und u (gegen den Uhrzeigersinn) in 30° Schritten
            int steps = (u.second - v.second + 12) % 12;
            // Wende die Transformation auf alle z element ziel an
            for(auto z : ziel)
            {
                // Verschiebe z um translate
                // Rotiere z um u um den Winkel alpha = steps * 30° 
                Vec v = z.first.add(translate).rotate(u.first, steps);
                // Die Richtung von z'
                int dir = (z.second + steps) % 12;
                check({v, dir});
            }
            update_best();

            match = 0;
            start_match.clear();
            no_match.clear();

            // Wenn u und v übereinstimmen, können alle Streichhölzer
            // der Menge Z' gespiegelt werden, wonach u und v immernoch
            // übereinstimmen. Diese Variante muss also auch betrachtet werden:
            Vec trans2 = u.first.move(u.second).add(v.first.multiply(-1));
            int steps2 = (steps + 6) % 12;
            for(auto z : ziel)
            {
                Vec v = z.first.add(trans2).rotate(u.first, steps2);
                int dir = (z.second + steps2) % 12;
                check({v, dir});
            }
            update_best();
        }
    }
}

int main()
{
    read_input();
    solve();
    write_output();
}