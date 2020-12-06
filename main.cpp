#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
using namespace std;

class page{
	public:
		int links;
		vector<string> linkpages;
		string words;
		double PR;
		page ();
};

void calPR(page (&pages)[501], double d ,double DIFF);

int main(int argc, char const *argv[])
{
	double d = stod(argv[1]);
	double DIFF = stod(argv[2]);
	page pages[501];
	string line,temp;
	bool tmp = false;
	int link_counter = 0;
	for (int i = 0; i < 501; ++i){
		temp = "web-search-files2/page"+to_string(i);
		ifstream p(temp);
		while (getline (p, line)){
			if (tmp == false){
				if (line == "---------------------"){
					tmp = true;
					pages[i].links = link_counter;
					link_counter = 0;
				}else{
					pages[i].linkpages.push_back(line);
					link_counter++;
				}
			}else{
				if (line != "---------------------"){
					pages[i].words = line;
				}
			}
		}
		p.close();
		tmp = false;	
	}

	calPR(pages,d,DIFF);
	double temppp = 0;
	for (int i = 0; i < 501; ++i){
		temppp += pages[i].PR;
		cout << pages[i].PR<<endl;
	}
	cout << temppp;
	string a;
	ofstream file;
	file.open("output/PageRankList", ios::out | ios::trunc);
	for (int i = 0; i < 501; ++i)
	{
		a = "page"+to_string(i);
		file << a << " "<< to_string(pages[i].links) << " "<< to_string(pages[i].PR)<<endl;
	}
	return 0;
}

page::page(){
	PR = 0;
}

void calPR(page (&pages)[501], double d ,double DIFF){
	for (int i = 0; i < 501; ++i){
		pages[i].PR = 1/501;
	}
	double diff = 0;
	double PRbefore;
	double temp = 0;
	do{
		diff = 0;
		for (int i = 0; i < 501; ++i){
			PRbefore = pages[i].PR;
			for (int j = 0; j < 501; ++j){
				vector<string>::iterator it = find(pages[j].linkpages.begin(), pages[j].linkpages.end(),"page"+to_string(i));
				if (it != pages[j].linkpages.end()){
			        temp += pages[j].PR/pages[j].links;
				}
			}
			pages[i].PR = (1-d)/501 + d*(temp);
			diff = diff + abs(PRbefore - pages[i].PR);
			temp = 0;
		}
	}while(diff >= DIFF);
	return;
}