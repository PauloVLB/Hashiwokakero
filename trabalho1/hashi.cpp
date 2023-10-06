#include <bits/stdc++.h>

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
    int val;
    int line;
    coord cd;

    cell() {
        val = 0;
        line = NO_LINE;
    }

    cell(int v) : val(v) {}

    bool is_island() {
        return (val >= 1 && val <= 8);
    }

    int qnt_adj() {
        return (this->lft != nullptr) + (this->rgt != nullptr) 
             + (this->top != nullptr) + (this->bot != nullptr); 
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
        if(this->lft != nullptr) ans.push_back(this->lft);
        if(this->rgt != nullptr) ans.push_back(this->rgt);
        if(this->top != nullptr) ans.push_back(this->top);
        if(this->bot != nullptr) ans.push_back(this->bot);

        return ans;
    }

    string get_cell_val(string dir) {
        if(dir == "lft") {
            if(this->lft != nullptr) return to_string(this->lft->val);
        } else if(dir == "rgt") {
            if(this->rgt != nullptr) return to_string(this->rgt->val);
        } else if(dir == "top") {
            if(this->top != nullptr) return to_string(this->top->val);
        } else if(dir == "bot") {
            if(this->bot != nullptr) return to_string(this->bot->val);
        } else {
            return "INVALID DIRECTION";
        }

        return "X";
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

void connect_cells(cell c1, cell c2) {
    if(!(are_same_line(c1, c2) || are_same_collum(c1, c2))) {
        return;
    }

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

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cell &c = board[i][j];

            if(c.is_island()) {
                if(c.qnt_adj() == 1) {
                    //if(c.val > 2){/*TÁ ERRADOOOOOO*/}                    
                    cout << "entrou aqui no " << i << " " << j << endl;
                    cell* to_connect = c.adj_list().back(); 
                    connect_cells(c, *to_connect);
                }
            }
        }
    }

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            print_cell_info(board[i][j]);
        }
        cout << endl;
    }
}