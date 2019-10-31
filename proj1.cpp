/* -----------------------------------------------------------------------------

FILE:           proj1.cpp
DESCRIPTION:    Project 1 for Fall 2019 CS 202-1001 at UNR; Reads in names from
					a file, lists the names in different orders
COMPILER:       MinGW C++ compiler
NOTES:          

MODIFICATION HISTORY:
Author				Date			Version
---------------		----------		--------------
Version 1			YYYY-MM-DD		x.x Description
Nick Learner		2019-09-01		1.0	Set-up + func prototypes
Nick Learner		2019-09-02		1.1	Made functions up thru SelectionSort()
Nick Learner		2019-09-03		1.2	Made remaining functions
Nick Learner		2019-09-04		1.3	Added parallel arrays for the order 
----------------------------------------------------------------------------- */

#include <iostream>
#include <fstream>

using namespace std;

// Global variables
const int NAME_LENGTH = 9;
const int NUM_OF_NAMES = 10;

// Function prototypes
void CStringArrayInitializer(char[][NAME_LENGTH]);
void UserInputFileName(char[], char[], int);
bool OpenInputFile(char[], fstream&);
void FileInput(char[][NAME_LENGTH], int[], fstream&);
void PrintNames(char[][NAME_LENGTH], int[], ostream&);
void LengthSort(const char[][NAME_LENGTH], char[][NAME_LENGTH], int[]);
void CopyCharArrays(const char[][NAME_LENGTH], char[][NAME_LENGTH]);
int LengthOfCString(const char[]);
void SelectionSort(int[], char[][NAME_LENGTH], int[]);
void AlphaBubbleSort(const char [][NAME_LENGTH], char [][NAME_LENGTH], int []);
bool CompareCStrings(const char[], const char[]);
bool OpenOutputFile(char[], fstream&);

int main()
{
	char inputfilename[32] = {' '};
	char outputfilename1[32] = {' '};
	char outputfilename2[32] = {' '};
	char inputfiletype[32] = "Input";
	char outputfiletype[32] = "output";
	fstream inputfile;
	fstream outputfile1;
	fstream outputfile2;
	char names[NUM_OF_NAMES][NAME_LENGTH];
	char lengthsortednames[NUM_OF_NAMES][NAME_LENGTH];
	char alphasortednames[NUM_OF_NAMES][NAME_LENGTH];
	int order[NUM_OF_NAMES];
	
	CStringArrayInitializer(names);
	CStringArrayInitializer(lengthsortednames);
	CStringArrayInitializer(alphasortednames);
	
	UserInputFileName(inputfiletype, inputfilename, 32);
	cout << "Length-sorted ";
	UserInputFileName(outputfiletype, outputfilename1, 32);
	cout << "Alphabetically-sorted ";
	UserInputFileName(outputfiletype, outputfilename2, 32);
	
	if (OpenInputFile(inputfilename, inputfile))
	{
		cout << "File " << inputfilename << " has been opened." << "\n";
		FileInput(names, order, inputfile);
		inputfile.close();
		cout << "File " << inputfilename << " has been closed." << "\n\n";
		cout << "Names in file " << inputfilename << " and their order:\n";
		PrintNames(names, order, cout);
		LengthSort(names, lengthsortednames, order);
		cout << endl;
		cout << "Names sorted by length with original input order:\n";
		PrintNames(lengthsortednames, order, cout);
		if (OpenOutputFile(outputfilename1, outputfile1))
		{
			PrintNames(lengthsortednames, order, outputfile1);
			outputfile1.close();
		}else;
		cout << endl;
		cout << "Names sorted alphabetically with original input order:\n";
		AlphaBubbleSort(lengthsortednames, alphasortednames, order);
		PrintNames(alphasortednames, order, cout);
		if (OpenOutputFile(outputfilename2, outputfile2))
		{
			PrintNames(alphasortednames, order, outputfile2);
			outputfile2.close();
		}else;
		cout << endl;
	}else;
	
	return 0;
}


/* -----------------------------------------------------------------------------
FUNCTION:          CStringArrayInitializer()
DESCRIPTION:       Initializes a 2d char array
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------*/

