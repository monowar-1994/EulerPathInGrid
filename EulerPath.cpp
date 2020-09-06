My Drive
/**
 * @file EulerPath.cpp
 * @author Monowar Anjum (anjumm1@myumanitoba.ca)
 * @brief This cpp file contains the codebase for solving the euler path problem as a take home test 
 * by 1Qbit.
 * @version 0.1
 * @date 2020-08-20
 * 
 * @copyright Copyright (c) 2020
 * 
 */


#include <iostream>
#include <map>
#include <utility>
#include <vector>
#include <ctime>
#include <cstdlib>
#include <cassert>
#include <stack>
#include <algorithm>

#define ROW_NUM 5
#define COL_NUM 10
#define ii pair< int, int >
#define vi vector< int >
#define mp make_pair

using namespace std;

int dir[4][2]; // This array holds the direction unit vectors
map<ii, int> index_of_nodes; // This contains the co-ordinate to node number mapping
map<int, ii> nodes_of_index; // This contains the node_number to co-ordinate mapping
vector< vi > graph(ROW_NUM*COL_NUM); // This contains the whole graph of the neighborhood in the form of an adjacency list

/*
The solution strategy is simple. I cut the graph in three cuts.
Then I solved for each cuts to have an euler circuit/path with minimum extra edge add.
Therefore, when the three euler circuits/path are put together end to end the form an 
euler tour on the whole graph while having to re-travel minimum number of edges.

compile command: 
1. navigate to the directory of this file
2. run this command with g++ 4.8 or higher: "g++ EulerPath.cpp -o euler_path.exe" ( I did this in windows)
3. run this command: euler_path.exe 

It should print the output in the console. 
You can uncomment the lines in main function to see the subgraphs of the original graph.
If you do have any question please let me know. I will be more than happy to help with queries.
*/

bool isValid(int x, int y, int m, int n) {
	/**
	 * @brief returns whether a point is valid grid co ordinate or not
	 *
	 */

	if (x < 0 || x >= m) {
		return false;
	}
	if (y < 0 || y >= n) {
		return false;
	}
	return true;
}

bool isValid(int x, int y, int a, int b, int m, int n) {
	/**
	 * @brief returns whether a point is valid grid co ordinate or not within given limit
     * 
     * @param a left column limit
	 * @param b right column limit
	 */

	if (x < a || x >= m) {
		return false;
	}
	if (y < b || y >= n) {
		return false;
	}
	return true;
}

pair<ii, ii> get_random_configuration() {
    /**
     * @brief This function generates two random start and end point for the walk.
     * For test purpose this function is not used.
     * 
     */
	srand(time(NULL));
	int s_x, s_y;
	int f_x, f_y;

	do {
		s_x = (rand() % (ROW_NUM - 2)) + 1;
		f_x = (rand() % (ROW_NUM - 2)) + 1;
	} while (s_x == f_x);

	do {
		s_y = (rand() % (COL_NUM - 2)) + 1;
		f_y = (rand() % (COL_NUM - 2)) + 1;
	} while (s_y == f_y);

	return mp(mp(s_x, s_y), mp(f_x, f_y));
}

pair<ii, ii> get_static_configuration() {
    /**
     * @brief generates two static points as start and end point of the walk.
     * 
     */
	return mp(mp(1, 2), mp(3, 7));
}

void init(int r, int c) {
	/**
	 * @brief This function initializes the grid neighbourhood to graph construction.
	 *
	 * @param r Number of rows in the grid
	 * @param c Number of columns in the grid
	 * @returns null
	 */

	dir[0][0] = 0; dir[0][1] = 1;
	dir[1][0] = 1; dir[1][1] = 0;
	dir[2][0] = 0; dir[2][1] = -1;
	dir[3][0] = -1; dir[3][1] = 0;
	int index = 0;
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {
			index_of_nodes[make_pair(i, j)] = index;
			nodes_of_index[index] = make_pair(i, j);
			index++;
		}
	}

	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < COL_NUM; j++) {

			ii point = make_pair(i, j);
			int idx = index_of_nodes[point];
			vi temp_adj_list;

			for (int k = 0; k < 4; k++) {
				int x = i + dir[k][0];
				int y = j + dir[k][1];

				if (isValid(x, y, ROW_NUM, COL_NUM)) {
					temp_adj_list.push_back(index_of_nodes[make_pair(x, y)]);
				}
			}
			graph[idx] = temp_adj_list;
		}
	}
}

