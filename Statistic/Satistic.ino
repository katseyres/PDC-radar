
float samples[] = {45.34, 47.2, 43.0, 500.21, 44.1, 45.342, 43.2, 46.0, 43.90};
int varianceError = 5;

void setup(){
    Serial.begin(9600);

    sort();

    Serial.println("Table sorted :");

    for(int i = 0; i < size(); i++){
        Serial.println(samples[i]);
        delay(10);
    }

    Serial.print("Mean : ");
    Serial.println(mean());

    Serial.print("Variance : ");
    Serial.println(variance());

    Serial.println(size());

    checkVariance();
}


void loop(){

}


float size(){
    byte numberOfElem = sizeof(samples) / sizeof(samples[0]);
    return numberOfElem;
}


void sort(){
    /*
    PROCEDURE tri_bulle(tabint)
        tab_trie EST UN BOOLEEN <- Faux
        i EST UN ENTIER
        tmp EST UN ENTIER
    DEBUT
        TANT QUE tab_trie == Faux
            tab_trie <- Vrai
            POUR i ALLANT DE 0 A taille_tab - 1 [SAUT DE 1]
                SI tabint[i] > tabint[i+1] ALORS
                    //Inverser les valeurs
                    tmp <- tabint[i+1]
                    tabint[i+1] <- tabint[i]
                    tabint[i] <- tmp
                    tab_trie <- Faux
            FIN POUR

            taille_tab <- taille_tab - 1
        FIN TANT QUE
    FIN    
    */

    bool tabSorted = false;

    while(tabSorted == false){
        tabSorted = true;

        for(int i = 0; i < size(); i++){
            if(samples[i] < samples[i + 1]){
                float tmp = samples[i + 1];
                samples[i + 1] = samples[i];
                samples[i] = tmp;
                tabSorted = false;
            }
        }
    }
}


float sum(){
    float sum = 0;

    for(int i = 0; i < size(); i++){
        sum += samples[i];
    }

    return sum;
}


float mean(){
    float mean = sum() / size();

    return mean;
}


float variance(){
    float result = 0;

    for(int i = 0; i < size(); i++){
        result += pow((samples[i] - mean()), 2);
    }

    float variance = result / size();

    return variance;
}


float checkVariance(){
    if(variance() > varianceError){
    }
    else {
        Serial.println("Ok");
    }
}