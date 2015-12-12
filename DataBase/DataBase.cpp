//============================================================================
// Name        : DataBase.cpp
// Author      : Real32
// Version     :
// Copyright   : Just say "thank you" to me. Free
// Description : Data base (hash table method). C++
//============================================================================
#include <iostream>
#include <fstream>
#include <memory.h>
using namespace std;


class DataBase
{
private:
struct data_zoo
{
  char param[12];
  char diet_type[3];
};

struct data_cage
{
  char param[12];
  char gender[2];
  char old[3]; // 3 ???
  char weight[5];
};
  unsigned long dictionary[1024];
public:
  DataBase();
  ~DataBase(){delete[]dictionary;}
  int hashing(char key[]);
  void add_ElementToZoo(char *file_name);
  void show_ElementsZoo(char *file_name);
  bool search_ElementZoo(char *file_name);
  bool search_ElementZoo(char *file_name, char *search_param);
  void delete_ElementZoo(char *file_name);

  void save_base(char *file_name);
  void load_base(char *file_name);

  void add_ElementToCage(char *file_name);
  void show_ElementsCage(char *file_name);
  bool search_ElementCage(char *file_name);
  void delete_ElementCage(char *file_name);
  void edit_ElementCage(char *file_name);
};

class GUI: public DataBase
{
private:
  int choice;
public:
  GUI(){ choice = 0;}
   void fill_zooTable();
   void fill_cageTable();
};

bool FileExists(const char *file_name)
{
    if(ifstream(file_name) == NULL)
    {
      cout<<"File "<<file_name<<" created"<<endl;
      ofstream newborn(file_name, ios::out | ios::binary);
    }
  return std::ifstream(file_name) != NULL;
}


DataBase::DataBase()
{
 for(int i=0; i < 1024; i++)
   dictionary[i]=0;
}

int DataBase::hashing(char key[])
{
 /*
  int hash = 0;
  char *p;
  for (p = key; *p!='\0'; p++)
  {
    hash = hash * 31 + (int)*p;  // 35 - +!
  }
  return (hash & 0x7fffffff) % 10;
  */

  int hash = 0;
  for (int i = 0; key[i]; i++)
    {
      hash = (hash * 35) + (unsigned int)key[i];
     // hash ++;//   37
    }
  return (hash & 0x7fffffff) % 32; // 32 the best!
}

void DataBase::add_ElementToZoo(char *file_name)
{
  data_zoo* page=NULL;
  page = new data_zoo;
  memset(page, 0, sizeof(data_zoo));
  FileExists(file_name);
  fstream in(file_name, ios::in | ios::out | ios::binary);

  in.clear();
  int i = 1;
  char fbuffer[12] ={""};
  cout<<"Enter type of animal: ";
  cin >>page->param;
  int shift = hashing(page->param); // shift = hash
  int position = shift*sizeof(data_zoo);
  cout<<"Enter diet type (predator - prd | herbivorous - hrb): ";
  cin >>page->diet_type;
  if(dictionary[shift] == 0)
  {
    dictionary[shift] = position;
    in.seekg(position, in.beg);
    in.write(page->param, sizeof(page->param));
    in.write(page->diet_type, sizeof(page->diet_type));
  }
  else if(dictionary[shift] == position)
  {
    in.clear();
    in.seekg(position, in.beg);
    in.read(fbuffer, sizeof(fbuffer));
    if(!strcmp(fbuffer, page->param))
    {
      cout<<"Animal is already in cage"<<endl;
    }
      else
      {
        while( dictionary[shift]!=0 && shift < 1024)
        {
          shift = shift + 35*i;
          position = shift * sizeof(data_zoo);
          i++;
        }
        cout << "Collision detected! new hash "<<shift<<" position: "<<position<<endl;
        dictionary[shift] = position;
        in.seekg(position, in.beg);
        in.write(page->param, sizeof(page->param));
        in.write(page->diet_type, sizeof(page->diet_type));
      }
  }
  delete[]fbuffer;
  delete[]page; // покушал - убери
  in.close();
}

