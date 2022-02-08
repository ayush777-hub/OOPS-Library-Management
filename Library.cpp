#include <iostream>
#include <iomanip>
#include <string>
#include <conio.h>
#include <windows.h>
#include<bits/stdc++.h>
#include<sys/time.h>

using namespace std;

void set_margin(int n = 3)
{
    for (int i = 0; i < n; i++)
    {
        cout << endl;
    }
}

void Admin_Window();
void Intro_page();
void Admin_Login();
bool no_Admin_is_Active();

void ClearScreen()
{
    HANDLE hStdOut;
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    DWORD count;
    DWORD cellCount;
    COORD homeCoords = {0, 0};

    hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    if (hStdOut == INVALID_HANDLE_VALUE)
        return;

    /* Get the number of cells in the current buffer */
    if (!GetConsoleScreenBufferInfo(hStdOut, &csbi))
        return;
    cellCount = csbi.dwSize.X * csbi.dwSize.Y;

    /* Fill the entire buffer with spaces */
    if (!FillConsoleOutputCharacter(
            hStdOut,
            (TCHAR)' ',
            cellCount,
            homeCoords,
            &count))
        return;

    /* Fill the entire buffer with the current colors and attributes */
    if (!FillConsoleOutputAttribute(
            hStdOut,
            csbi.wAttributes,
            cellCount,
            homeCoords,
            &count))
        return;

    /* Move the cursor home */
    SetConsoleCursorPosition(hStdOut, homeCoords);
}

void PressEnter_tocontinue() { 
  string str;
  int ctr = 0;
  while (1) {
    getline(cin, str);
    if (str == "") {
      ctr++;
      Sleep(1000);
      if(ctr==2)
        break;
    }
  }    
}


string get_string(string s){
    int n = s.size();
    vector<string> spec_char = {"","#","@","$","!","?","0","1","2","3","4","5","6","7","8","9","0"};
    int arr_size = spec_char.size();
    string ret = "";
    for(int i=0;i<n;i++){
        ret += s[i];
        int num = (rand()%3);
        for(int j=0;j<num;j++){
            ret += spec_char[(rand()%arr_size)];
        }
    }
    if(ret.size()>10){
        int ret_size = ret.size();
        string temp = "";
        for(int i = 0;i<10;i++){
            temp += ret[(rand()%ret_size)];
        }
        ret = temp;
    }
    return ret;
}

class Book;     //Forward declaration
class Admin;
class User;
class Student;
vector<Book> Book_list;
vector<Admin> Admin_list;
vector<Student> Student_list;

class Book{
    private:
        string BookName;
        vector<string> Authors;
        string Genre;
        float Price;
        int Edition;
        bool is_Issued;
        string issued_by;
        string BookID;
    public:
        Book(string BName,vector<string> Authors,string Genre,float Price,int Edition=1){
            this->BookName = BName;
            this->Authors = Authors;
            this->Genre = Genre;
            this->Price = Price;
            this->Edition = Edition;
            this->is_Issued = 0;
            this->Gen_bookid();
        }
        void Gen_bookid(){
            BookID = to_string(rand()%10007) + get_string(BookName) + get_string(Genre) + get_string(to_string(Edition));
            if(BookID.size()>15){
                int ret_size = BookID.size();
                string temp = "";
                for(int i = 0;i<15;i++){
                    temp += BookID[(rand()%ret_size)];
                }
                BookID = temp;
            }
            BookID += to_string(Edition);
        }
        void make_available(){
            is_Issued = 0;
            string issued_by = "";
        }
        void make_issued(string LIB_ID){
            issued_by = LIB_ID;
            is_Issued = 1;
        }
        void Book_desc(){
            cout<<"     "<<BookName<<endl;
            cout<<"This book is Edition no."<<Edition<<"."<<"This book is written by ";
            for(int i =0;i<Authors.size();i++){
                cout<<Authors[i];
                if(i==Authors.size()-1){
                    cout<<"."<<endl;
                }
                else{
                    cout<<", ";
                }
            }
            cout<<"The Genre of this book is \""<<Genre<<"\"."<<endl;
            cout<<"Price:"<<Price<<endl;
        }
        const string get_bookid(){
            return this->BookID;
        }
        const string get_issuer(){
            if(this->is_Issued)
                return this->issued_by;
            return "";
        }
        float get_Price(){
            return this->Price;
        }
        const string get_bookname(){
            return this->BookName;
        }
        void set_Bookname(const string bname){
            this->BookName = bname;
        }
        void set_Price(const float price){
            this->Price = Price;
        }
        void set_Edition(const int ed){
            this->Edition = ed;
        }
        void set_Genre(const string Genre){
            this->Genre = Genre;
        }
        void Update_Authors(const vector<string> auth_list){
            this->Authors = auth_list;
        }
};

