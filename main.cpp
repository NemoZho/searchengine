#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <map>
#include <sstream>
#include <iomanip>
using namespace std;

class page{
	public:
		int links;
		vector<string> linkpages;
		vector<string> Words;
		string words;
		double PR;
		page ();
		void fillWords();
};

void calPR(page (&pages)[501], double d ,double DIFF);
int _rank(double (&PRs)[501],double index);
vector<string> split(const string& str, const string& delim);

int main(int argc, char const *argv[])
{
	double d = stod(argv[1]);
	double DIFF = stod(argv[2]);
	page pages[501];
	double PRs[501];
	int RANK[501];
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

	for (int i = 0; i < 501; ++i){
		PRs[i] = pages[i].PR;
	}
	sort(PRs,PRs+501);
	for (int i = 0; i < 501; ++i){
		RANK[i] = _rank(PRs,pages[i].PR);
	}

	string a,ssDIFF;
	stringstream ssd;
	ofstream file;
	ssd << fixed << std::setprecision(0) << d*100;
	if (DIFF == 0.1){
		ssDIFF = "100";
	}
	if (DIFF == 0.01){
		ssDIFF = "010";
	}
	if (DIFF == 0.001){
		ssDIFF = "001";
	}
	file.open("output/pr_"+ssd.str()+"_"+ssDIFF+".txt", ios::out | ios::trunc);

	for (int i = 500; i >= 0; --i)
	{
		//cout << (find(RANK,RANK+501,i)-RANK) << endl;
		a = "page"+to_string(find(RANK,RANK+501,i)-RANK);
		file << a << " "<< to_string(pages[find(RANK,RANK+501,i)-RANK].links) << " "<< to_string(pages[find(RANK,RANK+501,i)-RANK].PR)<<endl;
	}
	file.close();

	for (int i = 0; i < 500; ++i){
		pages[i].fillWords();
	}

	map<string, vector<string> > _WORD;
	for (int i = 0; i < 500; ++i)
	{
		for (int j = 0; j < 20; ++j)
		{
			//cout << pages[i].Words[j]<<endl; 
			_WORD[pages[i].Words[j]].push_back("page"+to_string(i));
		}
	}
	map<string, vector<string> >::iterator iter;
	vector<string>::iterator iter_word;

	file.open("output/reverseindex.txt", ios::out | ios::trunc);
	for(iter = _WORD.begin(); iter != _WORD.end(); iter++){
		file << iter->first << " ";
		for (int i = 0; i < iter->second.size(); i++) {
       		file << iter->second.at(i) << ' ';
    	}
    	file << endl;
	}
	file.close();




	string keywords;
	vector<string> v;
	bool flag = false;
	while(keywords != "*end*"){	
		cout << "Enter Word: ";
		while(cin >> keywords){
			v.push_back(keywords);
			char ch = getchar();
	    	if(ch == '\n'){
	    		break;
	    	}
		}
		if (v.size() == 2){
			flag = true;
		}

		if (flag){
			cout << "AND(" << v[0] << " " << v[1] <<") ";
			if(_WORD.find(v[0]) != _WORD.end() && _WORD.find(v[1]) != _WORD.end()){
				for (int i = 0; i < _WORD[v[0]].size(); ++i){
					if(find(_WORD[v[0]].begin(), _WORD[v[0]].end(), _WORD[v[1]][i]) != _WORD[v[0]].end()) {
					    cout << _WORD[v[1]][i] << " ";
					}
				}
			}else{
				cout << "none";
			}
			cout << endl;
			cout << "OR(" << v[0] << " " << v[1] <<") "; 
			if (_WORD.find(v[0]) != _WORD.end() ){
				if (_WORD.find(v[1]) != _WORD.end() ){
					for (int i = 0; i < 501; ++i)
					{
						if ((find(_WORD[v[0]].begin(), _WORD[v[0]].end(), "page"+to_string(i)) != _WORD[v[0]].end()) || (find(_WORD[v[1]].begin(), _WORD[v[1]].end(), "page"+to_string(i)) != _WORD[v[1]].end())){
							cout << "page"+to_string(i) << " ";
						}
					}
				}else{
					for (int i = 0; i < 501; ++i)
					{
						if ((find(_WORD[v[0]].begin(), _WORD[v[0]].end(), "page"+to_string(i)) != _WORD[v[0]].end())){
							cout << "page"+to_string(i) << " ";
						}
					}
				}
			}else{
				if (_WORD.find(v[1]) != _WORD.end() ){
					string tty = _WORD[v[1]][0];
					for (int i = 0; i < 501; ++i)
					{
						if ((find(_WORD[v[1]].begin(), _WORD[v[1]].end(), "page"+to_string(i)) != _WORD[v[1]].end())){
							cout << "page"+to_string(i) << " ";
						}
					}
				}else{
					cout << "none";
				}
			}
					
		}
		else{
			if (_WORD.find(v[0]) != _WORD.end() ){
				for (int i = 0; i < _WORD[v[0]].size(); ++i)
				{
					cout << _WORD[v[0]][i] << " ";
				}
			}
		}
		cout << endl;
		keywords.clear();
		v.clear();
	}

	int tempcount =0;
	vector<string> _keywords_,result;
	ifstream q("list.txt");
	while (getline (q, line)){
		_keywords_.push_back(line);
	}
	q.close();
	file.open("output/result_"+ssd.str()+"_"+ssDIFF+".txt", ios::out | ios::trunc);
	for (int k = 0; k < _keywords_.size(); ++k)
	{
		v = split(_keywords_[k]," ");
		if (v.size() == 2){
			file << "AND(" << v[0] << " " << v[1] <<") ";
			if(_WORD.find(v[0]) != _WORD.end() && _WORD.find(v[1]) != _WORD.end()){
				for (int i = 0; i < _WORD[v[0]].size(); ++i){
					if(find(_WORD[v[0]].begin(), _WORD[v[0]].end(), _WORD[v[1]][i]) != _WORD[v[0]].end()) {
					    file << _WORD[v[1]][i] << " ";
					}
				}
			}else{
				file << "none";
			}
			file << endl;
			file << "OR(" << v[0] << " " << v[1] <<") "; 
			if (_WORD.find(v[0]) != _WORD.end() ){
				if (_WORD.find(v[1]) != _WORD.end() ){
					for (int i = 0; i < 501; ++i)
					{
						if ((find(_WORD[v[0]].begin(), _WORD[v[0]].end(), "page"+to_string(i)) != _WORD[v[0]].end()) || (find(_WORD[v[1]].begin(), _WORD[v[1]].end(), "page"+to_string(i)) != _WORD[v[1]].end())){
							result.push_back("page"+to_string(i));
							//file << "page"+to_string(i) << " ";

						}
					}
					for (int i = 500; i >= 0; --i){
						if (find(result.begin(),result.end(),"page"+to_string(find(RANK,RANK+501,i)-RANK)) != result.end())
						{
							file << "page"+to_string(find(RANK,RANK+501,i)-RANK) << " ";
							tempcount++;
						}
					}
					result.clear();
					tempcount = 0;
				}else{
					for (int i = 0; i < 501; ++i)
					{
						if ((find(_WORD[v[0]].begin(), _WORD[v[0]].end(), "page"+to_string(i)) != _WORD[v[0]].end())){
							//file << "page"+to_string(i) << " ";
							result.push_back("page"+to_string(i));
						}
					}
					for (int i = 500; i >= 0; --i){
						if (find(result.begin(),result.end(),"page"+to_string(find(RANK,RANK+501,i)-RANK)) != result.end())
						{
							if (tempcount != 10)
							{
								file << "page"+to_string(find(RANK,RANK+501,i)-RANK) << " ";
								tempcount++;
							}
						}
					}
					result.clear();
					tempcount = 0;
				}
			}else{
				if (_WORD.find(v[1]) != _WORD.end() ){
					
					for (int i = 0; i < 501; ++i)
					{
						if ((find(_WORD[v[1]].begin(), _WORD[v[1]].end(), "page"+to_string(i)) != _WORD[v[1]].end())){
							//file << "page"+to_string(i) << " ";
							result.push_back("page"+to_string(i));
						}
					}
					for (int i = 500; i >= 0; --i){
						if (find(result.begin(),result.end(),"page"+to_string(find(RANK,RANK+501,i)-RANK)) != result.end() )
						{
							if (tempcount != 10)
							{
								file << "page"+to_string(find(RANK,RANK+501,i)-RANK) << " ";
								tempcount++;
							}
						}
					}
					result.clear();
					tempcount = 0;
				}else{
					file << "none";
				}
			}
			file << endl;		
		}
		else{
			if (_WORD.find(v[0]) != _WORD.end() ){
				for (int i = 0; i < 501; ++i)
				{
					if ((find(_WORD[v[0]].begin(), _WORD[v[0]].end(), "page"+to_string(i)) != _WORD[v[0]].end())){
						//file << "page"+to_string(i) << " ";
						result.push_back("page"+to_string(i));
					}
				}
				for (int i = 500; i >= 0; --i){
					if (find(result.begin(),result.end(),"page"+to_string(find(RANK,RANK+501,i)-RANK)) != result.end() )
					{
						if (tempcount != 10)
						{
							file << "page"+to_string(find(RANK,RANK+501,i)-RANK) << " ";
							tempcount++;
						}
					}
				}
				result.clear();
				tempcount = 0;
			}else{
				file << "none";
			}
			file << endl;
		}
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

int _rank(double (&PRs)[501],double index){
	for (int i = 0; i < 501; ++i)
	{
		if (PRs[i] == index){
			return i;
		}
	}
	return 0;
}

void page::fillWords(){
	int counter = 0;
	string temp;
	int wordcount = 0;
	this->Words = split(this->words," ");
	
	return;
}

vector<string> split(const string& str, const string& delim) {  
	vector<string> res;  
	if("" == str) return res;   
	char * strs = new char[str.length() + 1] ;
	strcpy(strs, str.c_str());   

	char * d = new char[delim.length() + 1];  
	strcpy(d, delim.c_str());  

	char *p = strtok(strs, d);  
	while(p) {  
		string s = p;
		res.push_back(s);
		p = strtok(NULL, d);  
	}  

	return res;  
} 