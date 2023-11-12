#include <bits/stdc++.h>
#include <cstdio>

#define ii pair<int, int>
#define fs first
#define sc second

#define NO_LINE 0 // no line
#define HORIZONTAL 1 // horizontal line
#define D_HORIZONTAL 2  // double horizontal line
#define VERTICAL 3 // vertical line
#define D_VERTICAL 4 // double vertical line

#define salve cout << "salve" << endl;

using namespace std;

struct coord {
    int x, y;
    coord() {
        x = -1;
        y = -1;
    }
    coord(int m_x, int m_y): x(m_x), y(m_y) {}
};

struct cell {
    cell *lft, *rgt, *top, *bot;
    int init_val, val;
    int lft_value = 0;
    int rgt_value = 0;
    int top_value = 0;
    int bot_value = 0;
    int line;
    int id;
    coord cd;

    cell() {
        val = 0;
        init_val = 0;
        line = NO_LINE;
        id = -1;
    }

    cell(int v) : val(v) {}

    bool is_island() {
        return is_open_island() || is_closed_island();
    }
    bool is_open_island() {
        return (val >= 1 && val <= 8);
    }

    bool is_closed_island() {
        return val == -1;
    }
 
    int qnt_adj() {
        return (is_adjacent(this->lft)) + (is_adjacent(this->rgt)) 
             + (is_adjacent(this->top)) + (is_adjacent(this->bot)); 
    }

    void decrease_val() {
        val--;
        if(val == 0) {
            if(this->lft != nullptr)this->lft->rgt_value = 0;
            this->lft = nullptr;
            if(this->rgt != nullptr)this->rgt->lft_value = 0;
            this->rgt = nullptr;
            if(this->top != nullptr)this->top->bot_value = 0;
            this->top = nullptr;
            if(this->bot != nullptr)this->bot->top_value = 0;
            this->bot = nullptr;
            val = -1;
        }
    }

    bool add_line(int dir) {
        if(line == NO_LINE) {
            line = dir;
        } else {
            line++;
        }
        if(dir == HORIZONTAL) {
            if(this->top != nullptr && this->bot != nullptr) {
                this->top->bot = nullptr;
                this->bot->top = nullptr; 
            }
        } else {
            if(this->rgt != nullptr && this->lft != nullptr) {
                this->rgt->lft = nullptr;
                this->lft->rgt = nullptr; 
            }
        }
        return (line == D_HORIZONTAL || line == D_VERTICAL);
    }

    vector<cell*> adj_list() {
        vector<cell*> ans;
        if(is_adjacent(this->lft)) ans.push_back(this->lft);
        if(is_adjacent(this->rgt)) ans.push_back(this->rgt);
        if(is_adjacent(this->top)) ans.push_back(this->top);
        if(is_adjacent(this->bot)) ans.push_back(this->bot);

        return ans;
    }

    bool check_adj(cell* c){
        return this->lft == c || this->rgt == c || this->top == c || this->bot == c;
    }
    string get_cell_val(string dir) {
        if(dir == "lft") {
            if(is_adjacent(this->lft)) return to_string(this->lft->val);
        } else if(dir == "rgt") {
            if(is_adjacent(this->rgt)) return to_string(this->rgt->val);
        } else if(dir == "top") {
            if(is_adjacent(this->top)) return to_string(this->top->val);
        } else if(dir == "bot") {
            if(is_adjacent(this->bot)) return to_string(this->bot->val);
        } else {
            return "INVALID DIRECTION";
        }

        return "X";
    }

    friend bool is_adjacent(cell* c) {
        return c != nullptr && c->is_open_island();
    }
};

typedef vector<vector<cell>> board_t;

int n, m, qi; 
board_t board;
map<cell, vector<cell>> adj_list;
vector<pair<cell, cell>> edg_list;

bool are_same_line(cell c1, cell c2) {
    return c1.cd.x == c2.cd.x;
}

bool are_same_collum(cell c1, cell c2) {
    return c1.cd.y == c2.cd.y;
}

