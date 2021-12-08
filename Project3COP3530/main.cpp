#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdlib.h>
#include <list>
using namespace std;

int numberOfCharities = 8409; //Zoe said "Fix later"

class Charity {
private:

public:
    struct Node{
        Node* next;
        int weight;
        Charity* charnode;
        //string charname;

        Node() {
            next = nullptr;
            weight = 0;
        }
        Node(int weight) {
            this->weight = weight;
        }
    };
    string Name;
    string Category;
    string State;
    string AScore;
    string Subcategory;
    Node* head;

    Charity(){
        head = nullptr;
        Name = "empty";
        Category = "empty";
        State = "empty";
        AScore = "empty";
        Subcategory = "empty";
    };

};

// CSV Reading from stackoverflow user sastanin (https://stackoverflow.com/questions/1120140/how-can-i-read-and-parse-csv-files-in-c)
enum class CSVState {
    UnquotedField,
    QuotedField,
    QuotedQuote
};

vector<string> readCSVRow(const string &row) {
    CSVState state = CSVState::UnquotedField;
    vector<string> fields {""};
    size_t i = 0; // index of the current field
    for (char c : row) {
        switch (state) {
            case CSVState::UnquotedField:
                switch (c) {
                    case ',': // end of field
                        fields.push_back(""); i++;
                        break;
                    case '"': state = CSVState::QuotedField;
                        break;
                    default:  fields[i].push_back(c);
                        break; }
                break;
            case CSVState::QuotedField:
                switch (c) {
                    case '"': state = CSVState::QuotedQuote;
                        break;
                    default:  fields[i].push_back(c);
                        break; }
                break;
            case CSVState::QuotedQuote:
                switch (c) {
                    case ',': // , after closing quote
                        fields.push_back(""); i++;
                        state = CSVState::UnquotedField;
                        break;
                    case '"': // "" -> "
                        fields[i].push_back('"');
                        state = CSVState::QuotedField;
                        break;
                    default:  // end of quote
                        state = CSVState::UnquotedField;
                        break; }
                break;
        }
    }
    return fields;
}
vector<vector<string>> readCSV(ifstream &in) {
    vector<vector<string>> table;
    string row;
    while (!in.eof()) {
        getline(in, row);
        if (in.bad() || in.fail()) {
            break;
        }
        auto fields = readCSVRow(row);
        table.push_back(fields);
    }
    return table;
}
void weightRandomizer(Charity a, vector<Charity>& v){
    int edges = 1;
    for(int i = 0; i < edges; i++) {
        Charity::Node* b = new Charity::Node;
        int randomCharity = rand() % numberOfCharities;
        int randomWeight = rand() % 1000;
        //add into the first while adding into the other
        b->charnode = &v[randomCharity];
        b->weight = randomWeight;
        Charity::Node* temp = a.head;
        bool found = false;
        if(a.head == nullptr){
            a.head = b;
            found = true;
        }
        else{
            while(temp->next != nullptr) {
                if (temp->charnode->Name == v[randomCharity].Name || a.Name == v[randomCharity].Name){
                    found = true;
                    break;
                }
                temp = temp->next;
            }
        }
        if(!found){
            temp->next = b;
        }
        Charity::Node* c = new Charity::Node;
        //add into the first while adding into the other
        c->charnode = &a;
        c->weight = randomWeight;
        found = false;
        temp = v[randomCharity].head;
        if(temp == nullptr){
            v[randomCharity] = a;
            found = true;
        }
        else{
            while(temp->next != nullptr) {
                if (temp->charnode->Name == v[randomCharity].Name){
                    found = true;
                    break;
                }
                temp = temp->next;
            }
        }
        if(!found){
            temp->next = c;
        }

    }
}
int main()
{
    vector<Charity> charities;

    string filePath = "CLEAN_charity_data.csv";
    vector<vector<string>> charityData;

    ifstream inFile(filePath);
    if (inFile.is_open()) {
        charityData = readCSV(inFile);
    }

    for (int i = 1; i < charityData.size(); i++)
    {
        Charity a;
        for (int j = 0; j < charityData[i].size(); j++)
        {
            if(j == 0) {
                a.AScore = charityData[i][j];
            }
            if(j == 1) {
                a.Category = charityData[i][j];
            }
            if(j == 14) {
                a.Name = charityData[i][j];
            }
            if(j == 17) {
                a.State = charityData[i][j];
            }
            if(j == 18) {
                a.Subcategory = charityData[i][j];
            }
        }
        charities.push_back(a);
    }
    for (int i = 0; i < charities.size(); i++) {
        weightRandomizer(charities[i], charities);
    }
//    for (int i = 0; i < 2; i++) {
//        Charity::Node* temp = new Charity::Node;
//        temp = charities[i].head;
//        while(temp != nullptr){
//            cout << temp->weight << ":" << string(temp->charnode->Name) << " ";
//            temp = temp->next;
//        }
//        cout << endl;
//    }

    return 0;

}