class User{
    private:
        string FirstName;
        string LastName;
        string Role;                    //The role of user
        set<string> Books_Issued;       //stores bookids that has been issued by the user
        string LIB_ID;                  //Unique Library ID for each user
        string password;
    public:
        User(const string Fname,const string Lname,const string Role,const string password){
            this->FirstName = Fname;
            this->LastName = Lname;
            this->Role = Role;
            this->password = password;
        }
        virtual void gen_ULIB_ID() = 0;         //pure virtual function
        void Return_Book(const string bookid){  //Returns book and update in Book-class's booklist
            int flag = 0;
            if(Books_Issued.find(bookid) != Books_Issued.end()){
                flag = 1;
                string issued_book = *(Books_Issued.find(bookid));
                Books_Issued.erase(Books_Issued.find(bookid));
                cout<<"The book with id:"<<issued_book<<" is returned successfully."<<endl;
                for(int i =0;i<Book_list.size();i++){
                    if(Book_list[i].get_bookid() == issued_book){
                        Book_list[i].make_available();
                        break;
                    }
                }
            }
            if(flag==0){
                cout<<"No book with this bookid has been issued by you !!!"<<endl;
            }
        }
        void Return_All_Book(){                 //Returns book and update in Book-class's booklist
            while(!Books_Issued.empty()){
                string issued_book = *(Books_Issued.begin());
                Books_Issued.erase(Books_Issued.begin());
                cout<<"The book with id:"<<issued_book<<" is returned successfully."<<endl;
                for(int i =0;i<Book_list.size();i++){
                    if(Book_list[i].get_bookid() == issued_book){
                        Book_list[i].make_available();
                        break;
                    }
                }
            }
        }
        void Issue_Book(const string bookid){   //Issues a book and update in Book-class's booklist
            int flag = 0;
            for(int i=0;i<Book_list.size();i++){
                if(Book_list[i].get_issuer() != ""){
                    Book_list[i].make_issued(LIB_ID);
                    Sleep(1000);
                    flag = 1;
                    cout<<"The book has successfully been issued !!!"<<endl;
                    break;
                }
            }
            if(flag==0){
                cout<<"No book with the given bookid was found !!!"<<endl;

            }
        }
        void set_LIB_ID(string lib_id){         //Sets unique library id generated froom gen_ULIB_ID() in each derived class
            this->LIB_ID = lib_id;
        }
        string GetFullName() const{             //Returns full name of the user
            return (FirstName+" "+LastName); 
        }
        string GetRole() const{                 //Returns teh current role of the user
            //The const means this function cant alter the data members.
            return Role;
        }
        string Get_Lib_ID(){
            return this->LIB_ID;
        }
        bool Check_password(string pwd){
            return (this->password == pwd);
        }
        ~User(){
            if(!Books_Issued.empty()){
                cout<<"Returning the Books before leaving !!!"<<endl;
                Return_All_Book();
            }
        }
};

class Student:public User{
    private:
        string DeptName;
        char section;
        string course;
        string Reg_id;
        bool isActive;
        // string LIB_ID;
    public:
        Student(const string Fname,const string Lname,const string Dept,const char sec,const string course,const string Reg_id,const string password):User(Fname,Lname,"Student",password){
            this->DeptName = Dept;
            this->section = sec;
            this->course = course;
            this->Reg_id = Reg_id;
            this->isActive = 1;
            this->gen_ULIB_ID();
        }
        void gen_ULIB_ID(){
            string LIB_ID = to_string(rand()%10007) + get_string(this->GetFullName()) + get_string(course);
            if(LIB_ID.size()>15){
                int ret_size = LIB_ID.size();
                string temp = "";
                for(int i = 0;i<15;i++){
                    temp += LIB_ID[(rand()%ret_size)];
                }
                LIB_ID = temp;
            }
            LIB_ID += section;
            set_LIB_ID(LIB_ID);
        }
        bool Activity(){
            return isActive;
        }
        void Make_inactive(){
            isActive = 0;
        }
        void Make_active(){
            isActive = 1;
        }
        const string Get_reg_id(){
            return Reg_id;
        }
};

