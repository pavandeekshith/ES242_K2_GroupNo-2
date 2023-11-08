// 2*2*2 cube solver
#include <vector>
#include <array>
#include <algorithm>
#include <queue>
#include <iostream>
#include <bits/stdc++.h>
#define ll long long
using namespace std;
// 8 positions {0,0,0}=0 ,{0,0,1}=1 ,{0,1,0}=2 ,{0,1,1}=3 ,{1,0,0}=4 ,{1,0,1}=5 ,{1,1,0}=6 ,{1,1,1}=7  =====  {z-axis,y-axis,x-axis}
// 3 orientiations possible for each position
// maximum possible vertices of the graph are 8! * 3^8
enum color { Re=0, G=1, B=2, Y=3, O=4, W=5 };
// 8 possible cubes are {B,O,Y },{B,W,O},{B,Y,R},{B,R,W},{G,O,Y},{G,W,O},{G,Y,R},{G,R,W} pls dont consider order of colors
struct cube
{
    color board[8][3]; // 8 positions (0 to 7 positions) in each you enter the 3 colors of the cube 
};
color take_input(const char &s){
    if (s=='R') return Re;
    else if (s=='G') return G;
    else if (s=='B') return B;
    else if (s=='Y') return Y;
    else if (s=='O') return O;
    else if (s=='W') return W;
}
void read_board(cube& vertex)
{
    for (int pos=0; pos<8 ; pos++){
        for (int col=0; col<3 ; col++){
            char ch;
            scanf(" %c", &ch);
            color c=take_input(ch);
            vertex.board[pos][col] = c;
        }
    }
}
// R represents a clockwise turn of the right face of the cube.
// U represents a clockwise turn of the up (top) face of the cube.
// F represents a clockwise turn of the front face of the cube.
// R', U', F' represent an anti-clockwise turn of the right, up, and front faces of the cube resp.
// R2, U2, F2 represent a 180° turn of the right, up, and front faces of the cube resp.

cube move_R(const cube &vertex)
{
    // y-axis and z-axis are interchanged
    // 7 -> 3, 3 -> 1, 1 -> 5, 5 -> 7
    cube new_vertex=vertex;
    color pos_1[3],pos_3[3],pos_5[3],pos_7[3];


    for (int color=0; color<3 ; color++){
        pos_1[color]=vertex.board[1][color];
        pos_3[color]=vertex.board[3][color];
        pos_5[color]=vertex.board[5][color];
        pos_7[color]=vertex.board[7][color];
    }

    swap(pos_1[1],pos_1[2]);
    swap(pos_3[1],pos_3[2]);
    swap(pos_5[1],pos_5[2]);
    swap(pos_7[1],pos_7[2]);

    for (int color=0; color<3 ; color++){
        new_vertex.board[1][color]=pos_3[color];
        new_vertex.board[3][color]=pos_7[color];
        new_vertex.board[5][color]=pos_1[color];
        new_vertex.board[7][color]=pos_5[color];
    }
    return new_vertex;

}
cube move_R_(const cube &vertex){
    cube new_vertex=vertex;
    new_vertex=move_R(move_R(move_R(vertex)));
    return new_vertex;
}
cube move_R2(const cube & vertex)
{
    cube new_vertex=vertex;
    new_vertex=move_R(move_R(vertex));
    return new_vertex;
}

cube move_U(const cube &vertex)
{
    // x-axis and z-axis are interchanged
    // 7 -> 6, 6 -> 2, 2 -> 3, 3 -> 7
    cube new_vertex=vertex;
    color pos_2[3],pos_3[3],pos_6[3],pos_7[3];

    for (int color=0; color<3 ; color++){
        pos_2[color]=vertex.board[2][color];
        pos_3[color]=vertex.board[3][color];
        pos_6[color]=vertex.board[6][color];
        pos_7[color]=vertex.board[7][color];
    }
    swap(pos_2[0],pos_2[2]);
    swap(pos_3[0],pos_3[2]);
    swap(pos_6[0],pos_6[2]);
    swap(pos_7[0],pos_7[2]);

    for (int color=0; color<3 ; color++){
        new_vertex.board[2][color]=pos_6[color];
        new_vertex.board[3][color]=pos_2[color];
        new_vertex.board[6][color]=pos_7[color];
        new_vertex.board[7][color]=pos_3[color];
    }
    return new_vertex;
}
cube move_U2(const cube &vertex)
{
    cube new_vertex=vertex;
    new_vertex=move_U(move_U(vertex));
    return new_vertex;
}
cube move_U_(const cube &vertex)
{
    cube new_vertex=vertex;
    new_vertex=move_U(move_U(move_U(vertex)));
    return new_vertex;
}

