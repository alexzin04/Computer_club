#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <sstream>
#include <regex>
#include <algorithm>
#include <iomanip>

std::string format_time(int hour,int minute);

class Computer_table{
    int time_of_use;//time in minute
    int last_time_sit;//time when client sit in minute
    bool free;
    std::vector<int> use_hour;
    std::string name;
    public:
        Computer_table(){
            time_of_use=0;
            last_time_sit=0;
            free=true;
        }
        bool get_free() const {return free;}
        void sit_on(int hour,int minute,const std::string & name_client){
            free=false;
            name=name_client;
            if(!use_hour.empty()){
                if(hour!=use_hour[use_hour.size()-1]){//так как вектор по созданию получается отсортированым и время следущего события >= предыдушего то достаточно проверить последний оплаченый час на данном столе
                    use_hour.push_back(hour);
                }
            }
            else{
                use_hour.push_back(hour);
            }
            last_time_sit=hour*60+minute;
        }
        void get_out(int hour,int minute){
            name="";
            free=true;
            int times_in_minute=minute+hour*60;
            time_of_use+=times_in_minute-last_time_sit;
            int last_hour=last_time_sit/60+1;
            if(minute!=0){
                hour++;
            }
            while (last_hour!=hour){
                use_hour.push_back(last_hour);
                ++last_hour;
            }
        }
        std::string get_name() const {return name;}
        int get_time_of_use() const {return time_of_use;}
        int get_income(int price){return use_hour.size()*price;}
};

class Computer_club{
    int count_table;
    int count_for_hour;
    int time_start_hour;
    int time_start_minute;
    int time_finish_hour;
    int time_finish_minute;
    std::vector<Computer_table> tables;
    std::vector<std::string> users;
    std::queue<std::string> users_wait;
    std::ifstream in;
    bool check_file(const char* filename);//can open file and right format
    bool check_time(int hour,int minute);//check club open or not true - if club is open false - if club is not  open
    bool check_in_club(const std::string& name);//check users in club true - in club false - not in club
    bool check_free_table();//true - if free table in club false - if all table not free
    int number_of_table(const std::string & name);
    void get_time_from_str(const std::string line,int & hour,int & minute);
    void client_go_out(const std::string & name);
    void ID_1(const std::string & line);//Event_1
    void ID_2(const std::string & line);//Event_2
    void ID_3(const std::string & line);//Event_3
    void ID_4(const std::string & line);//Event_4
    void ID_11(const std::string & time,const std::string & name);//Event_11
    void ID_12(const std::string & time,const std::string & name,int number_of_table);//Event_12*/
    void ID_13(const std::string & time,const std::string & error);//Event_13
    void club_close();
    public:
        Computer_club(const char* filename){
            if(!check_file(filename)){
                exit(1);//error in input data
            }
            std::string line;
            in>>count_table;
            tables.resize(count_table);
            in>>line;
            get_time_from_str(line,time_start_hour,time_start_minute);
            in>>line;
            get_time_from_str(line,time_finish_hour,time_finish_minute);
            in>>count_for_hour;
            std::getline(in,line);
            std::cout<<format_time(time_start_hour,time_start_minute)<<std::endl;
            while(std::getline(in,line)){
                if (line.size()==0){//if \n in file
                    continue;
                }
                std::cout<<line<<std::endl;
                if(line[6]=='1'){
                    ID_1(line);
                }
                else if(line[6]=='2'){
                    ID_2(line);
                }
                else if(line[6]=='3'){
                    ID_3(line);
                }
                else{
                    ID_4(line);
                }
            }
            club_close();

        }
        ~Computer_club(){
            in.close();
        }

};

int help_function_for_param(const std::string & line,int &id,std::string & name,std::string & time){//if id==2 return number of table else return 0
    int table=0;
    time=line.substr(0,5);
    std::stringstream ss(line.substr(5));
    ss>>id>>name;
    if (id==2){
        ss>>table;
    }
    return table;
}

std::string format_time(int hour,int minute){
    std::ostringstream oss;
    oss<<std::setw(2)<<std::setfill('0')<<hour<<":"<<std::setw(2)<<std::setfill('0')<<minute;
    return oss.str();
}

std::string format_minute_to_hour(int time_minute){
    int hour=time_minute/60,minute=time_minute%60;
    return format_time(hour,minute);

}

void Computer_club::ID_1(const std::string & line){
    int hour,minute,id;
    std::string name,time;
    help_function_for_param(line,id,name,time);
    get_time_from_str(time,hour,minute);
    if(!check_time(hour,minute)){
        //generate error
        ID_13(time,"NotOpenYet");
        return;
    }
    if(check_in_club(name)){
        //generate error
        ID_13(time,"YouShallNotPass");
        return;
    }
    users.push_back(name);
}

void Computer_club::ID_2(const std::string & line){
    int id,number_of_table,hour,minute;
    std::string name,time;
    number_of_table=help_function_for_param(line,id,name,time);
    if (number_of_table>count_table){
        std::cerr<<"There is no table with such an index"<<std::endl;
        exit(1);
    }
    get_time_from_str(time,hour,minute);
    if(!check_in_club(name)){
        //generate error
        ID_13(time,"ClientUnknown");
        return;
    }
    if(!tables[number_of_table-1].get_free()){
        //generate error
        ID_13(time,"PlaceIsBusy");
        return;
    }
    else{
        tables[number_of_table-1].sit_on(hour,minute,name);
    }
}

