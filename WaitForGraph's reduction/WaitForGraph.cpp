//============================================================================
// Name        : WaitForGraph.cpp
// Author      : Real32
// Version     :
// Copyright   :
// Description : C++
//============================================================================
#include <iostream>
#include <fstream>
#include <string>

#define steps 2

using namespace std;

enum locks
{
  NONE,
  S,
  X
};

class TandR
{
private:
  int line_size;
  int column_size;
  int **trn_matrix; //  transaction matrix
  int **rqt_matrix; //  request matrix
  bool *check_steps;
public:
  TandR();
 // ~TandR();
  void load_graph(char *file_graph, char *file_wants);
  void show_graph();
  void find_deadlock();
};

TandR::TandR()
{
	line_size = 0;
	column_size = 0;
	check_steps = new bool[steps];
	for(int i = 0; i < steps; i++)
	{
	  check_steps[i] = true;
	}
}

void TandR::load_graph(char *file_graph, char *file_wants)
{
  fstream trn_file(file_graph, ios::in);
  fstream request_file(file_wants, ios::in);
  if((!trn_file.good())||(!request_file.good()))
  {
	  cout<<"Error opening files!"<<endl;
  }
  request_file.clear();
  trn_file.clear();
  string buffer;

  int i = 0;
  int j = 0;
  while(trn_file.good())
  {
    trn_file>>buffer;
    j++;
  }
   trn_file.clear();
   trn_file.seekg(0, trn_file.beg);
  while(trn_file.good())
  {
    getline(trn_file, buffer );
    i++;
  }
  j = j / i;
  cout << i <<" "<< j <<endl;
  line_size = i;
  column_size = j;
  trn_matrix = new int*[i]; //2d array initial
  for(int _i = 0; _i < i; _i++)
    {
      trn_matrix[_i] = new int[j];
    }
  trn_file.clear();
  trn_file.seekg(0, trn_file.beg);
  while(trn_file.good())
  {
    for( int _i = 0; _i < i; _i++)
    {
      for( int _j = 0; _j< j; _j++)
      {
        trn_file>>trn_matrix[_i][_j];
      }
    }
  }

  trn_file.close();
  i = 0;
  j = 0;
  while(request_file.good())
  {
    request_file>>buffer;
    j++;
  }
  request_file.clear();
  request_file.seekg(0, request_file.beg);
  while(request_file.good())
  {
    getline(request_file, buffer );
    i++;
  }
  j = j / i;
  cout << i <<" "<< j <<endl;
  rqt_matrix = new int*[i];//2d array initial
  for(int _i = 0; _i < i; _i++)
  {
    rqt_matrix[_i] = new int[j];
  }
  request_file.clear();
  request_file.seekg(0, request_file.beg);
  while(request_file.good())
  {
    for( int _i = 0; _i < i; _i++)
    {
      for( int _j = 0; _j< j; _j++)
      {
        request_file>>rqt_matrix[_i][_j];
      }
    }
  }
  request_file.close();
}


void TandR::show_graph()
{
  cout<<"Transactions: "<<endl;
  for( int i = 0; i < line_size; i++)
  {
    for( int j = 0;  j < column_size; j++)
    {
      switch(trn_matrix[i][j])
      {
      case NONE:
        cout<<"T"<<i<<"        "<<"A"<<j<<endl;
        break;
      case S:
        cout<<"T"<<i<<" -(S)-> "<<"A"<<j<<endl; // show graph like T -> A
        break;
      case X:
        cout<<"T"<<i<<" -(X)-> "<<"A"<<j<<endl; // show graph like T -> A
        break;
      }
    }
  }
  cout<<"Requests: "<<endl;
  for( int j = 0; j < line_size; j++)
  {
    for( int i = 0;  i < column_size; i++)
    {
      switch(rqt_matrix[i][j])
      {
      case NONE:
        cout<<"T"<<j<<"        "<<"A"<<i<<endl;
        break;
      case S:
        cout<<"T"<<j<<" <-(S)- "<<"A"<<i<<endl; // show graph like T -> A
        break;
      case X:
        cout<<"T"<<j<<" <-(X)- "<<"A"<<i<<endl; // show graph like T -> A
        break;
      }
    }
  }
  cout<<"------------------"<<endl;
}

