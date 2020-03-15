
//Pin trigger/echo
int trPin[] = {7, 5, 3}; 
int ecPin[] = {6, 4, 2}; 

static int nbSensor = 3; 
static int samples = 10; //don't forget to also modify data_size to match data array !!
static int responseFactor; //wat is dat ?

unsigned long startTime; //unsigned long -> variable sur 4bits ==  precision ! et positif uniquement
float distance = 0; //initialise la distance
//const int data_size = 10;
float data[10]; //liste des données de distances

unsigned long currentTime; 
unsigned long elapsedTime; 

int current_sensor = 0; //variable pour parcourir tout les capteurs
float varianceError = 0.75; //test 0.75 seams good

void setup() {
  Serial.begin(115200); 
  Serial.println("Configuring INPUT and OUTPUT ...");
  for(int i = 0; i < nbSensor; i++){
    pinMode(trPin[i], OUTPUT); 
    pinMode(ecPin[i], INPUT); 
  }
}

void loop() {

  if(current_sensor == nbSensor){ // au dernier capteur, reset pour recommencer la boucle
    current_sensor = 0;
  }else{
    startTime = millis(); // demarrage du chrono
    
    data_samples(); //update distance data
    sort(); //sort data

    currentTime = millis();
    elapsedTime = currentTime - startTime; //temps écoulé de la boucle

    Serial.print("Looptime : ");  //Affichage des données dans le serial
    Serial.println(elapsedTime); 
    
    display_data();
    checkVariance();

    current_sensor++;
  } 
  delay(200);
}

void display_data(){
  Serial.print("Capteur ");
  Serial.print(current_sensor + 1);
  Serial.println(" : ");
  for (int i=0; i<samples; i++){ //modify samples with size(data) !!!
      Serial.print(data[i]);
      Serial.print(" ; ");
    }
  Serial.println("");
  Serial.print("Average : ");
  Serial.println(mean());
  Serial.print("Variance : ");
  Serial.println(variance());
  Serial.println("");
}
void data_samples(){
  for (int j=0; j<samples; j++){
    for (int i=0; i<nbSensor; i++){
      distance = pulseSensor(trPin[i], ecPin[i]);
      data[j] = pulseSensor(trPin[i], ecPin[i]);
      delayMicroseconds(10);
    }
  }
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

float mean(){
  float sum = 0;

  for(int i = 0; i < size(); i++){ // parcourir la liste <sampleArray> pour afficher toutes les distances de l'echantillon
    sum += data[i];
  }

  float mean = sum / size();

  return mean;

}

void sort(){
  bool tabSorted = false;

  while(tabSorted == false){
    tabSorted = true;

    for(int i = 0; i < size(); i++){
      if(data[i] < data[i + 1]){
        float tmp = data[i + 1];
          data[i + 1] = data[i];
          data[i] = tmp;
          tabSorted = false;
      }
    }
  }
}

float variance(){
    float result = 0;

    for(int i = 0; i < size(); i++){
        result += pow((data[i] - mean()), 2);
    }

    float variance = result / size();

    return variance;
}

float checkVariance(){
    if(variance() > varianceError){
      Serial.println("Modifying data, variance not acceptable !");
      float IIQ = (data[5]+data[6])/2; //intervalle interquartil
      data[0] = IIQ;
      data[9] = IIQ;
      mean();
      variance();
      display_data();
    }
    else {
        Serial.println("Variance acceptable");
    }
}

int size(){
    byte numberOfElem = sizeof(data) / sizeof(data[0]);
    return numberOfElem;
}