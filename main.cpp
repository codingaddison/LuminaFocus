#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Arduino.h>
#include <ThreeWire.h>         //RTC函式庫導入 RTC library.
#include <RtcDS1302.h>          //RTC函式庫導入 RTC library.
#include <WiFi.h>           // WiFi函式庫導入 WiFi library.
#include <WiFiClient.h>     // WiFi Client函式庫導入 WiFi Client library.
#include <HTTPClient.h>     // HTTP通訊函式庫導入 HTTP communication library.
#include <AHT10.h>
#include <Preferences.h>    //離線保存NVS記憶體函式庫導入 Offline data save NVS memory library

AHT10 myAHT10(AHT10_ADDRESS_0X38);

char* ssid = "placeyourssid";  // WIFI SSID (初次燒入為空，開機設定後將保存於NVS中) WIFI SSID (empty for the first time, after boot setting will be saved in NVS
char* password = "placeyourpassword";

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
// fWPdgfnaFMJ6tdT2LBjtbWbID4gMJ6y6Z2RPurf8k4B
const char* lineToken = "placeyourselftoken";
// Initialize with the I2C addr 0x3C (for the 128x64)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

const uint8_t batterySymbolfull[] = { //81 ~ 100 %電池符號bitmap Full battery symbol bitmap.
    0b00111100,
    0b11111111,
    0b10000001,
    0b10111101,
    0b10111101,
    0b10000001,
    0b10111101,
    0b10111101,
    0b10000001,
    0b10111101,
    0b10111101,
    0b01111110
};

const uint8_t batterySymbol8050[] = { //50 ~ 80 %電池符號bitmap 80 ~ 61 % battery symbol bitmap.
    0b00111100,
    0b11111111,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10111101,
    0b10111101,
    0b10000001,
    0b10111101,
    0b10111101,
    0b01111110
};

const uint8_t batterySymbol5020[] = { //20 ~ 50 %電池符號bitmap 50 ~ 21 % battery symbol bitmap.
    0b00111100,
    0b11111111,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10111101,
    0b10111101,
    0b01111110
};

const uint8_t batterySymbol2005[] = {  //5 ~ 20 %電池符號bitmap 20 ~ 6 % battery symbol bitmap.
    0b00111100,
    0b11111111,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10111101,
    0b01111110
};


const uint8_t batterySymbolempty[] = { //0 ~ 5 %電池符號bitmap 0 ~ 5 % battery symbol bitmap.
    0b00111100,
    0b11111111,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b10000001,
    0b01111110
};

const uint8_t batterySymbolerr[] = { //電池無法正常讀取時的電池符號ERROR Battery symbol bitmap when battery cannot be read normally.
    0b00111100,
    0b11111111,
    0b10000001,
    0b10000001,
    0b11000011,
    0b10100101,
    0b10011001,
    0b10011001,
    0b10100101,
    0b11000011,
    0b10000001,
    0b01111110
};

const uint8_t wifiSymbolConnected4[] = { //已連線WiFi符號左邊部分 Connected WiFi symbol left part. 強訊號 Strong signal. 
  0b00000000,
  0b00011110,
  0b00100001,
  0b01011110,
  0b10100001,
  0b01011110,
  0b00100001,
  0b00001100,
  0b00001100,
  0b00001100
};
const uint8_t wifiSymbolConnected24[] = { //已連線WiFi符號右邊部分 Connected WiFi symbol right part. 強訊號 Strong signal.
  0b00000000,
  0b00000000,
  0b00000000,
  0b10000000,
  0b01000000,
  0b10000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};



