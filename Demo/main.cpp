#include <iostream>
#include <string>
#include <unordered_map>
#include <mutex>

#include "Executor.h"
#include "DataGeter.h"
#include "Logger.h"
#include "TelegramBot.h"
#include "tokens.h"
#include "ConditionJson.h"


#include <thread> // Для работы с потоками

// g++ -o main main.cpp Executor.cpp -pthread I2CLCD.cpp -lwiringPi RelayControlModule.cpp DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp DataCombine.cpp crcCalc.cpp -lz DataGeter.cpp DS18B20.cpp WeatherAPI.cpp -lcurl Logger.cpp jsonManager.cpp -I /path/to/nlohmann/json ChartDrawing.cpp -I/usr/include/cairo -lcairo TimeManager.cpp TelegramBot.cpp -I /path/to/nlohmann/json -lTgBot -lboost_system -lssl -lcrypto -lpthread  
// g++ -o executor Executor.cpp -pthread I2CLCD.cpp -lwiringPi RelayControlModule.cpp DataParsing.cpp TreeParsing.cpp MessageQueuing.cpp SerialComm.cpp DataCombine.cpp crcCalc.cpp -lz 
// g++ -o datagetter DataGeter.cpp DS18B20.cpp WeatherAPI.cpp -lcurl
// g++ -o logger Logger.cpp jsonManager.cpp -I /path/to/nlohmann/json ChartDrawing.cpp -I/usr/include/cairo -lcairo TimeManager.cpp 
//g++ -std=c++17 -o test TelegramBot.cpp -I /path/to/nlohmann/json -lTgBot -lboost_system -lssl -lcrypto -lpthread 


std::string doubleToString(double value) {
    char buffer[50];  // Буфер для хранения отформатированной строки
    sprintf(buffer, "%.2f", value);  // Форматируем число с 2 знаками после запятой
    return std::string(buffer);  // Возвращаем строку
}
std::string getDay(){
   return unixTimestampToDateString(to_unix_timestamp(std::chrono::system_clock::now()));
}

std::string getDayHMS(){
   return unixTimestampToTimeStringDate(to_unix_timestamp(std::chrono::system_clock::now()));
}




std::vector<std::unordered_map<std::string, std::string>> getterCommandsMap = {
    {{"TG_BOT", "getTemp"} , {"DATA_GETTER", "temp"}},
    {{"TG_BOT", "getTemp2"} , {"DATA_GETTER", "temp2"}},

    {{"TG_BOT", "getTempInBake"} , {"DATA_GETTER", "inBake"}},
    {{"TG_BOT", "getTempOutBake"} , {"DATA_GETTER", "outBake"}},

    {{"TG_BOT", "getTempOut"}, {"DATA_GETTER", "tempOut"}},

    {{"TG_BOT", "getChart"}, {"SAND_CHART", "true"}},
    {{"TG_BOT", "getLogChart"}, {"SAND_CHART", "true"}, {"SAND_LOG", "true"}},
    {{"TG_BOT", "getStates"}, {"SAND_STATES", "true"}},

    

    {{"TG_BOT", "getConditionJson"}, {"SAND_COND_JSON", "true"}},
    {{"TG_BOT", "getConditionTree"}, {"SAND_COND_TREE", "true"}}

    
};

