//Routine for calculating 
//the velocity from a pitot tube and MPXV7002DP pressure differential sensor
//the temperature and humidity from a DHS 22H
//storing all values in a CVS file

//pressure: static and pitot
float V_0 = 5.0; // supply voltage to the pressure sensor
float rho = 1.204; // density of air 

float pitotVout = 0.0;
float pitotVin = 0.0;
float pitotPressure = 0.0;

float staticVout = 0.0;
float staticVin = 0.0;
float staticPressure = 0.0;


int staticPin = A0;
int pitotPin = A1;

// parameters for averaging and offset
int staticOffset = 0;
int pitotOffset = 0;
int offsetSize = 10;
int velocMeanSize = 20;
int zeroSpan = 2;

//DHS: temp and humidity
#include <DHT.h>

//DHS constants
#define DHTPIN 7     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino

//DHS variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value

//data writing
#include <SD.h>

const int chipSelect = 4;


// setup and calculate offset, setup DHS
void setup() {
  Serial.begin(9600);

  //SD card configuration
  while (!Serial) {;}
  Serial.print("Initializing SD card...");
  pinMode(10, OUTPUT);
  if (!SD.begin(chipSelect)) 
  { Serial.println("Card failed, or not present");
    return;}
  Serial.println("card initialized.");
  
  dht.begin(); //DHT initialization

  //calculate offset for pitot-static pressure
  for (int ii=0;ii<offsetSize;ii++){
    pitotOffset += analogRead(pitotPin)- 512;
    staticOffset + =analogRead(staticPin) - 512;
  }
  pitotOffset /= offsetSize;
  staticOffset /= offsetSize;
}


void loop() {
  // PRESSURE
  float adcAvg = 0; float veloc = 0.0;
  
  // average a few ADC readings for stability
  for (int ii=0;ii<velocMeanSize;ii++){
    adcAvg += (analogRead(pitotPin) - pitotOffset) * 240 - (analogRead(staticPin) - staticOffset) * 240; // 1400 kPa, 5V - 240 kPa/V
  }
  adcAvg/=velocMeanSize;
  
  // make sure if the ADC reads below 512, then we equate it to a negative velocity
  if (adcAvg > 512-zeroSpan and adcAvg<512+zeroSpan){
  } else{
    if (adcAvg<512){
      veloc = -sqrt((-10000.0*(((adcAvg)/1023.0)-0.5))/rho);
    } else{
      veloc = sqrt((10000.0*(((adcAvg)/1023.0)-0.5))/rho);
    }
  }
  
  // TEMP & HUMIDITY
  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp= dht.readTemperature();

  File essData = SD.open("essData.txt", FILE_WRITE);

  if (essData) {
    //storing data to CSV file on SD cards
    essData.print(veloc, 3);
    essData.print(", ");
    essData.print(temp, 3);
    essData.print(", ");
    essData.print(hum, 3);
    essData.close();
  }
  else {
    Serial.println("error opening essData.txt");
  }
  
  //printing to IDE for real time monitoring
  Serial.print(veloc, 3); // print velocity;
  Serial.print(" "); //comma for formatting
  Serial.print(temp, 3);
  Serial.print(" "); //comma for formatting
  Serial.println(hum, 3);

  
  //Print temp and humidity values to serial monitor
  
  
  delay(1000); // delay for stability
}
