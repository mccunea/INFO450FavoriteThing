#include "stdafx.h"
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

const int READERROR = 100;
const int WRITEERROR = 200;
const int ARRAYSIZE = 100;

class Beer
{
	string beerCompany;
	string beerName;
	double abv;
	int	calories;
public:
	Beer();
	Beer(string com, string name, double ab, int cal);
	void CaptureBeerItem();
	void ShowBeerItem();
	int SaveBeerItem(ofstream &outfile);

	friend bool operator== (const Beer& b, const Beer& ba);

	
};

class BeerList
{
	Beer **list;
	int numrecords;
	int listsize;
	int ReallocateArray();
public:
	BeerList();
	~BeerList();
	void GetUserInput();
	void ShowBeerList();
	int SaveBeerList(string filename);
	int ReadBeerList(string filename);

	
};

bool operator== (const Beer& b, const Beer& ba) 
{
	if (b.beerCompany == ba.beerCompany && b.beerName == ba.beerName && b.abv == ba.abv && b.calories == ba.calories)
	{
		return 1;
	}
	else 
		return 0;
}

// default constructor - initialize empty
Beer::Beer()
{
	beerCompany = ""; beerName = ""; abv = 0.0;  calories = 0;
}
// overload constructor inialize w/ values
Beer::Beer(string com, string name, double ab, int cal)
{
	beerCompany = com;
	beerName = name;
	abv = ab;
	calories = cal;
}
// Capture Item from end users
void Beer::CaptureBeerItem()
{
	cout << "Beer Company? -->";
	getline(cin, beerCompany);
	cout << "Beer Name?  -->";
	getline(cin, beerName);
	cout << "ABV? -->";
	cin >> abv;
	cout << "Calories? -->";
	cin >> calories;
	cin.ignore();
	cin.clear();
	
}
// Display item to console
void Beer::ShowBeerItem()
{
	cout << "Company : " << beerCompany << " Name : " << beerName <<" ABV : " << abv << " Calories : " << calories << endl;
}
// Save Beer item to file given input file stream
int Beer::SaveBeerItem(ofstream& outfile)
{
	if (outfile.is_open())
	{
		outfile << beerCompany << "|" << beerName << "|" << abv << "|" << calories << endl;
		return 0;
	}
	else
		return WRITEERROR;
}



// Beer LIst constructor -allocate default space for array
BeerList::BeerList()
{
	list = new Beer*[ARRAYSIZE];
	numrecords = 0;
	listsize = ARRAYSIZE;
}
// deconstructor - free allocated memory
BeerList::~BeerList()
{
	for (int i = 0; i < numrecords; i++)
	{
		delete list[i];
	}
	delete[]list;
}
// Reallocate memory if already at array capacity
int BeerList::ReallocateArray()
{
	Beer **temp;
	temp = new Beer*[listsize + ARRAYSIZE];
	listsize = listsize + ARRAYSIZE;
	for (int i = 0; i < numrecords; i++)
	{
		temp[i] = list[i];
	}
	delete[]list;
	list = temp;
	return 0;
}

// get user input for the list
void BeerList::GetUserInput()
{
	bool duplicate = false;
	string answer = "Y";
	cout << "enter beer Y/N?" << endl;
	getline(cin, answer);
	while (answer == "Y" || answer == "y")
	{
		list[numrecords] = new Beer();
		list[numrecords]->CaptureBeerItem();
		if (numrecords >= 1)
		{
			for (int i = 0; i < numrecords; i++)
			{
				if (*list[numrecords] == *list[i])
				{
					duplicate = true;
				}
				i++;
			}

			if (duplicate == true)
			{
				cout << "Duplicate value will not be entered!";
				delete list[numrecords];
				//cout << "enter another beer Y/N?" << endl;
				//getline(cin, answer);
				duplicate = false;
			}
			else { numrecords++; }
		}
		
		
			cout << "enter another beer Y/N?" << endl;
			getline(cin, answer);
		

	}
}

// Show list to the console
void BeerList::ShowBeerList()
{
	for (int i = 0; i < numrecords; i++)
		list[i]->ShowBeerItem();
}

// Save entire list to file, given full-path file name
int BeerList::SaveBeerList(string filename)
{
	ofstream output(filename, ios::trunc);
	if (output)
	{
		for (int i = 0; i < numrecords; i++)
		{
			list[i]->SaveBeerItem(output);
		}
		output.close();
	}
	else
	{
		return WRITEERROR;
	}
	return 0;
}
// Read Beer list from a file, given full-path filename
// If file not found or error, return error code
int BeerList::ReadBeerList(string filename)
{
	string icom, iname, iabv, ica;
	ifstream infile(filename, ios::in);
	if (!infile)
	{
		cout << "File could not be opened for reading" << endl;
		return READERROR;
	}


	while (!infile.eof())
	{
		if (numrecords == listsize)
		{
			ReallocateArray();
		}
		getline(infile, icom, '|');
		if (!icom.empty())
		{
			getline(infile, iname, '|');
			getline(infile, iabv,'|');
			double ab = stoi(iabv);
			getline(infile, ica);
			int ca = stoi(ica);
			list[numrecords] = new Beer(icom, iname, ab, ca);
			numrecords++;
		}
	}
	
	infile.close();
	return 0;
}


int main()
{
	string filename = "c:\\users\\lob8463\\documents\\Beer4.txt";
	BeerList myBeer;
	string answer;
	int error;
	error = myBeer.ReadBeerList(filename);
	if (error)
	{
		cout << "Cannot read Beer - continue creating new list? Y/N -->";
		getline(cin, answer);
		if (answer != "Y" && answer != "y")
		{
			return 1;
		}
	}
	myBeer.GetUserInput();
	myBeer.SaveBeerList(filename);
	myBeer.ShowBeerList();

	return 0;
}