class Professor:public User{
    private:
        string DeptName;
        string Prof_id;
        // string LIB_ID;
    public:
        Professor(const string Fname,const string Lname,const string Dept,const string Prof_id,const string password):User(Fname,Lname,"Professor",password){
            this->DeptName = Dept;
            this->Prof_id = Prof_id;
            this->gen_ULIB_ID();
        }
        void gen_ULIB_ID(){
            string LIB_ID = to_string(rand()%10007) + get_string(this->GetFullName())  + get_string(Prof_id);
            if(LIB_ID.size()>15){
                int ret_size = LIB_ID.size();
                string temp = "";
                for(int i = 0;i<15;i++){
                    temp += LIB_ID[(rand()%ret_size)];
                }
                LIB_ID = temp;
            }
            set_LIB_ID(LIB_ID);
        }


};

class Admin:public User{
    private:
        // string LIB_ID;
        bool isActive;
    public:
        Admin(const string Fname,const string Lname,const string password):User(Fname,Lname,"Admin",password){
            this->isActive = 1;
        
            this->gen_ULIB_ID();
        }
        void gen_ULIB_ID(){
            string LIB_ID;
            LIB_ID = to_string(rand()%10007) + get_string(get_string(this->GetFullName())) ;

            if(LIB_ID.size()>15){
                int ret_size = LIB_ID.size();
                string temp = "";
                for(int i = 0;i<15;i++){
                    temp += LIB_ID[(rand()%ret_size)];
                }
                LIB_ID = temp;
            }
            set_LIB_ID(LIB_ID);
        }
        
        void AddBook(){                     //Add a book into the library
            //string BName,vector<string> Authors,string Genre,float Price,int Edition=1
            string Bname,Genre;
            float Price;
            int Edition;
            vector<string> Authors;
            cout<<"Enter the name of the Book:";
            // cin>>Bname;
            getline(cin >> ws,Bname);
            cout<<"ADD THE AUTHOR LIST!!!"<<endl;
            char ch = 'y';
            do{
                cout<<"Author Name:";
                string str;
                getline(cin >> ws,str);
                Authors.push_back(str);
                cout<<"Do you want to add more author?(y/n):";
                cin>>ch;
            }while(ch=='y');
            cout<<"Enter the Price:";
            cin>>Price;
            
            cout<<"Enter Edition of the book:";
            cin>>Edition;
            Book newbook = Book(Bname,Authors,Genre,Price,Edition);
            Book_list.push_back(newbook);
        }
        void RemoveBook(string bookid){     //Removes a book from the Library
            int booklist_size = Book_list.size();
            bool flag = 0;
            for(int i=0;i<booklist_size;i++){
                if(bookid == Book_list[i].get_bookid()){
                    Book_list.erase(Book_list.begin()+i);
                    flag = 1;
                    cout<<"Deleting ..."<<endl;
                    Sleep(1000);
                    cout<<"The book is successfully removed from Library !!!"<<endl;
                    break;
                }
            }
            if(flag==0){
                cout<<"Sorry No such book is found ... Enter a valid Bookid!!!"<<endl;
            }

        }
        void ApplyFine(string LIB_ID,string bookid){    //Applies Fine if book is damaged
            int booklist_size = Book_list.size();
            bool flag = 0;
            for(int i=0;i<booklist_size;i++){
                if(bookid == Book_list[i].get_bookid()){
                    flag = 1;
                    if(LIB_ID == Book_list[i].get_issuer()){
                        cout<<"The book is found damaged!!! So applying fine on LIB_ID:"<<LIB_ID<<endl;
                        cout<<"Price:"<<Book_list[i].get_Price()<<endl;
                        float fine = 0.07 * Book_list[i].get_Price();
                        Sleep(1000);   //Calculating fine so a lil time is taken
                        cout<<"Fine: "<<fine<<" (7% of the book price)"<<endl;
                    }
                    else{
                        cout<<"This book is not issued by this LIB_ID"<<endl;
                    }
                    break;
                }
            }
            if(flag==0){
                cout<<"Sorry Couldn't complete the process ... Enter a valid Bookid and its corresponding Issuer!!!"<<endl;
            }
        }
        int Check_Issue(string bookid){                 //Checks if tbe book is available for issuing or not
            int flag = 0;
            for(int i=0;i<Book_list.size();i++){
                if(bookid == Book_list[i].get_bookid()){
                    if(Book_list[i].get_issuer() == ""){
                        cout<<"This book is not issued and thus available !!!"<<endl;
                        flag = 1;
                    }
                    else{
                        flag = 2;
                        cout<<"This book is already issued by LAB_ID:\""<<Book_list[i].get_issuer()<<"\"."<<endl;
                    }
                }
            }
            if(flag==0){
                cout<<"The book with this bookid is not present!!!"<<endl;
            }
            return flag;
        }
        bool get_permission(){
            return (rand()%2);
        }
        bool Activity(){
            return this->isActive;
        }
};

