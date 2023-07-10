#include <fstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <cctype>
#include <cstdlib>
#include <algorithm>

using namespace std;

struct Key
{
	char letter;
	char symbol;
	int octave;
	float frequency;
	float wavelength;
};

struct Note
{
	char letter;
	char symbol;
	int octave;
	int duration;
};

struct Key* makeKey(int o, char l, char s, double f, double w) 
	/*
	* function is a pointer because I want to pass this information to other functions but passing structures by value takes up too much space 
	* and is not very practical and passing structures by reference is not practical either. normally the best way to pass info from sturctures
	* other functions is through dynamic allocation along with pointers
	*/ 
{
	struct Key* result = nullptr;	// dynamically allocate here because when using pointers with structures we always use the new operator
	result = new Key;

	result->octave = o;			 
	result->letter = l;
	result->symbol = s;
	result->frequency = f;
	result->wavelength = w;

	/*
	* setting the structures to the variables so that when load key loads in the information into the seperate variables, the load key
	* can call makeKey and input all the information received into the structures
	*/

	return result;
}

struct Note* makeNote(int o, char l, char s, double d)		// same thing is being done here as the makeKey function
{
	struct Note* result = nullptr;
	result = new Note;

	result->octave = o;
	result->letter = l;
	result->symbol = s;
	result->duration = d;

	return result;
}


void showKeyStruct(struct Key myKey)
{
	cout << "[";
	cout << myKey.letter;
	cout << myKey.symbol;
	cout << myKey.octave;
	cout << "] ==> (";
	cout << setw(7) << fixed << setprecision(4);
	cout << myKey.frequency << " * ";
	cout << setw(7) << fixed << setprecision(4);
	cout << myKey.wavelength << " = ";
	cout << myKey.frequency * myKey.wavelength;
	cout << ")" << endl;
}

void showNoteStruct(struct Note myNote)
{
	cout << "(";
	cout << myNote.letter;
	cout << myNote.octave;
	cout << myNote.symbol << ",";
	cout << setw(7) << fixed << setprecision(4) << myNote.duration << ")";
	cout << endl;
}

void showKeyVector(vector<Key> k)
{
	for (int i = 0; i < k.size(); i++)
	{
		showKeyStruct(k[i]);
	}
}

void showTuneVector(vector<Note> n)
{
	for (int i = 0; i < n.size(); i++)
	{
		showNoteStruct(n[i]);
	}
}

int fetchIndex(struct Note n, vector<Key> k)
{
	int result = -1;
	int i = 0;
	for (int i = 0; i < k.size(); i++)
	{
		if (k.at(i).letter == n.letter)					// comparing data fields of two different data structures.
		{												// one structure is a vector of structure
			if (k.at(i).octave == n.octave)
			{
				if (k.at(i).symbol == n.symbol)
				{
					result = i;
				}
			}
		}
	}
	return result; // return the index
}


void loadKey(vector <Key>* keys)
{
	ifstream keyFile("Keys.txt");	// open file 
	string token;

	struct Key* newKey = nullptr;		// create a datatype Key pointer variable 

	char keyLetter = '\0';
	char keySymbol = '\0';
	int octave = 0;
	double freq = 0.0;
	double wavelength = 0.0;

	while (!keyFile.eof())		// while keyfile has not reached the last line 
	{
		getline(keyFile, token, ',');		// read from the keyfile, store everything up until the comma(delimter) in token string
		octave = stoi(token);				// convert token into integer and store it in the int octave variable
		getline(keyFile, token, ',');		// repeat 
		keyLetter = token[0];
		getline(keyFile, token, ',');
		keySymbol = token[0];
		getline(keyFile, token, ',');
		freq = stod(token);
		getline(keyFile, token, '\n');
		wavelength = stod(token);

		newKey = makeKey(octave, keyLetter, keySymbol, freq, wavelength);		
		/*
		* we set the makeKey function to newKey because we want to push everything in the structure to the vector and you can see that 
		* in the next line. 
		* the keys vector pushes everything saved in the newkey key pointer variable (the structure) into a vector
		* when we call the makeKey function we are pushing all the information we read/inputted from the keys.txt into the structure because the
		* makekey function sets the structures equal to the variables.
		*/
		keys->push_back(*newKey);

	}
	keyFile.close();
}

