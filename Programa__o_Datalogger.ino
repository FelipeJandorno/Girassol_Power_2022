#include <SPI.h>
#include <SD.h>

const int chipSelect = 10;
float corrente; //A variável que vai armazenar a corrente medida pelo sensor
float sensibilidade = 0.066;
 
//vars do timer millis que determina o intervalo entre medidas
unsigned long period = 60000;  //modifique aqui o intervalo entre medidas, utilize multiplos de 60000ms   
unsigned long time_now = 0;
int tempo = 1;


//===================================================================================//

void setup() {

Serial.begin(9600);

Serial.print(F("Lendo cartao SD"));
delay(100);
  if (!SD.begin(chipSelect)) {
     Serial.println(F("Leitura Falhou"));
     while (1);
  }
Serial.println(F("Cartao SD Inicializado!"));
 
}


//===================================================================================//
 
void loop() {
 float corrente = calculaCorrente(filtroDaMedia());
 
  if (millis() >= time_now + period) {
  time_now += period;
     
      digitalWrite(6, HIGH);
      delay(2200);                      
      digitalWrite(6, LOW);        
 
 
        String dataString = "";
          dataString += String(corrente);
           
      File dataFile = SD.open("dados.csv", FILE_WRITE);
       
        if (dataFile) {
          dataFile.println(dataString);
          dataFile.close();
          Serial.println(dataString);
        }
          
        else {
          Serial.println(F("ERRO"));
       }
       tempo++; //contador de minutos
  }       
}


//===================================================================================//

float calculaCorrente(int sinalSensor) {
  return (float)(sinalSensor-508)*(5.000)/(1023.000*sensibilidade);
}


//===================================================================================//

int filtroDaMedia(){
  long somaDasCorrentes=0, mediaDasCorrentes;
  for(int i=0; i<1000; i++){
    somaDasCorrentes+=analogRead(A2);
    delay(1);
  }
  mediaDasrrentes=somaDasCorrentes/1000; 
  return mediaDasCorrentesCo;
}
