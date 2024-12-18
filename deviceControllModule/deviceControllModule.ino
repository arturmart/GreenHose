#include <Arduino.h>
#include "DataParser.h" // Include the DataParser class
#include "StringLimited.h" // Ensure this header file exists
#include "RelayController.h"
#include <Arduino_CRC32.h>
Arduino_CRC32 crc32;

#define BUFFER_SIZE 64
char inputBuffer[BUFFER_SIZE];
#define TERMINATOR '\n'

const int ERROR_NONE = 0;               
const int ERROR_SYNTAX = 1 << 0;       
const int ERROR_1L_NO_DATA = 1 << 1;   
const int ERROR_1L_TOO_MANY_DATA = 1 << 2;   
const int ERROR_INVALID_CRC = 1 << 3; 
const int ERROR_NULL_CRC = 1 << 4;   
const int ERROR_2L_NO_DATA_PACKETS = 1 << 5; // error chi
const int ERROR_2L_TOO_MANY_PACKETS = 1 << 6; 
const int ERROR_3L_WRONG_DATA_PACKETS = 1 << 7; 
const int PACKETS_COUNT = 1111 << 8;
const int GET_KEYWORD = 1 << 12;



const int RELAY_COUNT = 16;
int relayPin[RELAY_COUNT] = {30,32,34,36,38,40,42,44,22,24,26,28,4,5,6,7}; 
bool states[RELAY_COUNT];
RelayController relayController(relayPin, RELAY_COUNT); 

StringLimited<BUFFER_SIZE> str;   //83,0,1;83,1,1;83,2,1;83,3,1/61204cfc //83,0,0;83,1,0;83,2,0;83,3,0/745ec3dc
int feedback;
byte data[8][3];
DataParser parser;


 


StringLimited<BUFFER_SIZE> crcCalk(StringLimited<BUFFER_SIZE>& data,int base = HEX){
   
  
    uint32_t crc1 (crc32.calc(data.c_str(), data.size())); 
    char buf[1 + 8 * sizeof(unsigned long)];
	  ultoa(crc1, buf, base);
    StringLimited<BUFFER_SIZE> crc2(buf);
  
    return (crc2);
}

void serialWriteKeyWord(int feedback, enum DataParser::KEYWORD key){

      //Serial.print("feedbeck ");
      StringLimited<BUFFER_SIZE> write = "";

      switch (key){
        case DataParser::KEYWORD::KEYWORD_INITED:
            write = "dcmIsInited";
            //write += "/";
            //write.AddIntEnd(feedback,BIN);
       
            //Serial.println(write);
            Serial1.println(write+"/"+crcCalk(write));
          break;
        case DataParser::KEYWORD::KEYWORD_SHOWALL:
            write = "";
            write.AddIntEnd(relayController.getStates(),BIN);
            
            //write.AddIntEnd(feedback,BIN);
            //Serial.println(write);
            Serial1.println(write+"/"+crcCalk(write));
          break;
      }
      
      
   
    
}

void serialWriteFeedBack(int feedback){

      //Serial.print("feedbeck ");
      
      StringLimited<BUFFER_SIZE> write = "";
      
      write.AddIntEnd(feedback,BIN);
      


      Serial1.println(write+"/"+crcCalk(write));
   
      //if (feedback & ERROR_SYNTAX)                    Serial.println("[ER1-SY]");
      //if (feedback & ERROR_1L_NO_DATA)                Serial.println("[ER2-1N]");
      //if (feedback & ERROR_1L_TOO_MANY_DATA)          Serial.println("[ER3-1M]");
      //if (feedback & ERROR_INVALID_CRC)               Serial.println("[ER4-IC]");
      //if (feedback & ERROR_NULL_CRC)                  Serial.println("[ER5-NC]");
      //if (feedback & ERROR_2L_NO_DATA_PACKETS)        Serial.println("[ER6-2N]");
      //if (feedback & ERROR_2L_TOO_MANY_PACKETS)       Serial.println("[ER7-2M]");
      //if (feedback & ERROR_3L_WRONG_DATA_PACKETS)     Serial.println("[ER8-3W]");
}

