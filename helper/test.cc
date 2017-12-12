#include<iostream>
#include<fstream>
#include<string>
#include<typeinfo>

using namespace std;

void print_func(char* argv[])
{
	cout << argv[1] << endl;
}
class TEST {
public:
	int test;
	TEST()
	{
		cout << "Constructed!" << endl;
	}

	void TES_TEST()
	{
		cout << "Hello!" << endl;
	}

	void SET_TEST()
	{
		TES_TEST();
	}

	~TEST()
	{
		cout << "Destroyed!" << endl;
	}
};

int main(int argc, char** argv)
{
	/*
	int a = 42;
	string test = to_string(a);

	string test2 = "hello";

	cout << test+test2 << endl;

	TEST test = TEST();
	test.SET_TEST();

	ofstream myfile;
	ifstream anofile;
	string line;

	myfile.open("test.txt");
	myfile << "Writing to this file\n";
	myfile.close();
	
	anofile.open("test.txt");
	if(anofile.is_open())
	{
		while(getline(anofile,line))
		{
			cout << line << endl;
		}
	}
	*/

	print_func(argv);
	return 0;
}
