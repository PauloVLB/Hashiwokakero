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
    coord cd;

    cell() {
        val = 0;
        init_val = 0;
        line = NO_LINE;
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

int main() {
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
    int changed = 0;
    do {
        changed = 0;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                // print_cell_info(board[4][0]);
                // std::cout << std::endl;
                cell &c = board[i][j];

                if(c.is_island()) {
                    if(c.val == c.bot_value+c.lft_value+c.rgt_value+c.top_value){
                        for(cell* to_connect : c.adj_list()) {
                            connect_cells(c, *to_connect);
                        }
                    }
                    //Checa se ilha  de grau n restaante tem n adjacencias de grau 1
                    bool connect = true;
                    int adjacent = 0;
                    for(cell* to_connect : c.adj_list()) {
                        if(to_connect->val != 1)connect = false;
                        adjacent++;
                    }
                    if(connect && c.val == adjacent){
                        for(cell* to_connect : c.adj_list()) {
                            connect_cells(c, *to_connect);
                        }
                    }
                    //---------------------------------------------------------------

                    //checa por conexões óbvias envolvendo ilhas de grau 4, 6 e 8. 
                    // Ex: se uma ilha tem grau 4 e 2 adjacentes, liga tudo.
                    for(int i = 4; i <= 8; i = i + 2) {
                        int need_adj = i/2;
                        if(c.init_val == i && c.qnt_adj() == need_adj) {
                            changed++;
                            for(cell* to_connect : c.adj_list()) {
                                connect_cells(c, *to_connect);
                            }
                            for(cell* to_connect : c.adj_list()) {
                                connect_cells(c, *to_connect);
                            }
                        }
                    }
                    //--------------------------------------------------------------
                    for(int i = 4; i <= 8; i++) {
                        int need_adj = i/2 + i%2;
                        if(c.val == i && c.qnt_adj() == need_adj) {
                            changed++;
                            for(cell* to_connect : c.adj_list()) {
                                connect_cells(c, *to_connect);
                                /*if(i%2 == 0) {
                                    connect_cells(c, *to_connect);
                                }*/
                            }
                            if(i%2 == 0){
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
                    /*if(c.val > c.qnt_adj()) {
                        changed++;
                        for(cell* to_connect : c.adj_list()) {
                            connect_cells(c, *to_connect);
                        }
                    }*/
                }

                /*cout << "(" << c.cd.x << ", " << c.cd.y << ") = " << c.init_val << " ------------------\n";
                print_board();
                cout << "------------------\n";*/
            }
        }
    } while(changed != 0);

    // for(int i = 0; i < n; i++) {
    //     for(int j = 0; j < m; j++) {
    //         print_cell_info(board[i][j]);
    //     }
    //     cout << endl;
    // }
    print_edg_list();
    cout << endl;
    print_board();
}
