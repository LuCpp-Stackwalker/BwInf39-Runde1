luecken = []
luecken_suffix = []
woerter = []
adj = []

satz_zeichen = [",", ".", "!", ":", ";"]

def read_input():
    global luecken, woerter, luecken_suffix
    fin = open("words.in", "r", encoding="utf-8")
    line = fin.readline()
    # Zeilenumbruch zählt zur Länge des strings und muss entfernt werden!
    line = line.replace("\n", "")
    luecken = line.split(" ")
    luecken_suffix = [""] * len(luecken)
    # entferne Satzzeichen
    for i in range(len(luecken)):
        if luecken[i][-1] in satz_zeichen:
            luecken_suffix[i] = luecken[i][-1]
            luecken[i] = luecken[i][0:-1]
    line = fin.readline()
    line = line.replace("\n", "")
    woerter = line.split(" ")
    fin.close()

# Konstruieren des Graphen
def init_adj():
    global adj
    for word in woerter:
        x = []
        for j in range(len(luecken)):
            luecke = luecken[j]
            if len(word) == len(luecke):
                valid = True
                for c in range(len(word)):
                    if luecke[c] != '_' and luecke[c] != word[c]:
                        valid = False
                if valid:
                    x.append(j)
        adj.append(x)

def find_match(i, visited, match):
    for j in adj[i]:
        if not visited[j]:
            visited[j] = True
            if match[j] == -1 or find_match(match[j], visited, match):
                match[j] = i
                return True
    return False

def find_bipartite_matching():
    n = len(adj)
    match = [-1] * n
    for i in range(n):
        visited = [False] * n
        find_match(i, visited, match)
    
    return match

def write_output(match):
    fout = open("words.out", "w", encoding="utf-8")
    for i in range(len(match)):
        fout.write(woerter[match[i]] + luecken_suffix[i] + " ")
    fout.write("\n")

def main():
    read_input()
    init_adj()
    match = find_bipartite_matching()
    write_output(match)

if __name__ == "__main__":
    main()