void print_cell_info(cell c) {
    cout << "{ val: " << c.val << ", ";

    cout << "lft: ";
    cout << c.lft_value << ", ";
    
    cout << "rgt: ";
    cout << c.rgt_value << ", ";
    
    cout << "top: ";
    cout << c.top_value << ", ";
    
    cout << "bot: ";
    cout << c.bot_value;
    cout << " } ";
}

void print_cell(cell c) {
    if(c.is_island()) {
        cout << board[c.cd.x][c.cd.y].init_val << " ";
    } else {
        vector<string> switch_line(6);
        switch_line[NO_LINE] = " ";
        switch_line[HORIZONTAL] = "-";
        switch_line[D_HORIZONTAL] = "=";
        switch_line[VERTICAL] = "|";
        switch_line[D_VERTICAL] = "ǁ"; 

        if(c.line > D_VERTICAL) cout << "? ";
        else cout << switch_line[c.line] << " ";
    }
}

void connect_cells(cell &c1, cell &c2) {
    if(!((are_same_line(c1, c2) || are_same_collum(c1, c2)) && c1.check_adj(&c2) && c2.check_adj(&c1))) {
        return;
    }

    //adj_list[c1].push_back(c2);
    //adj_list[c2].push_back(c1);
    edg_list.push_back({c1, c2});

    c1.decrease_val();
    c2.decrease_val();
    bool full = false;
    if(are_same_line(c1, c2)) {
        int min_y = min(c1.cd.y, c2.cd.y);
        int max_y = max(c1.cd.y, c2.cd.y);
        int my_x = c1.cd.x;
        for(int j = min_y + 1; j <= max_y - 1; j++) {
            full = board[my_x][j].add_line(HORIZONTAL);
        }
        if(board[my_x][min_y].rgt_value > 0)board[my_x][min_y].rgt_value--;
        if(board[my_x][max_y].lft_value > 0)board[my_x][max_y].lft_value--;
        if(full){
            if(c1.cd.y < c2.cd.y){
                c1.rgt = nullptr;
                c2.lft = nullptr;
            }else{
                c2.rgt = nullptr;
                c1.lft = nullptr;
            }
        }
    } else {
        int min_x = min(c1.cd.x, c2.cd.x);
        int max_x = max(c1.cd.x, c2.cd.x);
        int my_y = c1.cd.y;
        for(int i = min_x + 1; i <= max_x - 1; i++) {
            full = board[i][my_y].add_line(VERTICAL);
        }
        if(board[min_x][my_y].bot_value > 0)board[min_x][my_y].bot_value--;
        if(board[max_x][my_y].top_value > 0)board[max_x][my_y].top_value--;
        if(full){
            if(c1.cd.x < c2.cd.x){
                c1.bot = nullptr;
                c2.top = nullptr;
            }else{
                c2.bot = nullptr;
                c1.top = nullptr;
            }
        }
    }
}

void print_board() {
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            print_cell(board[i][j]);
        }
        cout << endl;
    }
}

void print_edg_list() {
    cout << qi << " " << (int) edg_list.size() << endl;
    for(auto e : edg_list) {
        printf("%d %d %d %d\n", e.first.cd.x, e.first.cd.y,
                              e.second.cd.x, e.second.cd.y);
    }
}

// exato ------------------------------------------------------------------

vector<vector<int>> adj;
vector<int> d;
map<pair<ii, ii>, bool> cross;

bool check_degree(vector<vector<int>> &x) {
    for(int k = 0; k < qi; k++) {
        int qnt = 0;
        for(int v : adj[k]) {
            if(k > v) swap(v, k);
            qnt += x[k][v];
        }
        if(qnt != d[k]) {
            return false;
        }
    }

    return true;
}

bool check_cross(vector<vector<int>> &x) {
    for(auto [key, val] : cross) {
        int i = key.fs.fs;
        int j = key.fs.sc;

        int k = key.sc.fs;
        int l = key.sc.sc;

        int yij = x[i][j] != 0;
        int ykl = x[k][l] != 0;

        if(yij + ykl > 1) {
            return false;
        }
    }

    return true;
}