bool check_state(int *state, int state_size)
{
  bool check = false;
  for(int i = 0; i < state_size; i++)
  {
    if(state[i]==true)
    {
      check = true;
    }
    else
    {
      check = false;
      break;
    }
  }
  return check;
}

bool check_state(bool *state, int state_size)
{
  bool check = false;
  for(int i = 0; i < state_size; i++)
  {
	if(state[i]==true)
    {
      check = true;
    }
    else
    {
      check = false;
      break;
    }
  }
  return check;
}

void TandR::find_deadlock()
{
  int* null_column = new int[line_size]; // line_size > column_size
  int* X_capture = new int[line_size];
  int* S_capture = new int[line_size];
  while(check_state(check_steps, steps))
  {
	  check_steps[0] = false;
	  check_steps[1] = false;
    for( int j = 0; j < line_size; j++) // поиск столбца, в котором только нули. поиск транзакции,
    {									// которой ничего не нужно
      null_column[j] = true;
      for( int i = 0;  i < column_size; i++)
      {
        if (rqt_matrix[i][j] != 0)
        {
          null_column[j] = false;
          break;
        }
      }
    }

    for( int i = 0;  i < column_size; i++) // поиск строки, в которой есть Х. поиск захватов,
    {									   // с Х режимом
    	X_capture[i] = true;
      for( int j = 0;  j < line_size; j++)
      {
        if (rqt_matrix[i][j] == X)
        {
        	X_capture[i] = false;
        	cout<<i<<" X"<<endl;
          break;
        }
      }
    }

    for( int i = 0;  i < column_size; i++) // поиск строки, в которой есть S. поиск захватов,
    {									   // с S режимом
    	S_capture[i] = true;
      for( int j = 0;  j < line_size; j++)
      {
        if (rqt_matrix[i][j] == S)
        {
        	S_capture[i] = false;
        	cout<<i<<" S"<<endl;
          break;
        }
      }
    }

    for(int i = 0; i < line_size; i++) //из графа ожидания удаляются все дуги, исходящие из вершин-транзакций,
    {                                 //в которые не входят дуги из вершин-объектов
      if(null_column[i])
      {
        for( int j = 0;  j < column_size; j++)
        {
          trn_matrix[i][j]=NONE;
          null_column[i]= false;
        }
        check_steps[0] = true;
      }
    }

   show_graph();

    for( int i = 0; i < line_size; i++) // есть ли захват
    {
      null_column[i] = true;
      for( int j = 0;  j < column_size; j++)
      {
        if (trn_matrix[i][j] != 0)
        {
          null_column[j] = false;
          break;
        }
      }
    }

 //   for(int i = 0; i<line_size; i++)
 //   	cout<<X_capture[i];

    for( int j = 0;  j < column_size; j++)
      {
        if(null_column[j])
        {
          for( int i = 0; i < line_size; i++)
          {
        	 if(X_capture[j]!=S_capture[j])
             {
			   trn_matrix[i][j]=rqt_matrix[j][i];
			   rqt_matrix[j][i]=NONE; 		// "разворачиваем" дугу
			   check_steps[1] = true;
             }
          }
        }
      }

  show_graph();

    if(!check_state(check_steps, steps))
    {

      cout<<"Deadlock detected!"<<endl;
      cout<<"step 1 - "<<check_steps[0]<<" "<<"step 2 - "<<check_steps[1]<<endl;
    }
    else
    {
      cout<<"Transaction complete!"<<endl;
      cout<<"step 1 - "<<check_steps[0]<<" "<<"step 2 - "<<check_steps[1]<<endl;
      break;
    }
  }
}

int main()
{
  TandR tr;
  tr.load_graph("graph.txt", "wants.txt");
  tr.show_graph();
  tr.find_deadlock();
    return 0;
}