const uint8_t wifiSymbolConnected3[] = { //已連線WiFi符號左邊部分 Connected WiFi symbol left part.  弱訊號 Weak signal.
  0b00000000,
  0b00000000,
  0b00000000,
  0b00011110,
  0b00100001,
  0b01011110,
  0b00100001,
  0b00001100,
  0b00001100,
  0b00001100
};
const uint8_t wifiSymbolConnected23[] = { //已連線WiFi符號右邊部分 Connected WiFi symbol right part. 弱訊號 Weak signal.
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b10000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

const uint8_t wifiSymbolConnected2[] = { //已連線WiFi符號左邊部分 Connected WiFi symbol left part.  微弱訊號 Very weak signal.
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00011110,
  0b00100001,
  0b00001100,
  0b00001100,
  0b00001100
};
const uint8_t wifiSymbolConnected22[] = { //已連線WiFi符號右邊部分 Connected WiFi symbol right part. 微弱訊號 Very weak signal.
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

const uint8_t wifiSymbolConnected1[] = { //已連線WiFi符號左邊部分 Connected WiFi symbol left part.  無訊號 No signal.
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00001100,
  0b00001100,
  0b00001100
};
const uint8_t wifiSymbolConnected21[] = { //已連線WiFi符號右邊部分 Connected WiFi symbol right part. 無訊號 No signal.
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};

const uint8_t wifiSymbolDisConnected[] = { //未連線WiFi符號左邊部分 Disconnected WiFi symbol left part.
  0b00111111,
  0b00110011,
  0b01010010,
  0b10000110,
  0b00001100,
  0b01001100,
  0b00001100,
  0b00100001,
  0b00001100,
  0b00001100
};
const uint8_t wifiSymbolDisConnected2[] = { //未連線WiFi符號右邊部分 Disconnected WiFi symbol right part.
  0b00000000,
  0b00000000,
  0b10000000,
  0b01000000,
  0b00000000,
  0b10000000,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000
};
const uint8_t leftArrow[] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00011000,
  0b00011100,
  0b00011110,
  0b01111111,
  0b01111110,
  0b00011110,
  0b00011100,
  0b00011000,
  0b00000000,
};

// 定義右箭頭的自定義字形陣列
const uint8_t rightArrow[] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00001100,
  0b00011100,
  0b00011110,
  0b01111111,
  0b01111110,
  0b00011110,
  0b00011100,
  0b00001100,
  0b00000000,
};

#define BUTTON_L 18
#define BUTTON_M 19
#define BUTTON_R 23

#define RGB_R 27
#define RGB_G 26
#define RGB_B 25

#define BUZZER 33

#define VIBRATOR 14
ThreeWire myWire(15,12,13); // DAT, CLK, RST
RtcDS1302<ThreeWire> Rtc(myWire);    // 定義RTC時鐘模組腳位
int timezoneOffset = 8; // 時區調整，台灣為+8(預設台灣台北時區UTC +8) Time zone adjustment, Taiwan is +8. (default Taiwan Taipei time zone UTC +8) (NVS紀錄) (NVS record)
int aver = 0;

String UserID = "LEARNER"; //用戶統一編號 User uniform number.
bool BuzzerEnable = true; //蜂鳴器狀態 Buzzer status.
String message = ""; //Line通知訊息 Line notification message.

