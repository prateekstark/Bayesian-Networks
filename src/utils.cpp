#include "../include/utils.h"
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
