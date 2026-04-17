#include <chrono>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace std::chrono;

// --- Algoritmo 1: Insertion Sort O(n^2) ---
void insertionSort(vector<int> &arr) {
  int n = arr.size();
  for (int i = 1; i < n; i++) {
    int key = arr[i];
    int j = i - 1;
    while (j >= 0 && arr[j] > key) {
      arr[j + 1] = arr[j];
      j = j - 1;
    }
    arr[j + 1] = key;
  }
}

// --- Algoritmo 2: Merge Sort O(n log n) ---
void merge(vector<int> &arr, int left, int mid, int right) {
  int n1 = mid - left + 1;
  int n2 = right - mid;
  vector<int> L(n1), R(n2);

  for (int i = 0; i < n1; i++)
    L[i] = arr[left + i];
  for (int j = 0; j < n2; j++)
    R[j] = arr[mid + 1 + j];

  int i = 0, j = 0, k = left;
  while (i < n1 && j < n2) {
    if (L[i] <= R[j]) {
      arr[k] = L[i];
      i++;
    } else {
      arr[k] = R[j];
      j++;
    }
    k++;
  }
  while (i < n1) {
    arr[k] = L[i];
    i++;
    k++;
  }
  while (j < n2) {
    arr[k] = R[j];
    j++;
    k++;
  }
}

void mergeSort(vector<int> &arr, int left, int right) {
  if (left >= right)
    return;
  int mid = left + (right - left) / 2;
  mergeSort(arr, left, mid);
  mergeSort(arr, mid + 1, right);
  merge(arr, left, mid, right);
}

// --- Função Auxiliar: Gerar Vetor Aleatório ---
vector<int> gerarVetorAleatorio(int n) {
  vector<int> arr(n);
  for (int i = 0; i < n; i++) {
    arr[i] = rand() % 100000; // Números aleatórios até 100k
  }
  return arr;
}

// --- Função Principal ---
int main() {
  // Tamanhos das entradas (n) que vamos testar
  vector<int> tamanhos = {1000, 5000, 10000, 20000, 30000, 40000, 50000};
  int repeticoes = 5; // Repetir para tirar a média e minimizar efeitos externos

  // Cria e abre o arquivo CSV para salvar os resultados
  ofstream arquivo("resultados_tempos.csv");
  arquivo << "Tamanho (N),Insertion Sort (us),Merge Sort (us)\n";

  cout << "Iniciando os testes...\n";

  for (int n : tamanhos) {
    long long tempo_total_insertion = 0;
    long long tempo_total_merge = 0;

    for (int r = 0; r < repeticoes; r++) {
      // Gera um vetor base para que ambos os algoritmos ordenem exatamente os
      // mesmos números
      vector<int> vetorBase = gerarVetorAleatorio(n);

      // Teste do Insertion Sort
      vector<int> vetorInsertion = vetorBase;
      auto inicio = high_resolution_clock::now();
      insertionSort(vetorInsertion);
      auto fim = high_resolution_clock::now();
      tempo_total_insertion +=
          duration_cast<microseconds>(fim - inicio).count();

      // Teste do Merge Sort
      vector<int> vetorMerge = vetorBase;
      inicio = high_resolution_clock::now();
      mergeSort(vetorMerge, 0, vetorMerge.size() - 1);
      fim = high_resolution_clock::now();
      tempo_total_merge += duration_cast<microseconds>(fim - inicio).count();
    }

    // Calcula as médias
    long long media_insertion = tempo_total_insertion / repeticoes;
    long long media_merge = tempo_total_merge / repeticoes;

    // Salva no arquivo CSV
    arquivo << n << "," << media_insertion << "," << media_merge << "\n";

    // Mostra no console para você acompanhar
    cout << "N = " << n << " | Insertion: " << media_insertion
         << " us | Merge: " << media_merge << " us\n";
  }

  arquivo.close();
  cout << "\nTestes concluidos! Os dados foram salvos no arquivo "
          "'resultados_tempos.csv'.\n";

  return 0;
}
