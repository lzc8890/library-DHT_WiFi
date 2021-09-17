#define BLINKER_WIFI
#define BLINKER_MIOT_SENSOR   //小爱同学定义为传感器设备
#define BLINKER_MIOT_LIGHT    
#define BLINKER_PRINT Serial    //串口通信
#include <Blinker.h>
#include <DHT.h>
 
char auth[] = "点灯科技设备密钥";
char ssid[] = "WIFI名";
char pswd[] = "WIFI密码";
 
BlinkerNumber HUMI("humi");
BlinkerNumber TEMP("temp");
 
#define DHTPIN 2      //定义DHT11模块连接管脚io2

#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
 
DHT dht(DHTPIN, DHTTYPE);
 
float humi_read = 0, temp_read = 0;
 
void heartbeat()
{
    HUMI.print(humi_read);
    TEMP.print(temp_read);
}

void dataStorage()
{
    Blinker.dataStorage("temp", temp_read);
    Blinker.dataStorage("humi", humi_read);
}
void miotQuery(int32_t queryCode)
{
    BLINKER_LOG("MIOT Query codes: ", queryCode);

    switch (queryCode)
    {
        case BLINKER_CMD_QUERY_PM25_NUMBER :
            BLINKER_LOG("MIOT Query PM25");
            BlinkerMIOT.pm25(20);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_HUMI_NUMBER :
            BLINKER_LOG("MIOT Query HUMI");
            BlinkerMIOT.humi(humi_read);
            BlinkerMIOT.print();
            break;
        case BLINKER_CMD_QUERY_TEMP_NUMBER :
            BLINKER_LOG("MIOT Query TEMP");
            BlinkerMIOT.temp(temp_read);
            BlinkerMIOT.print();
            break;
        default :
            BlinkerMIOT.temp(temp_read);
            BlinkerMIOT.humi(humi_read);
            BlinkerMIOT.pm25(20);
            BlinkerMIOT.co2(20);
            BlinkerMIOT.print();
            break;
    }
}
 
void setup()
{
    Serial.begin(115200);
    BLINKER_DEBUG.stream(Serial);
    BLINKER_DEBUG.debugAll();
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
 
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachHeartbeat(heartbeat);
    BlinkerMIOT.attachQuery(miotQuery);
    Blinker.attachDataStorage(dataStorage);
    dht.begin();
}
 
void loop()
{
    Blinker.run();
 
    float h = dht.readHumidity();
    float t = dht.readTemperature();
 
    if (isnan(h) || isnan(t))
    {
        BLINKER_LOG("Failed to read from DHT sensor!");
    }
    else
    {
        BLINKER_LOG("Humidity: ", h, " %");
        BLINKER_LOG("Temperature: ", t, " *C");
        humi_read = h;
        temp_read = t-5;
    }
 
    Blinker.delay(2000);
}