void getFileName(string* ptr)
{
	string fileName;
	cout << "Please enter the filename: ";
	cin >> fileName;
	*ptr = fileName;
}


void loadNote(vector <Note>* tune, string file)
{
	ifstream noteFile(file);
	string token;

	struct Note* newNote = nullptr;
	char noteLetter = '\0';
	char noteSymbol = '\0';
	int octave = 0;
	int duration = 0;

	string noteLetterstr;
	string noteSymbolstr;
	string octavestr;
	string durationstr;

	while (!noteFile.eof())
	{
		getline(noteFile, token);
		cout << token << endl;

		noteLetterstr = token.substr(0, 1);
		noteLetter = noteLetterstr[0];

		octavestr = token.substr(1, 1);
		octave = stoi(octavestr);

		noteSymbolstr = token.substr(2, 1);
		noteSymbol = noteSymbolstr[0];

		durationstr = token.substr(4);
		duration = stoi(durationstr);

		newNote = makeNote(octave, noteLetter, noteSymbol, duration);
		tune->push_back(*newNote);
	}
	noteFile.close();
}

void drawNote(int minOct, int maxOct, int keyIndexVec, int noteDuration)
{
	int start;
	int stop = (maxOct + 1) * 17;

	for (start = minOct * 17; start < keyIndexVec; start++)
	{
		cout << ".";
	}

	if (noteDuration == 125)
	{
		cout << "|";
	}
	else if (noteDuration == 250)
	{
		cout << "+";
	}
	else if (noteDuration == 500)
	{
		cout << "o";
	}
	else if (noteDuration == 750)
	{
		cout << "O";
	}
	else if (noteDuration == 1000)
	{
		cout << "$";
	}
	else
	{
		cout << "-";
	}

	for (int i = keyIndexVec + 1; i < stop; i++)
	{
		cout << ".";
	}
	cout << endl;
}


void drawTune(vector<Key> key, vector<Note> note)
{
	int max = note.size();
	int minOct = -1;
	minOct = note[0].octave;
	int maxOct = -1;
	Note temp;

	for (int i = 1; i < max; i++)			// using loops that don't begin at 0.
	{
		if (note[i].octave < minOct)
		{
			minOct = note[i].octave;
		}
		if (note[i].octave > maxOct)
		{
			maxOct = note[i].octave;
		}
	}

	for (int i = 0; i < max; i++)
	{
		temp = note[i];
		int index = fetchIndex(temp, key);
		int duration = temp.duration;
		drawNote(minOct, maxOct, index, duration);
	}
}


void myName()
{
	cout << "Ermina Ashraf" << endl;
	cout << "CS 1337-502" << endl;
	cout << "Program #3" << endl;
}

void menu()
{
	vector<Key> keyMenu;
	vector<Note> noteMenu;
	string ptr;

	int input = 0;

	while (input != 8)
	{
		cout << "==================================" << endl;
		cout << "	------ Menu ------	" << endl;
		cout << "1. Load Key File" << endl;
		cout << "2. Show Loaded Keys" << endl;
		cout << "3. Set Tune Filename" << endl;
		cout << "4. Load Tune File" << endl;
		cout << "5. Show Tune File" << endl;
		cout << "6. Draw Tune File" << endl;
		cout << "7: Exit" << endl;
		cout << "==================================" << endl;
		cout << "Please select a menu option (1-9): ";
		cin >> input;

		switch (input)
		{
		case 1:
			loadKey(&keyMenu);
			break;
		case 2:
			showKeyVector(keyMenu);
			break;
		case 3:
			getFileName(&ptr);
			cout << ptr << endl;
			cin.clear();
			cin.ignore(1000, '\n');
			break;
		case 4:
			loadNote(&noteMenu, ptr);
			break;
		case 5:
			showTuneVector(noteMenu);
			break;
		case 6:
			drawTune(keyMenu, noteMenu);
			break;
		case 7:
			cout << endl << "You have exited the menu" << endl << "Have a nice day!";
			cout << endl << "-----------------------" << endl;
			break;
		}
	}
}


int main()
{
	myName();
	menu();
}