void DataBase::show_ElementsZoo(char *file_name)
{
  FileExists(file_name);
  ifstream in(file_name, ios::in | ios::binary);

  for(int i = 0; i < 1024; i++)
  {
    if(dictionary[i]!=0)
    {
       data_zoo* page=NULL;
       page = new data_zoo;
       in.clear();
       in.seekg(dictionary[i], in.beg);
       in.read(page->param, sizeof(page->param));
       in.read(page->diet_type,sizeof(page->diet_type));
       cout<<"Animal: "<<page->param<<" diet type: "<<page->diet_type<<endl;
       delete[]page;
    }
  }
  in.close();
}

bool DataBase::search_ElementZoo(char *file_name)
{
  FileExists(file_name);
  ifstream in(file_name, ios::in | ios::binary);

  char *search_param= new char[12];
  data_zoo* page=NULL;
  page = new data_zoo;
  memset(page, 0, sizeof(data_zoo));
  cout<<"Enter animal specie to search: ";
  cin >>search_param;
  int i = 1;
  int shift = hashing(search_param);
  bool true_search = 0;
  while( dictionary[shift]!=0 &&( shift < 1024 || !true_search ))
  {
    in.clear();
    in.seekg(dictionary[shift], in.beg);
    in.read(page->param, sizeof(page->param));
    in.read(page->diet_type,sizeof(page->diet_type));
    if(!strcmp(page->param, search_param))
    {
      cout<<"This is : "<<page->param<<" diet type: "<<page->diet_type<<endl;
      in.close();
      delete[]search_param;
      delete[]page;
      true_search=1;
      break;
    }
    else
    {
      shift = shift + 35*i;
      i++;
    }

  }

  if(!true_search)
  {
    delete[]search_param;
    delete[]page;
    cout<<"Bad request..."<<endl;
  }
  in.close();
  return true_search;
}


bool DataBase::search_ElementZoo(char *file_name, char *search_param)
{
  ifstream in(file_name, ios::in | ios::binary);
  data_zoo* page=NULL;
  page = new data_zoo;
  memset(page, 0, sizeof(data_zoo));
  int i = 1;
  int shift = hashing(search_param);
  bool true_search = 0;
  while( dictionary[shift]!=0 &&( shift < 1024 || !true_search ))
  {
    in.clear();
    in.seekg(dictionary[shift], in.beg);
    in.read(page->param, sizeof(page->param));
    in.read(page->diet_type,sizeof(page->diet_type));
    if(!strcmp(page->param, search_param))
    {
      //cout<<"Now in cage with: "<<page->param<<" - "<<page->diet_type<<endl;
      in.close();
      delete[]search_param;
      delete[]page;
      true_search=1;
      break;
    }
    else
    {
      shift = shift + 35*i;
      i++;
    }

  }

  if(!true_search)
  {
    delete[]search_param;
    delete[]page;
    //cout<<"Bad request..."<<endl;
  }
  in.close();
  return true_search;
}


void DataBase::delete_ElementZoo(char *file_name)
{
  FileExists(file_name);
  fstream in(file_name, ios::in | ios::out | ios::binary);

  char delete_param_dict[20];
  char base[6]=".base";
  char dict[6]=".dict";
  char delete_param[20];
  char *fbuffer = new char[12];
  char free_space[15] ={""};

  int i = 1;
  int position = 0;
  cout<<"Enter animal specie to delete: ";
  cin >>delete_param;
  int shift = hashing(delete_param);
  bool true_search = 0;
    while( dictionary[shift]!=0 && shift < 1024 || !true_search)
      {

        in.clear();
        in.seekg(dictionary[shift], in.beg);
        in.read(fbuffer, 12);
        if(!strcmp(fbuffer, delete_param))
        {
          in.clear();
          in.seekg(dictionary[shift], in.beg);
          in.write(free_space, sizeof(free_space));
          true_search=1;
          dictionary[shift]=0;
          cout<<delete_param<<" deleted!"<<endl;
          strcpy(delete_param_dict, delete_param);
          strcat(delete_param, base);
          strcat(delete_param_dict, dict);
          remove(delete_param);
          remove(delete_param_dict);
          in.close();
          delete[]fbuffer;
          delete[]delete_param;
          delete[]free_space;
          break;
        }
        else
        {
          shift = shift + 35*i;
          i++;
        }
      }

    if(!true_search)
    {
      cout<<"Bad request"<<endl;
      delete[]fbuffer;
      delete[]delete_param;
      delete[]free_space;
    }
  in.close();
}


