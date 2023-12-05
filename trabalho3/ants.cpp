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

#define MAX_IT 100
#define N_ANTS 40
#define RO 0.6
#define ALPHA 1
#define BETA 1
#define Q 5

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
        // esse bloco comentado resolve os dois niveis introdutorios restantes, porem prejudica outros
        // if(val == 1){
        //     if(this->lft != nullptr)this->lft->rgt_value = std::min(1,this->lft->rgt_value);
        //     if(this->rgt != nullptr)this->rgt->lft_value = std::min(1,this->rgt->lft_value);            
        //     if(this->top != nullptr)this->top->bot_value = std::min(1,this->top->bot_value);
        //     if(this->bot != nullptr)this->bot->top_value = std::min(1,this->bot->top_value);
        // }
        // else
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
board_t board, init_board;
map<cell, vector<cell>> adj_list;
vector<pair<cell, cell>> edg_list;
map<pair<ii, ii>, bool> cross;

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

// formiga ------------------------------------------------------------------

vector<vector<int>> x, init_x, adj_x, heuristic;
vector<vector<double>> phero[3];

vector<set<int>> adj;
vector<int> d, vis;
map<int, cell*> loc;

int check_degree() {
    int n_satur = 0;
    for(int k = 0; k < qi; k++) {
        if(adj_x[k].size() != d[k]) {
            n_satur++;
        }
    }
    return n_satur;
}

bool check_edges() {
    for(int u = 0; u < qi; u++) {
        for(int v : adj_x[u]) {
            if(!are_same_line(*loc[u], *loc[v]) && !are_same_collum(*loc[u], *loc[v])) {
                //cout << "falhou nas arestas" << endl;
                return false;
            }
        }
    }

    return true;
}

void dfs(int u) {
    vis[u] = 1;

    for(int v : adj_x[u]) {
        if(!vis[v]) {
            dfs(v);
        }
    }
}

int qnt_components() {
    vis = vector<int>(qi, 0);

    int ans = 0;
    for(int u = 0; u < qi; u++) {
        if(!vis[u]) {
            ans++;
            dfs(u);
        }
    }

    return ans;
}

bool check_connect() {
    if(qnt_components() == 1) {
        return true;
    } else {
        //cout << "falhou na conexao" << endl;
        return false;
    }
    //return qnt_components() == 1;
}

void make_adj_list(vector<vector<int>> &x) {
    adj_x = vector<vector<int>>(qi);
    for(int i = 0; i < qi; i++) {
        for(int j = i + 1; j < qi; j++) {
            for(int k = 0; k < x[i][j]; k++) { 
                adj_x[i].push_back(j);
                adj_x[j].push_back(i);
            }
        }
    }
}

int no_cross(){
    int n_crosses = 0;
    for(int i = 0; i < qi; ++i){
        for(int j = i+1; j < qi;++j){
            if(!are_same_line(*loc[i], *loc[j]) && !are_same_collum(*loc[i],*loc[j])){
                for(int adj_i : adj_x[i]){
                    for(int adj_j : adj_x[j]){
                        int a = i;
                        int b = adj_i;
                        int c = j;
                        int d = adj_j;
                        if(a > b)swap(a,b);
                        if(c > d)swap(c,d);
                        if(cross[{{a,b},{c,d}}]) n_crosses++;
                    }
                }
            }
        }
    }
    return n_crosses;
}
bool is_solution(vector<vector<int>> &x) {
    make_adj_list(x); // a partir de X, cria uma lista de adjacencia

    return check_edges() && check_degree() && check_connect() && no_cross();
}

vector<vector<int>> build_solution(int ant, vector<vector<int>> &x, 
                                   const vector<pair<int, int>> &cellsToTestConst) {
    vector<vector<int>> sol = x;

    std::random_device rd;
    std::mt19937 g(rd());

    vector<pair<int, int>> cellsToTest = cellsToTestConst;
    shuffle(cellsToTest.begin(), cellsToTest.end(), g);

    for(auto [i, j] : cellsToTest) {
        double p[3] = {0, 0, 0};
        double deno = 0;
        for(int k = x[i][j]; k < 3; k++) {
            p[k] = phero[k][i][j] * heuristic[i][j];
            deno += p[k];
        }

        for(int k = 0; k < 3; k++) {
            p[k] /= deno;
        }
        
        uniform_real_distribution<> r(0, 1);

        double random_number = r(g);
        int q_bridges = -1;
        if(random_number < p[0]) {
            q_bridges = 0;
        } else if(random_number < p[0] + p[1]) {
            q_bridges = 1;
        } else {
            q_bridges = 2;
        }

        sol[i][j] = q_bridges;
    }

    return sol;
}

int cost(vector<vector<int>> &sol) {
    make_adj_list(sol);

    int n_crosses = no_cross();
    int n_satur = check_degree();
    int n_components = qnt_components() - 1;

    return n_crosses + n_satur + n_components;
}

void update_pheromone(vector<vector<int>> &x,
                      vector<vector<int>> &sol_k,
                      int cost_k             
) {
    for(int i = 0; i < qi; i++) {
        for(int j = i + 1; j < qi; j++) {
            if(x[i][j] != sol_k[i][j]) {
                phero[sol_k[i][j]][i][j] += Q/cost_k;
            }
        }
    }
}

void init_info_values(const vector<pair<int, int>> &cellsToTest) {
    heuristic = vector<vector<int>>(qi, vector<int>(qi, 0));
    
    for(int i = 0; i < 3; i++) {
        phero[i] = vector<vector<double>>(qi, vector<double>(qi, 1));
    }

    for(auto [i, j] : cellsToTest) {
        heuristic[i][j] = min(loc[i]->val, loc[j]->val);
    }
}

