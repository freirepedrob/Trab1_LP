#include <LiquidCrystal.h>
#include <DHT.h>

const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define DHTPIN 6 
#define DHTTYPE DHT11 


DHT dht(DHTPIN, DHTTYPE);


const int TAMANHO_AMOSTRA = 10;

float leiturasTemperatura[TAMANHO_AMOSTRA];
float leiturasUmidade[TAMANHO_AMOSTRA];

int indiceAmostra = 0; 
const long INTERVALO_LEITURA_MS = 15L * 60L * 1000L; // 15 minutos
unsigned long tempoAnterior = 0;

// 2. FUNÇÃO INSERTION SORT

void insertionSort(float arr[], int n) {
  int i, j;
  float chave;
  
  for (i = 1; i < n; i++) {
    chave = arr[i]; 
    j = i - 1;

    while (j >= 0 && arr[j] > chave) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = chave; 
  }
}

// Função de Cálculo: Média Aparada 

float calcularMediaAparada(float arr[], int n) {
  if (n < 3) return arr[0]; 
  
  float soma = 0.0;
  for (int i = 1; i < n - 1; i++) {
    soma += arr[i];
  }
  
  return soma / (n - 2);
}


void setup() {
  Serial.begin(9600);
  Serial.println("Inicializando HDT11...");
  Serial.print("Intervalo de Leitura: ");
  Serial.print(INTERVALO_LEITURA_MS / 60000);
  Serial.println(" minutos.");
  

  lcd.begin(16, 2); 
  lcd.setCursor(0, 0);
  lcd.print("HDT11 15 min");
  
  dht.begin();
}


void loop() {
 
  unsigned long tempoAtual = millis();
  
  if (tempoAtual - tempoAnterior >= INTERVALO_LEITURA_MS) {
    tempoAnterior = tempoAtual;

    float h = dht.readHumidity();
    float t = dht.readTemperature();

    if (isnan(h) || isnan(t)) {
      Serial.println(F("Falha na leitura do sensor DHT!"));
      lcd.setCursor(0, 1);
      lcd.print("Leitura Falhou!");
      return; 
    }

    // Armazenamento
    if (indiceAmostra < TAMANHO_AMOSTRA) {
      leiturasUmidade[indiceAmostra] = h;
      leiturasTemperatura[indiceAmostra] = t;
      indiceAmostra++;
      
      Serial.print("Coletando... Amostra ");
      Serial.print(indiceAmostra);
      Serial.print(" de ");
      Serial.println(TAMANHO_AMOSTRA);

      lcd.setCursor(0, 0);
      lcd.print("T:"); lcd.print(t, 1); lcd.print(" C ");
      lcd.setCursor(0, 1);
      lcd.print("Amostra: ");
      lcd.print(indiceAmostra);
      lcd.print("/");
      lcd.print(TAMANHO_AMOSTRA);
      
    }

    // Processamento 

    if (indiceAmostra >= TAMANHO_AMOSTRA) {
      Serial.println("\n--- Amostra Completa! Processando... ---");

      // ORDENAÇÃO
      insertionSort(leiturasTemperatura, TAMANHO_AMOSTRA);
      insertionSort(leiturasUmidade, TAMANHO_AMOSTRA);

      // CÁLCULO DA MÉDIA
      float tempProcessada = calcularMediaAparada(leiturasTemperatura, TAMANHO_AMOSTRA);
      float umidProcessada = calcularMediaAparada(leiturasUmidade, TAMANHO_AMOSTRA);

      // EXIBIÇÃO SERIAL
      Serial.print("T. Aparada: ");
      Serial.print(tempProcessada, 1);
      Serial.print(" *C | U. Aparada: ");
      Serial.print(umidProcessada, 1);
      Serial.println(" %");

      // D. EXIBIÇÃO LCD
      lcd.clear(); 
      
      lcd.setCursor(0, 0); 
      lcd.print("Temp: ");
      lcd.print(tempProcessada, 1); 
      lcd.print(" C");

      lcd.setCursor(0, 1);
      lcd.print("Umid: ");
      lcd.print(umidProcessada, 1);
      lcd.print(" %");

      indiceAmostra = 0;
      Serial.println("--- Reiniciando Coleta ---");
      
      delay(300000); 
    }
  }
}