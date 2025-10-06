#include <iostream>
#include <queue>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <string>
#include <bitset>
#include <iomanip>

using namespace std;

// Nodo del árbol de Huffman
struct HuffmanNode {
    char symbol;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;

    HuffmanNode(char s, int f) : symbol(s), frequency(f), left(nullptr), right(nullptr) {}
    HuffmanNode(int f, HuffmanNode* l, HuffmanNode* r) : symbol('\0'), frequency(f), left(l), right(r) {}
};

// Comparador para priority_queue (min-heap)
struct Compare {
    bool operator()(HuffmanNode* a, HuffmanNode* b) {
        if (a->frequency != b->frequency) {
            return a->frequency > b->frequency;
        }
        // Desempate: nodo con símbolo menor tiene prioridad
        char minA = getMinSymbol(a);
        char minB = getMinSymbol(b);
        return minA > minB;
    }

    static char getMinSymbol(HuffmanNode* node) {
        if (node->left == nullptr && node->right == nullptr) {
            return node->symbol;
        }
        char minSym = 127;
        findMinSymbol(node, minSym);
        return minSym;
    }

    static void findMinSymbol(HuffmanNode* node, char& minSym) {
        if (node == nullptr) return;
        if (node->left == nullptr && node->right == nullptr) {
            if (node->symbol < minSym) minSym = node->symbol;
            return;
        }
        findMinSymbol(node->left, minSym);
        findMinSymbol(node->right, minSym);
    }
};

// Calcular frecuencias
unordered_map<char, int> calculateFrequencies(const string& text) {
    unordered_map<char, int> freq;
    for (char c : text) {
        freq[c]++;
    }
    return freq;
}

// Construir árbol de Huffman
HuffmanNode* buildHuffmanTree(unordered_map<char, int>& frequencies) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, Compare> pq;

    // Crear nodos hoja
    for (auto& pair : frequencies) {
        pq.push(new HuffmanNode(pair.first, pair.second));
    }

    // Caso especial: un solo símbolo
    if (pq.size() == 1) {
        HuffmanNode* single = pq.top();
        pq.pop();
        return new HuffmanNode(single->frequency, single, nullptr);
    }

    // Combinar nodos
    while (pq.size() > 1) {
        HuffmanNode* left = pq.top(); pq.pop();
        HuffmanNode* right = pq.top(); pq.pop();

        HuffmanNode* parent = new HuffmanNode(left->frequency + right->frequency, left, right);
        pq.push(parent);
    }

    return pq.top();
}

// Obtener longitudes de código (profundidad en el árbol)
void getCodeLengths(HuffmanNode* root, unordered_map<char, int>& lengths, int depth = 0) {
    if (root == nullptr) return;

    if (root->left == nullptr && root->right == nullptr) {
        lengths[root->symbol] = (depth == 0) ? 1 : depth; // Caso especial de 1 símbolo
        return;
    }

    getCodeLengths(root->left, lengths, depth + 1);
    getCodeLengths(root->right, lengths, depth + 1);
}

// Generar códigos no canónicos del árbol
void generateTreeCodes(HuffmanNode* root, unordered_map<char, string>& codes, string code = "") {
    if (root == nullptr) return;

    if (root->left == nullptr && root->right == nullptr) {
        codes[root->symbol] = code.empty() ? "0" : code; // Caso especial
        return;
    }

    generateTreeCodes(root->left, codes, code + "0");
    generateTreeCodes(root->right, codes, code + "1");
}

// Generar códigos canónicos
unordered_map<char, string> generateCanonicalCodes(unordered_map<char, int>& lengths) {
    // Crear lista de (símbolo, longitud)
    vector<pair<char, int>> symbolLengths;
    for (auto& pair : lengths) {
        symbolLengths.push_back({pair.first, pair.second});
    }

    // Ordenar por longitud y luego por símbolo
    sort(symbolLengths.begin(), symbolLengths.end(), [](auto& a, auto& b) {
        if (a.second != b.second) return a.second < b.second;
        return a.first < b.first;
    });

    unordered_map<char, string> canonicalCodes;
    int code = 0;
    int prevLength = 0;

    for (auto& pair : symbolLengths) {
        char symbol = pair.first;
        int length = pair.second;

        // Ajustar el código si la longitud cambió
        if (prevLength != 0 && length > prevLength) {
            code <<= (length - prevLength);
        }

        // Convertir a string binario
        string binaryCode = "";
        for (int i = length - 1; i >= 0; i--) {
            binaryCode += ((code >> i) & 1) ? '1' : '0';
        }

        canonicalCodes[symbol] = binaryCode;
        code++;
        prevLength = length;
    }

    return canonicalCodes;
}

// Comprimir texto
string compressText(const string& text, unordered_map<char, string>& codes) {
    string compressed = "";
    for (char c : text) {
        compressed += codes[c];
    }
    return compressed;
}

