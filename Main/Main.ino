#define PINO_SERVO_TAMPA 2
#define PINO_SERVO_INTERRUPTOR 3
#define PINO_LED_BRANCO 4 //Dormindo em paz
//Nervosômetro
#define PINO_BRAVO_1 5
#define PINO_BRAVO_2 6
#define PINO_BRAVO_3 7
#define PINO_INTERRUPTOR 9

#include <Servo.h>
Servo servo_tampa;
Servo servo_arm;

int brilho = 0;
int tipo_brilho = 1;
int nervosometro = 0;
unsigned long previousMillis = 0;        // Variável de controle do tempo
unsigned long intervaloMillis = 1000 * 3;     // Tempo em ms do intervalo a ser executado
unsigned long tempoRetemNervoso =0; //Tempo para ficar mais nervoso
unsigned long intervaloRetemNervoso =  1000 * 4;
int retemNervoso = 0;

int angulo_tampa_aberta = 0;
int angulo_tampa_fechada = 50;
int angulo_braco_pos1 = 70;
int angulo_braco_pos2 = 130;
int angulo_braco_pos3 = 180;


void led_status(int _status = 0)
{
  switch (_status) {
    case 0:
      digitalWrite(PINO_LED_BRANCO, HIGH); //Dormindo
      break;
    case 1:
      digitalWrite(PINO_LED_BRANCO, LOW); //Acordado
      break;
    case 2:
      digitalWrite(PINO_LED_BRANCO, ! digitalRead(PINO_LED_BRANCO)); //Muito Nervoso
      break;
  }
}

void setup() {
  Serial.begin(9600);
  
  //Configura os pinos PWM
  servo_tampa.attach(PINO_SERVO_TAMPA);
  servo_arm.attach(PINO_SERVO_INTERRUPTOR);

  //Posiciona os servos
  servo_tampa.write(angulo_tampa_fechada);
  servo_arm.write(angulo_braco_pos1);
  
  //Configura os pinos digitais
  pinMode(PINO_LED_BRANCO, OUTPUT);
  pinMode(PINO_BRAVO_1, OUTPUT);
  pinMode(PINO_BRAVO_2, OUTPUT);
  pinMode(PINO_BRAVO_3, OUTPUT);
  
  //Interruptor
  pinMode(PINO_INTERRUPTOR, INPUT_PULLUP);

  //Inicializa o nervosômetro
  digitalWrite(PINO_LED_BRANCO, HIGH);
  digitalWrite(PINO_BRAVO_1, LOW);
  digitalWrite(PINO_BRAVO_2, LOW);
  digitalWrite(PINO_BRAVO_3, LOW);

  led_status();
}

void atualiza_nervosometro()
{
  Serial.println("Nervosometro: " + String(nervosometro));

  if (nervosometro >= 4)
  {
    digitalWrite(PINO_BRAVO_1, !digitalRead(PINO_BRAVO_1));
    digitalWrite(PINO_BRAVO_2, !digitalRead(PINO_BRAVO_2));
    digitalWrite(PINO_BRAVO_3, !digitalRead(PINO_BRAVO_3));
    led_status(2);
    delay(100); 
  }else
  {
    if (nervosometro >= 1)
      digitalWrite(PINO_BRAVO_1, HIGH);
    else
      digitalWrite(PINO_BRAVO_1, LOW);
    
    if (nervosometro >= 2)
      digitalWrite(PINO_BRAVO_2, HIGH);
    else
      digitalWrite(PINO_BRAVO_2, LOW);
    
    if (nervosometro >= 3)
      digitalWrite(PINO_BRAVO_3, HIGH);
    else
      digitalWrite(PINO_BRAVO_3, LOW);

  }

}

void abre_tampa()
{
  servo_tampa.write(angulo_tampa_aberta);
  delay(100);
}

void fecha_tampa()
{
  servo_tampa.write(angulo_tampa_fechada);
  delay(100);
}

int desliga_interruptor()
{
  servo_arm.write(angulo_braco_pos3);
  delay(200);
  servo_arm.write(angulo_braco_pos2);
  delay(200);
  return ! interruptor_ligado();
}

void volta_dormir()
{
  servo_arm.write(angulo_braco_pos1);
  delay(90);
  fecha_tampa();
}

