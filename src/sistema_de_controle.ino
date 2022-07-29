#include "EmonLib.h" //INCLUSÃO DE BIBLIOTECA

#define correnteMin 0.01
#define Corrente_CAL 18.40 //VALOR DE CALIBRAÇÃO (DEVE SER AJUSTADO EM PARALELO COM UM MULTÍMETRO MEDINDO A CORRENTE DA CARGA)
const int pinoSensor = A2; //PINO ANALÓGICO EM QUE O SENSOR ESTÁ CONECTADO
float ruido = 0.08; //RUÍDO PRODUZIDO NA SAÍDA DO SENSOR (DEVE SER AJUSTADO COM A CARGA DESLIGADA APÓS CARREGAMENTO DO CÓDIGO NO ARDUINO)

EnergyMonitor emon1; //CRIA UMA INSTÂNCIA

#define set_bit(reg, bit)       (reg|= (1<<bit))
#define clear_bit(reg, bit)     (reg &= ~(1<<bit))
#define toggle_bit(reg, bit)    (reg^= (1<<bit))
#define test_bit(reg, bit)      (reg &(1<<bit))

//Porta ligada ao pino IN1 do modulo
#define porta_rele1 PD7
//Porta ligada ao pino IN2 do modulo
#define porta_rele2 PB0
//Porta ligada ao pino IN2 do modulo
#define porta_rele3 PB1  

void setup(){  
  
  //Define pinos para o rele como saida
  set_bit(PORTD, porta_rele1);
  set_bit(PORTB, porta_rele2);
  set_bit(PORTB, porta_rele3);
  /*pinMode(porta_rele1, OUTPUT); 
  pinMode(porta_rele2, OUTPUT);
  pinMode(porta_rele3, OUTPUT);*/

  //SENSOR DE CORRENTE
  Serial.begin(115200); //INICIALIZA A SERIAL
  emon1.current(pinoSensor, Corrente_CAL); //PASSA PARA A FUNÇÃO OS PARÂMETROS (PINO ANALÓGIO / VALOR DE CALIBRAÇÃO)
}

void loop(){
  emon1.calcVI(17,100); //FUNÇÃO DE CÁLCULO (17 SEMICICLOS / TEMPO LIMITE PARA FAZER A MEDIÇÃO)
  double corrente = emon1.Irms; //VARIÁVEL RECEBE O VALOR DE CORRENTE RMS OBTIDO
  corrente = corrente-ruido; //VARIÁVEL RECEBE O VALOR RESULTANTE DA CORRENTE RMS MENOS O RUÍDO
  
  if(corrente < 0){ //SE O VALOR DA VARIÁVEL FOR MENOR QUE 0, FAZ 
      corrente = 0; //VARIÁVEL RECEBE 0
  }
 
    Serial.print("Corrente medida: "); //IMPRIME O TEXTO NA SERIAL
    Serial.print(corrente); //IMPRIME NA SERIAL O VALOR DE CORRENTE MEDIDA
    Serial.println("A"); //IMPRIME O TEXTO NA SERIAL

   if(corrente >= correnteMin){
      digitalWrite(porta_rele1, HIGH);  //Desliga rele 1
      digitalWrite(porta_rele2, LOW); //Liga rele 2
      digitalWrite(porta_rele3, HIGH); //Desliga rele 3
      Serial.print("Corrente alta");
   } else if(corrente < correnteMin){
      Serial.print("Corrente baixa");
      digitalWrite(porta_rele1, LOW); //Liga rele 1
      digitalWrite(porta_rele2, HIGH); //Desliga rele 2
      digitalWrite(porta_rele3, LOW); //Liga rele 3
   }
} 
