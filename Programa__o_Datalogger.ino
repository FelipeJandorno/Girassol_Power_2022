#include <SPI.h>
#include <SD.h>
#include <EmonLib.h> //INCLUSÃO DE BIBLIOTECA

#define Corrente_CAL 18.40 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO MEDINDO A CORRENTE DA CARGA)

const int chipSelect = 10;
const int pinoSensor = A2; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO

float ruido = 0.08; //RUÍDO PRODUZIDO NA SAÍDA DO SENSOR (DEVE SER AJUSTADO COM A CARGA DESLIGADA APÓS CARREGAMENTO DO CÓDIGO NO ARDUINO)
 
//vars do timer millis que determina o intervalo entre medidas
unsigned long period = 60000;  //modifique aqui o intervalo entre medidas, utilize multiplos de 60000ms   
unsigned long time_now = 0;
int tempo = 1;

EnergyMonitor emon1; //CRIA UMA INSTÂNCIA

void setup() {

Serial.begin(9600);
pinMode(pinoSensor, INPUT); //DECLARA O PINO COMO ENTRADA

emon1.current(pinoSensor, Corrente_CAL); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO)
   
Serial.print(F("Lendo cartao SD"));
delay(100);
  if (!SD.begin(chipSelect)) {
     Serial.println(F("Leitura Falhou"));
     while (1);
  }
Serial.println(F("Cartao SD Inicializado!"));
 
}
 
 
void loop() {
 
 
  if (millis() >= time_now + period) {
  time_now += period;
     
      digitalWrite(6, HIGH);
      delay(2200);                      
      digitalWrite(6, LOW);        
 
 
        emon1.calcVI(17,100); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
        double corrente = emon1.Irms; //VARIÁVEL RECEBE O VALOR DE CORRENTE RMS OBTIDO
        corrente = corrente-ruido; //VARIÁVEL RECEBE O VALOR RESULTANTE DA CORRENTE RMS MENOS O RUÍDO
  
        if(corrente < 0){ //SE O VALOR DA VARIÁVEL FOR MENOR QUE 0, FAZ 
          corrente = 0; //VARIÁVEL RECEBE 0
         }
        
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
