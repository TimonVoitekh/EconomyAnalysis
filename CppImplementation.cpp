#include <iostream>
#include <fstream>
#include <math.h>
#include <vector>
#include <string>
using namespace std;

int N;
vector <vector<double> > tech_matrix; 
vector <double> final_demand, output;

vector <vector <double> > Identity(int n)
{
    vector <vector <double> > res (n , vector <double> (n , 0.0));
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
            if (i == j)
                res[i][j] = 1;
    }
    return res;
}

vector <vector <double> > Subtract_Matrices(vector <vector<double> > a , vector <vector<double> > b)
{
    vector <vector <double> > res (a.size() , vector <double> (a.size()));
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = 0; j < b.size(); j++)
            res[i][j] = a[i][j] - b[i][j];
    }
    return res;
}

vector <double> matrix_times_vector(vector <vector<double> > a , vector <double> b)
{
    vector <double> res (a.size() , 0);
    for (int i = 0; i < a.size(); i++)
    {
        for (int j = 0; j < b.size(); j++)
            res[i] +=  a[i][j] * b[j];
    }
    return res;
}

vector <vector <double> > transposeMatrix(vector <vector<double> > m)
{
    vector<vector<double> > res( m[0].size() , vector <double> (m.size(), 0)); 
    for (int i = 0; i < m.size(); i++)
    {
        for (int j = 0; j < m[i].size(); j++)
            res[j][i] = m[i][j];
    }
    return res;
}

vector <vector <double> > getMatrixMinor(vector <vector<double> > m , int Row , int Col)
{
    vector<vector<double> > res;
    for (int i = 0; i < m.size(); i++)
    {
        vector <double> temp;
        if (i == Row) continue;
        for (int j = 0; j < m[i].size(); j++)
        {
            if (j == Col) continue;
            temp.push_back(m[i][j]);
        }
        res.push_back(temp);
    }
    return res;
}

double getDeterminant(vector <vector<double> > m)
{
    if (m.size() == 2)
        return m[0][0] * m[1][1] - m[0][1] * m[1][0];
    double det = 0.0;
    for (int i = 0; i < m.size(); i++)
    {
        double temp = pow(-1.0 , i);
        det += temp * m[0][i] * getDeterminant(getMatrixMinor(m , 0 , i));
    }
    return det;
}

vector <vector <double> > getMatrixInverse(vector <vector<double> > m)
{
    vector <vector <double> > res, cofactors;
    double det = getDeterminant(m);
    for (int i = 0; i < m.size(); i++)
    {
        vector <double> temp_row;
        for (int j = 0; j < m.size(); j++)
        {
            vector <vector <double> > Minor = getMatrixMinor(m , i , j);
            double exp = pow(-1.0, i + j);
            temp_row.push_back(exp * getDeterminant(Minor));
        }
        cofactors.push_back(temp_row);
    }
    res = transposeMatrix(cofactors);
    for (int i = 0; i < res.size(); i++)
    {
        for (int j = 0; j < res.size(); j++)
            res[i][j] /= det;
    }
    return res;
}

int main()
{
    string str;
    ifstream file("Test2.txt");
    int select_mode;
    cout << "Manual input or reading from a file?\nManual input : 1\nReading from a file : 2\n";
    cin >> select_mode;
    cout << "Enter number of sectors: ";
    cin >> N;
    if (select_mode == 1)
    {
        vector <double> primary_inputs(N , 1) , output_of_primary_inputs(N);
        int amount_of_sectors = N, amount_of_primary_inputs;
        vector <vector <double> > technology(amount_of_sectors, vector <double> (amount_of_sectors));
        cout << "Input Technology Matrix:\n";
        for (int i = 0; i < amount_of_sectors; i++)
            for (int j = 0; j < amount_of_sectors; j++)
                cin >> technology[i][j];
        cout << "Input Demand Vector :\n";
        for (int i = 0; i < amount_of_sectors; i++)
        {
            double num;
            cin >> num;
            final_demand.push_back(num);
        }
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                primary_inputs[j] -= technology[i][j];
        }
        output = matrix_times_vector(getMatrixInverse(Subtract_Matrices(Identity(amount_of_sectors) , technology)), final_demand);
        for (int i = 0; i < N; i++)
            output_of_primary_inputs[i] = primary_inputs[i] * output[i];
        cout << "Total required output of sectors is : ";
        for (int i = 0; i < output.size(); i++)
            cout << fixed << setprecision(2) << output[i] << ' ';
        cout << '\n';
        cout << "Total required output of primary inputs is : ";
        for (int i = 0; i < N; i++)
            cout << fixed << setprecision(2) << output_of_primary_inputs[i] << ' ';
        cout << '\n';
    }
    else if (select_mode == 2)
    {
        vector <double> primary_inputs(N , 1) , output_of_primary_inputs(N);
        for (int i = 0; i < N; i++)
        {
            vector <double> temp;
            for (int j = 0; j < N + 1; j++)
            {
                string ch;
                file >> ch;
                for (int k = 0; k < ch.size(); k++)
                    if (ch[k] == ',') ch[k] = '.';
                if (j == N)
                    final_demand.push_back(stod(ch));
                else 
                    temp.push_back(stod(ch));
         
            }
            tech_matrix.push_back(temp);
        }
        output = matrix_times_vector(getMatrixInverse(Subtract_Matrices(Identity(N) , tech_matrix)), final_demand);
        for (int i = 0; i < N; i++)
        {
            for (int j = 0; j < N; j++)
                primary_inputs[j] -= tech_matrix[i][j];
        }
        for (int i = 0; i < N; i++)
            output_of_primary_inputs[i] = primary_inputs[i] * output[i];
        cout << "Total required output of sectors is : ";
        for (int i = 0; i < N; i++)
            cout << fixed << setprecision(2) << output[i] << ' ';
        cout << '\n';
        cout << "Total required output of primary inputs is : ";
        for (int i = 0; i < N; i++)
            cout << fixed << setprecision(2) << output_of_primary_inputs[i] << ' ';
        cout << '\n';
    }
    else 
        cout << "Invalid operation!\n";
    return 0;
}
