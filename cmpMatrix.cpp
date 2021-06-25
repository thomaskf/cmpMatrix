#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <cmath>
#include <iomanip>

#define VERSION "1.0"

using namespace std;

long index(long i, long j, long n) {
	long m = 0;

	if (i > j)
		return index(j, i, n);

	// before i
	if (i>0)
		m += i*(n-1) - (i-1)*i/2;

	// on i
	m += (j-i-1);

	return m;
}

void tokenizer(string seq, string separators, vector<string>* result) {
    // split the seq into many parts by "separators"
    // the vector<string> *result cannot be NULL
    result->clear();
    int startpos = (int) seq.find_first_not_of(separators);
    while (startpos != (int) string::npos) {
        int endpos = (int) seq.find_first_of(separators, startpos);
        if (endpos != (int) string::npos) {
            result->push_back(seq.substr(startpos, endpos-startpos));
            startpos = (int) seq.find_first_not_of(separators, endpos);
        } else {
            result->push_back(seq.substr(startpos));
            break;
        }
    }
}

// return the square of the difference between two numbers
double square_diff(double n1, double n2) {
	double diff = n1 - n2;
	return diff*diff;
}

int main(int argc, char** argv) {

	cout << "Version " << VERSION << endl;

	if (argc < 3) {
		cout << "To compute the root-mean-square difference between two symmetric distance matrices:" << endl;
		cout << "Syntax: " << argv[0] << " [dist matrix 1] [dist matrix 2]" << endl << endl;
		cout << "To reorder and output the first distance matrix as the order in the second distrance matrix:" << endl;
		cout << "Syntax: " << argv[0] << " [dist matrix 1] [dist matrix 2] -reorder > [out file]" << endl << endl;
		cout << "Note: all distance matrices have to be in phylip format" << endl;
		exit(1);
	}

	ifstream fin1, fin2, fin3;
	string aline;
	long dim = 0;
	long dim2 = 0;
	long arraySize;
	double* disMat;
	map<string,long> str2idx;
	map<string,long>::iterator itr;
	pair<map<string,long>::iterator,bool> ret;
	vector<string> token;
	long lineno;
	long i,j,i_t,j_t;
	// the array of names inside the second distrance matrix 
	vector<string> names;
	vector<long> cidx;
	double rms;
	long nVal;
	// reorder option is inputted
	bool toReOrder = false;

	if (argc >= 4 && strcmp(argv[3], "-reorder")==0) {
		toReOrder = true;
	}

	// read the first line inside the first distance-matrix file
	fin1.open(argv[1]);
	if (getline(fin1, aline)) {
		if (aline.length() > 0) {
			dim = atoi(aline.c_str());
		}
	}
	
	if (dim == 0) {
		fin1.close();
		cerr << "Error! The first number in the first file is not the dimension or the input dimension is zero" << endl;
		exit(1);
	}

	// allocate memory to the array
	arraySize = dim * (dim-1) / 2;
	disMat = new double[arraySize];

	// read the content of the first distance-matrix file
	lineno=0;
	i=0;
	while (lineno < dim && getline(fin1,aline)) {
		lineno++;
		tokenizer(aline, " ", &token);
		if (token.size() != dim+1) {
			cerr << "The number of columns in the line " << lineno+1 << " inside the first file is not correct" << endl;
			exit(1); 
		}
		// first item is the name
		ret = str2idx.insert(pair<string,long>(token[0], (long)str2idx.size()));
		if (ret.second == false) {
			cerr << "Duplicate name appears: " << token[0] << " inside the first file" << endl;
			exit(1);
		}
		// the other items are the distances
		// skip the first lineno items
		for (j=lineno+1; j<token.size(); j++) {
			disMat[i++] = atof(token[j].c_str());
		}
	}
	fin1.close();

	// read the first line inside the second distance-matrix file
	fin2.open(argv[2]);
	if (getline(fin2, aline)) {
		if (aline.length() > 0 ) {
			dim2 = atoi(aline.c_str());
		}
	}
	
	if (dim2 != dim) {
		fin2.close();
		cerr << "Error! the first number (which is dimension) inside the first file does not match with that in the second file" << endl;
		exit(1);
	}

	// read the sequence names
	lineno=0;
	while (lineno < dim && getline(fin2,aline)) {
		lineno++;
		tokenizer(aline, " ", &token);
		if (token.size() != dim+1) {
			cerr << "The number of columns in the line " << lineno+1 << " inside the second file is not correct" << endl;
			exit(1); 
		}
		names.push_back(token[0]);
	}
	fin2.close();

	// get the corresponding index of the names inside the second file
	for (i=0; i<dim; i++) {
		itr = str2idx.find(names[i]);
		if (itr == str2idx.end()) {
			cerr << "The name in the line " << i+2 << " inside the second file does not exist in the first file" << endl;
			exit(1); 
		}
		cidx.push_back(itr->second);
	}

	if (toReOrder) {
		// output the first distance-matrix file according to the order in the second file
		cout << dim << endl;
		for (i=0; i<dim; i++) {
			i_t = cidx[i];
			cout << names[i];
			for (j=0; j<dim; j++) {
				if (j == i) {
					cout << " " << 0;
				} else {
					j_t = cidx[j];
					cout << " " << disMat[index(i_t,j_t,dim)];
				}
			}
			cout << endl;
		}
		return 0;
	}

	// read the content of the second distance-matrix file
	fin3.open(argv[2]);
	getline(fin3,aline); // skip the first line
        lineno=0;
	rms=0.0;
	nVal=0;
        while (lineno < dim && getline(fin3,aline)) {
		i_t = cidx[lineno];
		lineno++;
                tokenizer(aline, " ", &token);
                // skip the first lineno items
                for (j=lineno+1; j<token.size(); j++) {
			j_t = cidx[j-1];
                        rms += square_diff( disMat[index(i_t,j_t,dim)]  , atof(token[j].c_str()) );
			nVal++;
                }
        }
        fin3.close();
	
	// output the root-mean-square value
	rms = sqrt( rms / (double)nVal );
	cout <<  setprecision(7) << "The root-mean-square difference is: " << rms << endl;
}
