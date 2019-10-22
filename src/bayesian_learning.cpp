#include "../include/utils.h"
 // The whole network represted as a list of nodes

int main(int argc, char **argv){
    string inputFileName = argv[1];
    string recordFileName = argv[2];
    cout<<inputFileName<<endl;
    cout<<recordFileName<<endl;
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