void treme_tampa()
{
  for (int i = 0; i < 10; i++)
  {
   servo_tampa.write(angulo_tampa_aberta);
   delay(50);
   servo_tampa.write(angulo_tampa_aberta - 30);
   delay(50);
  }
}
void acorda(int nivel_nervoso)
{
  int i = 0;
  int j = 0;
  switch (nivel_nervoso) {
    case 0:
      abre_tampa();
      if (!desliga_interruptor())
      {
        volta_dormir();
        delay(1000);
        if(interruptor_ligado())
        {
          abre_tampa();
          if (!desliga_interruptor())
          {
            delay(1000);
            if(interruptor_ligado())
            {
              if (!desliga_interruptor())
              {
                treme_tampa();
                volta_dormir();
                nervosometro++;
                delay(1000 * 5);
                return;
              }
            }
          }
        }
      }
      break;
    case 1:
      abre_tampa();
      for (i = 0; i < 5; i++)
      {
        desliga_interruptor(); 
      }
      volta_dormir();
        if (interruptor_ligado())
      {
        abre_tampa();
        treme_tampa();
        fecha_tampa();
        abre_tampa();
        for (i = 0; i < 10; i++)
        {
          if (interruptor_ligado())
            desliga_interruptor(); 
          else
            break;
        }
        delay(1000);
        volta_dormir();
        nervosometro++;
        delay(1000 * 5);
        return;
      }
      nervosometro++;
      break;
    case 2:
      for (i = 0; i < 5; i++)
      {
        abre_tampa();
        fecha_tampa();
      }
      abre_tampa();
      delay(2000);
      if (interruptor_ligado())
      {
        treme_tampa();
        for (i = 0; i < 10; i++)
        {
            desliga_interruptor(); 
        }
        volta_dormir();
      }else
      {
        volta_dormir();
        return;
      }
      delay(1000);
      abre_tampa();
      if (interruptor_ligado())
      {
        treme_tampa();
        for (i = 0; i < 10; i++)
        {
          if (interruptor_ligado())
            desliga_interruptor(); 
          else
          {
            volta_dormir();
            return;
          }
        }
        volta_dormir();
      }else
      {
        volta_dormir();
        nervosometro++;
        return;
      }
      break;
    case 3:

      for(i = 0; i <= 3; i++){
        if (interruptor_ligado())
        {
          abre_tampa();
          desliga_interruptor();
          volta_dormir();
        }
        else
        {
          volta_dormir();
          return;
        }
      }
      abre_tampa();
      for(i = 0; i <= 10; i++){
        if (interruptor_ligado())
          desliga_interruptor();
        else
        {
          volta_dormir();
          return;
        }
      }
      treme_tampa();
      delay(1000);
      volta_dormir();
      if (interruptor_ligado())
      {
        abre_tampa();
        desliga_interruptor();
        volta_dormir();
        nervosometro++;
        break;
      }
      break;
    case 4:

      for(i = 0; i <=3; i++)
      {
        for(j = 0; j <=3; j++)
        {
          atualiza_nervosometro();
          if(interruptor_ligado())
          {
            abre_tampa();
            fecha_tampa();   
          }else
          {
             volta_dormir();
             return;
          }
        }
        abre_tampa();
        for(j = 0; j <=5; j++)
        {
          atualiza_nervosometro();
          if(interruptor_ligado())
          {
             desliga_interruptor();
             treme_tampa();
           }else
          {
             volta_dormir();
             return;
          }
        }
        volta_dormir();
      }
      
      break;
    default:
      break;
  }
  volta_dormir();
}

int interruptor_ligado()
{
  return !digitalRead(PINO_INTERRUPTOR);
}

void loop() 
{
 
  atualiza_nervosometro();
  if (interruptor_ligado())
  {
    Serial.println("Interruptor" + String(digitalRead(PINO_INTERRUPTOR)));
    led_status(1);
    acorda(nervosometro);
    previousMillis = millis();    //Tempo atual em ms
    //Lógica para ficar nervoso com a encheção de saco
    if (retemNervoso == 0)
    {
      tempoRetemNervoso = millis();
    }
    retemNervoso++;
    Serial.println("Retem Nervoso: " + String(retemNervoso));
  }else
  {
    led_status();
    if (retemNervoso > 0)
    {
      if ((millis() - tempoRetemNervoso) > intervaloRetemNervoso)
      {
        retemNervoso = 0;
      }
      if (retemNervoso > 3)
      {
        retemNervoso = 0;
        nervosometro++; 
      }
    }
    if (nervosometro > 0)
    {
      if ((millis() - previousMillis) > (intervaloMillis * nervosometro))
      {
        previousMillis = millis();
        nervosometro--;     
      }
    }
  }
  
}