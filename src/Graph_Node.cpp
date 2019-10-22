#include "../include/Graph_Node.h"
    // Constructor- a node is initialised with its name and its categories
Graph_Node::Graph_Node(string name,int n,vector<string> vals){
    Node_Name = name;
    nvalues = n;
    values = vals;
}

string Graph_Node::get_name(){
    return Node_Name;
}

vector<int> Graph_Node::get_children(){
    return Children;
}
vector<string> Graph_Node::get_Parents(){
    return Parents;
}

vector<float> Graph_Node::get_CPT(){
    return CPT;
}

void Graph_Node::clear_Aux(){
    int CPT_size = CPT.size();
    int tempB, tempA = CPT_size/nvalues;
    for(int i=0;i<CPT.size();i++){
        tempB = i%tempA;
        aux_CPT[i] = 0.001;
        aux_count_CPT[tempB] = 0.1;
    }
}

void Graph_Node::clear_aux_count_CPT(){
    aux_count_CPT.clear();
    for(int i=0;i<CPT.size()/nvalues;i++){
        aux_count_CPT.push_back(0);
    }
}
void Graph_Node::initialise_aux_count_CPT(){
    int CPT_size = CPT.size();
    aux_count_CPT.clear();
    aux_CPT.clear();
    for(int i=0;i<CPT.size()/nvalues;i++){
        aux_count_CPT.push_back(0.1);
    }
    for(int i=0;i<CPT.size();i++){
        aux_CPT.push_back(0.001);
    }
}

void Graph_Node::smoothen_divide_Aux(){
    int CPT_size = CPT.size();
    int tempB, tempA = CPT_size/nvalues;
    for(int i=0;i<aux_CPT.size();i++){
        tempB = i%tempA;
        if(aux_CPT[i] < 0.0001 || aux_count_CPT[tempB] < 0.1){
            aux_CPT[i] = 0.001;
            aux_count_CPT[tempB] = 0.1;
        }
        else{
            aux_CPT[i] = aux_CPT[i]/aux_count_CPT[tempB];
        }
    }
    CPT = aux_CPT;
    clear_Aux();
}

int Graph_Node::get_nvalues(){
    return nvalues;
}

vector<string> Graph_Node::get_values(){
    return values;
}

void Graph_Node::view_values(){
    cout<<get_name()<<": ";
    vector<string> a = get_values();
    for(int i=0;i<a.size();i++){
        cout<<a[i]<<" ";
    }
    cout<<endl;
}

int Graph_Node::index_value(string str){
    for(int i=0;i<values.size();i++){
        if(values[i].compare(str) == 0){
            return i;
        }
    }
    return -1;
}

void Graph_Node::set_CPT(vector<float> new_CPT){
    CPT.clear();
    CPT=new_CPT;
}

void Graph_Node::set_random_CPT(){
    int n = CPT.size();
    for(int i=0;i<n;i++){
        if(CPT[i] == -1){
            CPT[i] = (float)rand()/(RAND_MAX);
        }
    }
}

void Graph_Node::view_node_CPT(){
    int n = CPT.size();
    for(int i=0;i<n;i++){
        cout<<CPT[i]<<" ";
    }
    cout<<endl;
}

void Graph_Node::set_Parents(vector<string> Parent_Nodes){
    Parents.clear();
    Parents=Parent_Nodes;
}

int Graph_Node::add_child(int new_child_index){
    for(int i=0;i<Children.size();i++){
        if(Children[i]==new_child_index)
            return 0;
    }
    Children.push_back(new_child_index);
    return 1;
}