void CStringArrayInitializer(char array[][NAME_LENGTH])
{
	for (int i = 0; i < NUM_OF_NAMES; i++)
	{
		for (int j = 0; j < NAME_LENGTH-1; j++)
		{
			array[i][j] = ' ';
		}
		array[i][NAME_LENGTH-1] = '\0';
	}
}

/* -----------------------------------------------------------------------------
FUNCTION:          UserInputFileName()
DESCRIPTION:       Gets a filename from user
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------*/

void UserInputFileName(char type[], char filename[], int length)
{
	cout << type << " filename:\n";
	
	cin.getline(filename, length);
	
	// For-loop to switch every space with an underscore
	for (int i = 0; i < length; i++)
	{
		if (filename[i] == ' ')
		{
			filename[i] = '_';
		}else;
	}
	
	cout << endl;
}


/* -----------------------------------------------------------------------------
FUNCTION:          OpenInputFile()
DESCRIPTION:       Opens a file from which the names are inputted
RETURNS:           bool (true if file is opened, false if file is not found)
NOTES:             
------------------------------------------------------------------------------*/

bool OpenInputFile(char filename[], fstream& inputfile)
{
	int count = 0;
	
	inputfile.open(filename, ios::in);
	if (inputfile.fail())
	{
		cout << "File " << filename << " failed to open." << "\n\n";
		return false;
	}
	else
		return true;
}


/* -----------------------------------------------------------------------------
FUNCTION:          FileInput()
DESCRIPTION:       Inputs the names into a 2d char array
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------*/

void FileInput(char names[][NAME_LENGTH], int order[], fstream& inputfile)
{
	for (int i = 0; i < NUM_OF_NAMES; i++)
	{
		order[i] = i;
		
		for (int j = 0; j < NAME_LENGTH; j++)
		{
			inputfile >> names[i][j];
			if (names[i][j] >= 65 && names[i][j] <= 90 && j > 0)
			{
				names[i+1][0] = names[i][j];
				names[i][j] = ' ';
				i++;
				order[i] = i;
				j = 0;
			}else;
		}
	}
}


/* -----------------------------------------------------------------------------
FUNCTION:          PrintNames()
DESCRIPTION:       Prints the list of names with their order as given
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------*/

void PrintNames(char names[][NAME_LENGTH], int order[], ostream& out)
{
	for (int i = 0; i < NUM_OF_NAMES; i++)
	{
		out << order[i] << ' ';
		
		for (int j = 0; j < NAME_LENGTH; j++)
		{
			out << names[i][j];
		}
		out << '\n';
	}
}


/* -----------------------------------------------------------------------------
FUNCTION:          LengthSort()
DESCRIPTION:       Sorts by length a given list of names into a different array
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------*/

void LengthSort(const char names[][NAME_LENGTH],
					char sortednames[][NAME_LENGTH], int order[])
{
	char tempname[NAME_LENGTH];
	int lengths[NUM_OF_NAMES];
	
	for (int i = 0; i < NUM_OF_NAMES; i++)
	{
		for (int j = 0; j < NAME_LENGTH; j++)
		{
			tempname[j] = names[i][j];
		}
		
		lengths[i] = LengthOfCString(tempname);
	}
	
	CopyCharArrays(names, sortednames);
	
	SelectionSort(lengths, sortednames, order);
}


/* -----------------------------------------------------------------------------
FUNCTION:          CopyCharArrays()
DESCRIPTION:       Copies the first char array to the second char array
RETURNS:           void
NOTES:             
------------------------------------------------------------------------------*/

void CopyCharArrays(const char names1[][NAME_LENGTH], char names2[][NAME_LENGTH])
{
	for (int i = 0; i < NUM_OF_NAMES; i++)
	{
		for (int j = 0; j < NAME_LENGTH; j++)
		{
			names2[i][j] = names1[i][j];
		}
	}
}


/* -----------------------------------------------------------------------------
FUNCTION:          LengthOfCString()
DESCRIPTION:       Measures the length of a given char array
RETURNS:           int
NOTES:             
------------------------------------------------------------------------------*/