int active_admin_index(){
    for(int i=0;i<Admin_list.size();i++){
        if(Admin_list[i].Activity()){
            return i;
        }
    }
    return -1;
}
void Add_Book(){
    ClearScreen();
    Sleep(500);
    cout<<"Add Book"<<endl;
    int index = active_admin_index();
    if(index != -1){
        Admin cur_admin = Admin_list[index];
        cur_admin.AddBook();
        set_margin();
        cout<<"Adding The Book!!!"<<endl;
        Sleep(500);
        cout<<"The book has been added !!!"<<endl;
        cout<<"Press Enter to continue";
        PressEnter_tocontinue();
        Admin_Window();
    }
    else{
        cout<<"Login Admin to add books!!!"<<endl;
        Sleep(1000);
        Admin_Login();
    }
}
void Delete_Book(){
    ClearScreen();
    Sleep(500);
    cout<<"Delete Book"<<endl;
    int index = active_admin_index();
    if(index != -1){

        Admin cur_admin = Admin_list[index];
        string bookid;
        cout<<"Enter the bookid you want to delete from library database!!!!";
        cin>>bookid;
        cur_admin.RemoveBook(bookid);
        set_margin(2);
        cout<<"Press Enter to continue";
        PressEnter_tocontinue();
        Admin_Window();
    }
    else{
        cout<<"Login Admin to Delete books!!!"<<endl;
        Sleep(1000);
        Admin_Login();
    }
}
void Update_Book(){
    ClearScreen();
    cout<<"Update Book"<<endl;
    string bookid;
    cout<<"ENTER THE BOOKID YOU WANT TO UPDATE:";
    cin>>bookid;
    cout<<"Searching for the book..."<<endl;
    Sleep(1000);
    int flag = 0;
    for(int i=0;i<Book_list.size();i++){
        if(Book_list[i].get_bookid() == bookid){
            cout << setw(75) << "1.Change BookName   " << setw(25) << "2.Update Authors List" << endl;
            cout << setw(75) << "3.Update BookEdition" << setw(25) << "4.Change Genre       " << endl;
            cout << setw(75) << "5.Change Price      " << setw(25) << "6.Quit               "<< endl;
            cout<<setw(60)<<"Enter your choice:";
            int choice;
            cin>>choice;
            switch(choice){
                case 1:{
                    string bname;
                    cout<<"Enter the new BookName:"<<endl;
                    getline(cin >> ws,bname);
                    // cin>>bname;
                    Book_list[i].set_Bookname(bname);
                    cout<<"The bookname has successfully been updated!!!"<<endl;
                    break;
                }
                case 2:{
                    cout<<"Enter the correct Author list:"<<endl;
                    set<string> auth_list;
                    char ch = 'y';
                    do{
                        string A_name;
                        cout<<"Author Name:";
                        getline(cin >> ws,A_name);
                        // cin>>A_name;
                        auth_list.insert(A_name);
                        cout<<"Do you want to add more author(y/n):";
                        cin>>ch;
                    }while(ch=='y');
                    vector<string> ret(auth_list.begin(),auth_list.end());
                    Book_list[i].Update_Authors(ret);
                    cout<<"The author list has successfully been updated!!!"<<endl;
                    break;
                }
                case 3:{
                    int Edition;
                    cout<<"Enter the Updated Edition:"<<endl;
                    cin>>Edition;
                    Book_list[i].set_Edition(Edition);
                    cout<<"The bookname has successfully been updated!!!"<<endl;
                    break;
                }
                case 4:{
                    string Genre;
                    cout<<"Enter the Genre:"<<endl;
                    getline(cin >> ws,Genre);
                    // cin>>Genre;
                    Book_list[i].set_Genre(Genre);
                    cout<<"The bookname has successfully been updated!!!"<<endl;
                    break;
                }
                case 5:{

                    float Price;
                    cout<<"Enter the Price:"<<endl;
                    Book_list[i].set_Price(Price);
                    cout<<"The bookname has successfully been updated!!!"<<endl;
                    break;
                }
                case 6:{
                    cout<<"Going to Admin Page!!!"<<endl;
                    Sleep(1000);
                    break;
                }
                default:
                {
                    cout<<"Invalid choice!!!"<<endl;
                    Sleep(1000);
                    Update_Book();
                    break;
                }
            }
            flag = 1;
            break;
        }
    }
    if(flag == 0){
        cout<<"The book with given bookid was not found!!!"<<endl;
        Sleep(1000);
        char ch = 'y';
        cout<<"Do you want to go to the previous page?(y/n):";
        cin>>ch;
        if(ch=='y'){
            Admin_Window();
        }
        else{
            Update_Book();
        }
    }
    else{
        Sleep(1000);
        cout<<"Press Enter to continue";
        PressEnter_tocontinue();
        Admin_Window();
    }
}
void My_Info(){
    ClearScreen();
    int act_admin = active_admin_index();
    cout<<"Welcome to My Information"<<endl;
    // Sleep(5000);
    if(act_admin==-1){
        cout<<"Login to see your Info!!!"<<endl;
        Sleep(3000);
        Admin_Login();
    }
    else{
        Admin cur_admin = Admin_list[act_admin];
        cout<<setw(50)<<"ADMIN INFORMATION"<<endl;
        set_margin(2);
        cout<<setw(15)<<"Name: "<<cur_admin.GetFullName()<<endl;
        cout<<setw(15)<<"Role: "<<cur_admin.GetRole()<<endl;
        cout<<setw(15)<<"ID:   "<<cur_admin.Get_Lib_ID()<<endl;
        set_margin();
        // Sleep(5000);
        cout<<"PRESS ENTER TO GO TO PREVIOUS PAGE!!!"<<endl;
        PressEnter_tocontinue();
        Admin_Window();
    }
}
void Issued_BookList(){
    ClearScreen();
    if(no_Admin_is_Active()){
        cout<<"You cant perform any task as no Admin is Active!!! Wait till some Admin enters the library!!!"<<endl;
        Sleep(1500);
        Admin_Login();
    }
    else{
        Sleep(500);
        cout<<"BookList"<<endl;
        set_margin();
        cout<<setw(10)<<"#ID"<<setw(40)<<"Book Name"<<setw(40)<<"Issued by(LIB_ID)"<<endl;
        for(int i=0;i<Book_list.size();i++){

            // Book_list[i].Book_desc();
            cout<<endl;
            cout<<setw(18)<<Book_list[i].get_bookid()<<setw(30)<<Book_list[i].get_bookname()<<setw(40)<<Book_list[i].get_issuer()<<endl;
            for(int j=0;j<120;j++){
                cout<<"-";
            }
            cout<<endl;

            // cout<<Book_list[i].get_bookid()<<endl;
        }
        cout<<"Press Enter to go back to Admin Menu!!!"<<endl;
        PressEnter_tocontinue();
        Admin_Window();
    }

}
void Fine_Management(){
    ClearScreen();
    int act_admin = active_admin_index();
    if(act_admin==-1){
        cout<<"Login to Enter Fine Management Dept.!!!"<<endl;
        Sleep(1500);
        Admin_Login();
    }
    else{
        cout<<"Fine Management"<<endl;
        string LIB_ID,Book_ID;
        cout<<"Enter the LIB_ID on which Fine is Imposed:";
        cin>>LIB_ID;
        cout<<"Enter the book id that has been damaged:";
        cin>>Book_ID;
        Admin cur_admin = Admin_list[act_admin];
        cur_admin.ApplyFine(LIB_ID,Book_ID);
        cout<<"Press Enter to go back to the previous page!!!"<<endl;
        Admin_Window();
    }


}

