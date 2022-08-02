#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

void print(string str)
{
    fstream vj;
    vj.open(str);
    char ch;
    string ind;
    if (!vj)
        cout << "file open error!!" << endl;
    else
    {
        while (vj >> noskipws >> ch)
        {
            ind += ch;
            // cout << ch; // Or whatever
        }
        cout << ind << endl;
    }
    vj.close();
}

// encoded map
void initialise_encode(unordered_map<string, int> &table)
{
    for (int i = 0; i <= 255; i++)
    {
        string ch = "";
        ch += char(i);
        table[ch] = i;
    }
}

// decoded map
void initialise_decode(unordered_map<int, string> &table)
{
    for (int i = 0; i <= 255; i++)
    {
        string ch = "";
        ch += char(i);
        table[i] = ch;
    }
}

void encoding(string str)
{
   fstream vj;
    string ind;
    vj.open(str); // file open
    char ch;
    if (!vj)
        cout << "file not found";
    else
    {
       while (vj >> noskipws >> ch)
        {
            ind += ch;
            // cout << ch; // Or whatever
        }
        cout << ind;
        vj.close();
    }

    string s1 = ind;

    cout << "Encoding\n";
    unordered_map<string, int> table;
    for (int i = 0; i <= 255; i++)
    {
        string ch = "";
        ch += char(i);
        table[ch] = i;
    }

    string p = "", c = "";
    p += s1[0];
    int code = 256;
    vector<int> output_code;
    cout << "String\tOutput_Code\tAddition\n";
    for (int i = 0; i < s1.length(); i++)
    {
        if (i != s1.length() - 1)
            c += s1[i + 1];
        if (table.find(p + c) != table.end())
        {
            p = p + c;
        }
        else
        {
            cout << p << "\t" << table[p] << "\t\t"
                 << p + c << "\t" << code << endl;
            output_code.push_back(table[p]);
            table[p + c] = code;
            code++;
            p = c;
        }
        c = "";
    }
    cout << p << "\t" << table[p] << endl;
    output_code.push_back(table[p]);
    // return output_code;

    vj.open(str);
    // here outpur array has been written to file
    for (int i : output_code)
    {
        vj << i;
        vj << ' ';
    }
    vj.close();
    
}
void decoding(string str)
{
    fstream vj;
    vj.open(str);
    vector<int> out;
    int num;
    while (vj)
    {
        vj >> num;
        out.push_back(num);
    }
    vj.close(); // closing file after reading number from it

    out.pop_back(); // last number ara hai file me do bar

    string res;
    vector<int> op = out;
    cout << "\nDecoding\n";
    unordered_map<int, string> table;
    for (int i = 0; i <= 255; i++)
    {
        string ch = "";
        ch += char(i);
        table[i] = ch;
    }
    int old = op[0], n;
    string s = table[old];
    string c = "";
    c += s[0];
    // cout << s;
    res += s; // updated
    int count = 256;
    for (int i = 0; i < op.size() - 1; i++)
    {
        n = op[i + 1];
        if (table.find(n) == table.end())
        {
            s = table[old];
            s = s + c;
        }
        else
        {
            s = table[n];
        }
        // cout << s;
        res += s; // updated
        c = "";
        c += s[0];
        table[count] = table[old] + c;
        count++;
        old = n;
    }
    
    std::ofstream ofs;
    ofs.open(str, std::ofstream::out | std::ofstream::trunc);
    vj.open(str);
    ofs << res; // writing content back to file
    ofs.close();

    vj.close();
    
    // printing content of file
    print(str);
}

// this will append data to encode.txt
void append(string str)
{
    fstream vj, vs;
    string ind;
    vs.open("encode.txt", std::ios_base::app); // file open
    vj.open(str);
    char ch;
    // this will read the data from file and store int a string
    if (!vj)
        cout << "file not found";
    else
    {
        while (vj >> noskipws >> ch)
        {
            ind += ch;
            // cout << ch; // Or whatever
        }
        cout << ind;
        vj.close();
    }
    vs << ind <<endl;  // content
    vs<<"#"<<endl;
    vs.close();
}

// this will transfer data to seperat files
// this will transfer data to seperat files
void depart()
{
    // vs.open("encode.txt"); // file open
    fstream vs("encode.txt", fstream::in);
    vector<string> bcom;
    char ch;
    string aps = "";
    while (vs >> noskipws >> ch)
    {
        if (ch == '#')
            break;
        aps += ch; // extracting file name
        if (ch == '\n')
        {
            bcom.push_back(aps);
            aps = "";
        }
    }
    fstream vj;
    for (int i = 0; i < bcom.size(); ++i)
    {
        string str = "";
        vj.open(bcom[i]);

        if (!vj)
        {
            cout << "Error in creating file!!!";
        }
        while (vs >> noskipws >> ch)
        {
            if (ch == '#')
            {
                vj.close();
                break;
            }
            else
                vj << ch;
        }
    }
    vs.close();
    cout << "Content of bcom" << endl;
    for (int i = 0; i < bcom.size(); ++i)
        cout << i << " " << bcom[i];
}

int main(int argc, char *argv[])
{
    string command = argv[1];
    cout<<command<<endl;
    
    if (command == "-c")
    {
        fstream cvj, vs;
        string ind ;
        vs.open("encode.txt", std::ios_base::app); // file open
        for (int i = 2; i < argc; i++)
        {
            vs << argv[i] << endl;
        }
        vs << '#'<<endl;
        vs.close();
        for (int i = 2; i < argc; i++)
        {
            string str = argv[i];
            append(str);
        }
        encoding("encode.txt");
    }
    else if (command == "-d")
    {
        // cout << "Decoding" << endl;
        string str = argv[2];

        decoding(str);
        
        depart();
    }

    return 0;
}