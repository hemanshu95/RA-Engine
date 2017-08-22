#include<iostream>
#include<stdio.h>
#include<vector>
#include<string>
#include<map>
#include<fstream>
#include<string>
#include <sstream>
#include<set>
using namespace std;
//public variables
//create table table_name(attribute1 type1,attribute2 type2) ;
//insert tablename value1 value2 string values are put in ""
class datatype
{
    /*
        Stores the contents of a cell based in its datatype
    */

    public:
        string s; // for storing string values
        int i;// for storing int values
        float f; // for storing float values
        int flag; // Stores the type of data the cell contains
        bool operator<(datatype i1) {

            /*
                Operator function to check whether two cells data values are equal or not
            */
            if ( flag==0 && i1.f<f) {
                return true;
            }
            if ( flag==1 && i1.i<i) {
                return true;
            }

            if ( flag==2 && i1.s.compare(s)<0) {
                return true;
            }
            return false;

        }
};
template<typename T>
struct equal_vector
{
    /*
        Structure for defining the comparison operator for  row vectors of different tables
    */
    bool operator() ( std::vector<T> left, std::vector<T> right) const
    {
        for(int i=0;i<left.size();i++)
        {
            if(left[i]<right[i])
                return true;
        }
        return false;
    }

};

bool valid_float(string x)
{
    /*
        Checks whether a string is a valid float value or not
    */

    bool point=true;
    for(int i=0;i<x.size();i++)
    {
        if(x[i]=='.')
        {
            if(point)
                point=false;
            else
            {
                cout<<"invalid float";
                return false;
            }

        }
        else if(x[i]<'0' || x[i]>'9')
        {
            cout<<"invalid float";
            return false;
        }
    }
    return true;
}
bool valid_int(string x)
{
    /*
        Checks whether a string is a valid int value or not
    */
    for(int i=0;i<x.size();i++)
    {
        if(x[i]<'0' || x[i]>'9')
        {
            return false;
        }
    }
    return true;
}
bool valid_string(string c1,string x,string c2)
{
    /*
        Checks whether a string is a valid string value containg any alphanumeric character or not
    */
    if(c1.compare("\"")!=0 || c2.compare("\"")!=0)
    {
        return false;
    }
    for(int i=0;i<x.size();i++)
    {
        if(x[i]==',' || x[i]=='{' || x[i]=='}' || x[i]=='(' || x[i]==')' || x[i]=='?' || x[i]=='['||x[i]==']' ||x[i]=='\\')
        {
            return false;
        }

    }
    return true;
}
string getval(datatype s)
{
    /*
    Returns the value of a cell in string format
    */
    if(s.flag==2)
        return s.s;
    if(s.flag==1)
        return " "+s.i;
    ostringstream os;
    os<<s.f;
    return os.str();
}
int parseint(string s)
{
    /*
    Converts a int value in string format to int format
    */
    int r=0;
    for(int i=0;i<s.size();i++)
    {
        r*=10;
        r+=(s[i]-'0');
    }
    return r;
}
float parsefloat(string s)
{
    /*
        Converts a float value in string format to float format
    */
    float r=0,d=0;
    int i=0;
    for(i=0;i<s.size();i++)
    {
        if(s[i]!='.')
        {
            r*=10;
            r+=(s[i]-'0');
        }

    }
    if(i!=s.size())
    {
        for(int j=s.size()-1;j>i;j--)
        {
            d+=(float)(s[j]-'0');
            d/=10.0;
        }
    }
    return r+d;
}
class table
{
    /*
    Class for creating table object containing all properties and values of a table
    */
    public:
        vector< vector<datatype> > values; //For storing the datavalues in the table
        vector<string> attr_name;  //For storing the attribute names

        vector<int> attr_type;  //For storing the attribute's data type
        void insert_row(string s1)
        {
            // inserting row in a table
            vector<datatype> row;
            int j=0;
            string ts="";
            for(int i=0;i<s1.size();i++)
            {
                if(s1[i]=='\t')
                {
                    datatype td;
                    if(attr_type[j]==0)
                    {
                        td.f=parsefloat(ts);
                        td.flag=0;
                    }
                    else if(attr_type[j]==1)
                    {
                        td.i=parseint(ts);
                        td.flag=1;
                    }
                    else
                    {
                        td.s=ts;
                        td.flag=2;
                    }
                    row.push_back(td);
                    j++;
                    ts="";
                }
                else
                    ts+=s1[i];
            }
            values.push_back(row);
        }
        void print()
        {
            //For printing a table
            cout<<endl;
            for(int i=0;i<attr_name.size();i++)
            {
                cout<<attr_name[i]<<"\t";
            }
            cout<<endl;
            for(int i=0;i<values.size();i++)
            {
                for(int j=0;j<values[i].size();j++)
                {
                    if(attr_type[j]==0)
                        cout<<values[i][j].f<<"\t";
                    else if(attr_type[j]==1)
                        cout<<values[i][j].i<<"\t";
                    else if(attr_type[j]==2)
                        cout<<values[i][j].s<<"\t";


                }
                cout<<endl;
            }
        }
};
table query(vector<string> statement);   //Function for sennding the statement into different functions like set difference , union

