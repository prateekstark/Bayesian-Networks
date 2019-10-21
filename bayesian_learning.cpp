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
#include<iomanip>
#include <ctime>
using namespace std;

bool isDataPointComplete(vector<string> dataPoint){
    string questionMark = "?";
    for(int i=0;i<dataPoint.size();i++){
        size_t found = dataPoint[i].find(questionMark);
        if(found != string::npos){
            return 0;
        }
    }
    return 1;
}

int findMissingFeature(vector<string> dataPoint){
    string questionMark = "?";
    for(int i=0;i<dataPoint.size();i++){
        size_t found = dataPoint[i].find(questionMark);
        if(found != string::npos){
            return i;
        }
    }
    return -1;
}

bool isStringPresentInVector(vector<string> v, string key){
    for(int i=0;i<v.size();i++){
        if(v[i].compare(key) == 0){
            return 1;
        }
    }
    return 0;
}

vector<string> replaceQuestionMark(string str, vector<string> dataPoint){
    int index = findMissingFeature(dataPoint);
    if(index != -1){
        dataPoint[index] = str;
    }
    return dataPoint;
}

vector<string> replaceQuestionMarkIndex(string str, vector<string> dataPoint, int index){
    dataPoint[index] = str;
    return dataPoint;
}


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
    Graph_Node(string name,int n,vector<string> vals){
        Node_Name = name;
        nvalues = n;
        values = vals;
    }

    string get_name(){
        return Node_Name;
    }

    vector<int> get_children(){
        return Children;
    }
    vector<string> get_Parents(){
        return Parents;
    }

    vector<float> get_CPT(){
        return CPT;
    }

    void clear_Aux(){
        int CPT_size = CPT.size();
        int tempB, tempA = CPT_size/nvalues;
        for(int i=0;i<CPT.size();i++){
            tempB = i%tempA;
            aux_CPT[i] = 0.001;
            aux_count_CPT[tempB] = 0.1;
        }
    }

    void clear_aux_count_CPT(){
        aux_count_CPT.clear();
        for(int i=0;i<CPT.size()/nvalues;i++){
            aux_count_CPT.push_back(0);
        }
    }
    void initialise_aux_count_CPT(){
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

    void smoothen_divide_Aux(){
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

    int get_nvalues(){
        return nvalues;
    }

    vector<string> get_values(){
        return values;
    }

    void view_values(){
        cout<<get_name()<<": ";
        vector<string> a = get_values();
        for(int i=0;i<a.size();i++){
            cout<<a[i]<<" ";
        }
        cout<<endl;
    }

    int index_value(string str){
        for(int i=0;i<values.size();i++){
            if(values[i].compare(str) == 0){
                return i;
            }
        }
        return -1;
    }

    void set_CPT(vector<float> new_CPT){
        CPT.clear();
        CPT=new_CPT;
    }

    void set_random_CPT(){
        int n = CPT.size();
        for(int i=0;i<n;i++){
            if(CPT[i] == -1){
                CPT[i] = (float)rand()/(RAND_MAX);
            }
        }
    }

    void view_node_CPT(){
        int n = CPT.size();
        for(int i=0;i<n;i++){
            cout<<CPT[i]<<" ";
        }
        cout<<endl;
    }

    void set_Parents(vector<string> Parent_Nodes){
        Parents.clear();
        Parents=Parent_Nodes;
    }
    
    int add_child(int new_child_index){
        for(int i=0;i<Children.size();i++){
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }
};

 // The whole network represted as a list of nodes
class network{
    list<Graph_Node> Pres_Graph;
public:
    int addNode(Graph_Node node){
        Pres_Graph.push_back(node);
        return 0;
    }

    void viewCPT(){
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

    vector<string> giveNodes(){
        vector<string> answer;
        list<Graph_Node>::iterator listIt;
        for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
            answer.push_back(listIt->get_name());
        }
        return answer;
    }

    void randomCPTinit(){
        list<Graph_Node>::iterator listIt;
        int count = 0;
        for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
            listIt->set_random_CPT();
            count++;
        }
    }
    
    int netSize(){
        return Pres_Graph.size();
    }

    void clear_All_Aux(){
        list<Graph_Node>::iterator listIt;
        for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
            listIt->clear_Aux();
            listIt->clear_aux_count_CPT();
        }
    }

    void smoothen_divide_all_Aux(){
        list<Graph_Node>::iterator listIt;
        for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
            listIt->smoothen_divide_Aux();
        }
    }

    void initialise_all_aux_count_CPT(){
        list<Graph_Node>::iterator listIt;
        for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
            listIt->initialise_aux_count_CPT();
        }
    }

    float giveProba(vector<string> dataPoint, string node){
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

    void updateCPTs(vector<string> v, float weight){
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

    vector<string> get_markov_blanket(string temp_name){
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
    int get_index(string val_name){
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
    list<Graph_Node>::iterator get_nth_node(int n){
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
    list<Graph_Node>::iterator search_node(string val_name){
        list<Graph_Node>::iterator listIt;
        for(listIt=Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
            if(listIt->get_name().compare(val_name)==0){
                return listIt;
            }
        }
        cout<<"node not found\n";
        return listIt;
    }
};

void write_network(string inputFileName, network Alarm){
    string line;
    int find=0;
    ifstream myfile(inputFileName);
    string temp;
    string name;
    vector<string> values;
    ofstream outputFile;
    outputFile.open("solved_alarm.bif");
    int variable_count = 0;

    if(myfile.is_open()){
        while(!myfile.eof()){
            stringstream ss;
            getline(myfile,line);
            ss.str(line);
            ss>>temp;
            if(temp.compare("network") == 0){
                outputFile << line << endl;
                getline(myfile,line);
                outputFile << line << endl;
            }

            else if(temp.compare("variable") == 0){
                outputFile << line << endl;
                getline (myfile,line);
                outputFile << line << endl;
                getline (myfile,line);
                outputFile << line << endl;
                getline (myfile,line);
                outputFile << line << endl;
            }

            else if(temp.compare("probability") == 0){
                outputFile << line << endl;
                getline (myfile, line);
                outputFile << "    table ";
                list<Graph_Node>::iterator listIt = Alarm.get_nth_node(variable_count);
                vector<float> CPT = listIt->get_CPT();
                for(int i=0;i<CPT.size();++i){
                    outputFile << CPT[i] << " ";
                }
                outputFile << ";" << endl;
                outputFile << "}" << endl;
                variable_count++;
            }
            else if(temp.compare("//") == 0){
                outputFile << line << endl;
            }   
        }
    }
    myfile.close();
    outputFile.close();
}


network read_network(string inputFileName, map<string, vector<string> > &valueMap, map<string, int> &valueNumMap){
    network Alarm;
    string line;
    int find=0;
    ifstream myfile(inputFileName); 
    string temp;
    string name;
    vector<string> values;
    if (myfile.is_open()){
        while (!myfile.eof()){
            stringstream ss;
            getline (myfile,line);
            ss.str(line);
            ss>>temp;
            if(temp.compare("variable") == 0){
                ss>>name;
                getline (myfile,line);
                stringstream ss2;
                ss2.str(line);
                for(int i=0;i<4;i++){
                    ss2>>temp;
                }
                values.clear();
                while(temp.compare("};")!=0){
                    values.push_back(temp);
                    ss2>>temp;
                }
                Graph_Node new_node(name,values.size(),values);
                valueMap.insert(pair<string, vector<string> >(name, values));
                valueNumMap.insert(pair<string, int>(name, values.size()));
                int pos=Alarm.addNode(new_node);
            }
            else if(temp.compare("probability") == 0){
                ss>>temp;
                ss>>temp;
                list<Graph_Node>::iterator listIt;
                list<Graph_Node>::iterator listIt1;
                listIt=Alarm.search_node(temp);
                int index=Alarm.get_index(temp);
                ss>>temp;
                values.clear();
                while(temp.compare(")") != 0){
                    listIt1=Alarm.search_node(temp);
                    listIt1->add_child(index);
                    values.push_back(temp); 
                    ss>>temp;
                }
                listIt->set_Parents(values);
                getline (myfile,line);
                stringstream ss2;
                ss2.str(line);
                ss2>> temp;
                ss2>> temp;
                
                vector<float> curr_CPT;
                string::size_type sz;
                while(temp.compare(";")!=0){
                    curr_CPT.push_back(atof(temp.c_str()));
                    ss2>>temp;
                }
                
                listIt->set_CPT(curr_CPT);
            }
            else{
                
            }   
        }
        
        if(find==1)
        myfile.close();
    }
    return Alarm;
}

int main(int argc, char **argv){
    string inputFileName = argv[1];
    string recordFileName = argv[2];
    network Alarm;
    map<string, vector<string> > valueMap;
    map<string, int> valueNumMap;
    Alarm=read_network(inputFileName, valueMap, valueNumMap);
    vector<string> nodeList = Alarm.giveNodes();
    Alarm.randomCPTinit();
    Alarm.initialise_all_aux_count_CPT();
    clock_t start = clock();
    clock_t stop = clock();
    float duration = float(stop - start)/CLOCKS_PER_SEC;
    ifstream fin;
    fin.open(recordFileName); 
    vector<string> row;
    vector<vector<string> > inputData;
    string line, word, temp;
    while(getline(fin, line)){
        row.clear();
        stringstream s(line);
        int i = 0; 
        while(getline(s, word, ' ')){
            row.push_back(word);
        }
        inputData.push_back(row);
    }
    fin.close();

    while(duration < 110){
        cout << duration << endl;
        for(int i=0;i<inputData.size();i++){
            vector<string> inputPoint = inputData[i];
            int missingFeature = findMissingFeature(inputPoint);         
            if(missingFeature == -1){
                Alarm.updateCPTs(inputPoint, 1);
            }
            else{
                list<Graph_Node>::iterator tempNode = Alarm.get_nth_node(missingFeature);
                string tempName = tempNode->get_name();
                int temp_num_values = valueNumMap[tempName];
                vector<string> temp_values = valueMap[tempName];
                vector<string> dataPoints[temp_num_values];
                float tempWeights[temp_num_values];
                float sum = 0;
                for(int j=0;j<temp_num_values; j++){ 
                    vector<string> completePoint = replaceQuestionMarkIndex(temp_values[j], inputPoint, missingFeature);
                    float proba = 1;
                    for(int k=0;k<nodeList.size();k++){
                        proba = proba*Alarm.giveProba(completePoint, nodeList[k]);
                    }
                    dataPoints[j] = completePoint;
                    tempWeights[j] = proba;
                    sum += proba;
                }
                for(int j=0;j<temp_num_values;j++){
                    Alarm.updateCPTs(dataPoints[j], (float)tempWeights[j]/sum);
                }

            }
        }
        Alarm.smoothen_divide_all_Aux();
        stop = clock();
        duration = float(stop - start)/CLOCKS_PER_SEC;
    }
    write_network(inputFileName, Alarm);
    return 0;
}