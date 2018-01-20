//
//  main.cpp
//  database
//
//  Created by Akansha_Manav_Jishnu on 11/9/17.
//  Copyright © 2017 Akansha_Manav_Jishnu. All rights reserved.
//

//Initializing Libraries
#include "mysql.h"
#include "mysql_time.h"
#include <iostream>
#include <cstring>
#include <string>
#include <curses.h>
#include <unistd.h>
#include <iomanip>
#include <termios.h>
#include <stdio.h>

//Initializing Functions
void login(MYSQL *connection);
void student_menu(int username);
void student_transcript (int username);
void student_enroll(int username);
void student_withdraw(int username);
void student_details(int username);
void course_details(int username,char choice[10]);
void logout();

using namespace std;


//Main function. Creates connection to the database. If connection is successful, login student.
int main (int argc, const char * argv[])
{
    
    MYSQL *connection, mysql;
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
    if (connection == NULL)
    {
        system("clear");
        cout<<"Connecting to the Database. Please wait.\n";
        cout << '-' << std::flush;
        for (int i=0;i<50;i++) {
            usleep(10000);
            std::cout << "\b\\" << std::flush;
            usleep(10000);
            std::cout << "\b|" << std::flush;
            usleep(10000);
            std::cout << "\b/" << std::flush;
            usleep(10000);
            std::cout << "\b-" << std::flush;
        }
        cout<<"Connection Unsuccessful. Please check connection paramteres.\n";
    }
    else
    {
        system("clear");
        cout<<"Connecting to the Database. Please wait.\n";
        cout << '-' << std::flush;
        for (int i=0;i<50;i++) {
            usleep(10000);
            std::cout << "\b\\" << std::flush;
            usleep(10000);
            std::cout << "\b|" << std::flush;
            usleep(10000);
            std::cout << "\b/" << std::flush;
            usleep(10000);
            std::cout << "\b-" << std::flush;
        }
        cout<<"\nSuccessfully Connected to Database\n";
        login(connection);
        
    }
}




//Login Screen. Prompts for student username and password. Shows errors if username/password is wrong. Takes to student menu if login successful.
void login (MYSQL *connection)
{
    char password[20];
    char username_string[20];
    int username;
    char query[100];
    system("clear");
    MYSQL_RES *res_set1;
    MYSQL_ROW row1;
    
    cout <<setw(50)<< "--------------------------------------------------------------" << endl;
    cout <<setw(50)<< "|            NORTHWESTERN STUDENT ACADEMIC SYSTEM            |" << endl;
    cout <<setw(50)<< "--------------------------------------------------------------" << endl;
    cout<<"LOGIN TO YOUR ACCOUNT";
    cout<<endl;
    cout<<"Enter StudentID : ";
    cin>>username_string;
    username=atoi(username_string);
    cout<<"Enter Password  : ";
    cin>>password;
    
    sprintf(query,"SELECT password FROM student WHERE Id = %d;", username);
    mysql_query(connection,query);
    res_set1=mysql_store_result(connection);
    int numrows1=(int)mysql_num_rows(res_set1);
    
    //When StudentID is incorrect
    if(numrows1==0)
    {
        cout<<"\nNo user found! Please check StudentID. Press ENTER to retry >> "<<flush;
        system("read");
        login(connection);
    }
    
    for(int i=1;i<=numrows1;i++)
    {
        row1=mysql_fetch_row(res_set1);
        if(row1!=NULL)
        {
            if (strcmp(row1[0],password)==0)
            {
                cout<<"\nLogging you in. Please Wait\n";
                cout << '-' << std::flush;
                for (int i=0;i<50;i++) {
                    usleep(10000);
                    std::cout << "\b\\" << std::flush;
                    usleep(10000);
                    std::cout << "\b|" << std::flush;
                    usleep(10000);
                    std::cout << "\b/" << std::flush;
                    usleep(10000);
                    std::cout << "\b-" << std::flush;
                }
                cout<<"\nLogin Successful\n";
                mysql_close(connection);
                student_menu(username);
            }
            else
            {
                cout<<"\nYour Password is incorrect! Press ENTER to retry >> "<<flush;
                system("read");
                login(connection);
            }
        }
    }
    
    //Empty all resources
    mysql_free_result(res_set1);
}





