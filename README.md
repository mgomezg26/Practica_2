# Práctica II: Codificación Huffman Canónica en C++

**Video de Sustentación:** [Ver video aquí](https://youtu.be/LINK_DEL_VIDEO)

## Equipo de Desarrollo

- **Daniel Muñeton** 
- **Mateo Gómez** 

## Descripción del Proyecto

Este proyecto implementa un sistema de compresión de texto utilizando la codificación Huffman canónica en C++. El programa recibe una cadena de texto (mínimo 30 caracteres), construye el árbol de Huffman basándose en las frecuencias de los caracteres, genera códigos canónicos optimizados, comprime el texto y calcula el ratio de compresión. La implementación incluye visualización del árbol, tablas de códigos y análisis detallado del proceso de compresión.

## Compilación y Ejecución

### Requisitos
- Compilador g++ con soporte para C++17
- Sistema operativo: Linux o Windows

### Compilar el programa principal
```bash
g++ -std=c++17 huffman.cpp -o huffman
```

### Ejecutar
```bash
./huffman
```

### Ejemplo de entrada/salida
```
Ingrese el texto (mínimo 30 caracteres): este es un ejemplo de compresion huffman canonica

=== TABLA DE FRECUENCIAS ===
' ': 6
'a': 4
'c': 3
'e': 5
...

=== RESULTADOS DE COMPRESIÓN ===
Tamaño original: 480 bits (60 chars x 8)
Tamaño comprimido: 312 bits
Ratio: 0.6500
Reducción: 35.00%
```

## Explicación de Huffman Canónico

La codificación Huffman canónica es una variante optimizada del algoritmo de Huffman que genera códigos únicos y ordenados. El proceso consiste en: primero, construir el árbol de Huffman tradicional y obtener las longitudes de código para cada símbolo. Luego, se ordenan los símbolos por longitud de código (ascendente) y, en caso de empate, por valor ASCII del símbolo (ascendente). Finalmente, se asignan códigos de manera incremental: el primer símbolo de longitud L recibe el código con L bits en cero, y cada símbolo subsecuente recibe el siguiente número binario, ajustando la longitud según sea necesario mediante desplazamientos de bits. Esta técnica garantiza que códigos más cortos sean lexicográficamente menores, facilitando la transmisión eficiente al solo necesitar enviar las longitudes de código en lugar del árbol completo.

## Decisiones de Diseño

### Criterio de desempate
Cuando dos nodos tienen la misma frecuencia durante la construcción del árbol, se utiliza el **símbolo ASCII menor** como criterio de desempate. Esto garantiza reproducibilidad y consistencia en la construcción del árbol.

### Caso especial
El programa maneja correctamente el caso de un **único símbolo distinto** asignándole longitud de código 1.

## 📊 Resultados de Pruebas

| Caso de Prueba | Tamaño Original (bits) | Tamaño Comprimido (bits) | Ratio | Reducción (%) |
|----------------|------------------------|--------------------------|-------|---------------|
| **Caso 1:** Muchos espacios | 488 | 298 | 0.6107 | 38.93% |
| **Caso 2:** Pocos símbolos distintos | 336 | 126 | 0.3750 | 62.50% |
| **Caso 3:** Distribución uniforme | 408 | 292 | 0.7157 | 28.43% |

### Análisis
- **Caso 1**: Alta frecuencia de espacios permite códigos cortos para este símbolo, mejorando la compresión.
- **Caso 2**: Pocos símbolos distintos con frecuencias similares logra la mejor compresión (62.5%).
- **Caso 3**: Distribución uniforme resulta en menor compresión, ya que todos los símbolos requieren códigos de longitud similar.

## 🗂️ Estructura del Repositorio


## Video de Sustentación

**Link:** [https://youtu.be/ZgCt3qlvYBs?feature=shared](https://youtu.be/ZgCt3qlvYBs?feature=shared)

El video incluye:
- Presentación del equipo
- Explicación teórica de Huffman y Huffman canónico
- Demostración en vivo del programa
- Análisis de complejidad y casos especiales
- Conclusiones sobre la utilidad del algoritmo

## Referencias

- Huffman, D. A. (1952). "A Method for the Construction of Minimum-Redundancy Codes"
