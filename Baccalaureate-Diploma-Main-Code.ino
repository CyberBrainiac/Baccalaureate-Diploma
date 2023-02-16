
#include <iarduino_DHT.h>  
#include <SoftwareSerial.h>
SoftwareSerial mySerial(7,8); 
iarduino_DHT sensor(3);   
int Relay = 5;           // Призначаємо 5 цифровий пін для реле
int FlagOK = 0;
int FlagAttention = 0;
int TemperatureOUT = 0;
int FlagDanger = 0;

void setup() {
  pinMode(Relay, OUTPUT);  // встановлюємо режим роботи - вихід
  digitalWrite (Relay, HIGH);
  Serial.begin (9600);  
  delay(1000);  
  Serial.println ("START"); 
  mySerial.begin(9600);  
  delay(1000);   
}
void loop() {   
    
    switch(sensor.read()){    
    case DHT_OK:               Serial.println((String) "sensor in the room: " + sensor.hum + "% - " + sensor.tem + "*C");  break;
    case DHT_ERROR_CHECKSUM:   Serial.println(         " sensor in the room: inequality KC");                     break;
    case DHT_ERROR_DATA:       Serial.println(         " sensor in the room: the answer does not respond to the sensor 'DHT'"); break;
    case DHT_ERROR_NO_REPLY:   Serial.println(         " sensor in the room: no answer");                          break;
    default:                   Serial.println(         " sensor in the room: ERROR");                               break;
  }
  
  delay(2000);
  if(sensor.tem <= 35 && FlagOK == 1 && sensor.tem >= 20)
  {
    SMSOk();
    FlagOK = 0;
    FlagAttention = 0;
    FlagDanger = 0;
    TemperatureOUT = 0;
    digitalWrite(Relay, HIGH); 
    delay(100);
    Serial.println ("OK");
    delay(1000);
  }

delay(100);
  if(sensor.tem >= 40 && TemperatureOUT == 0)
  {
    SMSTemperatureOUT();
    FlagOK = 1;
    TemperatureOUT = 1;
    Serial.println ("TemperatureOUT");
    delay(1000);
  }

  if (sensor.tem <= 20 && sensor.hum >= 80)
  {
    SMSDew();
    FlagOK = 1;
    Serial.println ("Danger of dew");
    delay(1000);
  }
  
  delay(100);
  if(sensor.tem >= 50 && FlagAttention == 0)
  {
    Serial.println ("Attention");
    Call();
    delay(2000);
    SMSAttention();
    FlagOK = 1;
    FlagAttention = 1;
    delay(1000);
  }
  
  delay(100);
  if(sensor.tem >= 60 && FlagDanger == 0){
  Serial.println ("DANGER");
  Serial.println ("Relay, LOW");
  digitalWrite(Relay, LOW);
  
    for (int i = 1; i <= 3; i++)
    {
        delay(1000);
        Call();
        delay(5*60*1000);
    }
    SMSDanger();
    FlagDanger = 1;
    delay(1000);
  }
  
}

void SMSDanger() {   
  mySerial.print("AT+CMGF=1\r\n");     
  delay(500);
  mySerial.println("AT + CMGS = \"+380968098737\"\r\n");        
  delay(1000);
  mySerial.println ("            DANGER");       // Передаємо текст "DANGER"
  mySerial.println ((String) "Temperature:   " + sensor.tem + " *C");          
  mySerial.println ((String) "Humidity:         " + sensor.hum + " %");       
  delay(100);
  mySerial.println((char)26);        
  delay(1000);  
}

void SMSAttention() {   
  mySerial.print("AT+CMGF=1\r\n");     
  delay(500);
  mySerial.println("AT + CMGS = \"+380968098737\"\r\n");        
  delay(1000);
  mySerial.println ("           ATTENTION");       // Передаємо текст "ATTENTION"
  mySerial.println ((String) "Temperature:   " + sensor.tem + " *C");          
  mySerial.println ((String) "Humidity:         " + sensor.hum + " %");       
  delay(100);
  mySerial.println((char)26);        
  delay(1000);  
}

void SMSDew() {
  mySerial.print("AT+CMGF=1\r\n");     
  delay(500);
  mySerial.println("AT + CMGS = \"+380968098737\"\r\n");        
  delay(1000);
  mySerial.println ("          Danger of dew");       
  mySerial.println ((String) " Temperature:   " + sensor.tem + " *C");          
  mySerial.println ((String) " Humidity:         " + sensor.hum + " %");       
  delay(100);
  mySerial.println((char)26);        
  delay(1000);  
  
  }

void SMSTemperatureOUT() {   
  mySerial.print("AT+CMGF=1\r\n");     
  delay(500);
  mySerial.println("AT + CMGS = \"+380968098737\"\r\n");        
  delay(1000);
  mySerial.println ("Temperature is out of tolerance");       
  mySerial.println ((String) " Temperature:   " + sensor.tem + " *C");          
  mySerial.println ((String) " Humidity:         " + sensor.hum + " %");       
  delay(100);
  mySerial.println((char)26);        
  delay(1000);  
}

void SMSOk() {   
  mySerial.print("AT+CMGF=1\r\n");     
  delay(500);
  mySerial.println("AT + CMGS = \"+380968098737\"\r\n");        
  delay(1000);
  mySerial.println ("            OK");   
  mySerial.println ("   Normal Temperature");
  mySerial.println ((String) "Temperature:   " + sensor.tem + " *C");          
  mySerial.println ((String) "Humidity:         " + sensor.hum + " %");       
  delay(100);
  mySerial.println((char)26);        
  delay(1000);  
}

void Call() {
  mySerial.println("ATD+ +380968098737;");
  delay(10000); 
  mySerial.println("ATH"); // повесить трубку
  mySerial.println((char)26);        
  delay(1000);  
  }