//Contains the student menu. Identifies each student by his/her username.
void student_menu(int username)
{
    MYSQL *connection, mysql;
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
    system("clear");
    
    if (connection == NULL)
    {
        //unable to connect
        cout<<"The database connection has been lost! Please check and try again.\n";
    }
    
    cout <<setw(50)<< "-----------------------------------------------------------" << endl;
    cout <<setw(50)<< "|                       STUDENT MENU                      |" << endl;
    cout <<setw(50)<< "-----------------------------------------------------------" << endl;
    
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);
    int year = t_tm->tm_year + 1900;
    int month = t_tm->tm_mon + 1;
    
    char quarter[2];
    
    if (month==1||month==2||month==3)
        strcpy(quarter,"Q2");
    
    else if (month==4||month==5||month==6)
        strcpy(quarter,"Q3");
    
    else if (month==7||month==8)
        strcpy(quarter,"Q4");
    
    else if (month==9||month==10||month==11||month==12)
        strcpy(quarter,"Q1");
    
    cout<<"Current Quarter: "<<quarter;
    cout<<'\n'<<"Current Year   : "<<year;
    cout<<'\n'<<"Currently enrolled courses:\n";
    
    MYSQL_RES *res_set2;
    MYSQL_ROW row2;
    char query[300];
    sprintf(query,"SELECT a.UoScode, b.uosname FROM transcript a, unitofstudy b WHERE a.uoscode=b.uoscode and studid=%d and year=%d and semester='%s';", username, year, quarter);
    mysql_query(connection,query);
    res_set2=mysql_store_result(connection);
    int numrows=0;
    if(res_set2!=NULL)
    {
        numrows=(int)mysql_num_rows(res_set2);
    }
    
    cout<<"-----------------------------------------------------------\n";
    cout<<"Course Code         Title"<<"\n";
    cout<<"-----------------------------------------------------------\n";
    
    for(int i=0;i<numrows;i++)
    {
        row2=mysql_fetch_row(res_set2);
        if(row2!=NULL)
        {
            cout<<row2[0]<<"            "<<row2[1]<<'\n';
        }
    }
    cout<<"-----------------------------------------------------------\n";
    char choice;
    cout<<'\n'<<"\nSelect from the following options:"<<"\n";
    cout<<"1. TRANSCRIPT"<<'\n';
    cout<<"2. ENROLL"<<'\n';
    cout<<"3. WITHDRAW"<<'\n';
    cout<<"4. PERSONAL DETAILS"<<'\n';
    cout<<"5. LOGOUT"<<'\n';
    cout<<"Enter your choice (1-5) >> ";
    cin>>choice;
    
    if (choice == '1')
    {
        mysql_close(connection);
        student_transcript(username);
    }
    else if (choice=='2')
    {
        mysql_close(connection);
        student_enroll(username);
    }
    else if (choice =='3')
    {
        mysql_close(connection);
        student_withdraw(username);
    }
    else if (choice =='4')
    {
        mysql_close(connection);
        student_details(username);
    }
    else if (choice == '5')
    {
        mysql_close(connection);
        logout();
    }
    else{
        cout<<"\nInvalid Choice! Press ENTER to retry\n";
        system("read");
        student_menu(username);
    }
    
    mysql_free_result(res_set2);
    
}




