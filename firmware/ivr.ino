// This #include statement was automatically added by the Particle IDE.
#include "DHT.h"

#include "Emic2TTS.h"
#include "SPIUart.h"
#include "Adafruit_FONA.h"

#define FONA_RST                    D1
#define FONA_RI_INTERRUPT           RX

#define FONA_INITIALIZED            0
#define FONA_ERROR_NOT_INITIALIZED  1
#define FONA_ERROR_NO_CALLER_NOTIF  2

#define EVENT_FONA                  "fona-event"

#define MAX_ARGS                    64
#define MAX_MESSAGE                 63
#define MAX_PHONE                   13

#define DHTPIN                      D0
#define DHTTYPE                     DHT22  

Adafruit_FONA fona = Adafruit_FONA(FONA_RST);
SPIUart SPIUart;
Emic2TtsModule emic;

int fonaState = FONA_ERROR_NOT_INITIALIZED;

char szNumber[MAX_PHONE + 1];
int temperature;
int humidity;
int dtmf_digit;

int tempdigit = 7;
int humdigit = 8;
int hangdigit = 9;

boolean isMessageSent = false;
int val = 0;

int dr = D2; //dr goes high when data ready
int d0 = D3;
int d1 = D4;
int d2 = D5;
int d3 = D6;

int dataReady = LOW;
int data[4] = {0,0,0,0};

int currentPinDigit = 3;
int userpin[4] = {0,0,0,0};
int pin = 9999;
bool isPinVerified;

DHT dht(DHTPIN, DHTTYPE);

int sendvoicemessage(String args){
    if(FONA_INITIALIZED != fonaState){
        return -100;
    }
    
    args.toCharArray(szNumber, MAX_MESSAGE);
    
    fona.callPhone(szNumber);
    
    return 1;
}

int setautoanswer(String args){
    int count = args.toInt();
    
    return fona.setAutoAnswer(count) ? 1 : 0;
}

int setpin(String args){
    pin = args.toInt();
    
    return 1;
}

void Publish(String message){
    Spark.publish(EVENT_FONA, message, 60, PRIVATE);
}

void setup() {
    Serial.begin(9600);
    Serial1.begin(9600);
    dht.begin();
    
    pinMode(d0, INPUT);    
    pinMode(d1, INPUT); 
    pinMode(d2, INPUT); 
    pinMode(d3, INPUT); 
    pinMode(dr, INPUT);
    
    Spark.function("sendvoice", sendvoicemessage);
    Spark.function("autoans", setautoanswer);
    
    SPIUart.begin();
    
    if(fona.begin(SPIUart)) {
        if(fona.callerIdNotification(true, FONA_RI_INTERRUPT)){
            fonaState = FONA_INITIALIZED;
            
            Serial.println("FONA Initialized!");
            
            fona.setAutoAnswer(2);
            
            Publish("FONA initialized");
        }
        else{
            Serial.println("FONA Caller Identificaiton not enabled");
            
            Publish("FONA Caller ID not Enabled");
            
            fonaState = FONA_ERROR_NO_CALLER_NOTIF;
        }
    }
    else{
        Publish("FONA not initialized");
        Serial.println("FONA not initialized");
        
        fonaState = FONA_ERROR_NOT_INITIALIZED;
    }
    
    
    emic.init(Serial1);
    
    attachInterrupt(D2, dtmf, RISING);
    
    Publish("SPIUart and Emic initialized");
}

void loop() {
    if(FONA_INITIALIZED == fonaState){
        char szTemp[64];
        int status = fona.getStatus();
        
        if(-1 == status){
            isMessageSent = false;
        }
        
        temperature = dht.readTemperature();
        humidity = dht.readHumidity();
        
        sprintf(szTemp, "Current temperature is %d, humidity is %d", temperature, humidity);
        
        Serial.println(szTemp);
        
        if(status == 0){
            if(!isMessageSent){
                //sprintf(szTemp, "Press %d for temperature %d for humidity or %d to hangup", tempdigit, humdigit, hangdigit);
                
                emic.say("Welcome to IVR Demo using Particle and GSM Breakout Board.");
                emic.say("Enter your 4 digit pin.");
                //emic.say(szTemp);
                
                isMessageSent = true;
            }
            else{
                dataReady = digitalRead(dr);
                
                if(dtmf_digit != 0){
                    Serial.println();    
                    Serial.print("DTMF digit received: ");
                    Serial.println(dtmf_digit, DEC); 
                        
                    if(isPinVerified){
                        szTemp[0] = 0;
                        
                        if(tempdigit == dtmf_digit){
                            sprintf(szTemp, "Current temperature is %d", temperature);
                        }
                        else if(humdigit == dtmf_digit){
                            sprintf(szTemp, "Current humidity is %d", humidity);
                        }
                        else if(hangdigit == dtmf_digit){
                            sprintf(szTemp, "Thank you for using this demo.");
                        }
                        
                        if(strlen(szTemp) > 0){
                            emic.say(szTemp);
                        }
                        
                        if(hangdigit == dtmf_digit){
                            fona.hangUp();
                            
                            isPinVerified = false;
                            currentPinDigit = 3;
                        }
                    }
                    else{
                        if(currentPinDigit >= 0){
                            userpin[currentPinDigit] = dtmf_digit;
                            
                            --currentPinDigit;
                        }
                        
                        if(currentPinDigit < 0){
                            currentPinDigit = 3;
                            
                            int pinentered = userpin[3] * 1000  + 
                                             userpin[2] * 100   + 
                                             userpin[1] * 10    + 
                                             userpin[0]; 
                            
                            Serial.println();
                            Serial.print("Current Pin ");
                            Serial.print(pin);
                            Serial.print(" User Entered Pin ");
                            Serial.print(pinentered);
                            Serial.println();
                            
                            if(pin == pinentered){
                                isPinVerified = true;
                                
                                sprintf(szTemp, "Press %d for temperature %d for humidity or %d to hangup", tempdigit, humdigit, hangdigit);
                
                                emic.say(szTemp);
                            }
                            else{
                                emic.say("You entered an invalid pin.");
                                isPinVerified = false;
                                
                                fona.hangUp();
                            }
                        }
                    }
                    
                    dtmf_digit = 0;
                }
            }
        }
        else{
            isPinVerified = false;
            currentPinDigit = 3;
        }
    }
    else{
        Serial.println("FONA not initialized");
    }

    delay(10);
}

void dtmf(){
    Serial.println("D2 Rising ISR");
    
    data[0] = digitalRead(d0);
    data[1] = digitalRead(d1);
    data[2] = digitalRead(d2);
    data[3] = digitalRead(d3);
    
    dtmf_digit = 8* data[3] +  4* data[2] + 2* data[1] + data[0] ;

    if (dtmf_digit==10)
        dtmf_digit =0;
}