void print_graph(vector<vi> graph) {
	/**
	 * @brief Prints the graph in the node_number -> adjacency_list format
	 *
	 */

	for (int i = 0; i < graph.size(); i++) {
		vi temp = graph[i];
		if (temp.size() != 0) {
			cout << i << " -> ";
			for (int j = 0; j < temp.size(); j++) {
				cout << temp[j] << " ";
			}
			cout << endl;
		}
	}
}

pair<bool, int> is_eulerian(vector<vi> graph) {
	/**
	 * @brief This function checks whether the graph has any eulerian circuits or paths.
	 * Complexity: O(V). V stands for number of vertices in the graph.
	 *
	 */
	int odd_degree_count = 0;
	int size = graph.size();
	for (int i = 0; i < size; i++) {
		if (graph[i].size() > 0 && graph[i].size() % 2 == 1) {
			odd_degree_count++;
		}
	}

	if (odd_degree_count == 0 || odd_degree_count == 2) {
		return make_pair(true, odd_degree_count);
	}
	return make_pair(false, odd_degree_count);
}

bool is_odd_degree(vector<vi> graph, ii node) {
    /**
     * @brief Utilit function to check if a given node has odd degree or not.
     * 
     */
	if (graph[index_of_nodes[node]].size() % 2 == 0) {
		return false;
	}
	return true;
}

int get_degree(vector<vi> graph, ii node) {
    /**
     * @brief utility function to get the degree of a node in a graph.
     * 
     */
	return graph[index_of_nodes[node]].size();
}

void add_edge(vector<vi> &g, int x, int y) {
    /**
     * @brief utility function to add the given edge to a graph.
     * 
     */
	g[x].push_back(y);
	g[y].push_back(x);
}

vector<vi> get_graph_of_the_start_cut(ii start_point) {
	/**
	 * @brief Basically this function takes the start point of the graph and cuts the graph
	 * from the beginning to the column of the start point. Therefore, This method returns
	 * a graph of size (ROW_NUMBER * (START_INDEX)). The topleft corner is (0,0) and the
	 * bottom right is (ROW_NUM -1 , START_INDEX).
	 *
	 * @param start_point This point specifies the column value of the start
	 * point of the walk.
	 */

	int boundary = start_point.second + 1;
	vector<vi> start_graph(ROW_NUM * COL_NUM);
	bool visited[ROW_NUM*COL_NUM];
	for (int i = 0; i < (ROW_NUM*COL_NUM); i++) {
		visited[i] = false;
	}

	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = 0; j < boundary; j++) {
			ii point = mp(i, j);
			int idx = index_of_nodes[point];
			vi temp_adjacency_list;
			for (int k = 0; k < 4; k++) {
				int x = i + dir[k][0];
				int y = j + dir[k][1];

				if (isValid(x, y, ROW_NUM, boundary)) {
					if (j == boundary - 1 && y == boundary - 1) {
						// Do nothing.
					}
					else {
						temp_adjacency_list.push_back(index_of_nodes[mp(x, y)]);
					}
				}
			}
			start_graph[idx] = temp_adjacency_list;
		}
	}

	 

	ii curr = start_point;
	int curr_degree;
	bool val;
	while (true) {
		visited[index_of_nodes[curr]] = true;
		curr_degree = start_graph[index_of_nodes[curr]].size();
		if (curr_degree % 2 == 0) {
			val = false;
		}
		else {
			val = true;
		}
		
		if (val) {
			// This node has odd degree. So, we would try to make it an even degree
			// By connecting this node with another node of odd degree.

			for (int k = 0; k < 4; k++) {
				int x = curr.first + dir[k][0];
				int y = curr.second + dir[k][1];

				if (isValid(x, y, ROW_NUM, boundary) ) {
					if (is_odd_degree(start_graph, mp(x, y))) {
                        add_edge(start_graph, index_of_nodes[curr],index_of_nodes[mp(x, y)] );
						curr = mp(x, y);
						break;
					}
					else {
						int d = get_degree(start_graph, mp(x, y));
						if (d == 2 && !visited[index_of_nodes[mp(x,y)]]) {
                            add_edge(start_graph, index_of_nodes[curr],index_of_nodes[mp(x, y)] );
							curr = mp(x, y);
							break;
						}
					}
				}
			}
		}
		else {
			bool flag = false;
			for (int k = 0; k < 4; k++) {
				int x = curr.first + dir[k][0];
				int y = curr.second + dir[k][1];
				
				if (isValid(x, y, ROW_NUM, boundary) ) {

					if (is_odd_degree(start_graph, mp(x, y))) {
						curr = mp(x, y);
						break;
					}
					else {
						int d = get_degree(start_graph, mp(x, y));
						if (d == 2 && !visited[index_of_nodes[mp(x, y)]]) {
							curr = mp(x, y);
							break;
						}
					}
					
				}
				if (mp(x, y) == start_point) {
					flag = true;
					break;
				}
			}
			if (flag) {
				break;
			}

		}
	}

	return start_graph;
}

