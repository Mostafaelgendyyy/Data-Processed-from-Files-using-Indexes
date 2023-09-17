#include <iostream>
#include <fstream>
#include <sstream>
#include<bits/stdc++.h>
using namespace std;

struct Employee{
    char Employee_ID[13];
    char Dept_ID[30];
    char Employee_Name[50];
    char Employee_Position[50];
}Pri;

struct Department{
    char Dept_ID[30];
    char Dept_Name[50];
    char Dept_Manager[50];
};

struct SIndex{
    int ptr;
    char Dept_ID[30];
};

struct DSIndex{
    int ptr;
    char Dept_Name[30];
};

struct linkedList{
    int ptr1;
    char Employee_ID[13];
    int ptr2;
};

struct DlinkedList{
    int ptr1;
    char Dept_ID[30];
    int ptr2;
};

struct  DPIndex {
	int ByteOffset;
	char Dept_ID[30];
};

struct  PIndex {
	int ByteOffset;
	char Employee_ID[13];
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
*                                                                   *
*              Get the Length Indicator in String                   *
*                                                                   *
********************************************************************/
string lengthindicator(int size){
    char sinchar[10];
    itoa(size,sinchar,10);
    string indicator=string(sinchar);
    if(size<10){
            indicator="0"+indicator;
    }
    return indicator;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
*                                                                   *
*Search in Primary and Secondary indexes in Department and Emoployee*
*                                                                   *
********************************************************************/
int Searchsec(vector<SIndex>Allsec,SIndex key){
 for(int i=0;i<Allsec.size();i++){
    if(strcmp(Allsec[i].Dept_ID,key.Dept_ID)==0){
        return Allsec[i].ptr;
    }
 }
 return -1;
}

int DepartmentSearchsec(vector<DSIndex>Allsec,DSIndex key){
 for(int i=0;i<Allsec.size();i++){
    if(strcmp(Allsec[i].Dept_Name,key.Dept_Name)==0){
        return Allsec[i].ptr;
    }
 }
 return -1;
}

int SearchinPrimary(vector<PIndex>Allpri, int Left, int Right, PIndex Pri){
     if (Right >= Left) {
        int mid = Left + (Right - Left) / 2;
        if (strcmp(Allpri[mid].Employee_ID,Pri.Employee_ID)==0)
            return Allpri[mid].ByteOffset;

        if (strcmp(Allpri[mid].Employee_ID,Pri.Employee_ID)>0)
            return SearchinPrimary(Allpri, Left, mid - 1, Pri);

        return SearchinPrimary(Allpri, mid + 1, Right, Pri);
    }
    return -1;
}

int SearchinPrimaryDept(vector<DPIndex>Allpri, int Left, int Right, DPIndex Pri){
     if (Right >= Left) {
        int mid = Left + (Right - Left) / 2;
        if (strcmp(Allpri[mid].Dept_ID,Pri.Dept_ID)==0)
            return Allpri[mid].ByteOffset;

        if (strcmp(Allpri[mid].Dept_ID,Pri.Dept_ID)>0)
            return SearchinPrimaryDept(Allpri, Left, mid - 1, Pri);

        return SearchinPrimaryDept(Allpri, mid + 1, Right, Pri);
    }
    return -1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
*                                                                   *
*      Add Employee and delete Employee and print Employee          *
*                                                                   *
********************************************************************/
void AddNewEmployee(Employee emp,fstream & MyFile,int Size,fstream&PrimaryIndex,fstream&SecondaryIndex,fstream&LinkedFile,fstream &cnt,vector<SIndex>arr){
    PIndex pri;
    MyFile.open("Employee.txt");
    PrimaryIndex.open("primary.txt");
    MyFile.seekp(0,ios::end);
    PrimaryIndex.seekp(0,ios::end);
    pri.ByteOffset=MyFile.tellp(); //////
    string delimeter="/";
    MyFile<<lengthindicator(Size);
    MyFile<<emp.Dept_ID;
    MyFile<<delimeter;
    MyFile<<emp.Employee_ID;
    MyFile<<delimeter;
    MyFile<<emp.Employee_Name;
    MyFile<<delimeter;
    MyFile<<emp.Employee_Position;
    MyFile<<delimeter;
    MyFile.close();
    strcpy(pri.Employee_ID,emp.Employee_ID);
    PrimaryIndex<<pri.Employee_ID;
    PrimaryIndex<<delimeter;
    PrimaryIndex<<lengthindicator(pri.ByteOffset);
    PrimaryIndex<<endl;
    PrimaryIndex.close();
    SecondaryIndex.open("secondary.txt");
    LinkedFile.open("linkedList.txt");
    cnt.open("counter.txt");
    string str="";
    int Count;
    cnt>>str;
    cnt.close();
    cnt.open("counter.txt");
    if(str.length()!=0){
        Count = atoi(str.c_str());
        Count++;
        cnt.seekp(0,ios::beg);
        string StrSize=lengthindicator(Count);
        cnt<<StrSize;
    }
    else{
        Count=0;
        cnt.seekp(0,ios::beg);
        cnt<<"01";
    }
    cnt.close();
    SIndex secondary;
    strcpy(secondary.Dept_ID,emp.Dept_ID);
    secondary.ptr=0;
    int Header=Searchsec(arr,secondary);
    if(Header==-1){
        SecondaryIndex.seekp(0,ios::end);
        LinkedFile.seekp(0,ios::end);
        SecondaryIndex<<emp.Dept_ID;
        SecondaryIndex<<"/";
        SecondaryIndex<<lengthindicator(Count);
        SecondaryIndex<<endl;
        SecondaryIndex.close();
        LinkedFile<<lengthindicator(Count);
        LinkedFile<<"/";
        LinkedFile<<emp.Employee_ID;
        LinkedFile<<"/";
        LinkedFile<<"-1";
        LinkedFile<<endl;
        LinkedFile.close();
        cout<<"sucessful"<<endl;
    }
    else{
        string str1="";
        while(LinkedFile>>str1){
            string sec="";
            int sizestr=str1.length();
            if(str1[sizestr-2]=='-'){
               sec+=str1[sizestr-2];
               sec+=str1[sizestr-1];
            }
            string sec2="";
            int f=str1.find('/');
            sec2+=str1.substr(0,f);
            if(sec=="-1"&&sec2==lengthindicator(Header)){
                LinkedFile.seekp(-3,ios::cur);
                LinkedFile<<lengthindicator(Count);
                Header=str1[0]-'0';
                cout<<"DONE"<<endl;
                break;
            }
            else if(sec2==lengthindicator(Header)){
                string n="";
                n+=str1[sizestr-2];
                n+=str1[sizestr-1];
                Header=atoi(n.c_str());
            }

        }

                LinkedFile.seekp(0,ios::end);
                LinkedFile<<lengthindicator(Count);
                LinkedFile<<"/";
                LinkedFile<<emp.Employee_ID;
                LinkedFile<<"/";
                LinkedFile<<"-1";
                LinkedFile<<endl;
                LinkedFile.close();
    }
}

void PrintEMPbyID(vector<PIndex>Allpri,char id[],int n,fstream&File){
PIndex Pri;
strcpy(Pri.Employee_ID,id);
Pri.ByteOffset=1;
int sret=SearchinPrimary(Allpri,0,n-1,Pri);
File.open("Employee.txt");
File.seekg(sret,ios::beg);
char chr;
string str="";
int numDelimeters=0;
while(File>>chr){
    if(chr=='/'){
        cout<<str<<"  ";
        str="";
        numDelimeters++;
    }
    else{
        str+=chr;
    }
    if(numDelimeters==4){
        break;
    }
}
cout<<endl;
File.close();
}

void deleteEmployee(vector<PIndex>Allpri,char id[],int n,fstream&File , fstream&PriFile){
    File.open("Employee.txt");
    PIndex inst;
    strcpy(inst.Employee_ID,id);
    inst.ByteOffset=1;
    int sret=SearchinPrimary(Allpri,0,n-1,inst);
    if(sret == -1){
     cout<<"Employee isn't Exist"<<endl;
     return ;
    }
    File.seekg(sret,ios::beg);
    File<<"*";
    File.close();
    cout<<"Successfully Deleted"<<endl;

    fstream NewFile;
    NewFile.open("test.txt",ios::out);
    PriFile.open("primary.txt");
    PIndex k;
    string str;
    while(PriFile>>str){
        int c=find(str.begin(),str.end(),'/')-str.begin();
        string cc=str.substr(0,c);
        if(id!=cc){
            NewFile<<str;
            NewFile<<'\n';
        }
    }
    NewFile.close();
    PriFile.close();
    char oldname[]="test.txt";
    char newname[]="primary.txt";
    remove(newname);
    rename(oldname,newname);
}

vector<string> GetEmployeesWorksonDept(vector<SIndex>Allsec,char key[],fstream&LinkedFile){
vector<string>a;
SIndex sec;
strcpy(sec.Dept_ID,key);
sec.ptr=0;
int Founder=Searchsec(Allsec,sec);
if(Founder==-1){
    cout<<"Not Found"<<endl;
}
else{
   LinkedFile.open("linkedList.txt");
   string str;
   while(LinkedFile>>str){
    int index=str.find('/');
    string cur=str.substr(0,index);
    int point=atoi(cur.c_str());
    string str="";
    string str2="";
    int count=0;
    if(point == Founder){
        for(int i=index+1;i<str.length();i++){
            if(str[i]!='/'&&count==0){
                str+=str[i];
            }
            else if(str[i]!='/'&&count==1){
                str2+=str[i];
            }
            else{
                count++;
            }
        }
        a.push_back(str);
        if(str2=="-1"){
            return a;
        }
        point=atoi(str2.c_str());
        Founder=point;
    }
   }

}
}

vector<string> GetallDept(vector<DSIndex>Allsec,char key[],fstream&LinkedFile){
vector<string>a;
DSIndex Dsec;
strcpy(Dsec.Dept_Name,key);
Dsec.ptr=0;
int Founder=DepartmentSearchsec(Allsec,Dsec);
if(Founder==-1){
    cout<<"Not Found"<<endl;
}
else{
   LinkedFile.open("DlinkedList.txt");
   string str;
   while(LinkedFile>>str){
    int index=str.find('/');
    string cur=str.substr(0,index);
    int point=atoi(cur.c_str());
    string T1="";
    string T2="";
    int count=0;
    if(point == Founder){
        for(int i=index+1;i<str.length();i++){
            if(str[i]!='/'&&count==0){
                T1+=str[i];
            }
            else if(str[i]!='/'&&count==1){
                T2+=str[i];
            }
            else{
                count++;
            }
        }
        a.push_back(T1);
        if(T2=="-1"){
            return a;
        }
        point=atoi(T2.c_str());
        Founder=point;
    }
   }

}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
*                                                                   *
*    Add Department and delete Department and print Department      *
*                                                                   *
********************************************************************/
void AddNewDepartment(Department Dept,fstream & MyFile,int Size,fstream&PrimaryIndex,fstream&SecondaryIndex,fstream&LinkedFile,fstream&cnt,vector<DSIndex>arr){
    DPIndex dpri;
    MyFile.open("Dept.txt");
    PrimaryIndex.open("primaryDep.txt");
    MyFile.seekp(0,ios::end);
    PrimaryIndex.seekp(0,ios::end);
    dpri.ByteOffset=MyFile.tellp();
    string delimeter="/";
    MyFile<<lengthindicator(Size);
    MyFile<<Dept.Dept_ID;
    MyFile<<delimeter;
    MyFile<<Dept.Dept_Name;
    MyFile<<delimeter;
    MyFile<<Dept.Dept_Manager;
    MyFile<<delimeter;
    MyFile.close();
    strcpy(dpri.Dept_ID,Dept.Dept_ID);
    PrimaryIndex<<dpri.Dept_ID;
    PrimaryIndex<<delimeter;
    PrimaryIndex<<lengthindicator(dpri.ByteOffset);
    PrimaryIndex<<endl;
    PrimaryIndex.close();

    SecondaryIndex.open("Dsecondary.txt");
    LinkedFile.open("DlinkedList.txt");
    cnt.open("Dcounter.txt");
    string str="";
    int Count;
    cnt>>str;
    cnt.close();
    cnt.open("Dcounter.txt");
    if(str.length()!=0){
        Count = atoi(str.c_str());
        Count++;
        cnt.seekp(0,ios::beg);
        string StrSize=lengthindicator(Count);
        cnt<<StrSize;
    }
    else{
        Count=0;
        cnt.seekp(0,ios::beg);
        cnt<<"01";
    }
    cnt.close();
    DSIndex secondary;
    strcpy(secondary.Dept_Name,Dept.Dept_Name);
    secondary.ptr=0;
    int Header=DepartmentSearchsec(arr,secondary);
    if(Header==-1){
        SecondaryIndex.seekp(0,ios::end);
        LinkedFile.seekp(0,ios::end);
        SecondaryIndex<<Dept.Dept_Name;
        SecondaryIndex<<"/";
        SecondaryIndex<<lengthindicator(Count);
        SecondaryIndex<<endl;
        SecondaryIndex.close();
        LinkedFile<<lengthindicator(Count);
        LinkedFile<<"/";
        LinkedFile<<Dept.Dept_ID;
        LinkedFile<<"/";
        LinkedFile<<"-1";
        LinkedFile<<endl;
        LinkedFile.close();
        cout<<"sucessful"<<endl;
    }
    else{
        string str1="";
        while(LinkedFile>>str1){
            string sec="";
            int sizestr=str1.length();

            if(str1[sizestr-2]=='-'){
               sec+=str1[sizestr-2];
               sec+=str1[sizestr-1];
            }
            string sec2="";
            int f=str1.find('/');
            sec2+=str1.substr(0,f);
            if(sec=="-1"&&sec2==lengthindicator(Header)){
                LinkedFile.seekp(-3,ios::cur);
                LinkedFile<<lengthindicator(Count);
                Header=str1[0]-'0';
                cout<<"DONE"<<endl;
                break;
            }
            else if(sec2==lengthindicator(Header)){
                string n="";
                n+=str1[sizestr-2];
                n+=str1[sizestr-1];
                Header=atoi(n.c_str());
            }

        }

                LinkedFile.seekp(0,ios::end);
                LinkedFile<<lengthindicator(Count);
                LinkedFile<<"/";
                LinkedFile<<Dept.Dept_ID;
                LinkedFile<<"/";
                LinkedFile<<"-1";
                LinkedFile<<endl;
                LinkedFile.close();
    }


}

void printDept(vector<DPIndex>Allpri,char id[],int n,fstream&File){
DPIndex DPri;
strcpy(DPri.Dept_ID,id);
DPri.ByteOffset=1;
int sret=SearchinPrimaryDept(Allpri,0,n-1,DPri);

File.open("Dept.txt");
File.seekg(sret,ios::beg);
char chr;
string data="";
int numDelimeters=0;
while(File>>chr){
    if(numDelimeters==3){
        break;
    }
    if(chr=='/'){
        cout<<data<<"  ";
        data="";
        numDelimeters++;
    }
    else{
        data+=chr;
    }

}
cout<<endl;
File.close();
}

void deleteDept(vector<DPIndex>Allpri,char id[],int n,fstream&File,fstream&PriFile){
File.open("Dept.txt");
DPIndex inst;
strcpy(inst.Dept_ID,id);
inst.ByteOffset=1;
int sret=SearchinPrimaryDept(Allpri,0,n-1,inst);
if(sret == -1){
 cout<<"This Department isn't Exist"<<endl;
 return ;
}
File.seekg(sret,ios::beg);
File<<"*";
File.close();
cout<<"Successfully Deleted"<<endl;

fstream NewFile;
NewFile.open("test.txt",ios::out);
PriFile.open("primaryDep.txt");
DPIndex pinst;
string str;
while(PriFile>>str){
    int c=find(str.begin(),str.end(),'/')-str.begin();
    string cc=str.substr(0,c);
    if(id!=cc){
        NewFile<<str;
        NewFile<<'\n';
    }
}
NewFile.close();
PriFile.close();
char oldname[]="test.txt";
char newname[]="primaryDep.txt";
remove(newname);
rename(oldname,newname);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
*                                                                   *
*    Write and Get data From Secondary index file and sort data     *
*                                                                   *
********************************************************************/
void WriteESecondary(SIndex secondaryArr[],int n,fstream&SecFile){
  SecFile.open("secondary.txt");
  SecFile.seekp(0,ios::beg);
  for(int i=0;i<n;i++){
    SecFile<<secondaryArr[i].Dept_ID;
    SecFile<<"/";
    SecFile<<secondaryArr[i].ptr;
    SecFile<<endl;
  }
  SecFile.close();
}

void WriteDSecondary(DSIndex secondaryArr[],int n,fstream&SecFile){
  SecFile.open("Dsecondary.txt");
  SecFile.seekp(0,ios::beg);
  for(int i=0;i<n;i++){
    SecFile<<secondaryArr[i].Dept_Name;
    SecFile<<"/";
    SecFile<<secondaryArr[i].ptr;
    SecFile<<endl;
  }
  SecFile.close();
}

vector<SIndex> GetEmpdatafromSecondary(fstream &SecondaryIndex){
  SecondaryIndex.open("secondary.txt");
  vector<SIndex> Employees;
  string secrecord;
  while(SecondaryIndex >> secrecord){
   int found=find(secrecord.begin(),secrecord.end(),'/')-secrecord.begin();
   SIndex s;
   strcpy(s.Dept_ID,secrecord.substr(0,found).c_str());
   s.ptr=atoi((secrecord.substr(found+1)).c_str());
   Employees.push_back(s);
  }
  SecondaryIndex.close();
  return Employees;
}

vector<DSIndex> GetDepartmentdatafromSecondary(fstream &SecondaryIndex){
  SecondaryIndex.open("Dsecondary.txt");
  vector<DSIndex> Employees;
  string secrecord;
  while(SecondaryIndex >> secrecord){
   int f=find(secrecord.begin(),secrecord.end(),'/')-secrecord.begin();
   DSIndex deptp;
   strcpy(deptp.Dept_Name,secrecord.substr(0,f).c_str());
   deptp.ptr=atoi((secrecord.substr(f+1)).c_str());
   Employees.push_back(deptp);
  }
  SecondaryIndex.close();
  return Employees;
}

void SecondaryQuickSort(SIndex secondaryArr[], int left, int right){
    int i = left, j = right;
    SIndex inst;
    string Mid= secondaryArr[(left + right) / 2].Dept_ID;
    while (i <= j) {
        while (secondaryArr[i].Dept_ID < Mid){i++;}
        while (secondaryArr[j].Dept_ID > Mid){j--;}
        if (i <= j) {
            inst = secondaryArr[i];
            secondaryArr[i] = secondaryArr[j];
            secondaryArr[j] = inst;
            i++;
            j--;
        }
    }
    if (left < j)
        SecondaryQuickSort(secondaryArr, left, j);
    if (i < right)
        SecondaryQuickSort(secondaryArr, i, right);
}

void DeptSecondaryQuickSort(DSIndex secondaryArr[], int left, int right){
    int i = left, j = right;
    DSIndex inst;
    string Mid= secondaryArr[(left + right) / 2].Dept_Name;
    while (i <= j) {
        while (secondaryArr[i].Dept_Name < Mid){i++;}
        while (secondaryArr[j].Dept_Name > Mid){j--;}
        if (i <= j) {
            inst = secondaryArr[i];
            secondaryArr[i] = secondaryArr[j];
            secondaryArr[j] = inst;
            i++;
            j--;
        }
    }
    if (left < j)
        DeptSecondaryQuickSort(secondaryArr, left, j);
    if (i < right)
        DeptSecondaryQuickSort(secondaryArr, i, right);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
*                                                                   *
*     Write and Get data From Primary index file and sort data      *
*                                                                   *
********************************************************************/
void WriteDeptPrimary(DPIndex primaryArr[],int n,fstream&PrimaryFile){
  PrimaryFile.open("primaryDep.txt");
  PrimaryFile.seekp(0,ios::beg);
  for(int i=0;i<n;i++){
    PrimaryFile<<primaryArr[i].Dept_ID;
    PrimaryFile<<"/";
    PrimaryFile<<primaryArr[i].ByteOffset;
    PrimaryFile<<endl;
  }
  PrimaryFile.close();
}

void WriteEmpPrimary(PIndex primaryArr[],int n,fstream&PrimaryFile){
  PrimaryFile.open("primary.txt");
  PrimaryFile.seekp(0,ios::beg);
  for(int i=0;i<n;i++){
    PrimaryFile<<primaryArr[i].Employee_ID;
    PrimaryFile<<"/";
    PrimaryFile<<primaryArr[i].ByteOffset;
    PrimaryFile<<endl;
  }
  PrimaryFile.close();
}

vector<PIndex> GetEmpdatafromPrimary(fstream &PriFile){

  PriFile.open("primary.txt");
  vector<PIndex> Employees;
  string allprirecord;
  while(PriFile >> allprirecord){
   int f=find(allprirecord.begin(),allprirecord.end(),'/')-allprirecord.begin();
   PIndex p;
   strcpy(p.Employee_ID,allprirecord.substr(0,f).c_str());
   p.ByteOffset=atoi((allprirecord.substr(f+1)).c_str());
   Employees.push_back(p);
  }
  PriFile.close();
  return Employees;
}

vector<DPIndex>GetDepartmentdatafromPrimary(fstream &PriFile){
  PriFile.open("primaryDep.txt");
  vector<DPIndex> Employees;
  string allprirecord;
  while(PriFile >> allprirecord){
   int f=find(allprirecord.begin(),allprirecord.end(),'/')-allprirecord.begin();
   DPIndex dept;
   strcpy(dept.Dept_ID,allprirecord.substr(0,f).c_str());
   dept.ByteOffset=atoi((allprirecord.substr(f+1)).c_str());
   Employees.push_back(dept);
  }
  PriFile.close();
  return Employees;
}

void EmpPrimaryQuickSort(PIndex primaryArr[], int left, int right){
    int i = left, j = right;
    PIndex inst;
    string mid= primaryArr[(left + right) / 2].Employee_ID;
    while (i <= j) {
        while (primaryArr[i].Employee_ID < mid){i++;}
        while (primaryArr[j].Employee_ID > mid){j--;}
        if (i <= j) {
            inst = primaryArr[i];
            primaryArr[i] = primaryArr[j];
            primaryArr[j] = inst;
            i++;
            j--;
        }
    }
    if (left < j)
        EmpPrimaryQuickSort(primaryArr, left, j);
    if (i < right)
        EmpPrimaryQuickSort(primaryArr, i, right);
}

void DeptPrimaryQuickSort(DPIndex primaryArr[], int left, int right){
    int i = left, j = right;
    DPIndex inst;
    string mid= primaryArr[(left + right) / 2].Dept_ID;
    while (i <= j) {
        while (primaryArr[i].Dept_ID < mid){i++;}
        while (primaryArr[j].Dept_ID > mid){j--;}
        if (i <= j) {
            inst = primaryArr[i];
            primaryArr[i] = primaryArr[j];
            primaryArr[j] = inst;
            i++;
            j--;
        }
    }
    if (left < j)
        DeptPrimaryQuickSort(primaryArr, left, j);
    if (i < right)
        DeptPrimaryQuickSort(primaryArr, i, right);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/********************************************************************
*                                                                   *
*               Query String Splitter Function                      *
*                                                                   *
********************************************************************/
vector<string> split(string str, char delimiter) {
  vector<string> internal;
  stringstream ss(str); // Turn the string into a stream.
  string tok;

  while(getline(ss, tok, delimiter)) {
    internal.push_back(tok);
  }

  return internal;
}

int main()
{
       fstream MyFile,PrimaryFile,SecondaryF,LinkedF,CountF;
       int choice;

       while(true){
            cout<<"1- Add New Employee"<<endl;
            cout<<"2- Add New Department"<<endl;
            cout<<"3- Delete Employee (ID)"<<endl;
            cout<<"4- Delete Department (ID)"<<endl;
            cout<<"5- print Employee (ID)"<<endl;
            cout<<"6- print Employee (DeptID)"<<endl;
            cout<<"7- print Department (ID)"<<endl;
            cout<<"8- print Department (name)"<<endl;
            cout<<"9- Write a Query"<<endl;
            cout<<"10- Exit"<<endl;
            cout<<"Enter your choose : ";
            cin>>choice;
            vector<SIndex>AllSecondary;
            vector<DSIndex>AllDeptSecondary;
            vector<PIndex>AllPrimary;
            vector<DPIndex>AllDeptPrimary;
            if(GetEmpdatafromSecondary(SecondaryF).size()!=0){
               AllSecondary=GetEmpdatafromSecondary(SecondaryF);
            }
            if(GetDepartmentdatafromSecondary(SecondaryF).size()!=0){
               AllDeptSecondary=GetDepartmentdatafromSecondary(SecondaryF);
            }
            if(GetEmpdatafromPrimary(PrimaryFile).size()!=0){
               AllPrimary=GetEmpdatafromPrimary(PrimaryFile);
            }
            if(GetDepartmentdatafromPrimary(PrimaryFile).size()!=0){
               AllDeptPrimary=GetDepartmentdatafromPrimary(PrimaryFile);
            }
            if(choice==1){
                 Employee emp;
                 cout<<"Enter Name of Employee : ";
                 cin.ignore();
                 string name;
                 getline(cin,name);
                 strcpy(emp.Employee_Name,name.c_str());
                 cout<<"Enter ID of Employee : ";
                 cin>>emp.Employee_ID;
                 cout<<"Enter ID of Department : ";
                 cin>>emp.Dept_ID;
                 cout<<"Enter Position of Employee : ";
                 cin>>emp.Employee_Position;
                 PIndex Pri;
                 strcpy(Pri.Employee_ID,emp.Employee_ID);
                 if(SearchinPrimary(AllPrimary,0,(AllPrimary.size()-1),Pri)!=-1){
                      cout<<"This Employee exists already"<<endl;
                 }
                 else {
                 int cur=strlen(emp.Dept_ID)+strlen(emp.Employee_ID)+strlen(emp.Employee_Name)+strlen(emp.Employee_Position)+4;
                 AddNewEmployee(emp,MyFile,cur,PrimaryFile,SecondaryF,LinkedF,CountF,AllSecondary);
                 PIndex cpyallpri[AllPrimary.size()];
                 SIndex cpyalldeptpri[AllSecondary.size()];
                 for(int i=0;i<AllPrimary.size();i++){
                    cpyallpri[i]=AllPrimary[i];
                 }
                 for(int i=0;i<AllSecondary.size();i++){
                    cpyalldeptpri[i]=AllSecondary[i];
                 }
                 EmpPrimaryQuickSort(cpyallpri,0,(AllPrimary.size()-1));
                 WriteEmpPrimary(cpyallpri,AllPrimary.size(),PrimaryFile);
                 SecondaryQuickSort(cpyalldeptpri,0,(AllSecondary.size()-1));
                 WriteESecondary(cpyalldeptpri,AllSecondary.size(),SecondaryF);
                 }
            }
            else if(choice==2){
                Department dept;
                cout<<"Enter Dept_ID : ";
                cin>>dept.Dept_ID;
                cout<<"Enter Dept_Name : ";
                cin.ignore();
                string name;
                getline(cin,name);
                strcpy(dept.Dept_Name,name.c_str());
                cout<<"Enter name of manager of Dept : ";
                string mng;
                getline(cin,mng);
                strcpy(dept.Dept_Manager,mng.c_str());
                DPIndex Pri;
                 strcpy(Pri.Dept_ID,dept.Dept_ID);
                 if(SearchinPrimaryDept(AllDeptPrimary,0,(AllDeptPrimary.size()-1),Pri)!=-1){
                      cout<<"This Department exists already"<<endl;
                 }
                 else {
                        int cur=strlen(dept.Dept_ID)+strlen(dept.Dept_Manager)+strlen(dept.Dept_Name)+3;
                        AddNewDepartment(dept,MyFile,cur,PrimaryFile,SecondaryF,LinkedF,CountF,AllDeptSecondary);
                         DPIndex arr[AllDeptPrimary.size()];
                         DSIndex Sarr[AllDeptSecondary.size()];
                         for(int i=0;i<AllDeptPrimary.size();i++){
                            arr[i]=AllDeptPrimary[i];
                         }
                         for(int i=0;i<AllDeptSecondary.size();i++){
                            Sarr[i]=AllDeptSecondary[i];
                         }
                         DeptPrimaryQuickSort(arr,0,(AllDeptPrimary.size()-1));
                         WriteDeptPrimary(arr,AllDeptPrimary.size(),PrimaryFile);
                         DeptSecondaryQuickSort(Sarr,0,(AllDeptSecondary.size()-1));
                         WriteDSecondary(Sarr,AllDeptSecondary.size(),SecondaryF);
                     }
             }
            else if(choice == 3){
                PIndex primary;
                cout<<"Enter ID of Employee : ";
                cin>>primary.Employee_ID;
                deleteEmployee(AllPrimary,primary.Employee_ID,AllPrimary.size(),MyFile,PrimaryFile);
            }
            else if(choice == 4){
               DPIndex deptprimary;
               cout<<"Enter ID of Department : ";
               cin>>deptprimary.Dept_ID;
                deleteDept(AllDeptPrimary,deptprimary.Dept_ID,AllDeptPrimary.size(),MyFile,PrimaryFile);
            }
            else if(choice == 5){
                PIndex primary;
                cout<<"Enter ID of Employee : ";
                cin>>primary.Employee_ID;
                if(SearchinPrimary(AllPrimary,0,(AllPrimary.size()-1),primary)!=-1){
                  PrintEMPbyID(AllPrimary,primary.Employee_ID,AllPrimary.size(),MyFile);
                }
                else{
                    cout<<"Employee with this ID isn't Exist"<<endl;
                }
            }
            else if(choice == 6){
                SIndex secondary;
               cout<<"Enter ID of Department of Employee : ";
               cin>>secondary.Dept_ID;
               vector<string>arr=GetEmployeesWorksonDept(AllSecondary,secondary.Dept_ID,LinkedF);
               PIndex ss;
               if(arr.size()==0){
                cout<<"Employee with this Department ID isn't Exist"<<endl;
               }
               for(int i=0;arr.size();i++){
                 strcpy(ss.Employee_ID,arr[i].c_str());
                 if(SearchinPrimary(AllPrimary,0,(AllPrimary.size()-1),ss)!=-1){
                   PrintEMPbyID(AllPrimary,ss.Employee_ID,AllPrimary.size(),MyFile);
                   cout<<"*******************************"<<endl;
                }
               }
            }
            else if(choice == 7){
                DPIndex deptprimary;
               cout<<"Enter ID of Department : ";
               cin>>deptprimary.Dept_ID;
               if(SearchinPrimaryDept(AllDeptPrimary,0,(AllDeptPrimary.size()-1),deptprimary)!=-1){
                  printDept(AllDeptPrimary,deptprimary.Dept_ID,AllDeptPrimary.size(),MyFile);
               }
               else{
                cout<<"Department with this ID isn't Exist"<<endl;
               }
            }
            else if(choice == 8){
                 DSIndex deptsecondary;
                 cout<<"Enter name of Departments : ";
                 cin>>deptsecondary.Dept_Name;
                 vector<string>arr=GetallDept(AllDeptSecondary,deptsecondary.Dept_Name,LinkedF);
                 DPIndex dd;
                 if(arr.size()==0){
                    cout<<"Department with this Name isn't Exist"<<endl;
                 }
                 else{
                   for(int i=0;i<arr.size();i++){
                    strcpy(dd.Dept_ID,arr[i].c_str());
                    if(SearchinPrimaryDept(AllDeptPrimary,0,(AllDeptPrimary.size()-1),dd)!=-1){
                        printDept(AllDeptPrimary,dd.Dept_ID,AllDeptPrimary.size(),MyFile);
                        cout<<"***********************************"<<endl;
                      }
                   }
                 }
            }
            else if(choice == 9){
            /************************************************************
            *Query form: select all from Employee where Dept_ID = 2     *
            ************************************************************/

                string str;
                cout<<"Enter your Query"<<endl;
                cin.ignore();
                getline(cin,str);
                string a,b,c,d;
                vector <string> vect = split(str,' ');
                a=vect[1];
                b=vect[3];
                c=vect[5];
                d=vect[7];

                if(a=="all"){
                    if(b=="Employee"){
                        if(c=="Dept_ID"){
                               SIndex sec;
                               strcpy(sec.Dept_ID,d.c_str());
                               vector<string>arr=GetEmployeesWorksonDept(AllSecondary,sec.Dept_ID,LinkedF);
                               PIndex prim;
                               if(arr.size()==0){
                                cout<<"Employee with this ID isn't Found"<<endl;
                               }
                               for(int i=0;arr.size();i++){
                                 strcpy(prim.Employee_ID,arr[i].c_str());
                                 if(SearchinPrimary(AllPrimary,0,(AllPrimary.size()-1),prim)!=-1){
                                   PrintEMPbyID(AllPrimary,prim.Employee_ID,AllPrimary.size(),MyFile);

                                }
                               }
                        }
                        else{
                            PIndex prim;
                            strcpy(prim.Employee_ID,d.c_str());
                            if(SearchinPrimary(AllPrimary,0,(AllPrimary.size()-1),prim)!=-1){
                              PrintEMPbyID(AllPrimary,prim.Employee_ID,AllPrimary.size(),MyFile);
                            }
                            else{
                                cout<<"Not Found"<<endl;
                            }
                        }
                    }
                    else{
                           if(c=="Dept_Name"){
                                   DSIndex DSec;
                                     strcpy(DSec.Dept_Name,d.c_str());
                                     vector<string>arr=GetallDept(AllDeptSecondary,DSec.Dept_Name,LinkedF);
                                     DPIndex DPrim;
                                     if(arr.size()==0){
                                        cout<<"Department with this name isn't Found"<<endl;
                                     }
                                     else{
                                       for(int i=0;i<arr.size();i++){
                                        strcpy(DPrim.Dept_ID,arr[i].c_str());
                                        if(SearchinPrimaryDept(AllDeptPrimary,0,(AllDeptPrimary.size()-1),DPrim)!=-1){
                                            printDept(AllDeptPrimary,DPrim.Dept_ID,AllDeptPrimary.size(),MyFile);
                                          }
                                       }
                                     }
                            }
                            else{
                               DPIndex dp;
                               strcpy(dp.Dept_ID,d.c_str());
                               if(SearchinPrimaryDept(AllDeptPrimary,0,(AllDeptPrimary.size()-1),dp)!=-1){
                                  printDept(AllDeptPrimary,dp.Dept_ID,AllDeptPrimary.size(),MyFile);
                               }
                               else{
                                cout<<"Not Found"<<endl;
                               }
                            }
                    }
                }
            }
            else if(choice==10){
               cout<<"We Wish You love our App"<<endl;
                return 0;
            }
            else {
                cout << "Wrong Input, Please Enter again."<< endl;
            }
       }
        return 0;
}
