'''
Indizierung der einzelnen Stellen:

                   0   1
                     2
                    
           3   4     6       9  10
             5     8   7      11
            
   12  13    15    18  19     21     24  25
     14    17  16    20     23  22     26
'''

adj = [-1] * 27
puzzle = []

def init():
    global puzzle
    
    adj[6] = 2
    adj[8] = 4
    adj[9] = 7
    adj[15] = 5
    adj[17] = 13
    adj[18] = 16
    adj[21] = 11
    adj[23] = 19
    adj[24] = 22
    
    puzzle = [0] * 9
    fin = open("puzzle.in")
    lines = fin.readlines()
    fin.close()

    for i in range(2, 11):
        puzzle[i-2] = lines[i].split(" ")
        for j in range(3):
            puzzle[i-2][j] = int(puzzle[i-2][j])


def nice_print(state):
    fout = open("puzzle.out", "w", encoding="utf-8")
    st = [""] * 27
    for i in range(27):
        st[i] = str(state[i])
        st[i] += " " * (3-len(st[i]))
    s = """Lösung gefunden:
                           {0}   {1}
                              {2}
                    
                    
                {3}   {4}      {6}       {9}  {10}
                   {5}      {8}   {7}       {11}
            
            
     {12}  {13}       {15}      {18}  {19}       {21}     {24}  {25}
       {14}       {17}  {16}      {20}        {23}  {22}     {26}
    """.format(*st)
    fout.write(s)
    fout.close()


# eigentliche Lösung der Aufgabe
def solve(cur, state, vis):
    # Gehe alle Teile des Puzzles durch
    for i in range(9):
        # Betrachte das Teil nur, wenn es zuvor noch nicht verwendet wurde (Backtracking)
        if not vis[i]:
            # Gehe alle Rotationen des Teils durch
            for k in range(3):
                possible = True
                for j in range(3):
                    # Setze die Indizes (siehe oben) auf die Werte des Teils
                    # (k + j) % 3 gibt die Position der j-ten Seite bei Rotation k an.
                    state[3*cur + j] = puzzle[i][(k + j) % 3]
                    # Gibt es einen Konflikt mit einem bereits platzierten Teil?
                    if adj[3*cur + j] != -1 and state[3*cur + j] != -state[adj[3*cur + j]]:
                        possible = False
                if possible:
                    if cur == 8:
                        # Lösung gefunden
                        return state
                    else:
                        vis[i] = True
                        # Führe die Suche rekursiv fort
                        res = solve(cur+1, state, vis)
                        # Falls dieser Pfad zu einer validen Lösung führt, muss nicht weiter gesucht werden
                        if(res):
                            return res
                        vis[i] = False
    return False


def main():
    init()
    res = solve(0, [0]*27, [False]*9)
    if res == False:
        fout = open("puzzle.out", "w", encoding="utf-8")
        fout.write("Das Puzzle besitzt keine Lösung!")
        fout.close()
    else:
        nice_print(res)
    
if __name__ == "__main__":
    main()
    