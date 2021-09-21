#include <iostream>
#include <stdio.h>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <utility>
#include <map>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include <string>
#include <memory.h>

// define
#define maxn 900+208
#define maxm 31

using namespace std;

struct node{
	int y,x; // coordinates
	int color; // color
	set<int> adjacent; // a set of adjacent nodes
	node():y(0),x(0),color(0){};
	node(int y, int x):y(y),x(x),color(0){}; //constructor
};



int matrix[maxm][maxm]{}; // original board
map<int,node> graph; // graph
vector<int> nodes;  // record
int mincol = 1; // minimum number of needed colors
int rowcnt=1, colcnt=1; // the height and width of the board
string input; // used to input a line at a time


bool cmp(int a, int b){ // to sort to papa standards
	return (graph[a].y < graph[b].y && graph[a].x < graph[b].x);
}

void readmat(){ // read original board
    while (getline(cin, input)){ // input a line at a time
        colcnt = 1;
        for (int i = 0; i<input.size(); i+=2){ // read two digits 
            if (isspace(input[i+1])){ // if singal digit
                matrix[rowcnt][colcnt] = input[i]-'0';
            }else{ // if double digit
                matrix[rowcnt][colcnt] = (input[i]-'0')*10+(input[i+1]-'0');
            }
            colcnt ++;
        }
        rowcnt ++;
    }
}


void connect(int y, int x){
    int cur =   matrix[y][x];
    int up = matrix[y-1][x]; 
    int left = matrix[y][x-1]; // connect current node with left and up
    if (left != 0 && left != cur){
        graph[cur].adjacent.insert(left);
        graph[left].adjacent.insert(cur);
    }
    if (up != 0 && up != cur){
        graph[cur].adjacent.insert(up);
        graph[up].adjacent.insert(cur);
    }
}

void build(){ // build the graph 
    for (int i = 1; i<rowcnt; ++i){
        for (int j = 1; j<colcnt; ++j){
            if (matrix[i][j] != 0){
                // if current node not in graph -> add to graph
                if (find(nodes.begin(),nodes.end(),matrix[i][j]) == nodes.end()){
                    graph[matrix[i][j]] = {i,j}; 
                    nodes.push_back(matrix[i][j]);
                }
                connect(i,j);
            }
        }
    }
}

bool check(int node, int col){  // if the coloring is good (for now)
    for (set<int>::iterator itr = graph[node].adjacent.begin(); itr != graph[node].adjacent.end(); itr++){
        if (graph[*itr].color == col){
            return false;
        }
    }
    return true; // true = good to go
}

bool dfs(int index){
    for (int i = 1; i<=mincol; ++i){
        bool error = 0;
        if (check(nodes[index],i) == 1){
        	graph[nodes[index]].color = i;
        	if (index == nodes.size()-1){
        		return 1;
        	}else{
	        	if (dfs(index+1) == 1){
	        		return 1;
	        	}else{
	        		error = 1;
	        	}
	        }
        }else{
        	error = 1;
        }
        if (i == mincol && error == 1){
        	for (int i = index; i<nodes.size();++i){
        		if (graph[nodes[i]].color == 0){
        			break;
        		}else{
        			graph[nodes[i]].color = 0;
        		}
        	}
        	return 0;
        }
    }
}

void printout(){ // print out the colors
    for (int i = 1; i<rowcnt; ++i){
        for (int j = 1; j<colcnt; ++j){
            if (matrix[i][j] == 0){
                cout << '`' << " ";
            }else{
                cout << char('a' -1 + graph[matrix[i][j]].color) << " ";
            }
        }
        cout << endl;
    }
}


int main(){
    freopen("/Users/hubert/Dropbox/Coding/schoolStuff/papaBeard/四色問題/color900.txt","r",stdin);
    freopen("/Users/hubert/Dropbox/Coding/schoolStuff/papaBeard/四色問題/color_out900.txt","w",stdout);
    // input 
    readmat();

    // build graph
    build();

    //sort to papabeard standards
    sort(nodes.begin(), nodes.end(), cmp);

    //dfs
    for (int i = 1; i<=5; ++i){ // we have to check up to 5 colors, because enclaves are allowed
    	mincol = i;
    	dfs(0);
    	bool notdone = 0;
    	for (int i = 0; i<nodes.size(); ++i){
            // check if there are any nodes uncolored 
    		if (graph[nodes[i]].color == 0){
    			notdone = 1;
    			break;
    		}
    	}
    	if (notdone == 1){
    		for (int i = 0; i<nodes.size(); ++i){
    			graph[nodes[i]].color = 0;
    		}
    	}else{
            // we can color all nodes with current mincol
    		break;
    	}
    }

    printout();

}