cube move_F(const cube &vertex)
{
    // x-axis and y-axis are interchanged
    // 4 -> 6, 7 -> 5, 6 -> 7, 5 -> 4
    cube new_vertex=vertex;
    color pos_4[3],pos_5[3],pos_6[3],pos_7[3];

    for (int color=0; color<3 ; color++){
        pos_4[color]=vertex.board[4][color];
        pos_5[color]=vertex.board[5][color];
        pos_6[color]=vertex.board[6][color];
        pos_7[color]=vertex.board[7][color];
    }
    swap(pos_4[0],pos_4[1]);
    swap(pos_5[0],pos_5[1]);
    swap(pos_6[0],pos_6[1]);
    swap(pos_7[0],pos_7[1]);
    for (int color=0; color<3 ; color++){
        new_vertex.board[4][color]=pos_5[color];
        new_vertex.board[5][color]=pos_7[color];
        new_vertex.board[6][color]=pos_4[color];
        new_vertex.board[7][color]=pos_6[color];
    }
    return new_vertex;
}
cube move_F2(const cube &vertex)
{
    cube new_vertex=vertex;
    new_vertex=move_F(move_F(vertex));
    return new_vertex;
}
cube move_F_(const cube &vertex)
{
    cube new_vertex=vertex;
    new_vertex=move_F(move_F(move_F(vertex)));
    return new_vertex;
}

bool is_cube_same(const cube& a, const cube &b)
{
    for (int pos=0; pos<8 ; pos++){
        for (int color=0; color<3 ; color++){
            if (a.board[pos][color] != b.board[pos][color]) return false;
        }
    }
    return true;
}
ll  ord (const cube& vertex)
{
    ll a = 0;
    ll base = 1;
    for (int pos = 0; pos < 8; ++pos) {
        for (int color = 0; color < 3; ++color) {
            int digit = vertex.board[pos][color];
            a += digit * base;
            base *= 3;
        }
    }
    return a;
}

constexpr int fact(int i)
{
    int p = 1;
    for (int j = 2; j <= i; ++j) {
        p *= j;
    }
    return p;
}

void print_board(const cube& vertex)
{
    for (int pos=0; pos<8 ; pos++){
        for (int color=0; color<3 ; color++){
            cout<<vertex.board[pos][color]<<" ";
        }
        cout<<endl;
    }
}

enum move { R=1, R_=2, R2=3, U=4, U_=5, U2=6, F=7, F_=8, F2=9 };
color opposite(color c)
{
    switch (c) {
    case Re: return O;
    case Y: return W;
    case G: return B;
    case B: return G;
    case O: return Re;
    case W: return Y;
    }
}
bool is_solved(cube& c)
{
    // Check first column
    color prev_color = c.board[0][0];
    for (int i = 1; i < 8; i += 1) {
        if (c.board[i][0] != opposite(prev_color)) {
            return false;
        }
        prev_color = c.board[i][0];
    }

    // Check second column
    prev_color = c.board[0][1];
    for (int i=1;i<8;i+=1){
        if (i%2!=0){
            if (c.board[i][1] != prev_color) {
                return false;
            }
            prev_color = c.board[i][1];
        }
        else{
            if (c.board[i][1] != opposite(prev_color)) {
                return false;
            }
            prev_color = c.board[i][1];
        }
    }
    // Check third column
    prev_color = c.board[0][2];
    for (int i=1;i<4;i+=1){
        if (i<=3){
            if (c.board[i][2] != prev_color) {
                return false;
            }
            prev_color = c.board[i][2];
        }
    }
    for (int i=4;i<8;i++){
        if (c.board[i][2] != opposite(prev_color)) {
            return false;
        }
    }
    return true;
}



