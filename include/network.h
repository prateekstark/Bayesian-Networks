#include "Graph_Node.h"
class network{
    list<Graph_Node> Pres_Graph;
public:
    int addNode(Graph_Node node);

    bool isStringPresentInVector(vector<string> v, string key);

    void viewCPT();

    vector<string> giveNodes();
    void randomCPTinit();
    
    int netSize();
    void clear_All_Aux();
    void smoothen_divide_all_Aux();

    void initialise_all_aux_count_CPT();

    float giveProba(vector<string> dataPoint, string node);

    void updateCPTs(vector<string> v, float weight);

    vector<string> get_markov_blanket(string temp_name);

    // get the index of node with a given name
    int get_index(string val_name);
// get the node at nth index
    list<Graph_Node>::iterator get_nth_node(int n);
//get the iterator of a node with a given name
    list<Graph_Node>::iterator search_node(string val_name);
};