vector<vi> get_graph_of_finish_cut(ii finish_point) {
	/**
	 * @brief This function gets the right side of the graph and returns it as a separate graph.
     * The top left point of the returned graph is (0,column index of the finish point).
     * The bottom right corner of the returned graph is (ROW_NUM-1, COL_NUM-1).
	 * 
     * @param finish_point the end co-ordinate of the walk.
	 */
	
	bool visited[ROW_NUM*COL_NUM];
	for (int i = 0; i < (ROW_NUM*COL_NUM); i++) {
		visited[i] = false;
	}
	int boundary = finish_point.second;
	vector<vi> finish_graph(ROW_NUM*COL_NUM);

	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = boundary; j < COL_NUM; j++) {
			ii point = mp(i, j);
			int idx = index_of_nodes[point];
			vi temp_adj_list;
			for (int k = 0; k < 4; k++) {
				int x = i + dir[k][0];
				int y = j + dir[k][1];

				if (isValid(x, y, 0, boundary, ROW_NUM, COL_NUM)) {
					if (j == boundary && y == boundary) {
						// Do nothing;
					}
					else {
						temp_adj_list.push_back(index_of_nodes[mp(x, y)]);
					}
				}
			}
			finish_graph[idx] = temp_adj_list;
		}
	}

	ii curr = finish_point;
	int curr_degree;
	bool val;

	while (true) {
		curr_degree = get_degree(finish_graph, curr);
		visited[index_of_nodes[curr]] = true;
		if (curr_degree % 2 == 1) {
			val = true;
		}
		else {
			val = false;
		}

		if (val) {
			for (int k = 0; k < 4; k++) {
				int x = curr.first + dir[k][0];
				int y = curr.second + dir[k][1];

				if (isValid(x, y, 0, boundary, ROW_NUM, COL_NUM)) {
					if (is_odd_degree(finish_graph, mp(x, y))) {
                        add_edge(finish_graph, index_of_nodes[curr],index_of_nodes[mp(x, y)] );
						curr = mp(x, y);
						break;
					}
					else {
						int d = get_degree(finish_graph, mp(x, y));
						if (d == 2 && !visited[index_of_nodes[mp(x, y)]]) {
                            add_edge(finish_graph, index_of_nodes[curr],index_of_nodes[mp(x, y)] );
							curr = mp(x, y);
							break;
						}
					}
				}
			}
		}
		else {
			bool flag = false; 
			for (int k = 0; k < 4; k++) {
				int x = curr.first + dir[k][0];
				int y = curr.second + dir[k][1];
				if (isValid(x, y, 0, boundary, ROW_NUM, COL_NUM)) {
					if (is_odd_degree(finish_graph, mp(x, y))) {
						curr = mp(x, y);
						break;
					}
					else {
						int d = get_degree(finish_graph, mp(x, y));
						if (d == 2 && !visited[index_of_nodes[mp(x, y)]]) {
							curr = mp(x, y);
							break;
						}
					}
				}
				if (mp(x, y) == finish_point) {
					flag = true;
					break;
				}
			}
			if (flag) {
				break;
			}

		}

	}
	return finish_graph;
}