void Admin_Window(){
    ClearScreen();
    cout<<setw(90)<<"Welcome To Admin Page"<<endl;
    set_margin();
    cout << setw(75) << "1.Add Book        " << setw(25) << "2.Delete Book        " << endl;
    cout << setw(75) << "3.Update Book     " << setw(25) << "4.My_Info            " << endl;
    cout << setw(75) << "5.Book List       " << setw(25) << "6.Fine Management    "<< endl;
    cout << setw(75) << "7.Quit            " << setw(25) << "8.Go to Previous Page"<< endl;

    set_margin();    
    int choice = -1;
    cout << setw(65) << "ENTER THE OPTION YOU WANT TO PROCEED WITH:" << endl;
    cin >> choice;
    switch (choice)
    {
        case 1:
        {
            Add_Book();
            break;
        }
        case 2:
        {
            Delete_Book();
            break;
        }
        case 3:
        {
            Update_Book();
            break;
        }
        case 4:
        {
            My_Info();
            break;
        }
        case 5:
        {
            Issued_BookList();
            break;
        }
        case 6:
        {
            Fine_Management();
            break;
        }
        case 7:
        {
            exit(1);
            break;
        }
        case 8:
        {
            Intro_page();
            break;
        }

        default:
        {
            cout << "Wrong Input!!! Choose Again" << endl;
            break;
        }
    }
    Admin_Window();
}
void User_Window(){
    cout<<"Hello User"<<endl;
    string str;
    cout<<"Press Enter to go to previous page !!!"<<endl;
    PressEnter_tocontinue();
    
}
bool isAccessAllowed(string LIB_ID,string pwd){
    //Here the logic should be 
    // saving username and password from 
    // admin register function into some file and 
    // reading the username and password from there
    // and validating the input given.
    
    //////////       LOGIC       ////////
    int flag = 0;
    for(int i=0;i<Admin_list.size();i++){
        if(Admin_list[i].Get_Lib_ID() == LIB_ID){
            flag = 1;
            return Admin_list[i].Check_password(pwd);
        }
    }
    return 0;

}


