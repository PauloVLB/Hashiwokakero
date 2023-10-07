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
            this->lft = nullptr;
            this->rgt = nullptr;
            this->top = nullptr;
            this->bot = nullptr;
            val = -1;
        }
    }

    void add_line(int dir) {
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
    }

    vector<cell*> adj_list() {
        vector<cell*> ans;
        if(is_adjacent(this->lft)) ans.push_back(this->lft);
        if(is_adjacent(this->rgt)) ans.push_back(this->rgt);
        if(is_adjacent(this->top)) ans.push_back(this->top);
        if(is_adjacent(this->bot)) ans.push_back(this->bot);

        return ans;
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


bool are_same_line(cell c1, cell c2) {
    return c1.cd.x == c2.cd.x;
}

bool are_same_collum(cell c1, cell c2) {
    return c1.cd.y == c2.cd.y;
}

void print_cell_info(cell c) {
    cout << "{ val: " << c.val << ", ";

    cout << "lft: ";
    cout << c.get_cell_val("lft") << ", ";
    
    cout << "rgt: ";
    cout << c.get_cell_val("rgt") << ", ";
    
    cout << "top: ";
    cout << c.get_cell_val("top") << ", ";
    
    cout << "bot: ";
    cout << c.get_cell_val("bot");
    cout << " } ";
}

void print_cell(cell c) {
    if(c.is_island()) {
        cout << board[c.cd.x][c.cd.y].init_val << " ";
    } else {
        switch (c.line) {
            case NO_LINE:
                cout << " ";
                break;
            case HORIZONTAL:
                cout << "-";
                break;
            case D_HORIZONTAL:
                cout << "=";
                break;
            case VERTICAL:
                cout << "|";
                break;
            case D_VERTICAL:
                cout << "^";
                break;
            default:
                cout << "?";
        }
        cout << " ";
    }
}

void connect_cells(cell &c1, cell &c2) {
    if(!(are_same_line(c1, c2) || are_same_collum(c1, c2))) {
        return;
    }

    c1.decrease_val();
    c2.decrease_val();

    if(are_same_line(c1, c2)) {
        int min_y = min(c1.cd.y, c2.cd.y);
        int max_y = max(c1.cd.y, c2.cd.y);
        int my_x = c1.cd.x;
        for(int j = min_y + 1; j <= max_y - 1; j++) {
            board[my_x][j].add_line(HORIZONTAL);
        }
    } else {
        int min_x = min(c1.cd.x, c2.cd.x);
        int max_x = max(c1.cd.x, c2.cd.x);
        int my_y = c1.cd.y;
        for(int i = min_x + 1; i <= max_x - 1; i++) {
            board[i][my_y].add_line(VERTICAL);
        }
    }
}

int main() {
    cin >> n >> m >> qi;

    board = board_t(n, vector<cell>(m));

    vector<cell*> last_island_r(n, nullptr), last_island_c(m, nullptr);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cell &c = board[i][j];
            cin >> c.val;
            c.init_val = c.val;

            c.cd.x = i;
            c.cd.y = j;
            
            c.lft = last_island_r[i];
            c.top = last_island_c[j];

            if(c.is_island()) {
                last_island_c[j] = &c; 
                last_island_r[i] = &c;  
            }
        }
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

    int changed = 0;
    do {
        changed = 0;
        for(int i = 0; i < n; i++) {
            for(int j = 0; j < m; j++) {
                cell &c = board[i][j];

                if(c.is_island()) {
                    if(c.qnt_adj() == 1) {
                        changed++;
                        //if(c.val > 2){/*TÁ ERRADOOOOOO*/}                    
                        //cout << "entrou aqui no " << i << " " << j << endl;
                        cell* to_connect = c.adj_list().back(); 
                        connect_cells(c, *to_connect);
                    }
                    if(c.val == 4 && c.qnt_adj() == 2) {
                        changed++;
                        for(cell* to_connect : c.adj_list()) {
                            connect_cells(c, *to_connect);
                            connect_cells(c, *to_connect);
                        }
                    }
                    if(c.val == 8 && c.qnt_adj() == 4) {
                        changed++;
                        for(cell* to_connect : c.adj_list()) {
                            connect_cells(c, *to_connect);
                            connect_cells(c, *to_connect);
                        }
                    }
                    
                }
            }
        }
    } while(changed != 0);

    /*for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            print_cell_info(board[i][j]);
        }
        cout << endl;
    }*/
    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            print_cell(board[i][j]);
        }
        cout << endl;
    }
}