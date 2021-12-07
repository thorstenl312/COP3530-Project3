#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
//asdfghjk Hi MAITHI
using namespace std;

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

int main()
{
    string filePath = "CLEAN_charity_data.csv";
    vector<vector<string>> charityData;

    ifstream inFile(filePath);
    if (inFile.is_open()) {
        charityData = readCSV(inFile);
    }

    for (int i = 0; i < charityData.size(); i++)
    {
        for (int j = 0; j < charityData[i].size(); j++)
        {
            cout << i << " : " << charityData[i][j] << endl;
        }
    }
}