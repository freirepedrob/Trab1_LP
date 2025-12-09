#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono> // Para medir o tempo
#include <random> // Para gerar dados simulados
#include <limits>

using namespace std;

// --- Interface Abstrata para os Sensores ---
class SensorDatabase {
public:
    virtual void insert(float value) = 0;
    virtual void remove(float value) = 0;
    virtual void printSorted() = 0;
    virtual float getMin() = 0;
    virtual float getMax() = 0;
    virtual void rangeQuery(float x, float y) = 0;
    virtual float getMedian() = 0;
    virtual string getName() = 0;
    virtual ~SensorDatabase() {}
};


// IMPLEMENTAÇÃO 1: LISTA ORDENADA (INSERTION SORT)

class SortedListSensor : public SensorDatabase {
private:
    vector<float> data;

public:
    string getName() override { return "Lista Ordenada (Insertion Sort)"; }

    // Inserção ordenada (Lógica do Insertion Sort aplicada a cada inserção)
    // Complexidade: O(N)
    void insert(float value) override {
        data.push_back(value);
        int i = data.size() - 1;
        // Move o elemento para a posição correta (Insertion Sort step)
        while (i > 0 && data[i] < data[i-1]) {
            swap(data[i], data[i-1]);
            i--;
        }
    }

    // Remoção
    // Complexidade: O(N)
    void remove(float value) override {
        for (auto it = data.begin(); it != data.end(); ++it) {
            if (*it == value) {
                data.erase(it);
                return; // Remove a primeira ocorrência
            }
        }
    }

    void printSorted() override {
        for (float v : data) cout << v << " ";
        cout << endl;
    }

    float getMin() override {
        if (data.empty()) return 0.0;
        return data.front(); // O(1) pois está ordenado
    }

    float getMax() override {
        if (data.empty()) return 0.0;
        return data.back(); // O(1) pois está ordenado
    }

    void rangeQuery(float x, float y) override {
        // cout << "Valores entre " << x << " e " << y << ": ";
        for (float v : data) {
            if (v >= x && v <= y) {
                // cout << v << " "; // Comentado para não poluir o benchmark
            }
            if (v > y) break; // Otimização: para se passar do limite
        }
        // cout << endl;
    }

    float getMedian() override {
        if (data.empty()) return 0.0;
        int n = data.size();
        if (n % 2 != 0)
            return data[n / 2];
        else
            return (data[(n - 1) / 2] + data[n / 2]) / 2.0;
    }
};

// IMPLEMENTAÇÃO 2: ÁRVORE BINÁRIA DE BUSCA (BST)

struct Node {
    float key;
    Node *left, *right;
    int size; // Armazena o tamanho da subárvore para cálculo rápido da mediana

    Node(float item) : key(item), left(nullptr), right(nullptr), size(1) {}
};

class BSTSensor : public SensorDatabase {
private:
    Node* root;

    int getSize(Node* node) {
        return (node == nullptr) ? 0 : node->size;
    }

    void updateSize(Node* node) {
        if (node) node->size = 1 + getSize(node->left) + getSize(node->right);
    }

