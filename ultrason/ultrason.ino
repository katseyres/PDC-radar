/*
  Script des capteurs a ultrason
    > Ce script envoie un signal a partir du Pin Trigger du premier capteur et lit le signal recu via Echo coucou
*/

int trPin[] = {7, 5, 3}; // position des trigger sur la board <Pin>
int ecPin[] = {6, 4, 2}; // position des echo sur la board <Pin>

static int nbSensor = 3; // choix du nombre de capteurs utilises
static int samples = 20; // choix du nombre d'echantillons a etudier
static int responseFactor;

unsigned long startTime; // variable pour le lancer le chrono ('unsigned long -> variable sur 4bits ==  precision ! et positif uniquement)
float distance = 0; // variable pour la distance de depart

unsigned long currentTime; // variable pour determiner un second temps
unsigned long elapsedTime; // variable pour determiner le temps d'execution de l'echantillonage


int i = 0; // entier necessaire pour une boucle dans loop()

void setup() {
  Serial.begin(115200); // utilise pour l'Arduino Mega

  for(int i=0; i < nbSensor; i++){
    pinMode(trPin[i], OUTPUT); // definit les Trigger sur OUTPUT
    pinMode(ecPin[i], INPUT); // definit les Echo sur INPUT
  }
}

void loop() {

  if(i == nbSensor){ // au dernier capteur, reset de i pour recommencer la boucle
    i = 0;
    Serial.println("");
  }else{
    startTime = millis(); // demarrage du chrono
    
    float sampleArray[samples];
    for (int j=0; j < samples; j++){
      distance = pulseSensor(trPin[i], ecPin[i]);
      sampleArray[j] = pulseSensor(trPin[i], ecPin[i]);
      delayMicroseconds(10);
    }
    //distance /= samples;

    currentTime = millis();
    elapsedTime = currentTime - startTime;

    Serial.print("Looptime : ");
    Serial.println(elapsedTime); // affiche le temps d'execution de l'echantillonage
  
    Serial.print("Capteur ");
    Serial.print(i + 1);
    Serial.println(" : ");

    float* mean = average(sampleArray);
    Serial.println(mean);
    
    Serial.print("      ");
    i++;
  }
  delay(200);
}


float pulseSensor(int trigger, int echo){
  digitalWrite(trigger, LOW);
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);

  float duration = pulseIn(echo, HIGH);
  float distance = duration * 0.034 / 2;
  
  return distance;
}


float average(float sampleArray){
  float sum = 0;

  for(int j = 0; j < samples; j++){ // parcourir la liste <sampleArray> pour afficher toutes les distances de l'echantillon
    sum += sampleArray[j];
  }

  float mean = sum / samples;

  return mean;

}