std::vector<std::unordered_map<std::string, std::string>> executeCommandsMap = {
    {{"TG_BOT", "BakeOn"} , {"EXECUTOR", "Bake_ON"}, {"MODE", "on"}, {"MODULE", "Bake"}},
    {{"TG_BOT", "BakeOff"}, {"EXECUTOR", "Bake_OFF"}, {"MODE", "off"}, {"MODULE", "Bake"}},
    {{"TG_BOT", "BakeAuto"},{"MODE", "auto"}, {"MODULE", "Bake"}},
    {{"TG_BOT", "PumpOn"}, {"EXECUTOR", "Pump_ON"}, {"MODE", "on"}, {"MODULE", "Pump"}},
    {{"TG_BOT", "PumpOff"}, {"EXECUTOR", "Pump_OFF"}, {"MODE", "off"}, {"MODULE", "Pump"}},
    {{"TG_BOT", "PumpAuto"}, {"MODE", "auto"},{"MODULE", "Pump"}},
    {{"TG_BOT", "Falcon1On"}, {"EXECUTOR", "Falcon1_ON"}, {"MODE", "on"}, {"MODULE", "Falcon1"}},
    {{"TG_BOT", "Falcon1Off"}, {"EXECUTOR", "Falcon1_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon1"}},
    {{"TG_BOT", "Falcon1Auto"}, {"MODE", "auto"},{"MODULE", "Falcon1"}},
    {{"TG_BOT", "Falcon2On"}, {"EXECUTOR", "Falcon2_ON"}, {"MODE", "on"}, {"MODULE", "Falcon2"}},
    {{"TG_BOT", "Falcon2Off"}, {"EXECUTOR", "Falcon2_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon2"}},
    {{"TG_BOT", "Falcon2Auto"}, {"MODE", "auto"},{"MODULE", "Falcon2"}},
    {{"TG_BOT", "Falcon3On"}, {"EXECUTOR", "Falcon3_ON"}, {"MODE", "on"}, {"MODULE", "Falcon3"}},
    {{"TG_BOT", "Falcon3Off"}, {"EXECUTOR", "Falcon3_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon3"}},
    {{"TG_BOT", "Falcon3Auto"}, {"MODE", "auto"},{"MODULE", "Falcon3"}},
    {{"TG_BOT", "Falcon4On"}, {"EXECUTOR", "Falcon4_ON"}, {"MODE", "on"}, {"MODULE", "Falcon4"}},
    {{"TG_BOT", "Falcon4Off"}, {"EXECUTOR", "Falcon4_OFF"}, {"MODE", "off"}, {"MODULE", "Falcon4"}},
    {{"TG_BOT", "Falcon4Auto"}, {"MODE", "auto"},{"MODULE", "Falcon4"}},
    {{"TG_BOT", "ReadNewJsonCond"}, {"READ_NEW_JSON_DATA", "true"}}

};






std::mutex Mutex;



void getterCommandTGBot(const std::string& arg);
void executorCommandTGBot(const std::string& arg);




//Registers Initilization

std::unordered_map<std::string, std::string> getterRegistor = {
    {"date","0"},
    {"dateDaily","0"},//Daily Time
    {"temp","-255"},
    {"temp2","-255"},

    {"inBake","-255"},
    {"outBake","-255"},

    {"tempOut","-255"},
};

std::unordered_map<std::string, bool> executorRegister = {
   
    {"Bake", false},
    {"Pump", false},
    {"Falcon1", false},
    {"Falcon2", false},
    {"Falcon3", false},
    {"Falcon4", false}

};

std::unordered_map<std::string, bool> autoModeExecutorRegister = {
    {"Bake", true},
    {"Pump", true},
    {"Falcon1", true},
    {"Falcon2", true},
    {"Falcon3", true},
    {"Falcon4", true}

};


void doExecuteAuto(const std::string& exe); //init

Executor executor;
DataGetter dataGetter;
Logger LOG("Log/Log"+getDay(),{"date","temp","temp2","inBake", "outBake","tempOut","Bake","Pump","Falcon1","Falcon2","Falcon3","Falcon4"});
TelegramBot bot("7804127004:AAEQkzTISnsoBpESYJQdVERP6gX10d6rA1c"/*TOKEN_TELEGRAM*/,getterCommandTGBot,executorCommandTGBot);
Composite  MainPattern(getterRegistor,doExecuteAuto, "main", "Always", {}, "");