void setup() {
  Serial.begin(9600);  // 設定serial 初始鮑率 9600 set serial baud rate 9600.
  myAHT10.begin();
  analogRead(32); // 設定GPIO32為ADC輸入 set GPIO32 as ADC input

  Preferences NVS;
    NVS.begin("config",true); // 初始化常用設定檔 initial common config file.
    bool keyExists = true; //初次燒入NVS時，會檢查是否有該key，如果沒有則會寫入初始值 When NVS is burned for the first time, it will check whether there is the key. If not, it will write the initial value.
    if(NVS.getBool("INIT",false) == false)
    {
      keyExists = NVS.putBool("INIT",true);
      keyExists = false;
    }
    else if(NVS.getBool("INIT",false) == true)
    {
      keyExists = true;
    }

    if (!keyExists)  //用戶統一編號讀取及初始檢查 User uniform number read and initial check.
    {
      NVS.putString("UserID", "LEARNER");  // 寫入初始值，預設010(初次號碼狀態) write initial value, default 010 (default number status).
    } 
    else 
    {
      UserID = NVS.getString("UserID", "LEARNER"); // 讀取NVS中config下的UserID，如果異常則回傳010 read UserID in NVS under config, if abnormal return 010. 
    }

    if (!keyExists)
    {
      NVS.putBool("BUZZER", true);  // 寫入初始值，預設010(初次號碼狀態) write initial value, default 010 (default number status).
    }
    else
    {
      BuzzerEnable = NVS.getBool("BUZZER", true); // 讀取NVS中config下的UserID，如果異常則回傳010 read UserID in NVS under config, if abnormal return 010.
    }

  NVS.end(); // 結束NVS end NVS.

  pinMode(BUTTON_L, INPUT_PULLUP);
  pinMode(BUTTON_M, INPUT_PULLUP);
  pinMode(BUTTON_R, INPUT_PULLUP);
  pinMode(RGB_R, OUTPUT);
  pinMode(RGB_G, OUTPUT);
  pinMode(RGB_B, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(VIBRATOR, OUTPUT);
  
  digitalWrite(RGB_G, HIGH);
  delay(200);
  digitalWrite(RGB_G, LOW);
  digitalWrite(RGB_B, HIGH);
  delay(200);
  digitalWrite(RGB_B, LOW);
  digitalWrite(RGB_R, HIGH);
  delay(200);
  digitalWrite(RGB_R, LOW);

  if(BuzzerEnable == true)
  {
    digitalWrite(BUZZER, HIGH);
     digitalWrite(VIBRATOR, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(200);
    digitalWrite(BUZZER, HIGH);
    delay(200);
    digitalWrite(BUZZER, LOW);
    delay(400);
    digitalWrite(VIBRATOR, LOW);
  } else if(BuzzerEnable == false)
  {
    digitalWrite(VIBRATOR, HIGH);
    delay(1000);
    digitalWrite(VIBRATOR, LOW);
  }

    Serial.print("compiled: ");
    Serial.print(__DATE__);
    Serial.println(__TIME__);
    Rtc.Begin();
    RtcDateTime compiled = RtcDateTime(__DATE__, __TIME__); //燒入程式時時間 TIME when burning program.
 
    if (!Rtc.IsDateTimeValid()) //如果RTC時間不正確，燒入程式時時間 If RTC time is incorrect, burn in the time when burning program.
    {
       
        Serial.println("RTC lost confidence in the DateTime!");
        Rtc.SetDateTime(compiled);
    }

    if (Rtc.GetIsWriteProtected()) //如果RTC被寫保護，解除保護 If RTC is write-protected, remove protection.
    {
        Serial.println("RTC was write protected, enabling writing now");
        Rtc.SetIsWriteProtected(false);
    }

    if (!Rtc.GetIsRunning()) //如果RTC沒有運行，開始運行 If RTC is not running, start running.
    {
        Serial.println("RTC was not actively running, starting now");
        Rtc.SetIsRunning(true);
    }

    RtcDateTime now = Rtc.GetDateTime(); //定義現在系統時間同RTC模組 Define now system time and RTC module.

    if (now < compiled) //如果RTC時間比燒入程式時時間早，更新時間 If RTC time is earlier than the time when burning program, update time.
    {
        Serial.println("RTC is older than compile time!  (Updating DateTime)");
        Rtc.SetDateTime(compiled);
    }
    else if (now > compiled) //如果RTC時間比燒入程式時時間晚，正常運行 If RTC time is later than the time when burning program, run normally.
    {
        Serial.println("RTC is newer than compile time. (this is expected)");
    }
    else if (now == compiled) //如果RTC時間和燒入程式時時間一樣，正常運行 If RTC time is the same as the time when burning program, run normally.
    {
        Serial.println("RTC is the same as compile time! (not expected but all is fine)");
    }


    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {  //OLED 初始化 OLED initialize.
        Serial.println(F("SSD1306 allocation failed"));
        for (;;);
    }


    display.clearDisplay();  //OLED初始測試 OLED initial test.
    display.setTextSize(2);
    display.setTextColor(WHITE);
    display.setCursor(0, 0);
    display.println("WELCOME \n  USE\nStudySync Phone V 1.0");
    display.display();
    delay(1000);
    display.clearDisplay();  //OLED初始測試 OLED initial test.

    WiFi.begin(ssid, password); // 嘗試初始化連線WIF，10秒內沒連上放棄連接 try to connect WIFI, if not connect in 10 seconds, give up.
    int timecount = 0;
    while (WiFi.status() != WL_CONNECTED && timecount < 20)
    {
      delay(500);
      Serial.print(".");
      timecount++;
    }

    if (WiFi.status() == WL_CONNECTED) {
    // 要發送的Line通知訊息
     message = "鴨鴨狗學測手機已上線!";

    // 使用HTTP Client程式庫發送POST請求
    HTTPClient http;
    http.begin("https://api.line.me/v2/bot/message/push");
    http.addHeader("Authorization", "Bearer " + String(lineToken));
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("{\"to\": \"Your_Line_User_ID\", \"messages\":[{\"type\":\"text\", \"text\":\"" + message + "\"}]}");

    if (httpResponseCode > 0) {
      Serial.print("Line notification sent. Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending Line notification. Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } 

}

unsigned long oledpre = 0;
unsigned long tempupdate = 0;
int averb = 0;

int getSignalLevel(int rssi) { // 取得WIFI訊號強度判斷 get WIFI signal strength judgment.
  if (rssi >= -50) {
    return 4; // 优秀信号强度
  } else if (rssi >= -65) {
    return 3; // 良好信号强度
  } else if (rssi >= -80) {
    return 2; // 一般信号强度
  } else if (rssi >= -95) {
    return 1; // 弱信号强度
  } else {
    return 0; // 信号强度未知
  }
}
bool buttonL = false;
bool buttonM = false;
bool buttonR = false;
int chargeSy = 0;
String page = "main";
bool yes = false;
String goingto = "main";

String choosepage(int way)
{

      if(goingto == "main")
      {
        goingto = "Tomato";
      }

      else if (goingto == "Tomato")
      {
        goingto = "Sound";
      }
        
      else if (goingto == "Sound")
      {
        goingto = "msg";
      }

      else if (goingto == "msg")
      {
        goingto = "main";
      }

}

int tomato = 25;
bool tomatoon = false;
unsigned long tomatocount = 0;
String tomatoset()
{
  tomatoon = true;
  tomatocount = millis();

}
float newT = 0;
float newH = 0;
int mc = 1;
void loop() {
    
    if(digitalRead(BUTTON_L) == LOW){
        buttonL = true;
        Serial.println("L");
    }
    if(digitalRead(BUTTON_M) == LOW){
        buttonM = true;
        Serial.println("M");
    }
    if(digitalRead(BUTTON_R) == LOW){
        buttonR = true;
        Serial.println("R");
    }

    if(millis() - tempupdate > 10000){
        tempupdate = millis();
        
    newT = myAHT10.readTemperature();
    newH = myAHT10.readHumidity();

    }
    if(aver > 30){
        aver = 0;
        averb = 0;
        }
        // 將ADC值映射到0到100的範圍
        int adcValue = analogRead(32); // 使用GPIO32作為ADC輸入
        int readadc = adcValue;
        int batteryPercentage = map(adcValue, 620, 865, 0, 100);

        // 顯示電量 (Serial Monitor)
        Serial.print("電量Battery percent:");
        Serial.print(batteryPercentage);
        Serial.println("%");
        if(batteryPercentage >= 0 and batteryPercentage <= 150){
            averb += batteryPercentage ;
            aver +=1;
            batteryPercentage = averb / aver;
        }
        else{
            batteryPercentage = -1;
            aver = 0;
            averb = 0;
        }
    
        RtcDateTime now = Rtc.GetDateTime(); //抓取當前時間 Get current time.
        String HH = String(now.Hour()); //定義小時字串 Define hour string.
        String MM = String(now.Minute()); //定義分鐘字串 Define minute string.
        String SS = String(now.Second()); //定義秒數字串 Define second string.
        if (HH.length() == 1) //如果小時字串長度為1，補0 If the length of the hour string is 1, fill in 0.
        {
        HH = "0" + HH;
        }
        if (MM.length() == 1) //如果分鐘字串長度為1，補0 If the length of the minute string is 1, fill in 0.
        {
        MM = "0" + MM;
        }
        if (SS.length() == 1) //如果秒數字串長度為1，補0 If the length of the second string is 1, fill in 0.
        {
        SS = "0" + SS;
        }
        // 顯示時間 (Serial Monitor)
        Serial.print("現在時間Now time:");
        Serial.print(HH + ":" + MM + ":" + SS);


    if((millis() - oledpre > 1000) and (page == "main")){
        oledpre = millis();
        display.clearDisplay();  //OLED初始測試 OLED initial test.
        
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.println(HH + ":" + MM );
        display.setTextSize(1);
        display.setCursor(62, 8);
        display.print(SS);

        if (batteryPercentage > 100){
            if(chargeSy == 0)
            {
                display.drawBitmap(92, 0, batterySymbolempty, 8, 12, 1);
                chargeSy = 1;
            }
            else if(chargeSy == 1)
            {
                display.drawBitmap(92, 0, batterySymbol2005, 8, 12, 1);
                chargeSy = 2;
            }
            else if(chargeSy == 2)
            {
                display.drawBitmap(92, 0, batterySymbol5020, 8, 12, 1);
                chargeSy = 3;
            }
            else if(chargeSy == 3)
            {
                display.drawBitmap(92, 0, batterySymbol8050, 8, 12, 1);
                chargeSy = 4;
            }
            else if(chargeSy == 4)
            {
                display.drawBitmap(92, 0, batterySymbolfull, 8, 12, 1);
                chargeSy = 0;
            }

        }else if (batteryPercentage >= 80) {
            display.drawBitmap(92, 0, batterySymbolfull, 8, 12, 1);
        } else if (batteryPercentage >= 50) {
            display.drawBitmap(92, 0, batterySymbol8050, 8, 12, 1);
        } else if (batteryPercentage >= 20) {
            display.drawBitmap(92, 0, batterySymbol5020, 8, 12, 1);
        } else if (batteryPercentage >= 5) {
            display.drawBitmap(92, 0, batterySymbol2005, 8, 12, 1);
        } else if (batteryPercentage >= 0){
            display.drawBitmap(92, 0, batterySymbolempty, 8, 12, 1);
        } else {
            display.drawBitmap(92, 0, batterySymbolerr, 8, 12, 1);
        }

        // 顯示電量 (OLED)

        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(100, 4);
        if (batteryPercentage > 105) {
            display.println("CHG");
        } else if (batteryPercentage >= 100)
        {
            display.println(String("100") + "%");
        }
        else if (batteryPercentage >= 10) {
            display.println(" " + String(batteryPercentage) + "%");
        } else if (batteryPercentage >= 0) {
            display.println("  " + String(batteryPercentage) + "%");
        } else if (batteryPercentage == -1) {
            display.println(" ERR");
        } else {
            display.println(" ERR");
        }

        if(WiFi.status() == WL_CONNECTED)
        {   
            if(getSignalLevel(WiFi.RSSI()) == 4)
            {
                display.drawBitmap(75, 0, wifiSymbolConnected4, 8, 10, 1); //顯示WIFI圖示 Display WIFI icon. (白色標示 White mark)
                display.drawBitmap(83, 0, wifiSymbolConnected24, 8, 10, 1); //顯示WIFI圖示 Display WIFI icon. (白色標示 White mark)
            }  
            else if (getSignalLevel(WiFi.RSSI()) == 3)
            {
                display.drawBitmap(75, 0, wifiSymbolConnected3, 8, 10, 1); //顯示WIFI圖示 Display WIFI icon. (白色標示 White mark)
                display.drawBitmap(83, 0, wifiSymbolConnected23, 8, 10, 1); //顯示WIFI圖示 Display WIFI icon. (黃色標示 Yellow mark)
            }
            else if (getSignalLevel(WiFi.RSSI()) == 2)
            {
                display.drawBitmap(75, 0, wifiSymbolConnected2, 8, 10, 1); //顯示WIFI圖示 Display WIFI icon. (白色標示 White mark)
                display.drawBitmap(83, 0, wifiSymbolConnected22, 8, 10, 1); //顯示WIFI圖示 Display WIFI icon. (黃色標示 Yellow mark)
            }
            else if (getSignalLevel(WiFi.RSSI()) == 1)
            {
                display.drawBitmap(75, 0, wifiSymbolConnected1, 8, 10, 1); //顯示WIFI圖示 Display WIFI icon. (黃色標示 Yellow mark)
                display.drawBitmap(83, 0, wifiSymbolConnected21, 8, 10, 1); //顯示WIFI圖示 Display WIFI icon. (黃色標示 Yellow mark)
            }
            else if (getSignalLevel(WiFi.RSSI()) == 0)
            {
                display.drawBitmap(75, 0, wifiSymbolConnected1, 8, 10,1); //顯示WIFI圖示 Display WIFI icon. (紅色標示 Red mark)
                display.drawBitmap(83, 0, wifiSymbolConnected21, 8, 10,1); //顯示WIFI圖示 Display WIFI icon. (紅色標示 Red mark)
            }   
            
          
        }

        else if(WiFi.status() != WL_CONNECTED)
        {
          display.drawBitmap(75, 0, wifiSymbolDisConnected, 8, 10, 1); //顯示WIFI圖示 Display WIFI icon. (未啟用紅色標示 Not enabled red mark)
          display.drawBitmap(82, 0, wifiSymbolDisConnected2, 8, 10, 1); //顯示WIFI圖示 Display WIFI icon. (未啟用紅色標示 Not enabled red mark)
        }

        display.setCursor(0, 20);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.println(String(newT) + "C " + String(newH) + "%");


        display.setCursor(0, 50);
        display.setTextSize(2);
        display.setTextColor(WHITE);
        if (buttonL)
        {
            choosepage(5);
            delay(200);
            buttonL = false;
        }
        
        if (buttonM)
        {   
            buttonM = false;
            page = goingto;
            delay(200);
        }

        if (buttonR)
        {
            choosepage(5);
            delay(200);
            buttonR = false;
        }

        if(tomatoon)
        { 
          int tomatomin = ((tomato*60*1000-(millis() - tomatocount)) / 1000)/60;
          int tomatosec = ((tomato*60*1000-(millis() - tomatocount)) / 1000)%60;
          display.setCursor(10, 35);
          display.setTextSize(1);
          display.setTextColor(WHITE);
          display.println("Tomato");
          
          display.setCursor(35, 45);
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.println(String(tomatomin) + ":" + String(tomatosec));
          

        }
        if(tomatoon and (millis() - tomatocount > (tomato * 60 * 1000)-3000))
        {
          
          tomatoon = false;
          digitalWrite(VIBRATOR, HIGH);
          if(BuzzerEnable == true)
          {
            digitalWrite(BUZZER, HIGH);
          }
          display.clearDisplay();
          display.setCursor(0, 30);
          display.setTextSize(2);
          display.setTextColor(WHITE);
          display.println("Tomato End !");
          display.display();

          if(BuzzerEnable == true)
          {
            delay(200);
            digitalWrite(BUZZER, LOW);
            delay(300);
            digitalWrite(BUZZER, HIGH);
            delay(200);
            digitalWrite(BUZZER, LOW);
            delay(300);
            digitalWrite(BUZZER, HIGH);
            delay(200);
            digitalWrite(BUZZER, LOW);
          }
          else if(BuzzerEnable == false)
          {
            delay(1400);
          }
          digitalWrite(VIBRATOR, LOW);
          if (WiFi.status() == WL_CONNECTED) {
    // 要發送的Line通知訊息
     message = "鴨鴨狗已完成番茄鐘(或計時器)! 時長: " + String(tomato) + "分鐘!";

    // 使用HTTP Client程式庫發送POST請求
    HTTPClient http;
    http.begin("https://api.line.me/v2/bot/message/push");
    http.addHeader("Authorization", "Bearer " + String(lineToken));
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("{\"to\": \"Your_Line_User_ID\", \"messages\":[{\"type\":\"text\", \"text\":\"" + message + "\"}]}");

    if (httpResponseCode > 0) {
      Serial.print("Line notification sent. Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending Line notification. Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } 
        }

        display.setCursor(90, 20);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.println(readadc);
        if(!tomatoon){
        display.setCursor(50, 50);
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.println(goingto);
        }
        
        display.display();
     
      
    }
    // page Tomato
    else if((millis() - oledpre > 1000) and (page == "Tomato") and (tomatoon != true)){
        oledpre = millis();
        display.clearDisplay();  //OLED初始測試 OLED initial test.
        
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(40, 20);
        display.println(String(tomato));
        
        display.setTextSize(1);
        display.setTextColor(WHITE);
        display.setCursor(70, 26);
        display.println("min");



        if (buttonL)
        {
            page = "main";
            buttonL = false;
        }
        
        if (buttonM)
        {
            buttonM = false; 
            tomatoset();
            display.clearDisplay(); 
            display.setTextSize(3);
            display.setTextColor(WHITE);
            display.setCursor(20, 10);
            display.println(String("SET !"));
                display.display();
            if(BuzzerEnable == true)
            {
              digitalWrite(BUZZER, HIGH);
              delay(200);
              digitalWrite(BUZZER, LOW);
            }
            digitalWrite(VIBRATOR, HIGH);
            delay(500);
            digitalWrite(VIBRATOR, LOW);
            delay(200);
            digitalWrite(VIBRATOR, HIGH);
            delay(500);
            digitalWrite(VIBRATOR, LOW);       
        }
 
        if (buttonR)
        {
            if(tomato < 25)
            {
              tomato += 1;
            }
            else if (tomato == 25)
            {
              tomato = 1;
            }
            buttonR = false;
        }
        
         display.display();

    }

    else if((millis() - oledpre > 1000) and (page == "Tomato") and (tomatoon == true)){
        oledpre = millis();
        display.clearDisplay();  //OLED初始測試 OLED initial test.
        
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.println(String("Tomato Working!"));
        display.display();
        delay(1000);
        page = "main";
    }
    
    //sound page
    if((millis() - oledpre > 1000) and (page == "Sound")){
        oledpre = millis();
        display.clearDisplay();  //OLED初始測試 OLED initial test.
        
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(0, 10);
        display.println(String("Sound"));
        display.setTextSize(3);
        display.setCursor(40, 30);
        if(BuzzerEnable == true)
        {
          display.println(String("ON"));
        }
        else if (BuzzerEnable == false)
        {
          display.println(String("OFF"));
        }
    
        
        if (buttonL)
        {
            page = "main";
            buttonL = false;
        }
        
        if (buttonM)
        {
            buttonM = false; 
            display.clearDisplay(); 
            display.setTextSize(3);
            display.setTextColor(WHITE);
            display.setCursor(20, 10);
            display.println(String("SET !"));
            display.display();
            digitalWrite(VIBRATOR, HIGH);
            delay(500);
            digitalWrite(VIBRATOR, LOW);
            delay(200);
            digitalWrite(VIBRATOR, HIGH);
            delay(500);
            digitalWrite(VIBRATOR, LOW);       
            if (WiFi.status() == WL_CONNECTED) {
    // 要發送的Line通知訊息
     message = "鴨鴨狗已設定手機蜂鳴器狀態! 狀態: " + String(BuzzerEnable) + "!";

    // 使用HTTP Client程式庫發送POST請求
    HTTPClient http;
    http.begin("https://api.line.me/v2/bot/message/push");
    http.addHeader("Authorization", "Bearer " + String(lineToken));
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("{\"to\": \"Your_Line_User_ID\", \"messages\":[{\"type\":\"text\", \"text\":\"" + message + "\"}]}");

    if (httpResponseCode > 0) {
      Serial.print("Line notification sent. Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending Line notification. Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } 
        }
 
        if (buttonR)
        {
            buttonR = false;
            if(BuzzerEnable == true)
            {
              BuzzerEnable = false;
            }
            else if (BuzzerEnable == false)
            {
              BuzzerEnable = true;
            }
        }
        
         display.display();
        
    }
    //msg mode
    if(milliseconds - oledpre > 1000 and page == "msg"){
        oledpre = millis();
        display.clearDisplay();  //OLED初始測試 OLED initial test.
        
        display.setTextSize(2);
        display.setTextColor(WHITE);
        display.setCursor(5, 10);
        display.println(String("msg"));
        display.setTextSize(3);
        display.setCursor(40, 30);
        display.println(String(mc));
      
        if(mc == 1)
        {
          message = "🐥";
        }
        else if (mc == 2)
        {
          message = "🐶";
        }
        else if (mc == 3)
        {
          message = "呱";
        }
        else if (mc == 4)
        {
          message = "喵";
        }
        else if (mc == 5)
        {
          message = "上課中";
        }
        else if (mc == 6)
        {
          message = "天氣很好";
        }
   
        if (buttonL)
        {
            page = "main";
            buttonL = false;
        }
        
        if (buttonM)
        {
            buttonM = false;
            if (WiFi.status() == WL_CONNECTED) {
    // 要發送的Line通知訊息
     message = "鴨鴨狗表達了:" + String(msg) + "!";

    // 使用HTTP Client程式庫發送POST請求
    HTTPClient http;
    http.begin("https://api.line.me/v2/bot/message/push");
    http.addHeader("Authorization", "Bearer " + String(lineToken));
    http.addHeader("Content-Type", "application/json");
    int httpResponseCode = http.POST("{\"to\": \"Your_Line_User_ID\", \"messages\":[{\"type\":\"text\", \"text\":\"" + message + "\"}]}");

    if (httpResponseCode > 0) {
      Serial.print("Line notification sent. Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error sending Line notification. Error code: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  }  
            display.clearDisplay(); 
            display.setTextSize(3);
            display.setTextColor(WHITE);
            display.setCursor(20, 10);
            display.println(String("SEND !"));
            display.display();
            digitalWrite(VIBRATOR, HIGH);
            delay(500);
            digitalWrite(VIBRATOR, LOW);
            delay(200);
            digitalWrite(VIBRATOR, HIGH);
            delay(500);
            digitalWrite(VIBRATOR, LOW);       
            
        }

        if (buttonR)
        {
            buttonR = false;
            if (mc < 6)
            {
              mc += 1;
            }
            else if (mc == 6)
            {
              mc = 1;
            }
        }

    
        
         display.display();}
}