void Admin_Login()
{
    ClearScreen();
    if(no_Admin_is_Active()){
        cout<<setw(90)<<"Welcome To Admin Login Page"<<endl;
        set_margin();
        string Lib_id,password;
        cout<<setw(80)<<"ENTER LIB_ID:";
        cin>>Lib_id;
        set_margin(1);
        cout<<setw(80)<<"ENTER PASSWORD:";
        cin>>password;
        if(isAccessAllowed(Lib_id,password)){
            cout<<"Opening Admin Window..."<<endl;
            Sleep(1000);
            Admin_Window();
        }
        else{
            cout<<setw(90)<<"Wrong UserName or Password!!!"<<endl;
            Sleep(2000);
            char ch = 'y';
            cout<<"Do you want to go back to previous page?(y/n)"<<endl;
            cin>>ch;
            if(ch=='y'){
                Intro_page();
            }
            else{
                Admin_Login();
            }
        }
    }
    else{
        Admin cur_admin = Admin_list[active_admin_index()];
        cout<<"Admin with LIB_ID \""<<cur_admin.Get_Lib_ID()<<"\" is currently active"<<endl;
        cout<<"Wait till he Logout!!!"<<endl;
        cout<<"Redirecting to previous page!!!"<<endl;
        Sleep(1500);
        Admin_Window();
    }
}

bool validate_lib_pass(string password){        //Validates the library password
    if(password=="My_Lib711"){
        return 1;
    }
    else{
        return 0;
    }
}

bool no_Admin_is_Active(){
    for(int i=0;i<Admin_list.size();i++){
        if(Admin_list[i].Activity() == 1){
            return 0;
        }
    }
    return 1;
}

void Register_Admin()                           //Adds a new Admin to the list
{
    ClearScreen();
    string Fname,Lname,password,Lib_password;
    cout<<setw(90)<<"Welcome To Admin Registration Page"<<endl;
    set_margin();
    cout<<setw(60)<<"Enter Your FirstName:  ";
    cin>>Fname;
    cout<<setw(60)<<"Enter Your LastName:   ";
    cin>>Lname;
    cout<<setw(60)<<"Enter Password:        ";
    cin>>password;
    cout<<setw(60)<<"Enter Library Password:";
    cin>>Lib_password;
    cout<<"Authenticating ... "<<endl;
    Sleep(1500);
    if(validate_lib_pass(Lib_password)){
        if(no_Admin_is_Active()){
            Admin_list.push_back(Admin(Fname,Lname,password));
            cout<<"Admin is registered Successfully !!!"<<endl;
            Sleep(1000);
            cout<<"Press Enter to continue";
            PressEnter_tocontinue();
            Admin_Window();
        }
        else{
            cout<<"No Can not create a new active admin as one of the admin is already active!!!"<<endl;
            cout<<"Redirecting to Login Page !!!"<<endl;
            Sleep(1500);
            Admin_Login();
        }
    }
    else{
        cout<<"Sorry The library password didnt match!!!"<<endl;
        Sleep(1000);
        cout<<"Wait Reloading the page..."<<endl;
        Sleep(3000);
        Register_Admin();
    }
}