void getterRegisterUpdate(){
   //Maded for same types 
   std::lock_guard<std::mutex> lock(Mutex);
   for(auto& pair  : getterRegistor){
      //std::cout<<pair.first <<" "<< dataGetter.getNewData(pair.first)<<std::endl;
      if(pair.first == "date"){
      
         pair.second =  std::to_string(to_unix_timestamp(std::chrono::system_clock::now()));
      }
      else if(pair.first == "dateDaily" ){
         pair.second =  getDayHMS();//Daily Time
      }
      else
         pair.second =  std::to_string(dataGetter.getNewData(pair.first));
   }

}
void getterRegisterLog(){

   std::lock_guard<std::mutex> lock(Mutex);
   //LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
   
   LOG.setData("date", getterRegistor["date"]);
   LOG.setData("dateDaily", getterRegistor["dateDaily"]);
   LOG.setData("temp", getterRegistor["temp"]);
   LOG.setData("temp2", getterRegistor["temp2"]);
   LOG.setData("inBake", getterRegistor["inBake"]);
   LOG.setData("outBake", getterRegistor["outBake"]);
   LOG.setData("tempOut", getterRegistor["tempOut"]);

   LOG.log();
}
void LogUnion(const std::string& first, const std::string& second){
   LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));//poxel
   LOG.setData(first,second);
   LOG.log();
}
void GetterAndLog(){
   
      getterRegisterUpdate();
      getterRegisterLog();
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   
}
void GetterAndLogLoop(){
   while(true){
      GetterAndLog();
   }
}


void doExecuteManual(const std::string& exe){
   std::lock_guard<std::mutex> lock(Mutex);

   for(auto it : executeCommandsMap){
      if(it["EXECUTOR"]  == exe) {
         
                 
                  std::cout<<"Try Do Manual "<<it["MODULE"]<<" "<<it["MODE"]<<" reg("<<executorRegister[it["MODULE"]]<<")"<<std::endl;
                  if(executorRegister[it["MODULE"]] != (it["MODE"] == "on")){

                     
                     executor.execute(exe);
                     LogUnion(it["MODULE"],it["MODE"]);
                     executorRegister[it["MODULE"]] = (it["MODE"] == "on");
                     std::cout<<"Do Manual "<<it["MODULE"]<<" "<<it["MODE"]<<" reg("<<executorRegister[it["MODULE"]]<<")"<<std::endl;
                  }
                  autoModeExecutorRegister[it["MODULE"]] = false;
             

         
      }
   }
    
}
void doExecuteAuto(const std::string& exe){
   
   for(auto it : executeCommandsMap){
      if(it["EXECUTOR"] == exe) {
         
         if(autoModeExecutorRegister[it["MODULE"]]){
                 
                  
                  if(executorRegister[it["MODULE"]] != (it["MODE"] == "on")){
                     executor.execute(exe);
                     LogUnion(it["MODULE"],it["MODE"]);
                     executorRegister[it["MODULE"]] = (it["MODE"] == "on");
                  }

         }
      }
   }

}

void TurnOffAllRCM(){
   executor.execute("Bake_OFF");
   executor.execute("Pump_OFF");
   executor.execute("Falcon1_OFF");
   executor.execute("Falcon2_OFF");
   executor.execute("Falcon3_OFF");
   executor.execute("Falcon4_OFF");
   
   
}

void getterCommandTGBot(const std::string& arg){
   
   std::cout<<"from \033[34m[TG]\033[0m "<<arg<<std::endl;
   //std::lock_guard<std::mutex> lock(Mutex);
   for(auto it : getterCommandsMap){
      if(arg == it["TG_BOT"]) {
         if(it.find("DATA_GETTER") != it.end()){
            double dt = dataGetter.getNewData(it["DATA_GETTER"]);
            bot.sendAllUserMessage(doubleToString(dt));
         }
         if(it.find("SAND_CHART") != it.end()){
            LOG.drawChart("Chart"+getDay(),"date",{"temp","temp2","inBake", "outBake","tempOut"},{"Bake","Pump","Falcon1","Falcon2","Falcon3","Falcon4"});
            bot.sendAllUserPhoto("Chart"+getDay()+".png", getDay()+" Chart");

         }
         if(it.find("SAND_LOG") != it.end()){
            bot.sendAllUserDocument("Log/Log"+getDay()+".json",getDay() + "Log");

         }
         if(it.find("SAND_STATES") != it.end()){
            std::string data = "Executor:\n";
            data = "Name | State | Auto\n";

            for(auto it : executorRegister){
               data+=it.first;
               data+=" ";
               data+=it.second ? "ON" : "OFF";
               data+=" ";
               data+=autoModeExecutorRegister[it.first]? "AUTO" : "MANUAL";
               data+="\n";
               
            }
            bot.sendAllUserMessage(data);
            

         }
         
         if(it.find("SAND_COND_TREE") != it.end()){
            bot.sendAllUserMessage(MainPattern.getTreeString());
            

         }
         if(it.find("SAND_COND_JSON") != it.end()){
            bot.sendAllUserDocument("Condition.json",getDay() + " Condition");

         }
      }
      
   }
}