vector<int> solve(const cube& src)
{
    queue<cube> q;
    q.push(src);
    map<ll, int> visited;
    map<ll, cube> parent;
    visited[ord(src)] = R;
    while(!q.empty()){
        cube u =q.front();
        q.pop();
        if (is_solved(u)) {
            vector<int> moves;
            cube c = u;
            int o = ord(c);
            while (!is_cube_same(c, src)) {
                moves.push_back(visited[o]);
                c = parent[o];
                o = ord(c);
            }
            reverse(moves.begin(), moves.end());
            return moves;
        }
        cube a = move_R(u);
        cube b = move_R_(u);
        cube c = move_R2(u);
        cube d = move_U(u);
        cube e = move_U_(u);
        cube f = move_U2(u);
        cube g = move_F(u);
        cube h = move_F_(u);
        cube i = move_F2(u);
        int aord = ord(a); 
        int cord = ord(c); 
        int dord = ord(d); 
        int eord = ord(e); 
        int bord = ord(b); 
        int ford = ord(f); 
        int gord = ord(g); 
        int hord = ord(h); 
        int iord = ord(i); 
        if (!visited[aord]) {
            visited[aord] = R;
            parent[aord] = u;
            q.push(a);
        }
        if (!visited[bord]) {
            visited[bord] = R_;
            parent[bord] = u;
            q.push(b);
        }
        if (!visited[cord]) {
            visited[cord] = R2;
            parent[cord] = u;
            q.push(c);
        }
        if (!visited[dord]) {
            visited[dord] = U;
            parent[dord] = u;
            q.push(d);
        }
        if (!visited[eord]) {
            visited[eord] = U_;
            parent[eord] = u;
            q.push(e);
        }
        if (!visited[ford]) {
            visited[ford] = U2;
            parent[ford] = u;
            q.push(f);
        }
        if (!visited[gord]) {
            visited[gord] = F;
            parent[gord] = u;
            q.push(g);
        }
        if (!visited[hord]) {
            visited[hord] = F_;
            parent[hord] = u;
            q.push(h);
        }
        if (!visited[iord]) {
            visited[iord] = F2;
            parent[iord] = u;
            q.push(i);
        }
        
    }
}


void print_moves(const vector<int>& moves)
{
    for (auto m: moves) {
        switch (m) {
        case R: printf("R "); break;
        case R_: printf("R' "); break;
        case R2: printf("R2 "); break;
        case U: printf("U "); break;
        case U_: printf("U' "); break;
        case U2: printf("U2 "); break;
        case F: printf("F "); break;
        case F_: printf("F' "); break;
        case F2: printf("F2 "); break;
        }
    }
    printf("\n");
}

int main()
{
    cube src;
    read_board(src);
    auto moves = solve(src);
    print_moves(moves);
    if (moves.size()==0 && !is_solved(src)) cout<<"Not valid cube config"<<endl;
    return 0;
}



// Re=0, G=1, B=2, Y=3, O=4, W=5 
// B Re W 
// G O Y  
// W O G  
// Re B Y  
// Y Re G  
// Y B O  
// Re G W  
// O W B 
// G Re Y 
// B Re Y
// G O Y
// B O Y
// G Re W
// B Re W
// G O W
// B O W

// 2 0 5
// 0 1 3
// 3 2 0
// 5 0 1
// 4 3 1
// 2 4 3
// 1 4 5
// 5 2 4

// 2 0 5
// 3 4 2
// 2 4 5
// 3 0 2
// 5 4 1
// 3 4 1
// 5 0 1
// 3 0 1

// 2 0 5
// 1 0 5
// 2 4 5
// 1 4 5
// 2 0 3
// 1 0 3
// 2 4 3
// 1 4 3