bool isUserAccessAllowed(string LIB_ID,string Password){
    //logic to allow user to perform task;
    return 1;//need to change logic
}

void User_Login()
{
    ClearScreen();
    Sleep(500);
    cout<<setw(90)<<"Welcome To User Login Page"<<endl;
    set_margin();
    string LIB_ID,password;
    cout<<setw(80)<<"ENTER LIB_ID:";
    cin>>LIB_ID;
    set_margin(1);
    cout<<setw(80)<<"ENTER PASSWORD:";
    cin>>password;
    if(isUserAccessAllowed(LIB_ID,password)){
        cout<<"Entering User Window..."<<endl;
        Sleep(500);
        User_Window();
    }
    else{
        cout<<setw(90)<<"Wrong UserName or Password!!!"<<endl;
        Sleep(2000);
        User_Login();
    }
}
bool if_new_user(string Reg_id){
    for(int i =0;i<Student_list.size();i++){
        if(Student_list[i].Get_reg_id() == Reg_id){
            cout<<"The student is already registered !!! Redirecting to User Login Page"<<endl;
            Sleep(1000);
            return 0;
        }
    }
    return 1;
}

void Register_User()
{
    ClearScreen();
    Sleep(500);
    string Fname,Lname,password,Dept,course,Reg_id;
    char section;
    cout<<setw(90)<<"Welcome To User Registration Page"<<endl;
    set_margin();
    cout<<setw(60)<<"Enter Your FirstName:  ";
    cin>>Fname;
    cout<<setw(60)<<"Enter Your LastName:   ";
    cin>>Lname;
    cout<<setw(60)<<"Enter Password:        ";
    cin>>password;
    cout<<setw(60)<<"Enter Department Name: ";
    cin>>Dept;
    cout<<setw(60)<<"Enter Course Taken:    ";
    cin>>course;
    cout<<setw(60)<<"Enter Registration ID: ";
    cin>>Reg_id;
    cout<<setw(60)<<"Enter Your Section:    ";
    cin>>section;

    Sleep(500);
    if(if_new_user(Reg_id)){
        Student_list.push_back(Student(Fname,Lname,Dept,section,course,Reg_id,password));
        cout<<"New student added successfully!!!"<<endl;
        
    }
    else{
        cout<<"The Student already exists in our database !!!"<<endl;
    }
    Sleep(2000);
    User_Window();
}


void Get_Info()
{
    ClearScreen();
    cout<<"INFO"<<endl;
    Sleep(2000);
}


void Intro_page()
{
    ClearScreen();
    Sleep(500);
    cout << setw(90) << "Welcome To A.K. Library!!!";
    set_margin();
    cout << setw(75) << "1.Admin Login" << setw(20) << "2.Register Admin" << endl;
    cout << setw(75) << "3.User Login " << setw(20) << "4.Register User " << endl;
    cout << setw(75) << "5.Get Info   " << setw(20) << "6.Quit          ";
    set_margin();
    int choice = -1;
    cout << setw(65) << "ENTER THE OPTION YOU WANT TO PROCEED WITH:" << endl;
    cin >> choice;
    switch (choice)
    {
        case 1:
        {
            Admin_Login();
            break;
        }
        case 2:
        {
            Register_Admin();
            break;
        }
        case 3:
        {
            User_Login();
            break;
        }
        case 4:
        {
            Register_User();
            break;
        }
        case 5:
        {
            Get_Info();
            break;
        }
        case 6:
        {
            exit(1);
            break;
        }

        default:
        {
            cout << "Wrong Input!!! Choose Again" << endl;
            break;
        }
    }
}
int main()
{
    while (1)
    {
        Intro_page();
    }

    return 0;
}