//Function to print the current Transcript of the student
void student_transcript(int username)
{
    system("clear");
    MYSQL *connection, mysql;
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
    
    if (connection == NULL)
    {
        //unable to connect
        cout<<"The database connection has been lost! Please check and try again.\n";
    }
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"SELECT a.UoScode, b.uosname, a.grade FROM transcript a, unitofstudy b WHERE a.uoscode=b.uoscode and studid=%d;", username);
    mysql_query(connection,query);
    res_set=mysql_store_result(connection);
    int numrows=(int)mysql_num_rows(res_set);
    
    cout <<setw(50)<< "-------------------------------------------------------------------------------" << endl;
    cout <<setw(50)<< "|                               STUDENT TRANSCRIPT                            |" << endl;
    cout <<setw(50)<< "-------------------------------------------------------------------------------" << endl<<endl<<endl;
    cout<<"-------------------------------------------------------------------------------\n";
    cout<<"S.No    CourseCode     Title                                    Grade"<<"\n";
    cout<<"-------------------------------------------------------------------------------\n";
    for(int i=0;i<numrows;i++)
    {
        row=mysql_fetch_row(res_set);
        if(row!=NULL)
        {
            cout<<setw(7)<<std::left<<i+1;
            cout<<cout.fill(' ');
            cout<<row[0]<<"       ";
            std::cout.fill(' ');
            std::cout.width(41);
            std::cout << std::left << row[1];
            if(row[2]==NULL)
                cout<<"Not Graded\n";
            else
                cout<<row[2]<<'\n';
        }
    }
    cout<<"-------------------------------------------------------------------------------\n";
    cout<<"\nEnter Course Number for more details  OR  Enter 0 to go back >> "<<flush;
    char choice[10];
    cin>>choice;
    
    
    if (strcmp(choice,"0")==0)
        student_menu(username);
    else
    {
        mysql_close(connection);
        course_details(username,choice);
    }
    mysql_free_result(res_set);
}






//This function shows course details
void course_details(int username,char choice[10])
{
    MYSQL *connection, mysql;
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
    
    if (connection == NULL)
    {
        //If database connection is lost
        cout<<"The database connection has been lost! Please check and try again.\n";
    }
    
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    char query[300];
    sprintf(query,"SELECT a.UoScode, b.uosname, a.year, a.semester, c.enrollment, c.maxenrollment, d.name, a.grade FROM unitofstudy b natural join transcript a natural join uosoffering c natural join faculty d WHERE d.id=c.instructorID and studid=%d and a.uoscode='%s';", username,choice);
    mysql_query(connection,query);
    res_set=mysql_store_result(connection);
    
    int numrows=(int)mysql_num_rows(res_set);
    
    if(numrows==0)
    {
        cout<<"Incorrect Course Number. \nPress ENTER to try again >> "<<flush;
        system("read");
        student_transcript(username);
    }
    else
        for(int i=0;i<numrows;i++)
        {
            cout<<"\n\nCOURSE DETAILS\n";
            row=mysql_fetch_row(res_set);
            if(row!=NULL)
            {
                cout<<"Code              : "<<row[0]<<'\n';
                cout<<"Title             : "<<row[1]<<'\n';
                cout<<"Year              : "<<row[2]<<'\n';
                cout<<"Quarter           : "<<row[3]<<'\n';
                cout<<"Enrollment Number : "<<row[4]<<'\n';
                cout<<"Max Enrollment    : "<<row[5]<<'\n';
                cout<<"Faculty Name      : "<<row[6]<<'\n';
                if(row[7]==NULL)
                    cout<<"Grade             : "<<"Not Graded"<<'\n';
                else
                    cout<<"Grade             : "<<row[7]<<'\n';
            }
        }
    cout<<"\nPress ENTER key to go back >> "<<flush;
    system("read");
    mysql_free_result(res_set);
    student_transcript(username);
}





