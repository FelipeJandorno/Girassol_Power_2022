#include <Servo.h>          // Inclui a Biblioteca
 
Servo Horizontal;           // Inicia o Servo da Horizontal

int ServoHorizontal = 90;   // Estabelece valor fixo à ServoHorizontal
 
int LimiteServoHorizontalMax = 180;  // Estabele os limites de rotação
int LimiteServoHorizontalMin = 37;   // Estabele os limites de rotação
  
int LDRDC = A0;             // Inicia LDRDC no pino A0
int LDRDB = A1;             // Inicia LDRDB no pino A1
 
void setup()  {
  Horizontal.attach(3);     // Inicia servo Horizontal na porta D3
  Horizontal.write(180);    // Inicia servo Horizontal na posição 180
  //delay(3000);              // Aguarda 3 segundos
  Serial.begin(115200);
}
 
void loop() {
  int LDC = analogRead(LDRDC);      // Leitura Analógica direita
  int LDB = analogRead(LDRDB);      // Leitura Analógica do LDR esquerda
 
 
  int tol = 50;
  int ValorDir = LDC;
  int ValorEsq = LDB;
  int DifDirEsq = ValorDir - ValorEsq;      // Diferença entre LED direita e esquerda
 
  /*---------------------------------------------------*/
   
  // Realiza a leitura e executa os movimentos referente ao Servo Horizontal
  if (-1 * tol > DifDirEsq || DifDirEsq > tol) {
    if (ValorDir > ValorEsq)  {
      ServoHorizontal = --ServoHorizontal;
      Serial.println ("TEM MAIS SOL NA DIREITA");
      delay(3000);
      if (ServoHorizontal < LimiteServoHorizontalMin)  {
        ServoHorizontal = LimiteServoHorizontalMin;
      }
    }
    else if (ValorDir < ValorEsq)  {
      ServoHorizontal = ++ServoHorizontal;
      Serial.println ( "TEM MAIS SOL NA ESQUERDA");
      delay(3000);
      if (ServoHorizontal > LimiteServoHorizontalMax)  {
        ServoHorizontal = LimiteServoHorizontalMax;
      }
    }
    else if (ValorDir = ValorEsq){
    Serial.println ("o MOTOR VAI FICAR PARADO");
    delay(3000);
    }
    Horizontal.write(ServoHorizontal);
    
  }
 
  delay(100);   // Aguarda 0,1 segundo
}