void doAction(int size){

  //Serial.println("Do Action");

  for (int i = 0; i < size; i++) {
          //Serial.print("Row "); Serial.print(i); Serial.print(": ");for (int j = 0; j < 3; j++) {Serial.print(data[i][j]);Serial.print(" ");}Serial.println();

          if(data[i][0] == 83) {
            relayController.relayWrite(data[i][1],data[i][2]);
            //Serial.print("Servo "); Serial.print(data[i][1]); Serial.print(" is "); Serial.println(data[i][2]);
            }
      }

      
}
void setup() {
    //Serial.begin(9600);
    //Serial.println("___________");
    Serial1.begin(57600); // Инициализация последовательного порта


    pinMode(13, OUTPUT);
    digitalWrite(13,HIGH);
    
    parser.setCRC(crcCalk);
  


    /*
    parser.setStr(str);
    feedback = parser.parse( data); // Парсим данные, передавая указатель на строку и массив байтов
    Serial.print("feedbeck "); Serial.println(feedback,BIN);
    int size = (feedback & PACKETS_COUNT)>>8;

    // Использование массива байтов
    for (int i = 0; i < size; i++) {
        Serial.print("Row "); Serial.print(i); Serial.print(": ");
        for (int j = 0; j < 3; j++) {
            Serial.print(data[i][j]);
            Serial.print(" ");
        }
        Serial.println();
    }*/

    /*

    if (feedback & ERROR_SYNTAX)                    Serial.println("[ERROR_SYNTAX] Ошибка синтаксиса.");
    if (feedback & ERROR_1L_NO_DATA)                Serial.println("[ERROR_1L_NO_DATA] Ошибка 1L. Нет данных или разделитля");
    if (feedback & ERROR_1L_TOO_MANY_DATA)          Serial.println("[ERROR_1L_TOO_MANY_DATA] Ошибка 1L. данных больше 2");
    if (feedback & ERROR_INVALID_CRC)               Serial.println("[ERROR_INVALID_CRC] Неверный CRC.");
    if (feedback & ERROR_NULL_CRC)                  Serial.println("[ERROR_NULL_CRC] пустой CRC.");
    if (feedback & ERROR_2L_NO_DATA_PACKETS)        Serial.println("[ERROR_2L_NO_DATA_PACKETS] Ошибка 2L. Нет данных пакетов.");
    if (feedback & ERROR_2L_TOO_MANY_PACKETS)       Serial.println("[ERROR_2L_TOO_MANY_PACKETS] Ошибка 2L.  данных больше 8.");
    if (feedback & ERROR_3L_WRONG_DATA_PACKETS)     Serial.println("[ERROR_3L_WRONG_DATA_PACKETS] Ошибка 3L. Неверный Количество пакетов.");
    if (feedback == ERROR_NONE)                     Serial.println("Парсинг выполнен успешно.");

    if (feedback & ERROR_SYNTAX)                    Serial.println("[ER1]");
    if (feedback & ERROR_1L_NO_DATA)                Serial.println("[ER2]");
    if (feedback & ERROR_1L_TOO_MANY_DATA)          Serial.println("[ER3]");
    if (feedback & ERROR_INVALID_CRC)               Serial.println("[ER4]");
    if (feedback & ERROR_NULL_CRC)                  Serial.println("[ER5]");
    if (feedback & ERROR_2L_NO_DATA_PACKETS)        Serial.println("[ER6]");
    if (feedback & ERROR_2L_TOO_MANY_PACKETS)       Serial.println("[ER7]");
    if (feedback & ERROR_3L_WRONG_DATA_PACKETS)     Serial.println("[ER8]");
    //if (feedback == ERROR_NONE)                     Serial.println("[]");

    */
    

    
}

void loop() {
    if (Serial1.available() > 0) {
      int presentSize = Serial1.readBytesUntil(TERMINATOR, inputBuffer, 64);
      inputBuffer[presentSize--] = NULL;
      str = inputBuffer;
      //Serial.println(str);
      

      parser.setStr(str);
      //int newFeedback = parser.parse( data);

      //if(parser.parse( data))
      
      feedback = parser.parse( data, doAction, serialWriteKeyWord);
      //Serial.print("feedbeck "); Serial.println(feedback,BIN);
      if(!(feedback & GET_KEYWORD))serialWriteFeedBack(feedback);

      
      //showall/8d984889
      //83,0,1;83,1,1;83,2,1;83,3,1/61204cfc
      //83,0,0;83,1,0;83,2,0;83,3,0/745ec3dc
      //83,0,1;83,1,0;83,2,0;83,3,0/6376a71c
      //83,3,1/1b4cc6ee

    }


      

}