bool aco(vector<vector<int>> &x, const vector<pair<int, int>>& cellsToTest) {
    init_info_values(cellsToTest); // inicializa valores heuristicos e feromonios
    
    vector<vector<int>> sol_ant[N_ANTS];
    vector<int> cost_sol(N_ANTS);
    vector<vector<int>> melhor;

    for(int it = 0; it < MAX_IT; it++) {
        int best_solution = INT_MAX;

        for(int k = 0; k < N_ANTS; k++) {
            sol_ant[k] = build_solution(k, x, cellsToTest);
            cost_sol[k] = cost(sol_ant[k]);

            //cout << "custo da solução: " << cost_sol[k] << endl;
            if(cost_sol[k] == 0) {
                x = sol_ant[k];
                return true;
            }
            if(cost_sol[k] < best_solution) {
                best_solution = cost_sol[k];
                melhor = sol_ant[k];
            }
        }

        for(auto [i, j] : cellsToTest) { // evaporação do feromonio
            for(int k = 0; k < 3; k++) {
                phero[k][i][j] = phero[k][i][j] * (1 - RO);
            }   
        }

        for(int k = 0; k < N_ANTS; k++) {
            if(cost_sol[k] == best_solution) {
                update_pheromone(x, sol_ant[k], cost_sol[k]);
            }

        }
    }

    x = melhor;
    return false;
}

// ------------------------------------------------------------------------

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
                loc[qi] = &c;
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


    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++){
            cell &c = board[i][j];
            if(c.top != nullptr)
                c.top_value = std::min(c.top->val,2);
            if(c.bot != nullptr)
                c.bot_value = std::min(c.bot->val,2);
            if(c.rgt != nullptr)
                c.rgt_value = std::min(c.rgt->val,2);
            if(c.lft != nullptr)
                c.lft_value = std::min(c.lft->val,2);
        }
    }

    adj = vector<set<int>>(qi);
    d = vector<int>(qi);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cell &c = board[i][j];

            if(c.is_island()) {
                for(cell* ad : c.adj_list()) {
                    adj[c.id].insert(ad->id);
                    adj[ad->id].insert(c.id);
                }
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

    int changed = 0;
    do {
        changed = 0;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                cell &c = board[i][j];
                if(c.is_island()) {
                    if(c.val == c.bot_value+c.lft_value+c.rgt_value+c.top_value){
                        for(cell* to_connect : c.adj_list()) {
                            changed++;
                            connect_cells(c, *to_connect);
                        }
                    }
                    for(int k = 3; k <= 8; k++) {
                        int need_adj = k/2 + k%2;
                        if(c.val == k && c.qnt_adj() == need_adj) {
                            changed++;
                            for(cell* to_connect : c.adj_list()) {
                                connect_cells(c, *to_connect);
                            }
                            if(k%2 == 0){
                                for(cell* to_connect : c.adj_list()) {
                                    connect_cells(c, *to_connect);
                                }
                            }
                        }
                    }
                    if(c.qnt_adj() == 1) {
                        changed++;
                        cell* to_connect = c.adj_list().back(); 
                        connect_cells(c, *to_connect);
                    }
                }
            }
        }
    } while(changed != 0);

    x = vector<vector<int>>(qi, vector<int>(qi, 0));
    init_x = vector<vector<int>>(qi, vector<int>(qi, 0));

    for(auto e : edg_list) {
        int u = e.fs.id;
        int v = e.sc.id;
        if(u > v) swap(u, v);
        x[u][v]++; 
        init_x[u][v] = x[u][v];
    }

    vector<ii> cellsToTest;

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cell &c = board[i][j];

            if(c.is_island() && c.qnt_adj() != 0) {
                for(cell * ad : c.adj_list()) {
                    int u = c.id;
                    int v = ad->id;

                    if(u < v) {
                        if(c.init_val != 1 || ad->init_val != 1) {
                            cellsToTest.push_back({u, v});
                        }
                    }
                }
            }
        }
    }

    aco(x, cellsToTest);
    changed = 0;
    do {
        changed = 0;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                cell &c = board[i][j];
                if(c.is_island()) {
                    if(c.val == c.bot_value+c.lft_value+c.rgt_value+c.top_value){
                        for(cell* to_connect : c.adj_list()) {
                            changed++;
                            connect_cells(c, *to_connect);
                        }
                    }
                    for(int k = 3; k <= 8; k++) {
                        int need_adj = k/2 + k%2;
                        if(c.val == k && c.qnt_adj() == need_adj) {
                            changed++;
                            for(cell* to_connect : c.adj_list()) {
                                connect_cells(c, *to_connect);
                            }
                            if(k%2 == 0){
                                for(cell* to_connect : c.adj_list()) {
                                    connect_cells(c, *to_connect);
                                }
                            }
                        }
                    }
                    if(c.qnt_adj() == 1) {
                        changed++;
                        cell* to_connect = c.adj_list().back(); 
                        connect_cells(c, *to_connect);
                    }
                }
            }
        }
    } while(changed != 0);
    
    for(int i = 0; i < qi; i++) {
        for(int j = i + 1; j < qi; j++) {
            for(int k = 0; k < x[i][j] - init_x[i][j]; k++) {
                connect_cells(*loc[i], *loc[j]);
            }
        }
    }
    //}

    // termina de marcar o tempo -----------------------------------------------
    chrono::steady_clock::time_point end = chrono::steady_clock::now();

    print_edg_list();
    cout << endl;
    
    cout << chrono::duration_cast<chrono::microseconds>(end - begin).count() << endl;
    cout << endl;
    
    print_board();
}