//Function to withdraw from a course
void student_withdraw(int username)
{
    MYSQL *connection, mysql;
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
    
    if (connection == NULL)
    {
        //If database connection is lost
        cout<<"The database connection has been lost! Please check and try again.\n";
    }
    
    //To read current system time
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);
    int year = t_tm->tm_year + 1900;
    int month = t_tm->tm_mon + 1;
    
    system("clear");
    char quarter[10];
    char nextquarter[10];
    int nextyear = 0;
    cout <<setw(50)<< "-----------------------------------------------------------" << endl;
    cout <<setw(50)<< "|               STUDENT COURSE WITHDRAWAL MENU            |" << endl;
    cout <<setw(50)<< "-----------------------------------------------------------" << endl;
    
    if (month==1||month==2||month==3)
    {
        strcpy(quarter,"Q2");
        strcpy(nextquarter,"Q3");
        nextyear=year;
    }
    else if (month==4||month==5||month==6)
    {
        strcpy(quarter,"Q3");
        strcpy(nextquarter,"Q4");
        nextyear=year;
    }
    
    else if (month==7||month==8)
    {
        strcpy(quarter,"Q4");
        strcpy(nextquarter,"Q1");
        nextyear=year;
    }
    
    else if (month==9||month==10||month==11||month==12)
    {
        strcpy(quarter,"Q1");
        strcpy(nextquarter,"Q2");
        nextyear=year+1;
    }
    
    cout<<"\nSelect the quarter and year: \n";
    cout<<"1. "<<quarter<<" "<<year<<'\n';
    cout<<"2. "<<nextquarter<<" "<<nextyear<<'\n';
    cout<<"0. RETURN TO MAIN MENU";
    cout<<"\n\nENTER YOUR CHOICE >> ";
    
    char menu_choice;
    cin>>menu_choice;
    
    
    char course_choice[10];
    
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    
    if(menu_choice=='1')
    {
        char query1[300];
        sprintf(query1,"SELECT a.UoScode, b.uosname FROM transcript a, unitofstudy b WHERE a.uoscode=b.uoscode and studid=%d and isnull(grade) and year=%d and semester='%s';", username, year, quarter);
        
        mysql_query(connection,query1);
        res_set=mysql_store_result(connection);
        int numrows=(int)mysql_num_rows(res_set);
        
        //If no course to withdraw from, display error.
        if(numrows==0)
        {
            cout<<"\n____There are no courses that you can withdraw from this quarter____";
            cout<<"\n\nPress ENTER key to return to Course Withdrawal Menu >> "<<flush;
            system("read");
            student_withdraw(username);
        }
        
        cout<<"-------------------------------------------------------------\n";
        cout<<"Course Code       Title"<<"\n";
        cout<<"-------------------------------------------------------------\n";
        for(int i=0;i<numrows;i++)
        {
            row=mysql_fetch_row(res_set);
            if(row!=NULL)
            {
                cout<<row[0]<<"          "<<row[1]<<'\n';
            }
        }
        cout<<"-------------------------------------------------------------\n";
        cout<<"\nEnter the course number OR 0 to return to Withdraw Menu >> ";
        cin>>course_choice;
        
        char query2[300];
        sprintf(query2,"call withdraw(%d,'%s','%s', %d, @res);", username, course_choice, quarter, year);
        mysql_query(connection,query2);
        res_set=mysql_store_result(connection);
        
        numrows=(int)mysql_num_rows(res_set);
        if (strcmp(course_choice,"0")==0)
        {
            student_withdraw(username);
        }
        else
        {
            for(int i=0;i<numrows;i++)
            {
                row=mysql_fetch_row(res_set);
                if(row!=NULL)
                {
                    cout<<'\n'<<row[0]<<'\t';
                }
            }
        }
        mysql_close(connection);
        
        
        
        
        MYSQL *connection, mysql;
        mysql_init(&mysql);
        connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
        
        if (connection == NULL)
        {
            //If database connection is lost
            cout<<"The database connection has been lost! Please check and try again.\n";
        }
        
        //Call trigger after each enroll/withdraw and display result for the course
        MYSQL_RES *res_setw;
        char query3[100];
        sprintf(query3,"SELECT * FROM TriggerWarning;");
        mysql_query(connection,query3);
        res_setw=mysql_store_result(connection);
        int numrows1=(int)mysql_num_rows(res_setw);
        
        if(numrows1!=0)
        {
            cout << "\nWARNING: Enrollment below 50%\n" << endl;
            mysql_free_result(res_setw);
        }
        mysql_close(connection);
    }
    
    //Choice 2
    else if(menu_choice=='2')
    {
        
        char query1[300];
        sprintf(query1,"SELECT a.UoScode, b.uosname FROM transcript a, unitofstudy b WHERE a.uoscode=b.uoscode and studid=%d and isnull(grade) and year=%d and semester='%s';", username, nextyear, nextquarter);
        mysql_query(connection,query1);
        res_set=mysql_store_result(connection);
        int numrows=(int)mysql_num_rows(res_set);
        
        //If no course to withdraw from, display error.
        if(numrows==0)
        {
            cout<<"\n____There are no courses that you can withdraw from this quarter____";
            cout<<"\n\nPress ENTER key to return to Course Withdrawal Menu >> "<<flush;
            system("read");
            student_withdraw(username);
        }
        
        
        cout<<"-------------------------------------------------------------\n";
        cout<<"Course Code       Title"<<"\n";
        cout<<"-------------------------------------------------------------\n";
        
        for(int i=0;i<numrows;i++)
        {
            row=mysql_fetch_row(res_set);
            if(row!=NULL)
            {
                cout<<row[0]<<"          "<<row[1]<<'\n';
                
            }
        }
        cout<<"-------------------------------------------------------------\n";
        cout<<"\nEnter the course number OR 0 to return to Withdraw Menu >> ";
        cin>>course_choice;
        
        char query2[300];
        sprintf(query2,"call withdraw(%d,'%s','%s', %d, @res);", username, course_choice, nextquarter, nextyear);
        mysql_query(connection,query2);
        res_set=mysql_store_result(connection);
        
        numrows=(int)mysql_num_rows(res_set);
        
        if (strcmp(course_choice,"0")==0)
        {
            student_menu(username);
        }
        else
        {
            for(int i=0;i<numrows;i++)
            {
                row=mysql_fetch_row(res_set);
                if(row!=NULL)
                {
                    cout<<'\n'<<row[0]<<'\t';
                    
                }
            }
        }
        mysql_close(connection);
        
        
        
        
        MYSQL *connection, mysql;
        mysql_init(&mysql);
        connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
        
        if (connection == NULL)
        {
            //If database connection is lost
            cout<<"The database connection has been lost! Please check and try again.\n";
        }
        
        //Call trigger after each enroll/withdraw and display result for the course
        MYSQL_RES *res_setw;
        char query3[100];
        sprintf(query3,"SELECT * FROM TriggerWarning;");
        mysql_query(connection,query3);
        res_setw=mysql_store_result(connection);
        int numrows1=(int)mysql_num_rows(res_setw);
        
        if(numrows1!=0)
        {
            cout << "\nWARNING: Enrollment below 50%\n" <<endl;
            mysql_free_result(res_setw);
        }
        mysql_close(connection);
    }
    else if (menu_choice=='0')
    {
        mysql_close(connection);
        student_menu(username);
    }
    mysql_close(connection);
    cout<<"\nPress ENTER key to return to Withdraw Menu >> "<<flush;
    system("read");
    student_withdraw(username);
    
}