void DataBase::add_ElementToCage(char *file_name)
{
  data_cage* page=NULL;
  page = new data_cage;
  memset(page, 0, sizeof(data_zoo));
  FileExists(file_name);
  fstream in(file_name, ios::in | ios::out | ios::binary);

  in.clear();
  int i = 1;
  char fbuffer[12] ={""};
  cout<<"Enter name of animal: ";
  cin >>page->param;
  int shift = hashing(page->param); // shift = hash
  int position = shift*sizeof(data_cage);
  cout<<"Enter gender: ";
  cin >>page->gender;
  cout<<"Enter old: ";
  cin >>page->old;
  cout<<"Enter weight: ";
  cin >>page->weight;
  if(dictionary[shift] == 0)
  {
    dictionary[shift] = position;
    in.seekg(position, in.beg);
    in.write(page->param, sizeof(page->param));
    in.write(page->gender, sizeof(page->gender));
    in.write(page->old, sizeof(page->old));
    in.write(page->weight, sizeof(page->weight));
  }
  else if(dictionary[shift] == position)
  {
    in.clear();
    in.seekg(position, in.beg);
    in.read(fbuffer, sizeof(fbuffer));

    if(!strcmp(fbuffer, page->param))
    {
      cout<<"Animal is already in cage"<<endl;
    }
      else
      {
        while( dictionary[shift]!=0 && shift < 1024)
        {
          shift = shift + 35*i;
          position = shift * sizeof(data_cage);
          i++;
        }
        cout << "Collision detected! new hash "<<shift<<" position: "<<position<<endl;
        dictionary[shift] = position;
        in.seekg(position, in.beg);
        in.write(page->param, sizeof(page->param));
        in.write(page->gender, sizeof(page->gender));
        in.write(page->old, sizeof(page->old));
        in.write(page->weight, sizeof(page->weight));
      }
  }
  delete[]fbuffer;
  delete[]page; // покушал - убери
  in.close();
}


void DataBase::show_ElementsCage(char *file_name)
{
  ifstream in(file_name, ios::in | ios::binary);

  for(int i = 0; i < 1024; i++)
  {
    if(dictionary[i]!=0)
    {
      data_cage* page=NULL;
      page = new data_cage;
      in.clear();
      in.seekg(dictionary[i], in.beg);
      in.read(page->param, sizeof(page->param));
      in.read(page->gender, sizeof(page->gender));
      in.read(page->old, sizeof(page->old));
      in.read(page->weight, sizeof(page->weight));
      cout<<endl<<"Name: "<<page->param<<" gender: "<<page->gender<<" old: "<<page->old<<" weight: "<<page->weight<<endl;
      delete[]page;
    }
  }
  in.close();
}

bool DataBase::search_ElementCage(char *file_name)
{
  ifstream in(file_name, ios::in | ios::binary);

  char *search_param= new char[12];
  data_cage* page=NULL;
  page = new data_cage;
  memset(page, 0, sizeof(data_zoo));
  cout<<"Enter animal name to search: ";
  cin >>search_param;
  int i = 1;
  int shift = hashing(search_param);
  bool true_search = 0;
  while( dictionary[shift]!=0 &&( shift < 1024 || !true_search ))
  {
    in.clear();
    in.seekg(dictionary[shift], in.beg);
    in.read(page->param, sizeof(page->param));
    in.read(page->gender, sizeof(page->gender));
    in.read(page->old, sizeof(page->old));
    in.read(page->weight, sizeof(page->weight));
    if(!strcmp(page->param, search_param))
    {
      cout<<"This is : "<<page->param<<" gender: "<<page->gender<<" old: "<<page->old<<" weight: "<<page->weight<<endl;
      in.close();
      delete[]search_param;
      delete[]page;
      true_search=1;
      break;
    }
    else
    {
      shift = shift + 35*i;
      i++;
    }

  }

  if(!true_search)
  {
    delete[]search_param;
    delete[]page;
    cout<<"Bad request..."<<endl;
  }
  in.close();
  return true_search;
}