int LengthOfCString(const char name[])
{
	int length = 0;
	
	for (int i = 0; i < NAME_LENGTH; i++)
	{
		if (name[i] != ' ' && name[i] != '\0')
		{
			length++;
		}else
		{
			return length;
		}
	}
	
	return length;
}


/* -----------------------------------------------------------------------------
FUNCTION:          SelectionSort()
DESCRIPTION:       Sorts an array of numbers and names from smallest to largest
RETURNS:		   void
NOTES:             Original function from the Starting Out With C++: From Control
					Structures through Objects textbook by Tony Gaddis (8th Ed.)
------------------------------------------------------------------------------- */

void SelectionSort(int lengths[], char sortednames[][NAME_LENGTH], int order[])
{
	int startscan, minindex, minvalue, temporder;
	char tempname[NAME_LENGTH];
	
	for (startscan = 0; startscan < (NUM_OF_NAMES-1); startscan++)
	{
		minindex = startscan;
		minvalue = lengths[startscan];
		
		for (int index = startscan + 1; index < NUM_OF_NAMES; index++)
		{
			if (lengths[index] < minvalue)
			{
				minvalue = lengths[index];
				minindex = index;
			}else;
		}
		
		lengths[minindex] = lengths[startscan];
		lengths[startscan] = minvalue;
		
		temporder = order[minindex];
		order[minindex] = order[startscan];
		order[startscan] = temporder;
		
		for (int i = 0; i < NAME_LENGTH; i++)
		{
			tempname[i] = sortednames[minindex][i];
			sortednames[minindex][i] = sortednames[startscan][i];
			sortednames[startscan][i] = tempname[i];
		}
	}
}


/* -----------------------------------------------------------------------------
FUNCTION:          AlphaBubbleSort()
DESCRIPTION:       Sorts an array of names alphabetically
RETURNS:		   void
NOTES:             Original function is also from the above mentioned textbook,
					and is similarly edited to include the names and order
------------------------------------------------------------------------------- */

void AlphaBubbleSort(const char names[][NAME_LENGTH],
						char sortednames[][NAME_LENGTH], int order[])
{
	bool swap;
	char tempname1[NAME_LENGTH], tempname2[NAME_LENGTH];
	int temporder;
	
	CopyCharArrays(names, sortednames);
	
	do
	{
		swap = false;
		for (int i = 0; i < (NUM_OF_NAMES-1); i++)
		{
			for (int j = 0; j < NAME_LENGTH; j++)
			{
				tempname1[j] = sortednames[i][j];
				tempname2[j] = sortednames[i+1][j];
			}
			
			if (CompareCStrings(tempname1, tempname2))
			{
				temporder = order[i];
				order[i] = order[i+1];
				order[i+1] = temporder;
				
				for (int j = 0; j < NAME_LENGTH; j++)
				{
					tempname1[j] = sortednames[i][j];
					sortednames[i][j] = sortednames[i+1][j];
					sortednames[i+1][j] = tempname1[j];
				}
				
				swap = true;
			}
		}
	}
	while (swap);
}


/* -----------------------------------------------------------------------------
FUNCTION:          CompareCStrings()
DESCRIPTION:       Compares two names to check which goes first alphabetically
RETURNS:		   bool
NOTES:             
------------------------------------------------------------------------------- */

bool CompareCStrings(const char firstname[], const char nextname[])
{
	for (int i = 0; i < (NAME_LENGTH-2); i++)
	{
		if (firstname[i] < nextname[i])
		{
		return false;	// If the names are already sorted, return false
		}else if (firstname[i] > nextname[i])
		{
			return true;// If the names need to be switched, return true
		}else;
	}
	
	return false;		// If the names are the same, return false
}



/* -----------------------------------------------------------------------------
FUNCTION:          OpenOutputFile()
DESCRIPTION:       Opens a file to output data into
RETURNS:           bool
NOTES:             
------------------------------------------------------------------------------- */
 
bool OpenOutputFile(char filename[], fstream& outputfile)
{
	outputfile.open(filename, ios::out);
	if (outputfile.fail())
	{
		cout << "File " << filename << " failed to open." << "\n\n";
		return false;
	}
	else
		return true;
}