//This function shows the personal details of the student
void student_details(int username)
{
    system("clear");
    MYSQL *connection, mysql;
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
    
    if (connection == NULL)
    {
        //If database connection is lost
        cout<<"The database connection has been lost! Please check and try again.\n";
    }
    
    MYSQL_RES *res_set;
    MYSQL_ROW row;
    
    char query1[300];
    char query2[300];
    char query3[300];
    
    sprintf(query1,"SELECT * FROM student WHERE id=%d;", username);
    mysql_query(connection,query1);
    res_set=mysql_store_result(connection);
    row = mysql_fetch_row( res_set );
    
    //To hide password
    char hidden_pass[13];
    strcpy(hidden_pass, row[2]);
    for(int i=1;i<=strlen(hidden_pass);i++)
    {if (i!=1 && i!=strlen(hidden_pass))
        hidden_pass[i-1]='*';
    }
    
    
    cout << "-----------------------------------" << endl;
    cout << "|        PERSONAL DETAILS :       |" << endl;
    cout << "-----------------------------------" << endl;
    cout << " StudentID : "<<row[0]<<endl;
    cout << " Name      : "<<row[1]<<endl;
    cout << " Password  : "<<hidden_pass<<endl;
    cout << " Address   : "<<row[3]<<endl;
    cout << "-----------------------------------"<<endl;
    cout <<"\nSelect from following options:\n";
    cout << "1. CHANGE PASSWORD"<<endl<<"2. CHANGE ADDRESS"<<endl<<"0. RETURN TO MENU"<<endl;
    cout << "\nEnter your selection >> "<<flush;
    string change;
    cin >> change;
    if(change == "1")
    {   system("clear");
        char newpassword[11];
        char compare_pass[11];
        cout << "\nCHANGE PASSWORD (Max Characters : 10)";
        cout << "\n\nEnter new password : ";
        cin >> newpassword;
        cout << "Re-enter  password : ";
        cin >> compare_pass;
        if (strcmp(newpassword, compare_pass)==0)
        {
            sprintf(query2,"UPDATE student SET student.Password = '%s' WHERE student.Id = %d;", newpassword,username);
            mysql_query(connection,"START TRANSACTION;");
            mysql_query(connection, query2);
            mysql_query(connection,"COMMIT;");
            res_set = mysql_store_result(connection);
            cout << "\n____Password has been successfully changed____" << endl;
            cout<<"\nPress ENTER key to return to Personal Details Menu>> "<<flush;
            system("read");
            student_details(username);
        }
        else
        {
            cout<<"\nPasswords do not match. \n\nPress ENTER key to return to Personal Details Menu >> "<<flush;
            system("read");
            student_details(username);
        }
    }
    else if( change == "2")
    {
        system("clear");
        cout << "\nCHANGE ADDRESS";
        cout << "\n\nEnter new address : ";
        char newaddress[49];
        cin >> newaddress;
        sprintf(query3,"UPDATE student SET student.address = '%s' WHERE student.Id = %d;", newaddress,username);
        mysql_query(connection,"START TRANSACTION;");
        mysql_query(connection, query3);
        mysql_query(connection,"COMMIT;");
        res_set = mysql_store_result(connection);
        cout << "\n____Address has been successfully changed____" << endl;
        cout<<"\nPress ENTER key to return to Personal Details Menu>> "<<flush;
        system("read");
        student_details(username);
    }
    else if (change == "0")
    {
        cout<<"\n\nReturning to Main Menu..."<<flush;
        usleep(1000000);
        student_menu(username);
    }
    else
    {
        cout<<"\nInvalid Choice! Press ENTER key to retry >> "<<flush;
        system("read");
        student_details(username);
    }
}