void DataBase::delete_ElementCage(char *file_name)
{
  fstream in(file_name, ios::in | ios::out | ios::binary);

  char *delete_param= new char[12];
  char *fbuffer = new char[12];
  char free_space[21] ={""};
  int i = 1;
  int position = 0;
  cout<<"Enter animal specie to delete: ";
  cin >>delete_param;
  int shift = hashing(delete_param);
  bool true_search = 0;
    while( dictionary[shift]!=0 && shift < 1024 || !true_search)
      {

        in.clear();
        in.seekg(dictionary[shift], in.beg);
        in.read(fbuffer, 12);
        if(!strcmp(fbuffer, delete_param))
        {
          in.clear();
          in.seekg(dictionary[shift], in.beg);
          in.write(free_space, sizeof(free_space));
          true_search=1;
          dictionary[shift]=0;
          cout<<delete_param<<" deleted!"<<endl;
          in.close();
          delete[]fbuffer;
          delete[]delete_param;
          delete[]free_space;
          break;
        }
        else
        {
          shift = shift + 35*i;
          i++;
        }
      }

    if(!true_search)
    {
      cout<<"Bad request"<<endl;
      delete[]fbuffer;
      delete[]delete_param;
      delete[]free_space;
    }
  in.close();
}

void DataBase::edit_ElementCage(char *file_name)
{
  fstream in(file_name, ios::in | ios::out | ios::binary);

  char *search_param= new char[12];
  data_cage* page=NULL;
  page = new data_cage;
  memset(page, 0, sizeof(data_zoo));
  cout<<"Enter animal name to edit: ";
  cin >>search_param;
  int i = 1;
  int choice = 0;
  int shift = hashing(search_param);
  bool true_search = 0;
  while( dictionary[shift]!=0 &&( shift < 1024 || !true_search ))
  {
    in.clear();
    in.seekg(dictionary[shift], in.beg);
    in.read(page->param, sizeof(page->param));
    if(!strcmp(page->param, search_param))
    {
      cout<<"What exactly do you want to edit?"<<endl;
      cout<<" 1 - old  2 - weight "<<endl;
      cout<<"Select : ";
      cin >>choice;
        switch(choice)
        {
          case 1:
            cout<<"Enter a new old : ";
            cin >>page->old;
            in.clear();
            in.seekg(dictionary[shift] + sizeof(page->param) + sizeof(page->gender), in.beg);
            in.write(page->old, sizeof(page->old));
            break;
          case 2:
            cout<<"Enter a new weight : ";
            cin >>page->weight;
            in.clear();
            in.seekg(dictionary[shift] + sizeof(page->param) + sizeof(page->gender) + sizeof(page->old), in.beg);
            in.write(page->weight, sizeof(page->weight));
            break;
          default:
            cout << "Bad request..."<<endl;
            break;

        }
      in.close();
      delete[]search_param;
      delete[]page;
      true_search=1;
      break;
    }
    else
    {
      shift = shift + 35*i;
      i++;
    }

  }

  if(!true_search)
  {
    delete[]search_param;
    delete[]page;
    cout<<"Bad request..."<<endl;
  }
  in.close();
}

void  GUI::fill_zooTable()
{
   load_base("dictionary.dict");
    cout<<"-----Fill zooTable-----"<<endl;
    cout<<" '1' For adding a new animal"<<endl;
    cout<<" '2' For searching animal"<<endl;
    cout<<" '3' For deleting animal"<<endl;
    cout<<" '4' For show all list"<<endl;
    cout<<" '5' Help"<<endl;
    cout<<"Back to menu - press '0'"<<endl;
  while(true)
  {
    cout<<"Select : ";
    cin >> choice;
    switch(choice)
    {
      case 1:
        add_ElementToZoo("zootable.base");
        break;
      case 2:
        search_ElementZoo("zootable.base");
        break;
      case 3:
        delete_ElementZoo("zootable.base");
        break;
      case 4:
        show_ElementsZoo("zootable.base");
        break;
      case 5:
        cout<<"-----------------------------------"<<endl;
        cout<<" '1' For adding a new animal"<<endl;
        cout<<" '2' For searching animal"<<endl;
        cout<<" '3' For deleting animal"<<endl;
        cout<<" '4' For show all list"<<endl;
        cout<<" '5' Help"<<endl;
        cout<<"Back to menu - press '0'"<<endl;
        cout<<"-----------------------------------"<<endl;
        break;
      case 0:
        save_base("dictionary.dict");
        return;
      default:
        cout << "Bad request..."<<endl;
        break;
    }
    cout<<"-----------------------------------"<<endl;
    cout<<"Help - press '5'"<<endl;
    cout<<"-----------------------------------"<<endl;
  }
}


