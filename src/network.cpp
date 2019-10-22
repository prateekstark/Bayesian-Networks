#include "../include/network.h"
int network::addNode(Graph_Node node){
    Pres_Graph.push_back(node);
    return 0;
}

void network::viewCPT(){
    list<Graph_Node>::iterator listIt;
    for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        cout<<listIt->get_name()<<": ";
        vector<float> tempVector = listIt->get_CPT();
        for(int i=0;i<tempVector.size();i++){
            cout << tempVector[i] << " ";
        }
        cout<<endl;
    }      
}

vector<string> network::giveNodes(){
    vector<string> answer;
    list<Graph_Node>::iterator listIt;
    for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        answer.push_back(listIt->get_name());
    }
    return answer;
}

void network::randomCPTinit(){
    list<Graph_Node>::iterator listIt;
    int count = 0;
    for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        listIt->set_random_CPT();
        count++;
    }
}

bool network::isStringPresentInVector(vector<string> v, string key){
    for(int i=0;i<v.size();i++){
        if(v[i].compare(key) == 0){
            return 1;
        }
    }
    return 0;
}


int network::netSize(){
    return Pres_Graph.size();
}

void network::clear_All_Aux(){
    list<Graph_Node>::iterator listIt;
    for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        listIt->clear_Aux();
        listIt->clear_aux_count_CPT();
    }
}

void network::smoothen_divide_all_Aux(){
    list<Graph_Node>::iterator listIt;
    for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        listIt->smoothen_divide_Aux();
    }
}

void network::initialise_all_aux_count_CPT(){
    list<Graph_Node>::iterator listIt;
    for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        listIt->initialise_aux_count_CPT();
    }
}

float network::giveProba(vector<string> dataPoint, string node){
    list<Graph_Node>::iterator tempNode = search_node(node);
    int CPT_size = tempNode->CPT.size();
    int tempIndex = 0;
    int tempInt = CPT_size;
    int value_feature = tempNode->index_value(dataPoint[get_index(node)]);
    int num_possible = tempNode->get_nvalues();
    tempIndex += tempInt*value_feature/num_possible;
    tempInt = tempInt/num_possible;
    vector<string> temp_parents = tempNode->get_Parents();
    
    for(int i=0;i<temp_parents.size();i++){
        list<Graph_Node>::iterator tempNodeItr = search_node(temp_parents[i]);
        int index = get_index(temp_parents[i]);
        value_feature = tempNodeItr->index_value(dataPoint[index]);
        num_possible = tempNodeItr->get_nvalues();
        tempIndex += tempInt*value_feature/num_possible;
        tempInt = tempInt/num_possible;
    }
    return tempNode->CPT[tempIndex];
}

void network::updateCPTs(vector<string> v, float weight){
    list<Graph_Node>::iterator listIt;
    for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        vector<string> temp_parents = listIt->get_Parents();
        int CPT_size = listIt->CPT.size();
        int tempIndex = 0;
        int a;
        int value_feature = listIt->index_value(v[get_index(listIt->get_name())]);
        int num_possible = listIt->get_nvalues();
        int tempInt = CPT_size;
        a = tempInt*value_feature/num_possible;
        tempIndex += tempInt*value_feature/num_possible;
        tempInt = tempInt/num_possible;
        for(int i=0;i<temp_parents.size();i++){
            list<Graph_Node>::iterator tempNodeItr = search_node(temp_parents[i]);
            value_feature = tempNodeItr->index_value(v[get_index(temp_parents[i])]);
            num_possible = tempNodeItr->get_nvalues();
            tempIndex += tempInt*value_feature/num_possible;
            tempInt = tempInt/num_possible;
        }
        listIt->aux_CPT[tempIndex] += weight;
        listIt->aux_count_CPT[tempIndex - a] += weight;
    }
}

vector<string> network::get_markov_blanket(string temp_name){
    vector<string> markov_blanket;
    list<Graph_Node>::iterator tempNode = search_node(temp_name);
    vector<string> parents = tempNode->get_Parents();
    for(int i=0;i<parents.size();i++){
        markov_blanket.push_back(parents[i]);
    }
    vector<int> children = tempNode->get_children();
    for(int i=0;i<children.size();i++){
        list<Graph_Node>::iterator tempNode1 = get_nth_node(children[i]);
        markov_blanket.push_back(tempNode1->get_name());
        vector<string> temp_parents = tempNode1->get_Parents();
        for(int j=0;j<temp_parents.size();j++){
            if(!isStringPresentInVector(markov_blanket, temp_parents[j]) && temp_parents[j].compare(temp_name) != 0){
                markov_blanket.push_back(temp_parents[j]);
            }
        }
    }
    return markov_blanket;
}


// get the index of node with a given name
int network::get_index(string val_name){
    list<Graph_Node>::iterator listIt;
    int count=0;
    for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        if(listIt->get_name().compare(val_name)==0)
            return count;
        count++;
    }
    return -1;
}

// get the node at nth index
list<Graph_Node>::iterator network::get_nth_node(int n){
   list<Graph_Node>::iterator listIt;
    int count=0;
    for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        if(count==n)
            return listIt;
        count++;
    }
    return listIt; 
}

//get the iterator of a node with a given name
list<Graph_Node>::iterator network::search_node(string val_name){
    list<Graph_Node>::iterator listIt;
    for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
        if(listIt->get_name().compare(val_name)==0){
            return listIt;
        }
    }
    cout<<"node not found\n";
    return listIt;
}