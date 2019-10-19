#include <iostream>
#include <string>
#include <vector>
#include <list>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <string.h>
#include <map>


// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a list of nodes where each node is represented as follows:
class Graph_Node{

public:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
    map<string,int> cpt_maps;
	vector<float> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

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
	int get_nvalues(){
		return nvalues;
	}
	vector<string> get_values(){
		return values;
	}
	void set_CPT(vector<float> new_CPT){
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes){
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index ){
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
    
	int netSize(){
		return Pres_Graph.size();
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

int main()
{
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
    			cout<<"hello"<<endl;
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

    for(int i=0;i<numVariable;i++){
    	cout<<Alarm.get_nth_node(i)->get_name()<<": ";
    	for(int j=0;j<missingData[i].size();j++){
    		cout<<missingData[i][j]<<" ";
    	}
    	cout<<endl;
    }
    // cout<<Alarm.get_nth_node(2)->Node_Name<<" "<<Alarm.get_nth_node(2)->cpt_maps.size()<<endl;
     //cout<<Alarm.get_nth_node(12)->Node_Name<<" "<<Alarm.get_nth_node(12)->cpt_maps.size()<<endl;

    return 0;
}




