#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string.h>
#include <random>
#include <map>
#include <iomanip>
#include <ctime>
using namespace std;

class Graph_Node{

public:
    string Node_Name;  // Variable name 
    vector<int> Children; // Children of a particular node - these are index of nodes in graph.
    vector<string> Parents; // Parents of a particular node- note these are names of parents
    int nvalues;  // Number of categories a variable represented by this node can take
    vector<string> values; // Categories of possible values
    map<string,int> cpt_maps;
    vector<float> CPT;
    vector<float> aux_CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning
    vector<float> aux_count_CPT;

public:
    // Constructor- a node is initialised with its name and its categories
    Graph_Node(string name,int n,vector<string> vals);

    string get_name();

    vector<int> get_children();
    vector<string> get_Parents();

    vector<float> get_CPT();

    void clear_Aux();

    void clear_aux_count_CPT();
    void initialise_aux_count_CPT();

    void smoothen_divide_Aux();

    int get_nvalues();

    vector<string> get_values();

    void view_values();

    int index_value(string str);

    void set_CPT(vector<float> new_CPT);

    void set_random_CPT();

    void view_node_CPT();

    void set_Parents(vector<string> Parent_Nodes);
    
    int add_child(int new_child_index);
};