    Node* insertRec(Node* node, float key) {
        if (node == nullptr) return new Node(key);
        if (key < node->key)
            node->left = insertRec(node->left, key);
        else
            node->right = insertRec(node->right, key);
        
        updateSize(node);
        return node;
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current && current->left != nullptr)
            current = current->left;
        return current;
    }

    Node* removeRec(Node* root, float key) {
        if (root == nullptr) return root;

        if (key < root->key)
            root->left = removeRec(root->left, key);
        else if (key > root->key)
            root->right = removeRec(root->right, key);
        else {
            // Nó com apenas um filho ou sem filhos
            if (root->left == nullptr) {
                Node* temp = root->right;
                delete root;
                return temp;
            } else if (root->right == nullptr) {
                Node* temp = root->left;
                delete root;
                return temp;
            }
            // Nó com dois filhos: pega o sucessor in-order (menor da direita)
            Node* temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = removeRec(root->right, temp->key);
        }
        updateSize(root);
        return root;
    }

    void inorderRec(Node* root) {
        if (root != nullptr) {
            inorderRec(root->left);
            cout << root->key << " ";
            inorderRec(root->right);
        }
    }

    void rangeQueryRec(Node* node, float x, float y) {
        if (node == nullptr) return;
        if (x < node->key) rangeQueryRec(node->left, x, y);
        if (node->key >= x && node->key <= y) {
            // cout << node->key << " "; // Comentado para benchmark
        }
        if (y > node->key) rangeQueryRec(node->right, x, y);
    }

    // Encontra o k-ésimo menor elemento (para mediana)
    float findKth(Node* node, int k) {
        if (node == nullptr) return -1;
        int leftSize = getSize(node->left);
        
        if (leftSize == k) return node->key;
        else if (leftSize > k) return findKth(node->left, k);
        else return findKth(node->right, k - leftSize - 1);
    }

public:
    BSTSensor() : root(nullptr) {}

    string getName() override { return "Árvore Binária de Busca (BST)"; }

    void insert(float value) override {
        root = insertRec(root, value);
    }

    void remove(float value) override {
        root = removeRec(root, value);
    }

    void printSorted() override {
        inorderRec(root);
        cout << endl;
    }

    float getMin() override {
        if (root == nullptr) return 0;
        return minValueNode(root)->key;
    }

    float getMax() override {
        if (root == nullptr) return 0;
        Node* current = root;
        while (current->right != nullptr) current = current->right;
        return current->key;
    }

    void rangeQuery(float x, float y) override {
        rangeQueryRec(root, x, y);
        // cout << endl;
    }

    float getMedian() override {
        int n = getSize(root);
        if (n == 0) return 0;
        
        if (n % 2 != 0) {
            return findKth(root, n / 2);
        } else {
            float v1 = findKth(root, (n - 1) / 2);
            float v2 = findKth(root, n / 2);
            return (v1 + v2) / 2.0;
        }
    }
};

// =========================================================
// FUNÇÃO DE TESTE E BENCHMARK
// =========================================================
void runBenchmark(SensorDatabase* db, const vector<float>& dataSet) {
    cout << "\n--- Testando: " << db->getName() << " ---" << endl;
    
    auto start = chrono::high_resolution_clock::now();
    
    // 1. Inserção
    for (float v : dataSet) {
        db->insert(v);
    }
    auto endInsert = chrono::high_resolution_clock::now();
    
    // 2. Consultas
    float minVal = db->getMin();
    float maxVal = db->getMax();
    float medVal = db->getMedian();
    
    // 3. Range Query (simulada)
    db->rangeQuery(20.0, 30.0);
    
    auto endQuery = chrono::high_resolution_clock::now();

    chrono::duration<double, milli> insertTime = endInsert - start;
    chrono::duration<double, milli> queryTime = endQuery - endInsert;

    cout << "Dados processados: " << dataSet.size() << endl;
    cout << "Tempo de Inserção: " << insertTime.count() << " ms" << endl;
    cout << "Tempo de Consulta (Min/Max/Med/Range): " << queryTime.count() << " ms" << endl;
    cout << "Resultados -> Min: " << minVal << " | Max: " << maxVal << " | Mediana: " << medVal << endl;
}

int main() {
    // Geração de dados aleatórios simulando sensores
    int numSamples = 10000; // Teste com 10.000 leituras (fator de escala)
    vector<float> sensorData;
    
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(15.0, 45.0); // Temperaturas entre 15 e 45 graus

    cout << "Gerando " << numSamples << " amostras de temperatura..." << endl;
    for(int i = 0; i < numSamples; i++) {
        sensorData.push_back(dis(gen));
    }

    // Instancia as duas abordagens
    SortedListSensor lista;
    BSTSensor arvore;

    // Executa os testes
    runBenchmark(&lista, sensorData);
    runBenchmark(&arvore, sensorData);

    return 0;
}