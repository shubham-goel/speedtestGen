#include <iostream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>
#include <list>
#include <fstream>
#include <ctime>
#include <chrono>
#include <string>
#include <cstdlib>
#include <stdio.h>
#include <iomanip>

using namespace std;

string intro=
"==================\n\
speedtestGen \n\
============\n\
This program generates customizable speed tests, checks the input, \n\
and prints score statistics to a file. \n\
 \n\
Source code is available at (github.com/shubham-goel/speedtestGen) \n\
Developed by @shubham-goel \n\
MIT License \n\
==================\n\
";

struct tm* get_curr_time() {
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    return now;
}

double chrono_to_sec(chrono::steady_clock::time_point end, chrono::steady_clock::time_point start){
	return (chrono::duration_cast<chrono::microseconds>(end - start).count())/1000000.0;
    // int r = input;
    // if(r%1000 > 500){
    //     r = r/1000 + 1;
    // }
    // else{
    //     r = r/1000;
    // }
    // return r;
}
int minimum(int a, int b){
    return (a<b)?a:b;
}
int random_num(int a, int b){
    //srand(time(NULL));
    if(b<=a){return 0;}
    int r = rand()%(b-a) + a;
    return r;
}
string clean(string str){
    int start,end,len=str.length();
    for(start=0;start<len;start++){
        if(str[start]!=' '){
            break;
        }
    }

    for(end=len-1;end>0;end--){
        if(str[end]!=' '){
            break;
        }
    }

//    char * res = new char[end-start+2];
//
//    int i=0;
//    for(;start<=end;i++,start++){
//        res[i] = str[start];
//    }
//    res[i]='\0';
//
//    return res;

    return str.substr(start,end-start+1);
}
class question{
public:
    int num1;
    int num2;
    int ans_entered;
    int ans;
    char type;
    chrono::steady_clock::time_point start,end;
    void print_to_file(ofstream&file, chrono::steady_clock::time_point init){
        if(is_correct()){
            file<<"Correct\t\t";
        }
        else{
            file<<"Wrong\t\t";
        }
        file<<"Time Taken: "<<setprecision(4)<<chrono_to_sec(end,start)<<"\t";
        file<<"Total Time: "<<setprecision(4)<<chrono_to_sec(end,init)<<"\t\t";
        file <<num1<<" "<<type<<" "<<num2<<" = ";
        if(is_correct()){
            file<< ans_entered;
        }
        else{
            file<<"Entered:"<< ans_entered<<"\tCorrect: "<<ans;
        }
        file<<endl;
        return;
    };
    bool get_ans(){
        cout<<num1<<" "<<type<<" "<<num2<<" = ";
        start=std::chrono::steady_clock::now();
        cin>>ans_entered;
        end= std::chrono::steady_clock::now();
        return is_correct();
    }
    bool is_correct(){
        return (ans_entered==ans);
    }
    question():num1(0),num2(0),ans(0){}

};

class add_ques: public question{
public:
        add_ques(int a, int b, int c, int d){
        type = '+';
        num1 = random_num(a,b);
        num2 = random_num(c,d);
        ans = num1 + num2 ;
    }

};
class subt_ques: public question{
public:
    subt_ques(int a, int b, int c, int d){
//        cout<<a<<" "<<b<<" "<<c<<" "<<d<<endl;

        type = '-';
        num1 = random_num(a,b);
//        cout<<num1<<endl;
        num2 = random_num(c,minimum(num1,d));
//        cout<<num2<<endl;
        ans = num1 - num2 ;
//        cout<<ans<<endl;
    }
};
class multi_ques: public question{
public:
    multi_ques(int a, int b, int c, int d){

        type = 'X';
        num1 = random_num(a,b);
        num2 = random_num(c,d);
        ans = num1 * num2 ;
    }
};
class div_ques: public question{
public:
    div_ques(int a, int b, int c, int d){

        type = '/';
        num1 = random_num(a,b);
        num2 = random_num(c,d);
        ans = num1 / num2 ;
        num1 = num2 * ans;
    }
};