void  GUI::fill_cageTable()
{

    char search_param[20];
    char search_param_dict[20];
    char base[6]=".base";
    char dict[6]=".dict";
    load_base("dictionary.dict");
    bool check = 0;
    show_ElementsZoo("zootable.base");
    cout<<"What cage do you want?"<<endl;
    cout<<"Enter : ";
    cin >> search_param;

    if( search_ElementZoo("zootable.base", search_param))
    {
      check = 1;
      strcpy(search_param_dict, search_param);
      strcat(search_param, base);
      strcat(search_param_dict, dict);
      load_base(search_param_dict);
      cout<<"-----Fill cageTable-----"<<endl;
      cout<<" '1' For adding a new animal"<<endl;
      cout<<" '2' For searching animal"<<endl;
      cout<<" '3' For deleting animal"<<endl;
      cout<<" '4' For show all list"<<endl;
      cout<<" '5' For editing animal"<<endl;
      cout<<" '6' Help"<<endl;
      cout<<"Back to menu - press '0'"<<endl;
    }
    else
    {
      check = 0;
      cout<<"Sorry, this animal is absent in zoo"<<endl;
      return;
    }
  while(check)
  {
    cout<<"Select : ";
    cin >> choice;
    switch(choice)
    {
      case 1:
        add_ElementToCage(search_param);
        break;
      case 2:
        search_ElementCage(search_param);
        break;
      case 3:
        delete_ElementCage(search_param);
        break;
      case 4:
        show_ElementsCage(search_param);
        break;
      case 5:
        edit_ElementCage(search_param);
        break;
      case 6:
        cout<<"-----------------------------------"<<endl;
        cout<<" '1' For adding a new animal"<<endl;
        cout<<" '2' For searching animal"<<endl;
        cout<<" '3' For deleting animal"<<endl;
        cout<<" '4' For show all list"<<endl;
        cout<<" '5' For editing animal"<<endl;
        cout<<" '6' Help"<<endl;
        cout<<"Back to menu - press '0'"<<endl;
        cout<<"-----------------------------------"<<endl;
        break;
      case 0:
        save_base(search_param_dict);
        return;
      default:
        cout << "Bad request..."<<endl;
        break;
    }
    cout<<"-----------------------------------"<<endl;
    cout<<"Help - press '6'"<<endl;
    cout<<"-----------------------------------"<<endl;
  }
}

void DataBase::save_base(char *file_name)
{
  ofstream fdiction(file_name, ios::out | ios::binary);
  for(int i=0; i < 1024; i++)
  {
    fdiction.write((char*)&dictionary[i],sizeof(unsigned long));
  }
  fdiction.close();
}

void DataBase::load_base(char *file_name)
{
  FileExists(file_name);
  for(int i=0; i < 1024; i++)
  dictionary[i]=0;
  ifstream fdiction(file_name, ios::in | ios::binary);
  for(int i=0; i < 1024; i++)
  {
    fdiction.read((char*)&dictionary[i],sizeof(unsigned long));
    if(dictionary[i]!=0)
    cout<<"Saved hash: "<< i<<" Position: "<< dictionary[i]<<endl;
  }

  fdiction.close();
}

int main()
{
  GUI gui;
  int choice = 0;
  while(true)
  {
    cout<< " For fill zooTable press '1' | For fill cages press '2' "<<endl;
    cout<< " To exit press '0' "<<endl;
    cout<<" Select : ";
    cin >> choice;
    switch (choice)
    {
      case 1:
          cout<<endl<< "Now you are in zooTable"<<endl<<endl;
           gui.fill_zooTable();
          break;
      case 2:
          cout <<endl<< "Now you are in cageTable"<<endl<<endl;
          gui.fill_cageTable();
          break;
      case 0:
          return 0;
      default:
          cout << "Bad request...";
          break;
    }
  }
}
