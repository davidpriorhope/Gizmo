#define sensore A0 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
#define sensorB A1 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
#define sensorG A2 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
#define sensorD A3 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
#define sensorA A4 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
#define sensorE A5 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)

#define Grove_Water_Sensor 41 // Attach Water sensor to Arduino Digital Pin 31
#define Pump 52 // Attach an Pump to Digital Pin 52 (or use onboard Pump)

int switchStateWater = 0;

//e string
int estepPin = 3;
int edirPin = 38;

//B string
int BstepPin = 11;
int BdirPin = 35;

//G
int GstepPin = 5;
int GdirPin = 30;

//D
int DstepPin = 10;
int DdirPin = 28;

//A
int AstepPin = 9;
int AdirPin = 26;

//E
int EstepPin = 6;
int EdirPin = 24;

int max_dist = 17;

int distance1 = max_dist * 0.25; //used for closest distance
int distance2 = max_dist * 0.5; //used for second closts distance
int distance3 = max_dist * 0.75; //used for second first distance
int distance4 = max_dist; // used for furthest distance
//assuming maximum detectable distance is 17


//TOP e string
int e_DtoDsharp = 2956;   //specifies number of microsteps to tune from D to Dsharp
int e_Dsharptoe = 6027;   //specifies number of rotations to tune from Dsharp to E
int e_etoF = 3881;   //specifies number of rotations to tune from E to F
int e_FtoFsharp = 3511;   //specifies number of rotations to tune from F to Fsharp
int counte = 0;

//B STRING
int B_AtoAsharp = 2215;
int B_AsharptoB = 3048;
int B_BtoC = 4533;
int B_CtoCsharp = 8547; //consider revising this
int countB = 0;

//G STRING
int G_FtoFsharp = 2857;
int G_FsharptoG = 1321;
int G_GtoGsharp = 4343;
int G_GsharptoA = 3263;
int countG = 0;

//D STRING
int D_CtoCsharp = 3692;
int D_CsharptoD = 4295;
int D_DtoDsharp = 5826;
int D_DsharptoE = 5658;
int countD = 0;


//A STRING
int A_GtoGsharp = 5264;
int A_GsharptoA = 6190;
int A_AtoAsharp = 6834;
int A_AsharptoB = 6904;
int countA = 0;

//E STRING
int E_DtoDsharp = 1437;
int E_DsharptoE = 2839;
int E_EtoF = 1751;
int E_FtoFsharp = 2173;
int countE = 0;


//Manual Tuning Stuff

int flickswitch = 7;
int flickstate = 0;

int string = 0;
int steppinValue = 0;
int dirpinValue = 0;

int buttonUp = 2;
int buttonDown = 12;
int buttonchange = 8;

int buttonStateUp = 0;
int buttonStateDown = 0;
int buttonStatechange = 0;

