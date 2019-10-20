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
#include <chrono>

using namespace std::chrono;
// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a list of nodes where each node is represented as follows:





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
		for(int i=0;i<CPT.size();i++){
			aux_CPT.push_back(0);
		}
	}

	void smoothen_divide_Aux(float sigma){
		for(int i=0;i<aux_CPT.size();i++){
			if(aux_CPT[i] == 0){
				aux_CPT[i] = 1.0/sigma;
			}
			else{
				aux_CPT[i] = aux_CPT[i]/sigma;
			}
		}
		CPT = aux_CPT;
	}
	int get_nvalues(){
		return nvalues;
	}

	vector<string> get_values(){
		return values;
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
    // add another node in a graph as a child of this node
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
        }
	}

	void smoothen_divide_all_Aux(float sigma){
		list<Graph_Node>::iterator listIt;
        for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
           	listIt->smoothen_divide_Aux(sigma);
        }
	}

	void updateCPTs(vector<string> v, float weight){
		list<Graph_Node>::iterator listIt;
        for(listIt = Pres_Graph.begin();listIt!=Pres_Graph.end();listIt++){
           	vector<string> temp_parents = listIt->get_Parents();
           	// vector<int> index_features;
           	vector<int> value_features;
           	vector<int> possibleValues;
           	// index_features.push_back(get_index(listIt->get_name()));
           	value_features.push_back(listIt->index_value(v[get_index(listIt->get_name())]));
           	possibleValues.push_back(listIt->get_nvalues());
           	for(int i=0;i<temp_parents.size();i++){
           		list<Graph_Node>::iterator tempNodeItr = search_node(temp_parents[i]);
           		possibleValues.push_back(tempNodeItr->get_nvalues());
           		// index_features.push_back(get_index(temp_parents[i]));
           		value_features.push_back(tempNodeItr->index_value(v[get_index(temp_parents[i])]));
           	}
           	int tempIndex = 0;
           	for(int i=0;i<possibleValues.size();i++){
           		tempIndex += value_features[i]*(possibleValues[i] - 1);
           	}
           	listIt->aux_CPT[tempIndex] += weight;
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

network read_network(){
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile("alarm.bif"); 
  	string temp;
  	string name;
  	vector<string> values;
    if (myfile.is_open()){
        //cout<<"myfile is open\n";
    	while (!myfile.eof()){
    		stringstream ss;
      		getline (myfile,line);
      		ss.str(line);
     		ss>>temp;
     		if(temp.compare("variable")==0){
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
     				int pos=Alarm.addNode(new_node);
     		}
     		else if(temp.compare("probability")==0){
     				ss>>temp;
     				ss>>temp;
                    list<Graph_Node>::iterator listIt;
                    list<Graph_Node>::iterator listIt1;
     				listIt=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0){
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

int main(){



	network Alarm;
	Alarm=read_network();
	cout<<Alarm.netSize()<<endl;
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    int numVariable;
    //creating dictionary
    for(int i=0;i<Alarm.netSize();i++){
        int num_node_parents = Alarm.get_nth_node(i)->Parents.size();
        if(num_node_parents == 0){
            for(int k=0;k<Alarm.get_nth_node(i)->nvalues;k++){
                Alarm.get_nth_node(i)->cpt_maps.insert(make_pair(Alarm.get_nth_node(i)->values[k],0));
            }
        }
        else{
            vector<int> npindex((num_node_parents+1),0);
            vector<string> keyvals((num_node_parents+1),"");
            vector<bool> zeroed((num_node_parents+1),true);
            for(int j=0;j<Alarm.get_nth_node(i)->CPT.size();j++){                
                for(int m = num_node_parents;m>=0;m--){
                    if(m == num_node_parents){
                        string parentName = Alarm.get_nth_node(i)->Parents[m-1];
                        int nvaluesize = Alarm.search_node(parentName)->nvalues;
                        int valueindex = npindex[m] % nvaluesize;
                        if(valueindex == 0){
                            zeroed[m] = true;
                        }
                        else{
                            zeroed[m]=false;
                        }
                        npindex[m] = valueindex+1;
                        keyvals[m] = Alarm.search_node(parentName)->values[valueindex];
                    }
                    else if(m == 0){
                        int nvaluesize = Alarm.get_nth_node(i)->nvalues;
                        if((zeroed[m+1])&&(j!=0)){
                            int valueindex=npindex[m]%nvaluesize;
                            if(valueindex==0){
                                zeroed[m]=true;
                            }
                            else{
                                zeroed[m]=false;
                            }
                            npindex[m]=valueindex+1;
                            keyvals[m]=Alarm.get_nth_node(i)->values[valueindex];
                        }
                        if(j==0)
                        {
                            keyvals[m]=Alarm.get_nth_node(i)->values[0];
                            npindex[m]=npindex[m]+1;
                            if(nvaluesize==1){
                                zeroed[m]=true;
                            }
                            else{
                                zeroed[m]=false;
                            }
                        }
                    }
                    else{
                        string parentName=Alarm.get_nth_node(i)->Parents[m-1];
                        int nvaluesize=Alarm.search_node(parentName)->nvalues;
                        if((zeroed[m+1])&&(j!=0)){
                            int valueindex=npindex[m]%nvaluesize;
                            if(valueindex==0){
                                zeroed[m]=true;
                            }
                            else{
                                zeroed[m]=false;
                            }
                            npindex[m]=valueindex+1;
                            keyvals[m]=Alarm.search_node(parentName)->values[valueindex];
                        }
                        if(!zeroed[m+1]){
                            zeroed[m]=false;
                        }
                        if(j==0){
                            keyvals[m]=Alarm.search_node(parentName)->values[0];
                            npindex[m]=npindex[m]+1;
                            if(nvaluesize==1){
                                zeroed[m]=true;
                            }
                            else{
                                zeroed[m]=false;
                            }
                        }
                    }
                }
                string key="";
                for(int b=0;b<(num_node_parents+1);b++){
                    key.append(keyvals[b]);
                }
                Alarm.get_nth_node(i)->cpt_maps.insert(make_pair(key,0));
            }
        }
    }
    numVariable = Alarm.netSize();
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////  
    vector<int> missingData[numVariable];
    //reading records.dat
    // File pointer 
    ifstream fin; 
    // Open an existing file 
    fin.open("records.dat"); 
    // Read the Data from the file 
    // as String Vector 
    vector<string> row;
    vector<vector<string> > inputData;
    string line, word, temp;
    int count = 0;
    while(getline(fin, line)){
        row.clear();
        
        // read an entire row and 
        // store it in a string variable 'line' 
        // used for breaking words
        stringstream s(line);
        // read every column data of a row and 
        // store it in a string variable, 'word'
        int i = 0; 
        while(getline(s, word, ' ')) { 
            //word=word.substr(1,(word.length()-2));
            // add all the column data 
            // of a row to a vector
            row.push_back(word);
            string questionMark = "?";
            size_t found = word.find(questionMark);
    		if(found != string::npos){
        		missingData[i].push_back(count);
        	}
        	i++;
        }
        inputData.push_back(row);
        count++;
        //read one row as a vector in above while loop
    }



    // for(int i=0;i<inputData.size();i++){
    // 	cout<<inputData.at(i).size()<<endl;
    // 	for(int j=0;j<inputData.at(i).size();j++){
    // 		cout<<inputData.at(i).at(j)<<" ";
    // 	}
    // 	cout<<endl;
    // }

    // for(int i=0;i<numVariable;i++){
    // 	cout<<Alarm.get_nth_node(i)->get_name()<<": ";
    // 	for(int j=0;j<missingData[i].size();j++){
    // 		cout<<missingData[i][j]<<" ";
    // 	}
    // 	cout<<endl;
    // }
    // cout<<Alarm.get_nth_node(2)->Node_Name<<" "<<Alarm.get_nth_node(2)->cpt_maps.size()<<endl;
    //cout<<Alarm.get_nth_node(12)->Node_Name<<" "<<Alarm.get_nth_node(12)->cpt_maps.size()<<endl;



    Alarm.randomCPTinit();
    Alarm.viewCPT();
    // int curr_params = 1;
    vector<vector<string> > completedData;
    vector<float> dataWeight;
	auto start = high_resolution_clock::now();
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<seconds>(stop - start);
    int timed=(int)duration.count();
    while(timed < 110){
    	completedData.clear();
    	dataWeight.clear();
    	// Step 1:
    	for(int i=0;i<inputData.size();i++){
			int missingFeature = findMissingFeature(inputData[i]);
    		if(missingFeature == -1){
    			completedData.push_back(inputData[i]);
    			dataWeight.push_back(1);
    		}
    		else{
    			list<Graph_Node>::iterator tempNode = Alarm.get_nth_node(i);
    			int temp_num_values = tempNode->get_nvalues();
    			vector<float> temp_CPT = tempNode->get_CPT();
    			vector<string> temp_markov_blanket = Alarm.get_markov_blanket(tempNode->get_name());
    			vector<string> temp_values = tempNode->get_values();
    			float arr[temp_num_values] = {0.1};


    			for(int j=0; j<temp_num_values; j++){
    				completedData.push_back(replaceQuestionMark(temp_values[j], inputData[j]));
    				dataWeight.push_back(arr[j]);
    			}
    		}
    	}


    	//Step 2:
    	for(int i=0;i<completedData.size();i++){
    		Alarm.updateCPTs(completedData[i], dataWeight[i]);
    	}
    	Alarm.smoothen_divide_all_Aux(inputData.size());

    	auto stop = high_resolution_clock::now();
        auto duration = duration_cast<seconds>(stop - start);
        int timed=(int)duration.count();
    }
    return 0;
}