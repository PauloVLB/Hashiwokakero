#include <bits/stdc++.h>

#define ii pair<int, int>
#define fs first
#define sc second

#define NO_LINE 0 // no line
#define HORIZONTAL 1 // horizontal line
#define VERTICAL 2 // vertical line

using namespace std;


int n, m, qi; 

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

    cell() {
        val = 0;
        line = NO_LINE;
    }

    cell(int v) : val(v) {}

    bool is_island() {
        return (val >= 1 && val <= 8);
    }
};

typedef vector<vector<cell>> board_t;

board_t board;

void print_cell_info(cell c) {
    cout << "{ val: " << c.val << ", ";

    cout << "lft: ";
    if(c.lft != nullptr) cout << c.lft->val << ", ";
    else cout << "X, ";
    
    cout << "rgt: ";
    if(c.rgt != nullptr) cout << c.rgt->val << ", ";
    else cout << "X, ";
    
    cout << "top: ";
    if(c.top != nullptr) cout << c.top->val << ", ";
    else cout << "X, ";
    
    cout << "bot: ";
    if(c.bot != nullptr) cout << c.bot->val << " } ";
    else cout << "X } ";
    
}

int main() {
    cin >> n >> m >> qi;

    board = board_t(n, vector<cell>(m));
    vector<cell*> last_island_r(n, nullptr), last_island_c(m, nullptr);

    for(int i = 0; i < n; i++) {
        for(int j = 0; j < m; j++) {
            cell &c = board[i][j];
            cin >> c.val;
            
            c.lft = last_island_r[i];
            c.top = last_island_c[j];

            if(c.is_island()) {
                if(last_island_r[i] != nullptr) last_island_r[i]->rgt = &c;
                if(last_island_c[j] != nullptr) last_island_c[j]->bot = &c;
                
                last_island_r[i] = &c;
                last_island_c[j] = &c; 
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