#include <bits/stdc++.h>
#include "color.h"

#define ii pair<int, int>
#define fs first
#define sc second

using namespace std;

map<ii, vector<ii>> adj;
map<ii, bool> vis;
int n, m;
int qnt_visited;
bool adj_check = 1;
vector<vector<int>> board;

void dfs(ii u) {
    //cout << "visitando " << u.fs << " " << u.sc << endl;
    vis[u] = 1;
    qnt_visited++;
    if(board[u.fs][u.sc] != adj[u].size() || adj[u].size() > 8) {
        //cout << "(" << u.fs << ", " << u.sc << ") ta errado" << endl;
        adj_check = 0;
    }

    for(ii v : adj[u]) {
        if(!vis[v]) {
            dfs(v);
        }
    }
}

int main() {
    cin >> m >> n;
    board = vector<vector<int>>(n, vector<int>(m));
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cin >> board[i][j];
        }
    }

    cin >> n >> m;
    ii source;
    for(int i = 0; i < m; i++) {
        int x1, y1, x2, y2; cin >> x1 >> y1 >> x2 >> y2;
        source = {x1, y1};
        adj[{x1, y1}].push_back({x2, y2});
        adj[{x2, y2}].push_back({x1, y1});
        vis[{x1, y1}] = 0;
        vis[{x2, y2}] = 0;
    }

    dfs(source);

    return !((n == qnt_visited) && adj_check);    
    printf("\nIlhas: %d\n", n);
    printf("Visitadas: %d\n", qnt_visited);
    printf("Checker de adjacencias: %d\n", adj_check);
}