// Function to enroll students by calling the procedure
void student_enroll(int username)
{
    MYSQL *connection, mysql;
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
    
    if (connection == NULL)
    {
        //If database connection is lost
        cout<<"The database connection has been lost! Please check and try again.\n";
    }
    
    time_t timer;
    time(&timer);
    tm* t_tm = localtime(&timer);
    int year = t_tm->tm_year + 1900;
    int month = t_tm->tm_mon + 1;
    
    char nextquarter[10];
    char quarter[10];
    system("clear");
    cout <<setw(50)<< "-----------------------------------------------------------" << endl;
    cout <<setw(50)<< "|               STUDENT COURSE ENROLLMENT MENU            |" << endl;
    cout <<setw(50)<< "-----------------------------------------------------------" << endl;
    
    int nextyear = 0;
    if (month==1||month==2||month==3)
    {
        strcpy(quarter,"Q2");
        strcpy(nextquarter,"Q3");
        nextyear=year;
    }
    else if (month==4||month==5||month==6)
    {
        strcpy(quarter,"Q3");
        strcpy(nextquarter,"Q4");
        nextyear=year;
    }
    
    else if (month==7||month==8)
    {
        strcpy(quarter,"Q4");
        strcpy(nextquarter,"Q1");
        nextyear=year;
    }
    
    else if (month==9||month==10||month==11||month==12)
    {
        strcpy(quarter,"Q1");
        strcpy(nextquarter,"Q2");
        nextyear=year+1;
    }
    
    
    cout<<"\nSelect the quarter and year: \n";
    cout<<"1. "<<quarter<<" "<<year<<'\n';
    cout<<"2. "<<nextquarter<<" "<<nextyear<<'\n';
    cout<<"0. RETURN TO MAIN MENU";
    cout<<"\n\nENTER YOUR CHOICE >> ";
    
    char choice1;
    cin>>choice1;
    
    if (choice1=='1')
    {
        
        MYSQL_RES *res_set;
        MYSQL_ROW row;
        char query1[700];
        sprintf(query1,"SELECT a.UoSCode,b.uosname,a.semester,a.year,d.type, a.MaxEnrollment FROM unitofstudy b natural join uosoffering a natural join lecture c natural join classroom d join student e WHERE e.id=%d and a.semester='%s' and a.year=%d ;",username, quarter,year);
        
        
        mysql_query(connection,query1);
        res_set=mysql_store_result(connection);
        int numrows=(int)mysql_num_rows(res_set);
        cout<<"-------------------------------------------------------------\n";
        cout<<"Course Code       Title"<<"\n";
        cout<<"-------------------------------------------------------------\n";
        for(int i=0;i<numrows;i++)
        {
            row=mysql_fetch_row(res_set);
            if(row!=NULL)
            {
                {
                    cout<<row[0]<<"          "<<row[1]<<'\n';
                }
                
            }
        }
        cout<<"-------------------------------------------------------------\n";
        cout<<"\nEnter the course number OR 0 to return to Enroll Menu >> ";
        char choice3[100];
        cin>>choice3;
        char query2[300];
        
        if (strcmp(choice3,"0")==0)
        {
            student_enroll(username);}
        
        sprintf(query2,"CALL enroll(%d,'%s','%s',%d, @red, @req, @req2);", username,choice3,quarter,year);
        mysql_query(connection,query2);
        res_set=mysql_store_result(connection);
        numrows=(int)mysql_num_rows(res_set);
        
        for(int i=0;i<numrows;i++)
        {
            row=mysql_fetch_row(res_set);
            if(row!=NULL)
            {
                char s[100];
                std::strcpy(s, row[0]);
                cout<<endl<<endl<<s<<'\t';
                if (row[1]!=NULL)
                {
                    cout<<"\nPre-req 1: "<<row[1]<<'\n';
                }
                if(row[2]!=NULL)
                {
                    cout<<"Pre-req 2: "<<row[2]<<'\n';
                }
            }
            else
            {
                cout<<"NULL";
            }
            
        }
        mysql_free_result(res_set);
        mysql_close(connection);
        
        
        MYSQL *connection, mysql;
        mysql_init(&mysql);
        connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
        
        if (connection == NULL)
        {
            //If database connection is lost
            cout<<"The database connection has been lost! Please check and try again.\n";
        }
        
        MYSQL_RES *res_setw;
        char query3[100];
        sprintf(query3,"SELECT * FROM TriggerWarning;");
        mysql_query(connection,query3);
        res_setw=mysql_store_result(connection);
        int numrows1=(int)mysql_num_rows(res_setw);
        
        //Call trigger after each enroll/withdraw and display result for the course
        if(numrows1!=0)
        {
            cout << "\nWARNING: Enrollment below 50%\n" << endl;
            mysql_free_result(res_setw);
        }
        mysql_close(connection);
        
    }
    
    else if (choice1=='2')
    {
        
        MYSQL_RES *res_set;
        MYSQL_ROW row = nullptr;
        
        char query[700];
        sprintf(query,"SELECT a.UoSCode,b.uosname,a.semester,a.year,d.type, a.MaxEnrollment FROM unitofstudy b natural join uosoffering a natural join lecture c natural join classroom d join student e WHERE e.id=%d and a.semester='%s' and a.year=%d;",username, nextquarter,nextyear);
        mysql_query(connection,query);
        res_set=mysql_store_result(connection);
        int numrows=(int)mysql_num_rows(res_set);
        cout<<"-------------------------------------------------------------\n";
        cout<<"Course Code       Title"<<"\n";
        cout<<"-------------------------------------------------------------\n";
        for(int i=0;i<numrows;i++)
        {
            row=mysql_fetch_row(res_set);
            if(row!=NULL)
            {
                cout<<row[0]<<"          "<<row[1]<<'\n';
                
            }
        }
        cout<<"-------------------------------------------------------------\n";
        cout<<"\nEnter the course number OR 0 to return to Enroll Menu >> ";
        char choice3[10];
        cin>>choice3;
        if (strcmp(choice3,"0")==0)
        {
            student_enroll(username);
            
        }
        
        sprintf(query,"CALL enroll(%d,'%s','%s',%d,@red,@req,@req2);", username,choice3,nextquarter,nextyear);
        mysql_query(connection,query);
        res_set=mysql_store_result(connection);
        numrows=(int)mysql_num_rows(res_set);
        
        //To display pre-requisites required
        for(int i=0;i<numrows;i++)
        {
            
            if(row!=NULL)
            {
                row=mysql_fetch_row(res_set);
                cout<<endl<<endl<<row[0]<<'\t';
                if (row[1]!=NULL)
                {
                    cout<<"\nPre-req 1: "<<row[1]<<'\n';
                }
                if(row[2]!=NULL)
                {
                    cout<<"Pre-req 2: "<<row[2]<<'\n';
                }
            }
        }
        mysql_free_result(res_set);
        mysql_close(connection);
        
        
        
        
        MYSQL *connection, mysql;
        mysql_init(&mysql);
        connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
        
        if (connection == NULL)
        {
            //If database connection is lost
            cout<<"The database connection has been lost! Please check and try again.\n";
        }
        
       
        //Call trigger after each enroll/withdraw and display result for the course
        MYSQL_RES *res_setw;
        char query3[100];
        sprintf(query3,"SELECT * FROM TriggerWarning;");
        mysql_query(connection,query3);
        res_setw=mysql_store_result(connection);
        int numrows1=(int)mysql_num_rows(res_setw);
        
        if(numrows1!=0)
        {
            cout << "\nWARNING: Enrollment below 50%\n" << endl;
            mysql_free_result(res_setw);
        }
        mysql_close(connection);
    }
    else if (choice1=='0')
    {
        mysql_close(connection);
        student_menu(username);
    }
    
    mysql_close(connection);
    
    
    
    cout<<"\nPress ENTER key to return to Enroll Menu >> "<<flush;
    system("read");
    student_enroll(username);
    
    
    
}





//Function to log user out of the system.
void logout()
{
    MYSQL *connection, mysql;
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql, "127.0.0.1", "root", "root", "project3-nudb", 3306, 0, 0);
    
    if (connection == NULL)
    {
        //If database connection is lost
        cout<<"The database connection has been lost! Please check and try again.\n";
    }
    
    cout<<"\nLogging out. Please Wait.\n";
    cout << '-' << std::flush;
    for (int i=0;i<50;i++) {
        usleep(10000);
        std::cout << "\b\\" << std::flush;
        usleep(10000);
        std::cout << "\b|" << std::flush;
        usleep(10000);
        std::cout << "\b/" << std::flush;
        usleep(10000);
        std::cout << "\b-" << std::flush;
    }
    system("clear");
    cout <<setw(50)<< "--------------------------------------------------------------" << endl;
    cout <<setw(50)<< "|            NORTHWESTERN STUDENT ACADEMIC SYSTEM            |" << endl;
    cout <<setw(50)<< "--------------------------------------------------------------" << endl;
    cout <<endl<<endl<<"You've logged out successfully. \nPress ENTER key to Login again >> "<<flush;
    system("read");
    login(connection);
    
}