void dfs(vector<vector<int>> &x, vector<int> &vis, int u) {
    vis[u] = 1;
    for(int v : adj[u]) {
        if(x[u][v] >= 1 && !vis[v]) {
            dfs(x, vis, v);
        }
    }
    
}

int qnt_components(vector<vector<int>> &x) {
    vector<int> vis(qi, 0);

    int ans = 0;
    for(int i = 0; i < qi; i++) {
        if(!vis[i]) {
            dfs(x, vis, i);
            ans++;
        }
    }

    return ans;
}

bool check_connect(vector<vector<int>> &x) {
    return qnt_components(x) == 1;
}

bool is_solution(vector<vector<int>> &x) {
    return check_degree(x) && check_cross(x) && check_connect(x);
}

void backtracking(vector<vector<int>>& x, int n, int row, int col) {
    if (row == n) {
        // All cells have been visited, print or process the matrix as needed
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                cout << x[i][j] << ' ';
            }
            cout << endl;
        }
        cout << "----------------" << endl;
        return;
    }

    if (col == n) {
        // Move to the next row
        backtracking(x, n, row + 1, 0);
        return;
    }

    // Check if the current cell is already set
    if (x[row][col] != -1) {
        // If already set, move to the next column
        backtracking(x, n, row, col + 1);
    } else {
        // Try each possible value for the current cell (0, 1, and 2)
        for (int value = 0; value <= 2; value++) {
            x[row][col] = value;
            backtracking(x, n, row, col + 1);
            // Backtrack: reset the value for the next iteration
            x[row][col] = -1;
        }
    }
}

// exato ------------------------------------------------------------------

int main() {
    chrono::steady_clock::time_point begin = chrono::steady_clock::now(); 
    // começa a marcar o tempo --------------------------------------------------
    
    cin >> m >> n >> qi; qi = 0;
    cout << m << " " << n << endl;

    board = board_t(n, vector<cell>(m));

    vector<cell*> last_island_r(n, nullptr), last_island_c(m, nullptr);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cell &c = board[i][j];
            cin >> c.val;
            cout << c.val << " ";

            c.init_val = c.val;

            c.cd.x = i;
            c.cd.y = j;
            
            c.lft = last_island_r[i];
            c.top = last_island_c[j];

            if(c.is_island()) {
                c.id = qi;
                qi++;
                last_island_c[j] = &c; 
                last_island_r[i] = &c;  
            }
        }
        cout << endl;
    }

    last_island_r = vector<cell*>(n, nullptr);
    last_island_c = vector<cell*>(m, nullptr);

    for(int i = n-1; i >= 0; i--) {
        for(int j = m-1; j >= 0; j--) {
            cell &c = board[i][j];
            
            c.rgt = last_island_r[i];
            c.bot = last_island_c[j];

            if(c.is_island()) {
                last_island_c[j] = &c; 
                last_island_r[i] = &c;  
            }
        }
    }

    adj = vector<vector<int>>(qi);
    d = vector<int>(qi);
    
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cell &c = board[i][j];

            if(c.is_island()) {
                cout << "vizinhos de " << c.id << ": ";
                for(cell* ad : c.adj_list()) {
                    cout << ad->id << " ";
                    adj[c.id].push_back(ad->id);
                    adj[ad->id].push_back(c.id);
                }
                cout << endl;
                d[c.id] = c.init_val;
            } else {
                if(c.qnt_adj() == 4) {
                    int ci = c.top->id;
                    int cj = c.bot->id;
                    int ck = c.lft->id;
                    int cl = c.rgt->id;
                    if(ci > cj) swap(ci, cj);
                    if(ck > cl) swap(ck, cl);
                    cross[{{ci, cj}, {ck, cl}}] = 1;
                    cross[{{ck, cl}, {ci, cj}}] = 1;
                }
            }
        }
    }

    vector<vector<int>> x(qi, vector<int>(qi, 0));
    //backtracking(x, qi, 0, 0);

    salve

   
    // termina de marcar o tempo -----------------------------------------------
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    /*print_edg_list();
    cout << endl;
   
    cout << chrono::duration_cast<chrono::microseconds>(end - begin).count() << endl;
    cout << endl;

    print_board();*/
}
