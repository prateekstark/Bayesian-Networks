#include "network.h"
bool isDataPointComplete(vector<string> dataPoint);
int findMissingFeature(vector<string> dataPoint);
vector<string> replaceQuestionMark(string str, vector<string> dataPoint);
vector<string> replaceQuestionMarkIndex(string str, vector<string> dataPoint, int index);
void write_network(string inputFileName, network Alarm);
network read_network(string inputFileName, map<string, vector<string> > &valueMap, map<string, int> &valueNumMap);