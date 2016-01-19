#include "entry.h"
#include "jourminal.h"
#include "user.h"
#include <cstdio>
#include <ncurses.h>
#include <stdlib.h>

vector<Entry*> Entry::entries;
int Entry::lastId;
Entry* Entry::activeEntry;
bool Entry::creating = false;

using namespace std;

void Entry::OpenFile (string filepath) {
    
    int key; //character we will store into from the keyboard
    int row = 1;
    int col = 0;
  
    initscr(); //initialize the screen

    noecho();
    nodelay(stdscr,FALSE);
    keypad(stdscr,TRUE);


    clear();
    refresh();
    int open = scr_restore(filepath.c_str());
    refresh();


    while ((key = getch()) != '~'){


    if (key == KEY_DOWN){

    move(++row, col);

    } else if (key == KEY_UP){
    
    if (row > 0){ 
    move(--row, col);
    }

    } else if (key == KEY_LEFT){

    if (col > 0){
    move(row, --col);
    }

    } else if (key == KEY_RIGHT){

    if(col < COLS){
    move(row, ++col);
    }

    } else if (key == 127){ //backspace

    if(col > 0){
    col--;
    } else{
    if (row > 0){
    col = COLS;
    row--;
    }
    }
    move(row, col);
    delch();

    } else if (key == '\n'){ //enter
    row++;
    col = 0;
    move(row, col);
    } else if (key == '$'){ //shift alt S (save)

    int save = scr_dump(filepath.c_str());
      endwin();
      return;

    } else{

    col++;
    addch(key);

    }

    move(0, COLS - 8);
    clrtoeol(); // clear line
    printw("%d , %d", row, col);
    move(row, col);
    refresh();

    }

    endwin();
}

Entry::Entry (int _id, int _jourminalId, string _title, 
              int _authorId, string _filename, time_t _dateCreated) {
    User* u = User::FindUserById(_authorId);
    this->id = _id;
    this->title = _title;
    this->author = u;
    this->filename = _filename;
    this->dateCreated = _dateCreated;
    this->jourminal = Jourminal::FindJourminalById(_jourminalId);
    this->jourminal->AddEntry(this);

    Entry::entries.push_back(this);
}

Entry::Entry (string _title) {

    std::string::iterator end_pos = std::remove(_title.begin(), _title.end(), ' ');
    _title.erase(end_pos, _title.end());

    this->id = ++Entry::lastId;
    this->title = _title;
    this->author = User::activeUser;
    srand(time(0));
    this->filename = to_string(rand()%1000000) +this->title;
    this->dateCreated = time(0);
    
    this->jourminal = Jourminal::activeJourminal;
    this->jourminal->AddEntry(this);

    ofstream file;

    Entry::entries.push_back(this);

    ostringstream oss;
    oss << this->dateCreated;

    

    file.open ("entry.dbjml", ios::in | ios::app);
    file << this->id << "||" << jourminal->GetId() << "||" << this->title << "||" 
    << this->author->GetId() << "||" << this->filename << "||" << oss.str() << "||" << "\n";
    file.close();

}

int Entry::GetId() {
    return this->id;
}

string Entry::GetTitle() {
    return this->title;
}

User* Entry::GetAuthor() {
    return this->author;
}
string Entry::GetFilename() {
    return this->filename;
}

time_t Entry::GetDateCreated() {
    return this->dateCreated;
}

Jourminal* Entry::GetJourminal() {
    return this->jourminal;
}

bool Entry::DataFileExists() {
    ifstream file("entry.dbjml");
    return file.good();
}

void Entry::CreateDataFile() {
    ofstream file;
    file.open ("entry.dbjml");
    file.close();
    cout << "-- Entry database file created." << endl;
}

void Entry::ScanEntryDatabase() {
    cout << "- Scanning Entry database." << endl;
    string line;
    ifstream file ("entry.dbjml");

    if (file.is_open()) {
        while (getline (file,line)) {
            std::string s = line;
            std::string delimiter = "||";

            vector<string> entryData;

            size_t pos = 0;
            string token;
            string::size_type sz;
            while ((pos = s.find(delimiter)) != std::string::npos) {
                token = s.substr(0, pos);
                s.erase(0, pos + delimiter.length());
                entryData.push_back(token);
            }

            Entry::lastId = stoi (entryData[0],&sz);

            Entry * entryToAdd = new Entry(Entry::lastId, 
                                           stoi(entryData[1]), 
                                           entryData[2],
                                           stoi(entryData[3]),
                                           entryData[4], 
                                           (time_t)stoi(entryData[5]));
        }
        file.close();
    }


    cout << "-- Scanning is complete." << endl;
}

void Entry::CreateEntry() {
    string title;
    cout << "Creating new entry." << endl;
    cout << "New entry title: ";

    cin.ignore(1, '\n');
    getline(cin, title);

    Entry *e = new Entry(title);

    cout << "New entry created!" << endl;
    Entry::creating = false;
}

void Entry::ConfigureEntryDatabase() {
    cout << "- Configuring entry database." << endl;

    if (Entry::DataFileExists()) {
        cout << "-- Database file found." << endl;
    } else {
        cout << "-- Database file not found. Creating one now." << endl;
        Entry::CreateDataFile();
    }

    Entry::ScanEntryDatabase();
}

void Entry::OpenActive() {
    cout << "You are in: " << Jourminal::activeJourminal->GetTitle() <<"/" << 
    Entry::activeEntry->GetTitle() << endl;

    string action;

    cout << "Type <d> to delete, <o> to read/edit, and <b> to go back" << endl;
    cout << "Command: ";

    cin >> action;

    if (action.compare("b")==0) {
        Entry::activeEntry = nullptr;
    } else if (action.compare("o")==0) {
        OpenFile(Entry::activeEntry->GetFilepath());
        //Entry::OpenFile("test.jml");
    } 
    else if (action.compare("d")==0) {
        Entry::DeleteActive();
    } 
    else {
        cout<< "Sorry. Command not understood" << endl;
    }
    
}

void Entry::DeleteActive() {
    cout << Entry::entries.size() << endl;
    Jourminal::activeJourminal->RemoveEntry(Entry::activeEntry);
    for (uint i = 0; i< Entry::entries.size(); i++) {
        if (Entry::activeEntry == Entry::entries[i]) {
            Entry::entries.erase(Entry::entries.begin() + i);
        }
    }

    ostringstream oss;
    oss << "entries/" << Entry::activeEntry->GetFilename() << ".jml";

    remove(oss.str().c_str());

    delete Entry::activeEntry;
    Entry::activeEntry = nullptr;

    Entry::RewriteEntries();
}

string Entry::GetFilepath() {
    return "entries/" + Entry::activeEntry->GetFilename() + ".jml";
}

void Entry::RewriteEntries() {
    std::ofstream ofs;
    ofs.open("entry.dbjml", std::ofstream::out | std::ofstream::trunc);
    ofs.close();

    ofstream file;
    file.open ("entry.dbjml", ios::in | ios::app);

    for (uint i =0; i< Entry::entries.size(); i++) {
        Entry*e = Entry::entries[i];
        ostringstream oss;
        oss << e->GetDateCreated();
        file << e->GetId() << "||" << e->GetJourminal()->GetId() 
        << "||" << e->GetTitle() << "||" << e->GetAuthor()->GetId() 
        << "||" << e->GetFilename() << "||" << oss.str() << "||" << "\n";
    }

    
    file.close();    
}