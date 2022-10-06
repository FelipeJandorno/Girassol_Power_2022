#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <Servo.h>

#define F_CPU 16000000UL  //16MHz

void timer(void);
void my_setup(void);
void my_loop(void);
void control_system(void);
int ad_conv_a3(void);

/*Sistema de Controle - inicialização*/
#define correnteMin 0.5

float corrente; //A variável que vai armazenar a corrente medida pelo sensor
float sensibilidade = 0.066; 

//Porta ligada ao pino IN1 do modulo
int porta_rele1 = 7;
//Porta ligada ao pino IN2 do modulo
int porta_rele2 = 8;
//Porta ligada ao pino IN2 do modulo
int porta_rele3 = 9;

/*LED e Motor - inicialização*/
Servo Horizontal;                     // Inicia o Servo da Horizontal

int ServoHorizontal = 90;             // Estabelece valor fixo à ServoHorizontal
 
int LimiteServoHorizontalMax = 180;   // Estabele os limites de rotação
int LimiteServoHorizontalMin = 37;    // Estabele os limites de rotação
  
int LDRDC = A0;                       // Inicia LDRDC no pino A0
int LDRDB = A1;                       // Inicia LDRDB no pino A1

//===============================================================//

ISR(TIMER0_OVF_vect){
  TCNT0 = 6;
  timer();
}

//===============================================================//

int main(){
  my_setup();
  while(1) my_loop();
  return 0;
}

//===============================================================//

void my_setup(){
  /*config timer*/
  
  TCCR0B |= (1<<CS01) | (1<<CS00);
  TCNT0 = 6;

  /*setup config*/
  pinMode(porta_rele1, OUTPUT);
  pinMode(porta_rele2, OUTPUT);
  pinMode(porta_rele3, OUTPUT);

  Serial.begin(9600);
  
  /*stop timer*/
  TIMSK0 = 0x01;
  sei();

  /*AD config*/
  ADCSRA  = 0x93;   //liga conversão AD, fator de divisão de 8
  ADMUX   = 0x41;   //tensão de ref. de 5V, canal A3

  /*LED e Motor*/
  Horizontal.attach(3);
  Horizontal.write(180);
}

//===============================================================//

void my_loop(){  
}

//===============================================================//

void timer(void){

  static unsigned int temp_01 = 1;
  
  if(temp_01 < 500){
    temp_01++;
  } else{
    //control_system();
    LDR_e_MOTORES();
    temp_01 = 1;
  }
}

//=====================CONTROL SYSTEM============================//


void control_system(){
  float corrente = calculaCorrente(filtroDaMedia());
  Serial.print("Corrente");
  Serial.println(corrente);
  
    if(corrente < 0){ //SE O VALOR DA VARIÁVEL FOR MENOR QUE 0, FAZ 
      corrente = 0; //VARIÁVEL RECEBE 0
      Serial.println(corrente);
      Serial.println("A"); //IMPRIME O TEXTO NA SERIAL
    }
    if ( corrente >= 0)  { // Se não houver objeto na frente não envia nada
      Serial.println(corrente);
      Serial.println("A"); //IMPRIME O TEXTO NA SERIAL
    }

  //Faz o controle dos relés através da leitura da corrente
  
   if(corrente >= correnteMin){
      digitalWrite(porta_rele1, LOW);  //Desliga rele 1
      digitalWrite(porta_rele2, HIGH); //Liga rele 2
      digitalWrite(porta_rele3, LOW); //Desliga rele 3
   } else{
      digitalWrite(porta_rele1, HIGH); //Liga rele 1
      digitalWrite(porta_rele2, LOW); //Desliga rele 2
      digitalWrite(porta_rele3, HIGH); //Liga rele 3
   }
}

// A função "calculaCorrente" vai converter o sinal amostrado pelo arduino em A2 num valor de corrente.
float calculaCorrente(int sinalSensor) {
  return (float)(sinalSensor-508)*(5.000)/(1023.000*sensibilidade);
}



int filtroDaMedia(){
  long somaDasCorrentes=0, mediaDasCorrentes;
  for(int i=0; i<1000; i++){
    somaDasCorrentes+=ad_conv_a3();
    _delay_ms(1);
  }
  mediaDasCorrentes=somaDasCorrentes/1000; 
  return mediaDasCorrentes;
}

//=========================CONVERSOR AD==========================//

int ad_conv_a3(){
  static int analogH, analogL, analog;

   ADCSRA |= (1<<ADSC);

  while(!(ADCSRA&=~(1<<ADIF)));

  ADCSRA |= (1<<ADIF);

  analogL = ADCL;
  analogH = ADCH;

  analog = (analogH<<8) | analogL;

  return analog;
}

int ad_conv_a0(){
  static int analogH, analogL, analog;
  ADMUX = 0x40;
  ADCSRA  = 0x93;

   ADCSRA |= (1<<ADSC);

  while(!(ADCSRA&=~(1<<ADIF)));

  ADCSRA |= (1<<ADIF);

  analogL = ADCL;
  analogH = ADCH;

  analog = (analogH<<8) | analogL;

  return analog; 
}

int ad_conv_a1(){
  ADMUX = 0x41;
  ADCSRA  = 0x93;
  
  static int analogH, analogL, analog;

  ADCSRA |= (1<<ADSC);

  while(!(ADCSRA&=~(1<<ADIF)));

  ADCSRA |= (1<<ADIF);

  analogL = ADCL;
  analogH = ADCH;

  analog = (analogH<<8) | analogL;

  return analog;
}
//==================== LED E MOTORES ============================//

void LDR_e_MOTORES(){
  int LDC = ad_conv_a0();      // Leitura Analógica direita                      ad_conv_ax
  int LDB = ad_conv_a1();      // Leitura Analógica do LDR esquerda              ad_conv_ax2

 //Serial.println("ola");
 Serial.println (LDC);
 //Serial.println (LDB);
  int tol = 50;
  int ValorDir = LDC;
  int ValorEsq = LDB;
  int DifDirEsq = ValorDir - ValorEsq;      // Diferença entre LED direita e esquerda
 
  /*---------------------------------------------------*/
   
  // Realiza a leitura e executa os movimentos referente ao Servo Horizontal
  if (-1 * tol > DifDirEsq || DifDirEsq > tol) {
    if (ValorDir > ValorEsq)  {
      ServoHorizontal = --ServoHorizontal;
      
      if (ServoHorizontal < LimiteServoHorizontalMin)  {
        ServoHorizontal = LimiteServoHorizontalMin;
      }
    }
    else if (ValorDir < ValorEsq)  {
      ServoHorizontal = ++ServoHorizontal;
      
      if (ServoHorizontal > LimiteServoHorizontalMax)  {
        ServoHorizontal = LimiteServoHorizontalMax;
      }
    }
    else if (ValorDir = ValorEsq){
    
    }
    Horizontal.write(ServoHorizontal);
  }
}












/*OBSERVAÇÕES:

01. Verificar alteração no filtroDaMedia no 'loop'
02. ADCSRA  = 0x93; tá dando problema

*/