// Mostrar árbol en formato textual (preorden)
void displayTree(HuffmanNode* root, string prefix = "", bool isLeft = true) {
    if (root == nullptr) return;

    cout << prefix;
    cout << (prefix.empty() ? "Root: " : (isLeft ? "+--L: " : "+--R: "));

    if (root->left == nullptr && root->right == nullptr) {
        cout << "'" << (root->symbol == ' ' ? "SPACE" : string(1, root->symbol))
             << "' (freq: " << root->frequency << ")" << endl;
    } else {
        cout << "[" << root->frequency << "]" << endl;
    }

    if (root->left || root->right) {
        if (root->left) {
            displayTree(root->left, prefix + (isLeft ? "|  " : "   "), true);
        }
        if (root->right) {
            displayTree(root->right, prefix + (isLeft ? "|  " : "   "), false);
        }
    }
}

// Convertir bits a hexadecimal
string bitsToHex(const string& bits) {
    string hex = "";
    for (size_t i = 0; i < bits.length(); i += 4) {
        string nibble = bits.substr(i, min(size_t(4), bits.length() - i));
        while (nibble.length() < 4) nibble += "0"; // Padding

        int value = stoi(nibble, nullptr, 2);
        hex += "0123456789ABCDEF"[value];
    }
    return hex;
}

int main() {
    cout << "=== CODIFICACION HUFFMAN CANONICA ===" << endl << endl;

    // Entrada
    string text;
    cout << "Ingrese el texto (minimo 30 caracteres): ";
    getline(cin, text);

    if (text.length() < 30) {
        cerr << "Error: El texto debe tener al menos 30 caracteres." << endl;
        return 1;
    }

    cout << "\nTexto ingresado: \"" << text << "\"" << endl;
    cout << "Longitud: " << text.length() << " caracteres" << endl << endl;

    // 1. Calcular frecuencias
    auto frequencies = calculateFrequencies(text);

    cout << "=== TABLA DE FRECUENCIAS ===" << endl;
    vector<pair<char, int>> sortedFreq(frequencies.begin(), frequencies.end());
    sort(sortedFreq.begin(), sortedFreq.end());

    for (auto& p : sortedFreq) {
        cout << "'" << (p.first == ' ' ? "SPACE" : string(1, p.first))
             << "': " << p.second << endl;
    }
    cout << endl;

    // 2. Construir árbol de Huffman
    HuffmanNode* root = buildHuffmanTree(frequencies);

    cout << "=== ARBOL DE HUFFMAN ===" << endl;
    displayTree(root);
    cout << endl;

    // 3. Obtener longitudes de código
    unordered_map<char, int> codeLengths;
    getCodeLengths(root, codeLengths);

    // 4. Generar códigos no canónicos
    unordered_map<char, string> treeCodes;
    generateTreeCodes(root, treeCodes);

    // 5. Generar códigos canónicos
    auto canonicalCodes = generateCanonicalCodes(codeLengths);

    // 6. Mostrar tabla de códigos
    cout << "=== TABLA DE CODIGOS ===" << endl;
    cout << left << setw(10) << "Simbolo" << setw(8) << "Freq"
         << setw(10) << "Longitud" << setw(15) << "Arbol (Huff)"
         << setw(15) << "Canonico" << endl;
    cout << string(60, '-') << endl;

    vector<pair<char, int>> sortedSymbols(codeLengths.begin(), codeLengths.end());
    sort(sortedSymbols.begin(), sortedSymbols.end(), [&](auto& a, auto& b) {
        if (a.second != b.second) return a.second < b.second;
        return a.first < b.first;
    });

    for (auto& p : sortedSymbols) {
        char sym = p.first;
        cout << left << setw(10) << (sym == ' ' ? "'SPACE'" : "'" + string(1, sym) + "'")
             << setw(8) << frequencies[sym]
             << setw(10) << codeLengths[sym]
             << setw(15) << treeCodes[sym]
             << setw(15) << canonicalCodes[sym] << endl;
    }
    cout << endl;

    // 7. Comprimir texto
    string compressed = compressText(text, canonicalCodes);

    // 8. Calcular ratio de compresión
    int originalSize = text.length() * 8;
    int compressedSize = compressed.length();
    double ratio = (double)compressedSize / originalSize;
    double reduction = (1.0 - ratio) * 100;

    cout << "=== RESULTADOS DE COMPRESION ===" << endl;
    cout << "Tamanio original: " << originalSize << " bits ("
         << text.length() << " chars x 8)" << endl;
    cout << "Tamanio comprimido: " << compressedSize << " bits" << endl;
    cout << "Ratio: " << fixed << setprecision(4) << ratio << endl;
    cout << "Reduccion: " << fixed << setprecision(2) << reduction << "%" << endl << endl;

    // 9. Mostrar primeros 128 bits
    cout << "=== SALIDA COMPRIMIDA ===" << endl;
    cout << "Primeros 128 bits:" << endl;
    cout << compressed.substr(0, min(size_t(128), compressed.length())) << endl << endl;

    cout << "Representacion hexadecimal (primeros 32 nibbles):" << endl;
    string hex = bitsToHex(compressed);
    cout << hex.substr(0, min(size_t(32), hex.length())) << endl;

    return 0;
}