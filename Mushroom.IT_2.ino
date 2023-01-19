#define BLYNK_TEMPLATE_ID "TMPLgvOHpQsz"
#define BLYNK_DEVICE_NAME "Mushroom IT"
#define BLYNK_AUTH_TOKEN "CBUaKqvSNQpagA1-Rvd18HUgBHpN3CT7"

#define BLYNK_PRINT Serial
#define BLYNK_FIRMWARE_VERSION "0.2.0"
#define BLYNK_DEBUG 
#define APP_DEBUG

#define RELAY_1  D2
#define RELAY_2  D3
#define RELAY_3  D5

#define RELAY_4  D6
#define dataPin  D0
#define clockPin D1


#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SHT1x.h>


char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] = "OPPO 57";
char pass[] = "1111111110";

bool pump_1 = 0;
bool pump_2 = 0;
bool pump_3 = 0;
bool heater = 0;

bool ModeSwitch = false;

float temp_NPU = 0;
float humidity_NPU = 0;

int tempmax=0;
int tempmin=0;
int humimax=0;
int humimin=0;

float humidityOpen = 50;
float humidityClose = 70;

float TempOpen = 25;
float TempClose = 27;


SHT1x sht1x(dataPin, clockPin);

BlynkTimer timer;

BLYNK_CONNECTED()
{
  Blynk.syncAll();
}

BLYNK_WRITE(V0)
{
  pump_1 = param.asInt();
}

BLYNK_WRITE(V1)
{
  pump_2 = param.asInt();
}

BLYNK_WRITE(V2)
{
  pump_3 = param.asInt();
}

BLYNK_WRITE(V5)
{
  heater = param.asInt();
}

BLYNK_WRITE(V6)
{
  ModeSwitch = param.asInt();

  
}

BLYNK_WRITE(V7)
{
  humidityOpen = param.asInt();

  
}

BLYNK_WRITE(V8)
{
  humidityClose = param.asInt();

  
}
BLYNK_WRITE(V9)
{
  TempOpen = param.asInt();

  
}
BLYNK_WRITE(V10)
{
  TempClose = param.asInt();

  
}


void AutoAction()
{
  if(ModeSwitch == true){

    
    if(humidity_NPU <= humidityOpen)
    {
      digitalWrite(RELAY_1, LOW);
      digitalWrite(RELAY_2, LOW);
      pump_1 = false;
      pump_2 = false;
      
           
      Blynk.virtualWrite(V0,0);  // Turn the widget attached to V1 On
      Blynk.virtualWrite(V1,0);  // Turn the widget attached to V2 On
      
//      Blynk.virtualWrite(V3,1);  // Turn the widget attached to V3 On
//      Blynk.virtualWrite(V4,1);  // Turn the widget attached to V4 On
    }
    else if(humidity_NPU >= humidityClose)
    {
      digitalWrite(RELAY_1, HIGH);
      digitalWrite(RELAY_2, HIGH);

      pump_1 = true;
      pump_2 = true;
      Blynk.virtualWrite(V0,1);  // Turn the widget attached to V1 off
      Blynk.virtualWrite(V1,1);  // Turn the widget attached to V2 off
    }
    
    if(temp_NPU <= TempOpen)
    {
      digitalWrite(RELAY_4, LOW);
      Blynk.virtualWrite(V5,0);  // Turn the widget attached to V1 On
//      Blynk.virtualWrite(V2,1);  // Turn the widget attached to V2 On
//      Blynk.virtualWrite(V3,1);  // Turn the widget attached to V3 On
//      Blynk.virtualWrite(V4,1);  // Turn the widget attached to V4 On
      heater = false;
      
    }
    else if(temp_NPU >= TempClose)
    {
      
      digitalWrite(RELAY_4, HIGH);
      Blynk.virtualWrite(V5,1);  // Turn the widget attached to V1 off
      
//      Blynk.virtualWrite(V1,0);  // Turn the widget attached to V2 off
      heater = true;
    }

    if (pump_3)
    {
      digitalWrite(RELAY_3, HIGH);
      
    }else
    {
      digitalWrite(RELAY_3, LOW);
    }

  }
  
  
}

void manualAction()
  
{

    
  if(ModeSwitch == false){
  Blynk.virtualWrite(V0,pump_1); 
  Blynk.virtualWrite(V1,pump_2); 
  Blynk.virtualWrite(V2,pump_3); 
  Blynk.virtualWrite(V5,heater); 
     
    if (pump_1)
    {
      digitalWrite(RELAY_1, HIGH);
      
    }
    else
    {
      digitalWrite( RELAY_1, LOW);
    }
    
    if (pump_2)
    {
      digitalWrite(RELAY_2, HIGH);
    }
    else
    {
      digitalWrite(RELAY_2, LOW);
    }
  
    if (pump_3)
    {
      digitalWrite(RELAY_3, HIGH);
    }
    else
    {
      digitalWrite(RELAY_3, LOW);
    }
    if (heater)
    {
      digitalWrite(RELAY_4, HIGH);
    }
    else
    {
      digitalWrite(RELAY_4, LOW);
    }
  }
    
}

void Sensor()
{
 float temp_c; // ค่าอุณหภูมิ ที่แสดงหน่วยเป็น องศาเซลเซียส
 float humidity; // ค่าความชื้น
 temp_c = sht1x.readTemperatureC();
 humidity = sht1x.readHumidity();
 Serial.print("Temperature: ");
 Serial.print(temp_c);
 Serial.print("C / ");
 Serial.print(humidity);
 Serial.println("%");
 
  if (isnan(humidity) || isnan(temp_c))
  {
      Blynk.virtualWrite(V1,0);  // Turn the widget attached to V1 off
      Blynk.virtualWrite(V2,0);  // Turn the widget attached to V2 off
      Blynk.virtualWrite(V3,0);  // Turn the widget attached to V3 off
      Blynk.virtualWrite(V4,0);  // Turn the widget attached to V4 off
    return;
  }
  temp_NPU = temp_c;
  humidity_NPU = humidity;
  
  Blynk.virtualWrite(V3, temp_c);
  Blynk.virtualWrite(V4, humidity);
  
}


void setup()
{
  Serial.begin(115200);
  
  pinMode(RELAY_1, OUTPUT);
  pinMode(RELAY_2, OUTPUT);
  pinMode(RELAY_3, OUTPUT);
  pinMode(RELAY_4, OUTPUT);
  
  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH); 

  
  
  WiFi.begin(ssid, pass); //เชื่อมต่อ WiFi
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(250);
    Serial.print(".");
  }

  Serial.println("เชื่อมต่อสำเร็จ");

  Blynk.connect(); //เชื่อมต่อไปยัง Blynk
  delay(3000);
  Blynk.config(auth); //กำหนด Token key

  digitalWrite(RELAY_1, HIGH);
  digitalWrite(RELAY_2, HIGH);
  digitalWrite(RELAY_3, HIGH);
  digitalWrite(RELAY_4, HIGH);

  timer.setInterval(5, manualAction);
  timer.setInterval(1000, Sensor);
  timer.setInterval(5, AutoAction);
}
  
void loop()
{
  Blynk.run();
  timer.run();
}