vector<vi> get_graph_of_the_middle(ii start_point, ii finish_point) {
    /**
     * @brief This function returns the graph of the middle portion.
     * In this graph, we look for an euler path instead of an euler circuit like the other two functions.
     * 
     * @param start_point start co-ordinate of the euler path.
     * @param finish_point end co-ordinate of the euler walk. 
     */
	int col1 = start_point.second;
	int col2 = finish_point.second;
	vector<vi> middle_graph(ROW_NUM*COL_NUM);
    
	for (int i = 0; i < ROW_NUM; i++) {
		for (int j = col1; j <= col2; j++) {
			ii point = mp(i, j);
			int idx = index_of_nodes[point];
			vi temp_adj_list;
			for (int k = 0; k < 4; k++) {
				int x = i + dir[k][0];
				int y = j + dir[k][1];

				if (isValid(x, y, 0, col1, ROW_NUM, col2+1)) {
					temp_adj_list.push_back(index_of_nodes[mp(x, y)]);
				}
			}
			middle_graph[idx] = temp_adj_list;
		}
	}
	
	int start_degree = get_degree(middle_graph, start_point);
	int finish_degree = get_degree(middle_graph, finish_point);
	bool visited[ROW_NUM*COL_NUM];
	for (int i = 0; i < ROW_NUM*COL_NUM; i++) {
		visited[i] = false;
	}
	if (start_degree % 2 == 1 && finish_degree % 2 == 1) {
		// This means any other node with odd degree must be transferred to even degree; 
		
		ii curr;
		visited[index_of_nodes[start_point]] = true;
		for (int k = 0; k < 4; k++) {
			int x = start_point.first + dir[k][0];
			int y = start_point.second + dir[k][1];

			if (isValid(x, y, 0, col1, ROW_NUM, col2 + 1) && is_odd_degree(middle_graph, mp(x, y))) {
				curr = mp(x, y);
				break;
			}
		}

		while (true) {
			visited[index_of_nodes[curr]] = true;
			if (curr == finish_point) { // We will not modify anything of the start and end point. 
				for (int k = 0; k < 4; k++) {
					int x = curr.first + dir[k][0];
					int y = curr.second + dir[k][1];

					if (isValid(x, y, 0, col1, ROW_NUM, col2 + 1)) {
						if (is_odd_degree(middle_graph, mp(x, y))) {
							curr = mp(x, y);
							break;
						}
						else {
							int d = get_degree(middle_graph, mp(x, y));
							if (d == 2 && !visited[index_of_nodes[mp(x, y)]]) {
								curr = mp(x, y);
								break;
							}
						}
					}
				}
				continue;
			}
			int curr_degree = get_degree(middle_graph, curr);
			if (curr_degree%2 == 1) {
				// A node with odd degree. Therefore, Should be converted to even degree.
                // In order to do that we generate the neighbours and pick another one with odd degree and add an edge
				for (int k = 0; k < 4; k++) {
					int x = curr.first + dir[k][0];
					int y = curr.second + dir[k][1];

					if (isValid(x, y, 0, col1, ROW_NUM, col2 + 1)) {
						if (is_odd_degree(middle_graph, mp(x, y)) && mp(x, y) != finish_point) {
							add_edge(middle_graph, index_of_nodes[curr], index_of_nodes[mp(x, y)]);
							curr = mp(x, y);
							break;
						}
						else {
                            // If odd degree nodes are not available to add edge, we take even degree ones.
                            // However, we only do that to boundary nodes. Internal nodes with degree 4 are not used.
							int d = get_degree(middle_graph, mp(x, y));
							if (d == 2 && !visited[index_of_nodes[mp(x, y)]]) {
								add_edge(middle_graph, index_of_nodes[curr], index_of_nodes[mp(x, y)]);
								curr = mp(x, y);
								break;
							}
						}
					}
				}
				if (curr == start_point) {
					break;
				}
			}
			else {
				// A node with even degree. Therefore, should look in the neighbours for odd degree and transfer
				// Current node to that.
				for (int k = 0; k < 4; k++) {
					int x = curr.first + dir[k][0];
					int y = curr.second + dir[k][1];
					if (isValid(x, y, 0, col1, ROW_NUM, col2 + 1)) {
						if (is_odd_degree(middle_graph, mp(x, y)) && mp(x, y) != finish_point) {
							curr = mp(x, y);
							break;
						}
						else if (is_odd_degree(middle_graph, mp(x, y)) && mp(x, y) == finish_point) {
                            // This condition makes sure we don't change the odd degree of the end point of the walk.
							curr = mp(x, y);
							break;
						}
						else {
							int d = get_degree(middle_graph, mp(x, y));
							if (d == 2 && !visited[index_of_nodes[mp(x, y)]]) {
								curr = mp(x, y);
								break;
							}
						}
					}
				}
				if (curr == start_point) {
					break;
				}
			}
		}

	}
	return middle_graph;
}