vector< string > tables;                          //For storing the names of tabels
vector< vector< pair<string,int> > > table_attr;  //For storing attribute names and types

vector< pair<string,string> > attrib;//attribute name,table name
table readtable(string table_name,int tn)
{
    // For readding table for file to engine memory
    table temp_table;
    for(int i=0;i<table_attr[tn].size();i++)
    {
        temp_table.attr_name.push_back(table_attr[tn][i].first);
        temp_table.attr_type.push_back(table_attr[tn][i].second);
    }
    ifstream myfile;
    string file="data/tables/"+table_name+".dat";
    myfile.open(file.c_str());
    string s1,ts;
    while(!myfile.eof())
    {
        getline(myfile,s1);
        if(s1.compare(""))
            temp_table.insert_row(s1);
    }
    return temp_table;
}
table pro(vector<string> condition,table t1)
{
    /*Function for performing project operation*/
    int n=t1.attr_name.size();

    bool att[n];//Stores which all attributes would be include in the new table
    for(int i=0;i<n;i++)
    {
        att[i]=false;
    }
    for(int i=0;i<condition.size();i++)
    {
        for(int j=0;j<n;j++)
        {
            if(t1.attr_name[j].compare(condition[i])==0)
                att[j]=true;
        }
    }

    table res;
    for(int i=0;i<n;i++)
    {
        if(att[i])
        {
            res.attr_name.push_back(t1.attr_name[i]);
            res.attr_type.push_back(t1.attr_type[i]);
        }
    }
    map<string,bool> duplicates;// Used for checking duplicate rows
    for(int j=0;j<t1.values.size();j++)
    {
        vector<datatype> temp2;
        string s="";
        for(int i=0;i<n;i++)
        {
            if(att[i])
            {
                temp2.push_back(t1.values[j][i]);
                s+=getval(t1.values[j][i])+"\t";
            }
        }
        if(duplicates[s]==false)
        {
            res.values.push_back(temp2);
            duplicates[s]=true;
        }
    }
    return res;
}
int check_att(string atn,vector<string> atns)
{
    //Returns the position of an attribute in a vector<string > if present else -1
    for(int i=0;i<atns.size();i++)
    {
        if(atns[i].compare(atn)==0)
        {
            return i;
        }
    }
    return -1;
}
table rename(vector<string> condition, table t1)
{
    /*
        Rename operation for table that changes the names of attributes of a table
    */
    table y;
    if(condition.size()==1)
        return t1;
    if(condition.size()!=t1.attr_name.size()+1)
    {
        cout<<"Enter name of all the attributes of table in rename command\n";
        return y;
    }
    for(int i=1;i<condition.size();i++)
    {
        t1.attr_name[i-1]=condition[i];
    }
    return t1;
}
table sel(vector<string> condition,table t1)
{

    table res;
    res.attr_name=t1.attr_name;
    res.attr_type=t1.attr_type;
    if(condition.size()<3)
    {
        cout<<"Invalid statement";
        return res;
    }

    int x,y;
    for(int i=0;i<t1.values.size();i++)
    {
        int j=0;
        bool res2=true;
        while(j<condition.size())//Checks each row for all the conditions
        {
            x=check_att(condition[j],t1.attr_name);
            if(j+2<condition.size())
                y=check_att(condition[j+2],t1.attr_name);
            else
            {
                cout<<"Invalid statement";
                return res;
            }
            if(condition[j+1].compare("<")==0)//Less than operator
            {
                if(y==-1)
                {
                    if(t1.attr_type[x]==0)
                    {
                        if(valid_float(condition[j+2]))
                        {
                            res2=res2&(t1.values[i][x].f<parsefloat(condition[j+2]));
                        }
                        else
                        {
                            cout<<"Invalid statement";
                            return res;
                        }
                    }
                    else if(t1.attr_type[x]==1)
                    {
                        if(valid_int(condition[j+2]))
                        {
                            res2=res2&(t1.values[i][x].i<parseint(condition[j+2]));
                        }
                        else
                        {
                            cout<<"Invalid statement";
                            return res;
                        }
                    }
                    else
                    {
                        if(j+4<condition.size() && condition[j+2].compare("\"")==0 && condition[j+4].compare("\"")==0 )
                            res2=res2&(t1.values[i][x].s<condition[j+3]);

                    }
                }
                else
                {
                   if(t1.attr_type[x]!=t1.attr_type[y])
                   {
                       cout<<"Invalid statement";
                        return res;
                   }
                   if(t1.attr_type[x]==0)
                   {
                       res2=res2&(t1.values[i][x].f<t1.values[i][y].f);
                   }

                   if(t1.attr_type[y]==1)
                   {
                       res2=res2&(t1.values[i][x].i<t1.values[i][y].i);
                   }

                   if(t1.attr_type[x]==2)
                   {
                       res2=res2&(t1.values[i][x].s<t1.values[i][y].s);
                   }
                }

            }
            else if(condition[j+1].compare(">")==0)//Greater than operator
            {
                if(y==-1)
                {
                    if(t1.attr_type[x]==0)
                    {
                        if(valid_float(condition[j+2]))
                        {
                            res2=res2&(t1.values[i][x].f>parsefloat(condition[j+2]));
                        }
                        else
                        {
                            cout<<"Invalid statement";
                            return res;
                        }
                    }
                    else if(t1.attr_type[x]==1)
                    {
                        if(valid_int(condition[j+2]))
                        {
                            res2=res2&(t1.values[i][x].i>parseint(condition[j+2]));
                        }
                        else
                        {
                            cout<<"Invalid statement";
                            return res;
                        }
                    }
                    else
                    {
                        if(j+4<condition.size() && condition[j+2].compare("\"")==0 && condition[j+4].compare("\"")==0 )
                            res2=res2&(t1.values[i][x].s>condition[j+3]);

                    }
                }
                else
                {
                   if(t1.attr_type[x]!=t1.attr_type[y])
                   {
                       cout<<"Invalid statement";
                        return res;
                   }
                   if(t1.attr_type[x]==0)
                   {
                       res2=res2&(t1.values[i][x].f>t1.values[i][y].f);
                   }

                   if(t1.attr_type[y]==1)
                   {
                       res2=res2&(t1.values[i][x].i>t1.values[i][y].i);
                   }

                   if(t1.attr_type[x]==2)
                   {
                       res2=res2&(t1.values[i][x].s>t1.values[i][y].s);
                   }
                }

            }
            if(condition[j+1].compare("=")==0)//Equal operator
            {
                if(y==-1)
                {
                    if(t1.attr_type[x]==0)
                    {
                        if(valid_float(condition[j+2]))
                        {
                            res2=res2&(t1.values[i][x].f==parsefloat(condition[j+2]));
                        }
                        else
                        {
                            cout<<"Invalid statement";
                            return res;
                        }
                    }
                    else if(t1.attr_type[x]==1)
                    {
                        if(valid_int(condition[j+2]))
                        {
                            res2=res2&(t1.values[i][x].i==parseint(condition[j+2]));
                        }
                        else
                        {
                            cout<<"Invalid statement";
                            return res;
                        }
                    }
                    else
                    {
                        if(j+4<condition.size() && condition[j+2].compare("\"")==0 && condition[j+4].compare("\"")==0 )
                            res2=res2&(t1.values[i][x].s==condition[j+3]);

                    }
                }
                else
                {
                   if(t1.attr_type[x]!=t1.attr_type[y])
                   {
                       cout<<"Invalid statement";
                        return res;
                   }
                   if(t1.attr_type[x]==0)
                   {
                       res2=res2&(t1.values[i][x].f==t1.values[i][y].f);
                   }

                   if(t1.attr_type[y]==1)
                   {
                       res2=res2&(t1.values[i][x].i==t1.values[i][y].i);
                   }

                   if(t1.attr_type[x]==2)
                   {
                       res2=res2&(t1.values[i][x].s==t1.values[i][y].s);
                   }
                }

            }
            if(!res2)
                break;
            if(condition[j+2].compare("\"")==0)
                j+=6;
            else j+=4;
        }
        if(res2)
            res.values.push_back(t1.values[i]);
    }
    return res;
}
int find_table(string table_name)
{
    /*
    Finds whether a table exists with a given name
    */
    ifstream myfile;
    myfile.open("data/db.dat");
    string s1;
    int j=0;
    while(!myfile.eof())
    {
        getline(myfile,s1);
        string s2="";
        for(int i=0;i<s1.size();i++)
        {
            if(s1[i]=='\t')
            {
                break;
            }
            s2+=s1[i];
        }
        if(s2.compare(table_name)==0)
            return j;
        j++;
    }
    return -1;
}
table unionf(table t1,table t2)
{
    /*
    Performs union operation between two tables
    */
    table res;
    if(t1.attr_name.size()!=t2.attr_name.size())
    {
        cout<<"Invalid union operations";
        return res;
    }
    //Checks whether 2 tables have same attributes name order and same types
    for(int i=0;i<t1.attr_name.size();i++)
    {
        if(t1.attr_name[i].compare(t2.attr_name[i]) || t1.attr_type[i]!=t2.attr_type[i])
        {
            cout<<"Invalid union operations";
            return res;
        }
    }
    for(int i=0;i<t1.attr_name.size();i++)
    {
        res.attr_name.push_back((t1.attr_name[i]));
        res.attr_type.push_back((t1.attr_type[i]));
    }
    set<vector<datatype> ,equal_vector<datatype> > tsuni;//Temporary set for union:A set containg the rows in a table
    for(int i=0;i<t1.values.size();i++)
    {
        tsuni.insert(t1.values[i]);
    }
    for(int i=0;i<t2.values.size();i++)
    {
        if(tsuni.find(t2.values[i])==tsuni.end())//Checks whether that row already exists or not
            tsuni.insert(t2.values[i]);
    }
    for(set<vector<datatype> >::iterator i1=tsuni.begin();i1!=tsuni.end();i1++)
    {
        vector<datatype> y = (*i1);
        res.values.push_back(y);//Pushs the values from set to result table
    }
    return res;
}
table interf(table t1,table t2)
{
    /*
    Performs intersection operation between two tables
    */
    table res;
    if(t1.attr_name.size()!=t2.attr_name.size())
    {
        cout<<"Invalid inter operations";
        return res;
    }
    //Checks whether 2 tables have same attributes name order and same types
    for(int i=0;i<t1.attr_name.size();i++)
    {
        if(t1.attr_name[i].compare(t2.attr_name[i]) || t1.attr_type[i]!=t2.attr_type[i])
        {
            cout<<"Invalid inter operations";
            return res;
        }
    }
    for(int i=0;i<t1.attr_name.size();i++)
    {
        res.attr_name.push_back((t1.attr_name[i]));
        res.attr_type.push_back((t1.attr_type[i]));
    }
    set<vector<datatype> ,equal_vector<datatype> > tsinter; //Temporary set for intersection:A set containg the rows in a table
    for(int i=0;i<t1.values.size();i++)
    {
        tsinter.insert(t1.values[i]);
    }

    for(int i=0;i<t2.values.size();i++)
    {
        if(tsinter.find(t2.values[i])!=tsinter.end())
        {
             res.values.push_back(t2.values[i]);//Pushs the values from set to result table
        }
    }
    return res;
}
table setd(table t1,table t2)
{
    /*
    Performs set difference operation between two tables
    */
    table res;
    if(t1.attr_name.size()!=t2.attr_name.size())
    {
        cout<<"Invalid inter operations";
        return res;
    }
    //Checks whether 2 tables have same attributes name order and same types
    for(int i=0;i<t1.attr_name.size();i++)
    {
        if(t1.attr_name[i].compare(t2.attr_name[i]) || t1.attr_type[i]!=t2.attr_type[i])
        {
            cout<<"Invalid inter operations";
            return res;
        }
    }
    for(int i=0;i<t1.attr_name.size();i++)
    {
        res.attr_name.push_back((t1.attr_name[i]));
        res.attr_type.push_back((t1.attr_type[i]));
    }
    set<vector<datatype> ,equal_vector<datatype> > tsinter; //Temporary set for set difference:A set containg the rows in a table
    for(int i=0;i<t2.values.size();i++)
    {
        tsinter.insert(t2.values[i]);
    }
    for(int i=0;i<t1.values.size();i++)
    {
        if(tsinter.find(t1.values[i])==tsinter.end())//Checks whether that row exists in 1st table and not in 2nd table
            res.values.push_back(t1.values[i]);//Pushs the values from set to result table
    }
    return res;
}
int find_attr(vector<string> vec ,string att)
{
     //Returns the position of an attribute in a vector<string > if present else -1

    for(int i=0;i<vec.size();i++)
    {
        if(vec[i].compare(att)==0)
            return i;
    }
    return -1;
}
table cart(table t1,table t2)
{
    /*
    Performs a function of catesian product between 2 tables
    */
    table res;
    for(int i=0;i<t1.attr_name.size();i++)
    {
        res.attr_name.push_back(t1.attr_name[i]);
        res.attr_type.push_back(t1.attr_type[i]);
    }
    for(int i=0;i<t2.attr_name.size();i++)
    {
        int r=find_attr(t1.attr_name,t2.attr_name[i]);//Checks whether an attribute in 2nd table exists in 1st table or not
        if(r==-1)
        {
            res.attr_name.push_back(t2.attr_name[i]);
            res.attr_type.push_back(t2.attr_type[i]);
        }
        else //if it exists then that attribute is named as 1.attributen_name and 2.attributen_name in the result table
        {
            res.attr_name[r]="1."+res.attr_name[r];
            res.attr_name.push_back("2."+t2.attr_name[i]);
            res.attr_type.push_back(t2.attr_type[i]);
        }
    }
    for(int i=0;i<t1.values.size();i++)
    {
        for(int j=0;j<t2.values.size();j++)
        {
            vector< datatype > temp;//temporary row vector
            for(int k=0;k<t1.attr_name.size();k++)
            {
                temp.push_back(t1.values[i][k]);
            }

            for(int k=0;k<t2.attr_name.size();k++)
            {
                temp.push_back(t2.values[j][k]);
            }
            res.values.push_back(temp);
        }
    }
    return res;
}
table query(vector<string> statement)
{
    //Splitted string into vector of string is being processed and send to different functional operational calls
    table values;
    if(statement.size()==0)
    {
        cout<<"Invalid statements";
        return values;
    }
    int tn=find_table(statement[0]);

    if(tn>=0)//Returns a table
    {

        return readtable(statement[0],tn);
    }
    else if(statement[0].compare("project")==0) //If the operation is a project operation
    {
        //then checks if the statement is a valid project statement
        if(statement.size()<7 || statement[1].compare("{"))
        {
            cout<<"Invalid statements";
            return values;
        }
        vector<string> condition;
        bool f1=false;
        int i;
        for(i=2;i<statement.size();i++)
        {
            if(statement[i].compare("}")==0)
            {
                f1=true;
                i++;
                break;
            }
            else
            {
                if(statement[i].compare(","))
                    condition.push_back(statement[i]);
            }
        }
        if(!f1 || i>=statement.size() || statement[i].compare("("))
        {
            cout<<"Invalid use of curly brackets";
            return values;
        }

        int brackets=1,j;
        vector<string> new_statement;
        for(j=i+1;j<statement.size();j++)
        {
            if(statement[j].compare("(")==0)
                brackets++;
            if(statement[j].compare(")")==0)
                brackets--;
            if(brackets==0 && statement[j].compare(")")==0)
                break;
            new_statement.push_back(statement[j]);
        }

        values=query(new_statement);//Calling a nested operation
        values=pro(condition,values);//Call of project operation on the nested query
    }
    else if(statement[0].compare("select")==0)//If the operation is a select operation
    {
        //then checks if the statement is a valid select statement
        if(statement.size()<9 || statement[1].compare("{"))
        {
            cout<<"Invalid statements";
            return values;
        }
        vector<string> condition;
        bool f1=false;
        int i;
        for(i=2;i<statement.size();i++)
        {
            if(statement[i].compare("}")==0)
            {
                f1=true;
                i++;
                break;
            }
            else
            {
                condition.push_back(statement[i]);
            }
        }
        if(!f1 || i>=statement.size() || statement[i].compare("(")||statement[statement.size()-1].compare(")"))
        {
            cout<<"Invalid use of curly brackets";
            return values;
        }
        int brackets=1,j;
        vector<string> new_statement;
        for(j=i+1;j<statement.size();j++)
        {
            if(statement[j].compare("(")==0)
                brackets++;
            if(statement[j].compare(")")==0)
                brackets--;
            if(brackets==0 && statement[j].compare(")")==0)
                break;
            new_statement.push_back(statement[j]);
        }
        values=query(new_statement);//Calling a nested  operation
        values=sel(condition,values);//Call of select operation on the nested query

    }
    else if(statement[0].compare("rename")==0)//If the operation is a rename operation
    {
        //then checks if the statement is a valid rename statement
        if(statement.size()<9 || statement[1].compare("{") || statement[statement.size()-1].compare(")"))
        {
            cout<<"Invalid statements";
            return values;
        }
        vector<string> condition;
        bool f1=false;
        int i;
        for(i=2;i<statement.size();i++)
        {
            if(statement[i].compare("}")==0)
            {
                f1=true;
                i++;
                break;
            }
            else
            {
                condition.push_back(statement[i]);
            }
        }
        if(!f1 || i>=statement.size() || statement[i].compare("("))
        {
            cout<<"Invalid use of curly brackets";
            return values;
        }
        int brackets=1,j;
        vector<string> new_statement;
        for(j=i+1;j<statement.size();j++)
        {
            if(statement[j].compare("(")==0)
                brackets++;
            if(statement[j].compare(")")==0)
                brackets--;
            if(brackets==0 && statement[j].compare(")")==0)
                break;
            new_statement.push_back(statement[j]);
        }
        values=query(new_statement);//Calling a nested operation
        values=rename(condition,values);//Call of rename operation on the nested query

    }
    else if(statement[0].compare("union")==0)//If the operation is a union operation
    {
        //then checks if the statement is a valid union statement
        if(statement.size()<6 ||statement[1].compare("(")||statement[statement.size()-1].compare(")"))
        {
            cout<<"Invalid Statement\n";
            return values;
        }
        vector<string> statement1,statement2;
        int i=2,dbrackets=0,brackets=0;

        for(;i<statement.size()-1;i++)
        {
            if(statement[i].compare("(")==0)
                brackets++;
            if(statement[i].compare(")")==0)
                brackets--;
            if(statement[i].compare("{")==0)
                dbrackets++;
            if(statement[i].compare("}")==0)
                dbrackets--;

            if(statement[i].compare(",")==0 && dbrackets==0 && brackets==0)
                break;
            statement1.push_back(statement[i]);
        }
        if(i==statement.size()-1)
        {
            cout<<"Invalid statements";
            return values;
        }
        i++;
        for(;i<statement.size()-1;i++)
        {
            statement2.push_back(statement[i]);
        }
        table t1=query(statement1);//Calling a nested 1st attribute operation
        table t2=query(statement2);//Calling a nested 2nd attribute operation
        values=unionf(t1,t2);//Call of union operation on the nested query
    }
    else if(statement[0].compare("inter")==0)//If the operation is a intersection operation
    {
        //then checks if the statement is a valid intersection statement

        if(statement.size()<6 ||statement[1].compare("(")||statement[statement.size()-1].compare(")"))
        {
            cout<<"Invalid Statement\n";
            return values;
        }
        vector<string> statement1,statement2;
        int i=2,dbrackets=0,brackets=0;

        for(;i<statement.size()-1;i++)
        {
            if(statement[i].compare("(")==0)
                brackets++;
            if(statement[i].compare(")")==0)
                brackets--;
            if(statement[i].compare("{")==0)
                dbrackets++;
            if(statement[i].compare("}")==0)
                dbrackets--;

            if(statement[i].compare(",")==0 && dbrackets==0 && brackets==0)
                break;
            statement1.push_back(statement[i]);
        }
        if(i==statement.size()-1)
        {
            cout<<"Invalid statements";
            return values;
        }
        i++;
        for(;i<statement.size()-1;i++)
        {
            statement2.push_back(statement[i]);
        }
        table t1=query(statement1);//Calling a nested 1st attribute operation
        table t2=query(statement2);//Calling a nested 2nd attribute operation
        values=interf(t1,t2);//Call of intersection operation on the nested query
    }
    else if(statement[0].compare("setd")==0)//If the operation is a set difference operation
    {
        //then checks if the statement is a valid set difference statement

        if(statement.size()<6 ||statement[1].compare("(")||statement[statement.size()-1].compare(")"))
        {
            cout<<"Invalid Statement\n";
            return values;
        }
        vector<string> statement1,statement2;
        int i=2,dbrackets=0,brackets=0;

        for(;i<statement.size()-1;i++)
        {
            if(statement[i].compare("(")==0)
                brackets++;
            if(statement[i].compare(")")==0)
                brackets--;
            if(statement[i].compare("{")==0)
                dbrackets++;
            if(statement[i].compare("}")==0)
                dbrackets--;

            if(statement[i].compare(",")==0 && dbrackets==0 && brackets==0)
                break;
            statement1.push_back(statement[i]);
        }
        if(i==statement.size()-1)
        {
            cout<<"Invalid statements";
            return values;
        }
        i++;
        for(;i<statement.size()-1;i++)
        {
            statement2.push_back(statement[i]);
        }
        table t1=query(statement1);//Calling a nested 1st attribute operation
        table t2=query(statement2);//Calling a nested 2nd attribute operation
        values=setd(t1,t2);//Call of set difference operation on the nested query
    }
    else if(statement[0].compare("cart")==0)//If the operation is a cartesian product operation
    {
        //then checks if the statement is a valid cartesian product statement

        if(statement.size()<6 ||statement[1].compare("(")||statement[statement.size()-1].compare(")"))
        {
            cout<<"Invalid Statement\n";
            return values;
        }
        vector<string> statement1,statement2;
        int i=2,dbrackets=0,brackets=0;

        for(;i<statement.size()-1;i++)
        {
            if(statement[i].compare("(")==0)
                brackets++;
            if(statement[i].compare(")")==0)
                brackets--;
            if(statement[i].compare("{")==0)
                dbrackets++;
            if(statement[i].compare("}")==0)
                dbrackets--;

            if(statement[i].compare(",")==0 && dbrackets==0 && brackets==0)
                break;
            statement1.push_back(statement[i]);
        }
        if(i==statement.size()-1)
        {
            cout<<"Invalid statements";
            return values;
        }
        i++;
        for(;i<statement.size()-1;i++)
        {
            statement2.push_back(statement[i]);
        }
        table t1=query(statement1);//Calling a nested 1st attribute operation
        table t2=query(statement2);//Calling a nested 2nd attribute operation
        values=cart(t1,t2);//Call of cartesian product operation on the nested query
    }
    return values;
}
void input_table_attr()
{
    ifstream myfile;
    myfile.open("data/db.dat");
    string s1;
    int j=0;
    tables.clear();
    table_attr.clear();
    while(!myfile.eof())
    {
        getline(myfile,s1);
        string s2="";
        int i;
        for(i=0;i<s1.size();i++)
        {
            if(s1[i]=='\t')
            {
                i++;
                break;
            }
            s2+=s1[i];
        }
        tables.push_back(s2);
        vector< pair<string,int> > t1;
        table_attr.push_back(t1);

        int k1=0;
        s2="";
        for(;i<s1.size();i++)
        {
            if(s1[i]=='\t')
            {
               table_attr[j].push_back(make_pair(s2,s1[i+1]-'0'));
               i+=2;
               s2="";
            }
            else
                s2+=s1[i];
        }
        j++;
    }
}
void insert_dbfile(string table_name,vector< pair<string,int> > attributes)
{
    /*
    Inserts a table properties into the database file containg data about table structures
    */
    if(find_table(table_name)>=0)//While creating atble checks whether that table already exists or not
    {
        cout<<"Table already exists by this name";
        return ;
    }
    ofstream myfile;
    myfile.open("data/db.dat",ofstream::app);
    string res=table_name+"\t";
    for(int i=0;i<attributes.size();i++)
    {
        res+=attributes[i].first + "\t" + (char)('0'+attributes[i].second) + "\t";
    }
    res+="\n";
    myfile<<res;
    myfile.close();
    input_table_attr();

}