class speed_test{
    int num_ques;
    list<question*> questions;
    pair<int,int> add_range[2] , subt_range[2] , multi_range[2] , div_range[2] ;
    int score;
    chrono::steady_clock::time_point init,end;
    void set_ranges(){
        cout<<"Please Enter Number Ranges as <number start> <number end>.\n";

        cout<<"\nFor Addition:\n \tEnter Range for first number : "; get_pair(add_range[0]);
        cout<<"\tEnter Range for second number : "; get_pair(add_range[1]);

        cout<<"\nFor Subtraction:\n \tEnter Range for first number : "; get_pair(subt_range[0]);
        cout<<"\tEnter Range for second number : "; get_pair(subt_range[1]);

        cout<<"\nFor Multiplication:\n \tEnter Range for first number : "; get_pair(multi_range[0]);
        cout<<"\tEnter Range for second number : "; get_pair(multi_range[1]);

        cout<<"\nFor Division:\n \tEnter Range for first number : "; get_pair(div_range[0]);
        cout<<"\tEnter Range for second number : "; get_pair(div_range[1]);
//
//        cout<<"Range set"<<endl;
       cout<<endl;
    }
    void get_pair(pair<int,int>&a ){
        cin>> a.first;
        cin>> a.second;
        if(a.second<=a.first){
            cout<< "\nPLEASE ENTER CORRECT RANGES\n";
        }
//        cout<< a.first<<" "<<a.second<<endl;

    }
    void generate_test(){

        auto it = questions.begin();
        for(int i=0; i<num_ques ; i++){
            question* new_ques;

//            int random=random_num(0,4);
//            cout<<"type of question: "<<random<<endl;

            switch(random_num(0,4)){
            case 0: //Addition
                new_ques = new add_ques(add_range[0].first, add_range[0].second, add_range[1].first, add_range[1].second );
//                cout<<"Addition Ques generated"<<endl;
                break;
            case 1: //Subtraction
                new_ques = new subt_ques(subt_range[0].first, subt_range[0].second, subt_range[1].first, subt_range[1].second );
//                cout<<"Subt Ques generated"<<endl;
                break;
            case 2: //Multiplication
                new_ques = new multi_ques(multi_range[0].first, multi_range[0].second, multi_range[1].first, multi_range[1].second );
//                cout<<"Multi Ques generated"<<endl;
                break;
            case 3: //Division
                new_ques = new div_ques(div_range[0].first, div_range[0].second, div_range[1].first, div_range[1].second );
//                cout<<"Div Ques generated"<<endl;
                break;
            default:
//                cout<<"Unhandled Case!!"<<endl;
                break;
            }

            questions.push_back(new_ques);
        }

        cout<<"Test generated"<<endl;
    }
    void get_answers(){
        auto it = questions.begin();

        init=std::chrono::steady_clock::now();

        for(int i=0;it!=questions.end();i++,it++){
            cout<<i<<".\t";
            if((*it)->get_ans()){
                score++;
            }
        }

        end=std::chrono::steady_clock::now();;

        cout<<"\nYour Score: "<<get_score()<<"/"<<num_ques<<"\n\n";
    }
    int get_score(){
        return score;
    }
    void print_to_file(){
        char c;
        char year[50],mon[50],day[50],hour[50],min[50],sec[50];
        string fname;
        struct tm* now = get_curr_time();
        sprintf(year,"%d", now->tm_year + 1900);
        sprintf(mon,"%d",(now->tm_mon + 1));
        sprintf(day,"%d",now->tm_mday);
        sprintf(hour,"%d",now->tm_hour);
        sprintf(min,"%d",now->tm_min);
        sprintf(sec,"%d",now->tm_sec);
        cout<<"Enter Y if date/time displayed correct => ";
        cout<<day<<'-'<<mon<<'-'<<year<<" : "<<hour<<"hrs "<<min<<"mins "<<sec<<"sec\t";
        cin>>c;
        if (c == 'Y' or c == 'y')
        {
            fname = string(day) + '-' + string(mon) + '-' + string(year) + "--" + string(hour) + '.' + string(min) + '.' + string(sec);
        }
        else
        {
            cout<<"\nSaving Results to file"<<endl;
            cout<<"Enter File Name : ";
            cin>>fname;
        }
        fname = "Results/"+fname;

        ofstream file;
        file.open(clean(fname)+".txt");
        cin.get();
        while(!file.is_open()){
            cout<<"\nERROR CREATING FILE : " + clean(fname)+".txt" + "\n";
            cout<<"Please create 'Results' folder if missing"<<endl;
            cout<<"Press Enter to continue ";
            cin.get();
            cout<<endl;
            file.open(clean(fname)+".txt");
        }
        // cout<<"Saving results to file : "<<clean(fname)+".txt"<<endl;

        file << "SPEED TEST"<<endl<<endl;
        file<< "Score : "<<get_score()<<"/"<<num_ques<<"\t\t"<<setprecision(4)<<"Total Time Taken: "<<chrono_to_sec(end,init)<<"\n";

        auto it = questions.begin() ;
        for(int i=1;it!=questions.end();i++,it++){
            file<<i<<"\t";
            (*it)->print_to_file(file,init);
        }
        cout<<"\nSaved results to file : "<<clean(fname)+".txt"<<endl<<endl;
    }
public:
    speed_test(int x):num_ques(x),score(0){
        set_ranges();
        generate_test();
        cout<<"\n\nEnter Y to start speed-test timer : ";
        char c='n';
        cin>>c;
        while(! (c=='y' or c=='Y')){
            cout<<"Enter Y to start : ";
            cin>>c;
        }
        cout<<endl;
        get_answers();
        print_to_file();
        cout<<"Thank you for using using speedtestGen!"<<endl;
    }
};


int main(){
    cout<<intro<<endl;
    srand(time(NULL));
    int num=0;
    cout<<"Enter Number of Questions : ";
    cin>>num;
    cout<<endl;
    speed_test myTest(num);
    cout<<"\nPress Enter to exit";
    cin.get();
    cin.get();
}