void sendConditionToTgBot(const std::string& str){
   bot.sendAllUserDocument(str,getDay() + " Old Condition");
}

void executorCommandTGBot(const std::string& arg){
   std::cout<<"from \033[34m[TG]\033[0m "<<arg<<std::endl;
   //std::lock_guard<std::mutex> lock(Mutex);
   for(auto it : executeCommandsMap){
      if(arg == it["TG_BOT"]) {

         if(it.find("EXECUTOR") != it.end()){
            doExecuteManual(it["EXECUTOR"]);
         }
         if(it.find("MODE") != it.end()){
            if(it["MODE"] == "auto"){
               autoModeExecutorRegister[it["MODULE"]] = true;
            }
         }
         if(it.find("READ_NEW_JSON_DATA") != it.end()){

            // Шаг 1: Проверка синтаксиса Condition_New.json
            if (validateJsonSyntax("Condition_New.json", MainPattern)) {
               // Шаг 2: Если все правильно, продолжаем
               // Шаг 3 и 4: Переименование и отправка
               renameAndProcessFiles(sendConditionToTgBot);
               std::cout << "Composite has been successfully updated." << std::endl;
            } else {
               std::cerr << "Error: Invalid JSON syntax in Condition_New.json" << std::endl;
            }
            std::cout<<"Condition restored!"<<std::endl;
         }
      }
   }
}

int nowSec(){
   return getDaySecondsFromTimePoint(std::chrono::system_clock::now());
}

int fromSec(int h, int m ,int s){
   return  getDaySecondsFrom3Int(h,m,s);

}



int main(){
  

  


   

   
   
   GetterAndLog();

   std::thread logThread(&GetterAndLogLoop);

   MainPattern.setRoot(fromJSONRecursively(jmCond.read_json_from_file()));

   std::thread botThread(&TelegramBot::run, &bot);

   TurnOffAllRCM();

   while(true){
      //getterRegisterUpdate();
      //getterRegisterLog();
      

      //if(nowSec() > fromSec(06,28,1))

      executor.execute("lcd",getterRegistor["temp"]);
      MainPattern.executeAll();



      /*
      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      doExecuteAuto("Bake_ON");
      

      std::this_thread::sleep_for(std::chrono::milliseconds(100));
      doExecuteAuto("Bake_OFF");
      

      if(std::stod(getterRegistor["temp"]) > 24){
         doExecuteAuto("Pump_ON");
      }

      if(std::stod(getterRegistor["temp"]) < 22){
         doExecuteAuto("Pump_OFF");
      }
      */
      

   }

   if (botThread.joinable()) {
        botThread.join();
    }

   if (logThread.joinable()) {
        logThread.join();
    }

   /*
   while(true){

      
      

      LOG.setData("temp",std::to_string( dataGetter.getNewData("temp")));
      
      LOG.setData("tempOut", std::to_string(dataGetter.getNewData("tempOut")));
      LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
      LOG.log();

      
      executor.execute("R1_ON");
      LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
      LOG.setData("R1","on");
      LOG.log();
      //std::this_thread::sleep_for(std::chrono::seconds(1));
      executor.execute("R1_OFF");
      LOG.setData("date", std::to_string(to_unix_timestamp(std::chrono::system_clock::now())));
      LOG.setData("R1","off");
      LOG.log();

      LOG.drawChart("mainChart","date",{"temp","tempOut"},{"R1","R2","R3","R4","R5","R6","R7","R8"});
      
      

      
   }
 

    std::this_thread::sleep_for(std::chrono::seconds(6));
    */

    std::cout << "Завершение программы" << std::endl;
    return 0;

}