void remove_edge(vector<vi> &g, int x, int y) {
    /**
     * @brief basic utility function to remove an edge from the graph.
     * 
     */
	for (int i = 0; i < g[x].size(); i++) {
		if (g[x][i] == y) {
			g[x].erase(g[x].begin() + i);
			break;
		}
	}
	for (int j = 0; j < g[y].size(); j++) {
		if (g[y][j] == x) {
			g[y].erase(g[y].begin() + j);
			break;
		}
	}
}

std::ostream& operator<<(std::ostream& os, const std::vector<int> &input)
{
	for (auto const& i : input) {
		os << i << " ";
	}
	return os;
}

vi euler_circuit(vector<vi> &g, int start_node) {
    /**
     * @brief This is the non recursive version of the  Hierholzer’s Algorithm.
     * The pseudocode I implemented can be found here:  https://cp-algorithms.com/graph/euler_path.html
     * 
     * @param start_node start and end point of a closed euler circuit.
     */
	stack<int> st;
	vi ans;
	st.push(start_node);

	while (!st.empty()) {
		int top = st.top();
		if (g[top].size() == 0) {
			ans.push_back(top);
			st.pop();
		}
		else {
			int u = g[top][0]; // Random pick;
			remove_edge(g, top, u);
			st.push(u);
		}
	}
	return ans;
}

vi euler_path(vector<vi> &g, int start, int end) {
    /**
     * @brief This function finds the euler path. The previous one finds the euler circuit.
     * The difference is @param g has exactly two nodes with odd degree and we use them
     * as start and end point. All other nodes of the graph is even degree.
     * 
     * @param start start point of the euler path
     * @param end end point of the euler path
     */
	stack<int> st;
	vi ans;
	st.push(start);

	while (!st.empty()) {
		int top = st.top();
		if (g[top].size() == 0) {
			ans.push_back(top);
			st.pop();
		}
		else {
			int u = g[top][0]; // Random pick;
			remove_edge(g, top, u);
			st.push(u);
			if (u == end) {
				while (!st.empty()) {
					ans.push_back(st.top());
					st.pop();
				}
			}
		}
	}
	
	reverse(ans.begin(), ans.end());
	vi ans1 = euler_circuit(g, end);
	for (int k = 1; k < ans1.size(); k++) {
		ans.push_back(ans1[k]);
	}
	return ans;
}
int main(int argc, char const *argv[])
{
	init(ROW_NUM, COL_NUM);
	// If the following line is uncommented the code will pick two points at random and run the algorithm on that.
	//pair<ii,ii> config = get_random_configuration();
	pair<ii, ii> config = get_static_configuration();
	cout << "Start: " << config.first.first << " , " << config.first.second << endl;
	cout << "End: " << config.second.first << " , " << config.second.second << endl;

	vector<vi> s_graph = get_graph_of_the_start_cut(config.first);
	//print_graph(s_graph);
	pair<bool, int> res = is_eulerian(s_graph);
	if (res.first && res.second == 0) {
		cout << "Start Graph is eulerian circuit." << endl;
	}	
	vi walk = euler_circuit(s_graph, index_of_nodes[config.first]);
	
	// Finish Graph Processing
	vector<vi> f_graph = get_graph_of_finish_cut(config.second);
	//print_graph(f_graph);
	res = is_eulerian(f_graph);
	if (res.first && res.second == 0) {
		cout << "Finish Graph is eulerian circuit" << endl;
	}
	vi walk1 = euler_circuit(f_graph, index_of_nodes[config.second]);


	// Middle Graph processing 
	vector<vi> m_graph = get_graph_of_the_middle(config.first, config.second);
	//print_graph(m_graph);
	res = is_eulerian(m_graph);
	if (res.first && res.second == 2) {
		cout << "The middle graph has two nodes with odd degree. There is an euler path." << endl;
	}
	vi walk2 = euler_path(m_graph, index_of_nodes[config.first], index_of_nodes[config.second]);
	//Final processing of the walks
	vi final_walk;
	for (int i = 0; i < walk.size(); i++) {
		final_walk.push_back(walk[i]);
	}
	for (int i = 1; i < walk2.size(); i++) {
		final_walk.push_back(walk2[i]);
	}
	for (int i = 1; i < walk1.size(); i++) {
		final_walk.push_back(walk1[i]);
	}
	cout << "Optimized euler trail with minimum backtracking: "<<final_walk.size() << endl;
	for (int i = 0; i < final_walk.size(); i++) {
		ii point = nodes_of_index[final_walk[i]];
		cout << "( " << point.first << " , " << point.second << " ) "<<endl;
	}
	cout << endl;
	return 0;
}