void Computer_club::ID_3(const std::string & line){
    int id;
    std::string name,time;
    help_function_for_param(line,id,name,time);
    if(check_free_table()){
        //generate error
        ID_13(time,"ICanWaitNoLonger");
        return;
    }
    if(users_wait.size()==count_table){
        //client go out
        client_go_out(name);
        ID_11(time,name);
        return;
    }
    users_wait.push(name);
}

void Computer_club::ID_4(const std::string & line){
    int id,hour,minute,number_table;
    std::string name,time;
    help_function_for_param(line,id,name,time);
    get_time_from_str(time,hour,minute);
    if(!check_in_club(name)){
        //generate error
        ID_13(time,"ClientUnknown");
        return;
    }
    number_table=number_of_table(name);
    if (number_table>count_table){
        std::cerr<<"There is no table with such an index"<<std::endl;
        exit(1);
    }
    if (number_table!=-1){
        tables[number_table].get_out(hour,minute);
        client_go_out(name);
        if(!users_wait.empty()){
            do{
            name=users_wait.front();
            users_wait.pop();
            }while(!check_in_club(name));//if people who stand in que go out 
            tables[number_table].sit_on(hour,minute,name);
            ID_12(time,name,number_table+1);
        }
    }else{
        client_go_out(name);
    }
    
}

void Computer_club::ID_12(const std::string & time,const std::string & name,int number_of_table){
    std::cout<<time<<" 12 "<<name<<" "<<number_of_table<<std::endl;
}

void Computer_club::ID_11(const std::string & time,const std::string & name){
     std::cout<<time<<" 11 "<<name<<std::endl;
}

void Computer_club::ID_13(const std::string & time,const std::string & error){
    std::cout<<time<<" 13 "<<error<<std::endl;
}

void Computer_club::club_close(){
    std::string time=format_time(time_finish_hour,time_finish_minute);
    std::sort(users.begin(),users.end());
    for(int i=0;i<users.size();++i){
        int number=number_of_table(users[i]);
        if (number!=-1){
            tables[number].get_out(time_finish_hour,time_finish_minute);
        }
        ID_11(time,users[i]);
    }
    std::cout<<time<<std::endl;
    for(int i=0;i<tables.size();++i){
        std::cout<<i+1<<" "<<tables[i].get_income(count_for_hour)<<" "<<format_minute_to_hour(tables[i].get_time_of_use())<<std::endl;;
    }
}

void Computer_club::client_go_out(const std::string & name){
    auto it=std::find(users.begin(),users.end(),name);
    if(it!=users.end()){
        users.erase(it);
    }
    else{
        std::cerr<<"The client tries to leave but he was not in the club "<<name<<std::endl;
        exit(1);
    }
}

void Computer_club::get_time_from_str(const std::string line,int & hour,int & minute){
    std::stringstream ss(line);
    char skip;
    ss>>hour>>skip>>minute;
}

int Computer_club::number_of_table(const std::string & name){
    for(int i=0;i<tables.size();++i){
        if(tables[i].get_name()==name){
            return i;
        }
    }
    return -1;
}

bool Computer_club::check_free_table(){
    for(int i=0;i<tables.size();++i){
        if(tables[i].get_free()){
            return true;
        }
    }
    return false;
}

bool Computer_club::check_time(int hour,int minute){
    if (hour>time_start_hour && hour<time_finish_hour){
        return true;
    }
    if (hour==time_start_hour && minute>=time_start_minute){
        return true;
    }
    if (hour==time_finish_hour && minute<time_finish_minute){//если пришел ровно в момент закрытия то все равно ошибка(клуб закрыт) если нужно исправить добавить <= в сравнение минут
        return true;
    }
    return false;
}

bool Computer_club::check_in_club(const std::string& name){
    if(users.empty()){
        return false;
    }
    auto it=std::find(users.begin(),users.end(),name);
    if(it!=users.end()){
        return true;
    }
    return false;
}

bool is_valid_time(const std::string & time){
    std::regex time_patern(R"(^(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9])$)");
    return std::regex_match(time,time_patern);
}

bool Computer_club::check_file(const char* filename){
    in.open(filename);
    if (!in.is_open()){
        std::cerr << "Error in open fail"<<std::endl;
        return false;
    }
    int count_lines=0;
    std::string line;
    while(std::getline(in,line)){
        if(count_lines==0 || count_lines==2){
            if (!std::all_of(line.begin(), line.end(), isdigit)){
                std::cerr << "Error in lines: " <<line << std::endl;
                return false;
            }
        } 
        else if(count_lines==1){
            std::istringstream iss(line);
            std::string time1, time2;
            iss >> time1 >> time2;
            if (!is_valid_time(time1) || !is_valid_time(time2)) {
                std::cerr <<"Error in lines: " <<line << std::endl;
                return false;
            }
        }
        else {
                if(line.size()==0){
                    continue;
                }
                std::regex line_pattern(R"(^(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9]) ([1|3|4]) ([a-z0-9_-]+)$)");
                std::regex line_pattern_2(R"(^(0[0-9]|1[0-9]|2[0-3]):([0-5][0-9]) 2 ([a-z0-9_-]+) \d$)");
                if(!(std::regex_match(line, line_pattern) || std::regex_match(line, line_pattern_2))){
                    std::cerr <<"Error in lines: " <<line<< std::endl;
                    return false;
                }
        }
        count_lines++;
    }
    in.clear();
    in.seekg(0,std::ios::beg);
    return true;
}



int main(int argc,char** argv){
    if (argc!=2){
        std::cerr<<"Неверный запуск программы, необходим один файл на вход"<<std::endl;
        exit(1);
    }
    Computer_club club(argv[1]);

}