bool validate_values_insert(vector<string> statement,int n1)
{
    //Validates whether the values to be inserted in a table follows table structure or not
    vector< pair<string,int> > attrs=table_attr[n1];
    string s="";
    string table_name=statement[1];
    for(int j=0,k=2;j<attrs.size() && k<statement.size();j++)
    {
        if(attrs[j].second==0)//If attribute type is float
        {
            if(!valid_float(statement[k]))
                return false;
            s+=statement[k]+"\t";

        }
        else if(attrs[j].second==1)//If attribute type is int
        {
            if(!valid_int(statement[k]))
                return false;
            s+=statement[k]+"\t";
        }
        else
        {
            if(k+2<statement.size())
            {
                if(!valid_string(statement[k],statement[k+1],statement[k+2]))
                    return false;
                s+=statement[k+1]+"\t";
                k+=2;
            }
            else
                return false;
        }
        k++;
    }
    ofstream myfile;
    string file="data/tables/"+table_name+".dat";
    myfile.open(file.c_str(),ios::out | ios::app);

    myfile<<s<<"\n";
    myfile.close();
    return true;
}
void insert_into(vector<string> statement)
{
    //The insert command is processed in this function
    string table_name=statement[1];
    int x=find_table(table_name);
    if(x<0)
    {
        cout<<"Table doesn't exists\n";
        return ;
    }
    if(validate_values_insert(statement,x))
    {
        cout<<"A valid insert.\n";
    }
    else
    {
        cout<<"Enter a valid insert statement\n";
    }

}
void create_table(vector<string> statement)
{
    //The create table command is processed in this function
    string table_name=statement[2];
    if(statement[3].compare("(")!=0)
    {
        cout<<"Invalid statement\n";
        return;
    }
    int i=4;
    vector< pair<string,int> > attributes;
    string a_name;
    int a_type;
    while(i<statement.size())
    {
        if(i+2>=statement.size())
        {
            cout<<"Invalid statement\n";
            return;
        }
        a_name=statement[i];
        if(statement[i+1]=="float")
            a_type=0;
        else if(statement[i+1]=="int")
            a_type=1;
        else if(statement[i+1]=="string")
            a_type=2;

        else
        {
            cout<<"Invalid datatype\n";
            return;
        }
        if(statement[i+2].compare(",")!=0&&statement[i+2].compare(")")!=0)
        {
            cout<<"Invalid statement\n";
            return;
        }
        attributes.push_back(make_pair(a_name,a_type));
        if(statement[i+2].compare(")")==0)
        {
            if(statement.size()-1!=i+2)
            {
                cout<<"Invalid statement\n";
                return;
            }
        }
        i+=3;
    }
    insert_dbfile(table_name,attributes);
}
void calloperation(vector<string> statement)
{
    /*
    Checks whether the string input is calling DDL or DML or a query command
    */
    if(statement[0].compare("create")==0)
    {
        if(statement.size()<7 || statement[1].compare("table")!=0)
        {
            cout<<"Invalid Statement\n";
            return;
        }
        else if(statement[1].compare("table")==0)
        {
            create_table(statement);
            cout<<"Your table is successfully created\n";
        }
    }
    else if(statement[0].compare("insert")==0)
    {
        if(statement.size()<3)
        {
            cout<<"Invalid Statement\n";
            return;
        }
        insert_into(statement);
    }
    else
    {
        table res=query(statement);
        res.print();
    }
}
void operate(string s)
{
    /*
        Tokenises the given string input into a vector of string based on tokenizing about space, ",",{,},(,),'''"''',<,>,=,&,;
    */
    vector<string> result;
    string temp="";
    int brackets=0,cbrackets=0;
    bool doubleq=false;
    for(int i=0;i<s.size();i++)
    {
        if(doubleq)
        {
            if(s[i]!='"')
            {
                temp+=s[i];
            }
            else
            {
                doubleq=false;
                result.push_back(temp);
                result.push_back("\"");
                temp="";
            }
            continue;
        }

        if(s[i]=='"')
        {
            doubleq=true;
            if(temp.compare("")!=0)
            {
                result.push_back(temp);
            }
            result.push_back("\"");
            temp="";
            continue;
        }
        if(s[i]=='{')
        {
            if(temp.compare("")!=0)
                result.push_back(temp);
            cbrackets++;
            result.push_back("{");
            temp="";
            continue;
        }
        if(s[i]=='}')
        {
            if(temp.compare("")!=0)
                result.push_back(temp);
            cbrackets--;
            if(cbrackets<0)
            {
                cout<<"Invalid use of curly brackets";
            }
            result.push_back("}");
            temp="";
            continue;
        }
        if(s[i]=='(')
        {
            if(temp.compare("")!=0)
                result.push_back(temp);
            brackets++;
            result.push_back("(");
            temp="";
            continue;
        }
        if(s[i]==')')
        {
            if(temp.compare("")!=0)
                result.push_back(temp);
            brackets--;
            if(brackets<0)
            {
                cout<<"Invalid use of brackets";
            }
            result.push_back(")");
            temp="";
            continue;
        }
        if(s[i]==',')
        {
            if(temp.compare("")!=0)
            {
                result.push_back(temp);
            }
            result.push_back(",");
            temp="";
            continue;
        }
        if(s[i]=='<')
        {
            if(temp.compare("")!=0)
            {
                result.push_back(temp);
            }
            result.push_back("<");
            temp="";
            continue;
        }
        if(s[i]=='>')
        {
            if(temp.compare("")!=0)
            {
                result.push_back(temp);
            }
            result.push_back(">");
            temp="";
            continue;
        }
        if(s[i]=='=')
        {
            if(temp.compare("")!=0)
            {
                result.push_back(temp);
            }
            result.push_back("=");
            temp="";
            continue;
        }
        if(s[i]=='&')
        {
            if(temp.compare("")!=0)
            {
                result.push_back(temp);
            }
            result.push_back("&");
            temp="";
            continue;
        }
        if(s[i]==';' )
        {
            if(i!=s.size()-1)
            {
                result.clear();
                cout<<"Invalid statement";
                return;
            }
            else
            {
                if(temp.compare("")!=0)
                {
                    result.push_back(temp);
                }
                break;
            }
            continue;
        }
        if(s[i]==' ')
        {
            if(temp.compare("")!=0)
            {
                result.push_back(temp);
            }
            temp="";
            continue;
        }
        else
        {
            temp+=s[i];
        }
    }
    if(doubleq)
    {
        cout<<"Invalid statement\n";
        return;
    }

    calloperation(result);//Calls the operation to select which command operation would be called
}
int main()
{
    bool flag=true;
    string statement,tempstate;
    input_table_attr();
    while(1)
    {
        flag=true;
        statement="";
        while(flag)
        {
            cout<<" > ";
            getline(cin,tempstate);
            if(tempstate.size()>=1 && tempstate[tempstate.size()-1]==';')//A command should always be ended with a ';'
            {
                flag=false;
                statement+=tempstate;
            }
            else
            {
                statement+=tempstate+" ";
            }
        }
        if(statement.size()>0)
            operate(statement);
        else
            cout<<"Invalid statement\n";
    }
    return 0;
}