void setup() {
  //Serial.begin(9600); // start the serial port

  pinMode(GstepPin, OUTPUT);
  pinMode(GdirPin, OUTPUT);

  pinMode(BstepPin, OUTPUT);
  pinMode(BdirPin, OUTPUT);

  pinMode(EstepPin, OUTPUT);
  pinMode(EdirPin, OUTPUT);

  pinMode(AstepPin, OUTPUT);
  pinMode(AdirPin, OUTPUT);

  pinMode(DstepPin, OUTPUT);
  pinMode(DdirPin, OUTPUT);

  pinMode(estepPin, OUTPUT);
  pinMode(edirPin, OUTPUT);

  pinMode(Grove_Water_Sensor, INPUT); // The Water Sensor is an Input
  pinMode(Pump, OUTPUT); // The Pump is an Output

  digitalWrite(Pump, LOW);

  pinMode(flickswitch, INPUT);

  pinMode(buttonUp, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(buttonchange, INPUT);

}

void loop() {

  //MANUAL TUNING MODE

  flickstate = digitalRead(flickswitch);

  if (flickstate == LOW) {

    digitalWrite(Pump, LOW);

    //Serial.println("Manual Tuning Mode");

    buttonStateUp = digitalRead(buttonUp);
    buttonStateDown = digitalRead(buttonDown);
    buttonStatechange = digitalRead(buttonchange);

    if (buttonStatechange == HIGH)  {
      string = string + 1;
      if (string >= 6) {
        string = 0;
      }
      //Serial.println("I am on string ");
      //Serial.println(string);
      delay(1000);
    }



    if (string == 0) {
      dirpinValue = edirPin;
      steppinValue = estepPin;
    }

    if (string == 1) {
      dirpinValue = BdirPin;
      steppinValue = BstepPin;
    }

    if (string == 2) {
      dirpinValue = GdirPin;
      steppinValue = GstepPin;
    }

    if (string == 3) {
      dirpinValue = DdirPin;
      steppinValue = DstepPin;
    }

    if (string == 4) {
      dirpinValue = AdirPin;
      steppinValue = AstepPin;
    }

    if (string == 5) {
      dirpinValue = EdirPin;
      steppinValue = EstepPin;
    }


    if (buttonStateUp == HIGH)  {

      //Serial.println("up button pressed");
      digitalWrite(dirpinValue, HIGH); // Enables the motor to move in a particular Gdirection
      digitalWrite(steppinValue, HIGH);
      delayMicroseconds(500);
      digitalWrite(steppinValue, LOW);
      delayMicroseconds(500);

    }

    if (buttonStateDown == HIGH)  {
      //Serial.println("string was 0 and low");
      digitalWrite(dirpinValue, LOW); // Enables the motor to move in a particular Gdirection
      digitalWrite(steppinValue, HIGH);
      delayMicroseconds(500);
      digitalWrite(steppinValue, LOW);
      delayMicroseconds(500);
    }
  }

  if (flickstate == HIGH) {

    //Serial.println("Performance Mode");


    //PUMP CODE

    switchStateWater = digitalRead(Grove_Water_Sensor);

    if (switchStateWater == LOW) {
      digitalWrite(Pump, HIGH);
    }

    else {
      digitalWrite(Pump, LOW);
    }

    //READING IR SENSORs

    float volts1 = analogRead(sensorE) * 0.0048828125; // value from sensor * (5/1024)
    int distanceE = 13 * pow(volts1, -1); // worked out from datasheet graph

    float volts2 = analogRead(sensorA) * 0.0048828125; // value from sensor * (5/1024)
    int distanceA = 13 * pow(volts2, -1); // worked out from datasheet graph

    float volts3 = analogRead(sensorD) * 0.0048828125; // value from sensor * (5/1024)
    int distanceD = 13 * pow(volts3, -1); // worked out from datasheet graph

    float volts4 = analogRead(sensorG) * 0.0048828125; // value from sensor * (5/1024)
    int distanceG = 13 * pow(volts4, -1); // worked out from datasheet graph

    float volts5 = analogRead(sensorB) * 0.0048828125; // value from sensor * (5/1024)
    int distanceB = 13 * pow(volts5, -1); // worked out from datasheet graph

    float volts6 = analogRead(sensore) * 0.0048828125; // value from sensor * (5/1024)
    int distancee = 13 * pow(volts6, -1); // worked out from datasheet graph

    //Serial.println(distanceB);



    //TUNING CODE

    //Out of range tuning

    if (distancee > distance4)  {
      if (counte > 0) { // e string From Dsharp to D
        digitalWrite(edirPin, LOW);

        digitalWrite(estepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(estepPin, LOW);
        delayMicroseconds(500);
        counte = counte - 1;
      }

    }

    if (distanceB > distance4)  {
      if (countB > 0) { // B string From Asharp to A
        digitalWrite(BdirPin, LOW);

        digitalWrite(BstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(BstepPin, LOW);
        delayMicroseconds(500);
        countB = countB - 1;
      }

    }

    if (distanceG > distance4)  {
      if (countG > 0) {
        digitalWrite(GdirPin, LOW);

        digitalWrite(GstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(GstepPin, LOW);
        delayMicroseconds(500);
        countG = countG - 1;
      }
    }

    if (distanceD > distance4)  {
      if (countD > 0) {
        digitalWrite(DdirPin, HIGH);

        digitalWrite(DstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(DstepPin, LOW);
        delayMicroseconds(500);
        countD = countD - 1;
      }
    }


    if (distanceA > distance4)  {
      if (countA > 0) {
        digitalWrite(AdirPin, HIGH);

        digitalWrite(AstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(AstepPin, LOW);
        delayMicroseconds(500);
        countA = countA - 1;
      }
    }

    if (distanceE > distance4)  {
      if (countE > 0) {
        digitalWrite(EdirPin, HIGH);

        digitalWrite(EstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(EstepPin, LOW);
        delayMicroseconds(500);
        countE = countE - 1;
      }

    }




    // BETWEEN DISTANCE 3 AND 4

    if (distancee > distance3 and distancee <= distance4) {

      if (counte <= e_DtoDsharp) { //from D to Dsharp - up
        digitalWrite(edirPin, HIGH);
        digitalWrite(estepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(estepPin, LOW);
        delayMicroseconds(500);
        counte = counte + 1;
      }

      if (counte > e_DtoDsharp) { //From e to Dsharp - down
        digitalWrite(edirPin, LOW);

        digitalWrite(estepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(estepPin, LOW);
        delayMicroseconds(500);
        counte = counte - 1;
      }


    }

    if (distanceB > distance3 and distanceB <= distance4) {

      if (countB <= B_AtoAsharp) { //from A to Asharp - up
        digitalWrite(BdirPin, HIGH);
        digitalWrite(BstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(BstepPin, LOW);
        delayMicroseconds(500);
        countB = countB + 1;
      }

      if (countB > B_AtoAsharp) { //From B to Asharp - down
        digitalWrite(BdirPin, LOW);

        digitalWrite(BstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(BstepPin, LOW);
        delayMicroseconds(500);
        countB = countB - 1;
      }


    }


    //string G

    if (distanceG > distance3 and distanceG <= distance4) {

      if (countG <= G_FtoFsharp) {
        digitalWrite(GdirPin, HIGH);
        digitalWrite(GstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(GstepPin, LOW);
        delayMicroseconds(500);
        countG = countG + 1;
      }

      if (countG > G_FtoFsharp) { //From G to Fsharp - down
        digitalWrite(GdirPin, LOW);

        digitalWrite(GstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(GstepPin, LOW);
        delayMicroseconds(500);
        countG = countG - 1;
      }
    }

    //string D

    if (distanceD > distance3 and distanceD <= distance4) {

      if (countD <= D_CtoCsharp) {
        digitalWrite(DdirPin, LOW);
        digitalWrite(DstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(DstepPin, LOW);
        delayMicroseconds(500);
        countD = countD + 1;
      }

      if (countD > D_CtoCsharp) {
        digitalWrite(DdirPin, HIGH);
        digitalWrite(DstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(DstepPin, LOW);
        delayMicroseconds(500);
        countD = countD - 1;
      }
    }



    //String A

    if (distanceA > distance3 and distanceA <= distance4) {

      if (countA <= A_GtoGsharp) {
        digitalWrite(AdirPin, LOW);
        digitalWrite(AstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(AstepPin, LOW);
        delayMicroseconds(500);
        countA = countA + 1;
      }

      if (countA > A_GtoGsharp) {
        digitalWrite(AdirPin, HIGH);
        digitalWrite(AstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(AstepPin, LOW);
        delayMicroseconds(500);
        countA = countA - 1;
      }

    }


    //String E

    if (distanceE > distance3 and distanceE <= distance4) {

      if (countE <= E_DtoDsharp) {
        digitalWrite(EdirPin, LOW);
        digitalWrite(EstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(EstepPin, LOW);
        delayMicroseconds(500);
        countE = countE + 1;
      }

      if (countE > E_DtoDsharp) {
        digitalWrite(EdirPin, HIGH);
        digitalWrite(EstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(EstepPin, LOW);
        delayMicroseconds(500);
        countE = countE - 1;
      }
    }


    //BETWEEN DISTANCE 2 AND 3

    //TUNING  e

    if (distancee > distance2 and distancee <= distance3) {
      if (counte <= e_DtoDsharp + e_Dsharptoe) { //From Dsharp to e

        digitalWrite(edirPin, HIGH);

        digitalWrite(estepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(estepPin, LOW);
        delayMicroseconds(500);
        counte = counte + 1;
      }

      if (counte > e_DtoDsharp + e_Dsharptoe) { //From F to e

        digitalWrite(edirPin, LOW);

        digitalWrite(estepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(estepPin, LOW);
        delayMicroseconds(500);
        counte = counte - 1;
      }

    }

    //TUNING  B

    if (distanceB > distance2 and distanceB <= distance3) {
      if (countB <= B_AtoAsharp + B_AsharptoB) { //From Asharp to B

        digitalWrite(BdirPin, HIGH);

        digitalWrite(BstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(BstepPin, LOW);
        delayMicroseconds(500);
        countB = countB + 1;
      }

      if (countB > B_AtoAsharp + B_AsharptoB) { //From C to B

        digitalWrite(BdirPin, LOW);

        digitalWrite(BstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(BstepPin, LOW);
        delayMicroseconds(500);
        countB = countB - 1;
      }

    }


    //TUNING  G

    if (distanceG > distance2 and distanceG <= distance3) {
      if (countG <= G_FtoFsharp + G_FsharptoG) { //Fsharp to G

        digitalWrite(GdirPin, HIGH);

        digitalWrite(GstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(GstepPin, LOW);
        delayMicroseconds(500);
        countG = countG + 1;
      }

      if (countG > G_FtoFsharp + G_FsharptoG) { //Gsharp to G

        digitalWrite(GdirPin, LOW);

        digitalWrite(GstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(GstepPin, LOW);
        delayMicroseconds(500);
        countG = countG - 1;
      }

    }


    //TUNING  D

    if (distanceD > distance2 and distanceD <= distance3) {
      if (countD <= D_CtoCsharp + D_CsharptoD) { //Csharp to D

        digitalWrite(DdirPin, LOW);

        digitalWrite(DstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(DstepPin, LOW);
        delayMicroseconds(500);
        countD = countD + 1;
      }

      if (countD > D_CtoCsharp + D_CsharptoD) { //Dsharp to D

        digitalWrite(DdirPin, HIGH);

        digitalWrite(DstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(DstepPin, LOW);
        delayMicroseconds(500);
        countD = countD - 1;
      }

    }


    //Tuning A

    if (distanceA > distance2 and distanceA <= distance3) {
      if (countA <= A_GtoGsharp + A_GsharptoA) { //Gsharp to A

        digitalWrite(AdirPin, LOW);

        digitalWrite(AstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(AstepPin, LOW);
        delayMicroseconds(500);
        countA = countA + 1;
      }

      if (countA > A_GtoGsharp + A_GsharptoA) { //Asharp to A

        digitalWrite(AdirPin, HIGH);

        digitalWrite(AstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(AstepPin, LOW);
        delayMicroseconds(500);
        countA = countA - 1;
      }

    }


    //Tuning E

    if (distanceE > distance2 and distanceE <= distance3) {
      if (countE <= E_DtoDsharp + E_DsharptoE) { //Dsharp to E

        digitalWrite(EdirPin, LOW);

        digitalWrite(EstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(EstepPin, LOW);
        delayMicroseconds(500);
        countE = countE + 1;
      }

      if (countE > E_DtoDsharp + E_DsharptoE) { //F to E

        digitalWrite(EdirPin, HIGH);

        digitalWrite(EstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(EstepPin, LOW);
        delayMicroseconds(500);
        countE = countE - 1;
      }

    }




    //BETWEEN DISTANCE 1 AND 2

    //TUNING E

    if (distanceE > distance1 and distanceE <= distance2) {
      if (countE <= E_DtoDsharp + E_DsharptoE + E_EtoF) { //From E to F
        digitalWrite(EdirPin, LOW);

        digitalWrite(EstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(EstepPin, LOW);
        delayMicroseconds(500);
        countE = countE + 1;
      }
      if (countE > E_DtoDsharp + E_DsharptoE + E_EtoF) { //From E to F
        digitalWrite(EdirPin, HIGH);

        digitalWrite(EstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(EstepPin, LOW);
        delayMicroseconds(500);
        countE = countE - 1;
      }
    }


    //TUNING A

    if (distanceA > distance1 and distanceA <= distance2) {
      if (countA <= A_GtoGsharp + A_GsharptoA + A_AtoAsharp) { //From A to Asharp
        digitalWrite(AdirPin, LOW);

        digitalWrite(AstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(AstepPin, LOW);
        delayMicroseconds(500);
        countA = countA + 1;
      }
      if (countA > A_GtoGsharp + A_GsharptoA + A_AtoAsharp) { //From B to Asharp
        digitalWrite(AdirPin, HIGH);

        digitalWrite(AstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(AstepPin, LOW);
        delayMicroseconds(500);
        countA = countA - 1;
      }
    }

    //Tuning D

    if (distanceD > distance1 and distanceD <= distance2) {
      if (countD <= D_CtoCsharp + D_CsharptoD + D_DtoDsharp) { //From D to Dsharp
        digitalWrite(DdirPin, LOW);

        digitalWrite(DstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(DstepPin, LOW);
        delayMicroseconds(500);
        countD = countD + 1;
      }
      if (countD > D_CtoCsharp + D_CsharptoD + D_DtoDsharp) { //E to Dsharp
        digitalWrite(DdirPin, HIGH);

        digitalWrite(DstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(DstepPin, LOW);
        delayMicroseconds(500);
        countD = countD - 1;
      }
    }


    //Tuning G

    if (distanceG > distance1 and distanceG <= distance2) {
      if (countG <= G_FtoFsharp + G_FsharptoG + G_GtoGsharp) { //From G to Gsharp
        digitalWrite(GdirPin, HIGH);

        digitalWrite(GstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(GstepPin, LOW);
        delayMicroseconds(500);
        countG = countG + 1;
      }
      if (countG > G_FtoFsharp + G_FsharptoG + G_GtoGsharp) { //From A to Gsharp
        digitalWrite(GdirPin, LOW);

        digitalWrite(GstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(GstepPin, LOW);
        delayMicroseconds(500);
        countG = countG - 1;
      }
    }


    //Tuning B

    if (distanceB > distance1 and distanceB <= distance2) {
      if (countB <= B_AtoAsharp + B_AsharptoB + B_BtoC) { //From B to C
        digitalWrite(BdirPin, HIGH);

        digitalWrite(BstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(BstepPin, LOW);
        delayMicroseconds(500);
        countB = countB + 1;
      }
      if (countB > B_AtoAsharp + B_AsharptoB + B_BtoC) { //From Csharp to C
        digitalWrite(BdirPin, LOW);

        digitalWrite(BstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(BstepPin, LOW);
        delayMicroseconds(500);
        countB = countB - 1;
      }
    }


    //TUNING e

    if (distancee > distance1 and distancee <= distance2) {
      if (counte <= e_DtoDsharp + e_Dsharptoe + e_etoF) { //From e to F
        digitalWrite(edirPin, HIGH);

        digitalWrite(estepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(estepPin, LOW);
        delayMicroseconds(500);
        counte = counte + 1;
      }
      if (counte > e_DtoDsharp + e_Dsharptoe + e_etoF) { //From e to F
        digitalWrite(edirPin, LOW);

        digitalWrite(estepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(estepPin, LOW);
        delayMicroseconds(500);
        counte = counte - 1;
      }
    }

    //BELOW DISTANCE 1

    //TUNING E
    if (distanceE <= distance1) {
      if (countE <= E_DtoDsharp + E_DsharptoE + E_EtoF + E_FtoFsharp) { //From F to Fsharp

        digitalWrite(EdirPin, LOW);

        digitalWrite(EstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(EstepPin, LOW);
        delayMicroseconds(500);
        countE = countE + 1;
      }
    }

    //TUNING A
    if (distanceA <= distance1) {
      if (countA <= A_GtoGsharp + A_GsharptoA + A_AtoAsharp + A_AsharptoB) { //From Asharp to B

        digitalWrite(AdirPin, LOW);

        digitalWrite(AstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(AstepPin, LOW);
        delayMicroseconds(500);
        countA = countA + 1;
      }
    }

    //TUNING D
    if (distanceD <= distance1) {
      if (countD <= D_CtoCsharp + D_CsharptoD + D_DtoDsharp + D_DsharptoE) { //From Dsharp to E

        digitalWrite(DdirPin, LOW);

        digitalWrite(DstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(DstepPin, LOW);
        delayMicroseconds(500);
        countD = countD + 1;
      }
    }

    //TUNING G
    if (distanceG <= distance1) {
      if (countG <= G_FtoFsharp + G_FsharptoG + G_GtoGsharp + G_GsharptoA) { //From Gsharp to A

        digitalWrite(GdirPin, HIGH);

        digitalWrite(GstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(GstepPin, LOW);
        delayMicroseconds(500);
        countG = countG + 1;
      }
    }


    //TUNING B
    if (distanceB <= distance1) {
      if (countB <= B_AtoAsharp + B_AsharptoB + B_BtoC + B_CtoCsharp) { //From Gsharp to A

        digitalWrite(BdirPin, HIGH);

        digitalWrite(BstepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(BstepPin, LOW);
        delayMicroseconds(500);
        countB = countB + 1;
      }
    }


    //TUNING e
    if (distancee <= distance1) {
      if (counte <= e_DtoDsharp + e_Dsharptoe + e_etoF + e_FtoFsharp) { //From F to Fsharp

        digitalWrite(edirPin, HIGH);

        digitalWrite(estepPin, HIGH);
        delayMicroseconds(500);
        digitalWrite(estepPin, LOW);
        delayMicroseconds(500);
        counte = counte + 1;
      }
    }

    //Serial.println(countE);

  }

}
