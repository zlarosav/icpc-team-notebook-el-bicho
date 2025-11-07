# DondeEstásCR7 - Notebook ACM ICPC

Notebook de algoritmos y estructuras de datos del equipo **"Dónde estás CR7"** para competencias de programación competitiva ACM ICPC.

Este proyecto genera automáticamente un PDF a partir de snippets de código C++ organizados por categorías, optimizado para llevar impreso a competencias, con soporte para impresión en formato de cuadernillo (libro).

## 🔧 Requisitos

### Software necesario:

1. **Python 3.7+**
   - Descargar desde: https://www.python.org/downloads/

2. **MiKTeX** (Distribución LaTeX para Windows)
   - Descargar desde: https://miktex.org/download
   - Durante la instalación, seleccionar "Install missing packages on-the-fly: Yes"

3. **Librerías Python**:
   ```powershell
   pip install pypdf
   ```

## 📁 Estructura del Proyecto

```
DondeEst-sCR7/
│
├── Snippets/                    # Carpeta principal de códigos
│   ├── Algos/                   # Categoría de algoritmos
│   │   ├── Binary Search.cpp
│   │   ├── Binary Search.template  # Descripción opcional del algoritmo
│   │   └── ...
│   ├── Graph/                   # Categoría de grafos
│   │   ├── BFS.cpp
│   │   ├── DFS.cpp
│   │   ├── folder.template      # Descripción opcional de la carpeta
│   │   └── ...
│   ├── DP/                      # Programación dinámica
│   └── ...
│
├── img/                         # Imágenes (ej: portada)
│   └── cr7.jpg
│
├── generate_pdf.py              # Script principal para generar PDF
├── create_booklet.py            # Script para crear formato cuadernillo
├── config.py                    # Configuración (opcional)
├── preamble.tex                 # Preámbulo LaTeX (auto-generado)
├── requirements.txt             # Dependencias Python
│
├── Snippets.tex                 # Archivo LaTeX generado
├── Snippets.pdf                 # PDF final generado
└── Snippets_Booklet.pdf         # PDF en formato cuadernillo
```

## 🚀 Instalación

### En el PC original:

1. **Clonar o copiar el repositorio**
   ```powershell
   git clone https://github.com/zlarosav/DondeEst-sCR7.git
   cd DondeEst-sCR7
   ```

2. **Instalar dependencias Python**
   ```powershell
   pip install -r requirements.txt
   ```
   
   O manualmente:
   ```powershell
   pip install pypdf
   ```

3. **Instalar MiKTeX**
   - Descargar e instalar desde https://miktex.org/download
   - Asegurarse de que `pdflatex` esté en el PATH del sistema

### En un PC nuevo:

1. **Instalar Python 3.7+**
   - https://www.python.org/downloads/
   - Durante la instalación, marcar "Add Python to PATH"

2. **Instalar MiKTeX**
   - https://miktex.org/download
   - Configurar para instalar paquetes automáticamente

3. **Copiar el proyecto completo**
   - Copiar toda la carpeta `DondeEst-sCR7/` al nuevo PC

4. **Instalar dependencias**
   ```powershell
   cd DondeEst-sCR7
   pip install pypdf
   ```

## 💻 Uso

### 1. Agregar nuevos snippets

Para agregar algoritmos o técnicas al notebook:

1. Navega a la carpeta `Snippets/`
2. Crea una carpeta para la categoría (si no existe): `Snippets/MiCategoria/`
3. Agrega tus archivos `.cpp` con las implementaciones
4. **(Opcional)** Crea archivos `.template` para descripciones:
   - `folder.template`: Descripción de la categoría completa
   - `NombreArchivo.template`: Descripción del algoritmo (complejidad, uso, etc.)

**Ejemplo de estructura:**
```
Snippets/
└── Graph/
    ├── BFS.cpp
    ├── BFS.template          # "BFS - O(V+E) - Búsqueda en anchura"
    ├── Dijkstra.cpp
    └── folder.template       # "Algoritmos de teoría de grafos"
```

**Ejemplo de .template para algoritmo:**
```
Dijkstra - O((V+E)log V)
Encuentra el camino más corto desde un nodo fuente a todos los demás nodos.
Requiere aristas con pesos no negativos.
```

### 2. Generar el PDF normal

```powershell
python generate_pdf.py
```

Esto creará:
- `preamble.tex` (si no existe)
- `Snippets.tex` (código LaTeX generado)
- `Snippets.pdf` (PDF final en formato vertical, una columna)

**Salida esperada:**
```
✅ Encontrado pdflatex en: C:\...\pdflatex.exe
PDF generado exitosamente!
```

### 3. Generar el PDF en formato cuadernillo (para imprimir como libro)

```powershell
python create_booklet.py
```

**Salida esperada:**
```
Hoja 1: Anverso [36, 1], Reverso [2, 35]
Hoja 2: Anverso [34, 3], Reverso [4, 33]
...
✅ Cuadernillo creado exitosamente!
📄 Páginas originales: 33
📄 Páginas en cuadernillo: 36 (con 3 páginas en blanco)
📋 Hojas a imprimir: 9 (imprimir ambas caras)
💾 Guardado en: Snippets_Booklet.pdf
```

Esto creará `Snippets_Booklet.pdf` con 2 páginas por cara (4 por hoja), ordenadas para formar un libro al doblarlas.

## 🎨 Personalización

### Configurar título, autor y fecha

Edita `generate_pdf.py` en la sección `PDFConfig`:

```python
@dataclass
class PDFConfig:
    title: str = "Mi Título"
    author: str = "Mi Nombre"
    date_format: str = "%d/%m/%Y"
```

### Cambiar colores del código

En `generate_pdf.py`, dentro del método `ensure_preamble()`:

```latex
basicstyle=\ttfamily,
keywordstyle=\color{darkblue}\ttfamily,      # Palabras clave (int, for, etc)
stringstyle=\color{magenta}\ttfamily,        # Cadenas de texto
commentstyle=\color{OliveGreen}\ttfamily,    # Comentarios //
morecomment=[l][\color{Purple}]{\#},         # Directivas #define, #include
```

Colores disponibles: `darkblue`, `OliveGreen`, `Purple`, `magenta`, `RedOrange`, `cyan`, `red`, `blue`, `green`, etc.

### Ajustar márgenes

En `generate_pdf.py`, sección de márgenes:

```latex
%%% Margenes
\addtolength{\textheight}{1.5in}     # Altura del texto
\addtolength{\topmargin}{-0.75in}    # Margen superior

\addtolength{\textwidth}{2.0in}      # Ancho del texto
\addtolength{\oddsidemargin}{-1.0in} # Margen izquierdo
```

### Cambiar imagen de portada

Reemplaza `img/cr7.jpg` con tu propia imagen, o edita `generate_pdf.py`:

```python
lines.append("\\centering{\\includegraphics[width=5.5cm]{img/tu_imagen.jpg}}\\\\[0.5cm]\n")
```

## 📖 Impresión en Formato Libro

### Para llevar a competencias ACM ICPC:

El formato cuadernillo está optimizado para llevar el notebook impreso a competencias de manera compacta y fácil de consultar.

### Instrucciones de impresión:

1. **Abrir** `Snippets_Booklet.pdf`

2. **Configurar impresora:**
   - Modo: **Doble cara** (o imprimir impares, luego voltear e imprimir pares)
   - Escala: **100%** o "Tamaño real" (sin ajustar a la página)
   - Orientación: **Vertical/Portrait**

3. **Imprimir todas las páginas**

4. **Apilar las hojas** en orden (página 1 arriba, última abajo)

5. **Doblar todo el conjunto** por la mitad verticalmente

6. **Grapar en el centro** del doblez (donde se pliega)

### Resultado:
Un cuadernillo donde las páginas están en orden correcto (1, 2, 3, 4...) cuando hojeas el libro.

## 🔍 Solución de Problemas

### Error: "pdflatex no encontrado"

**Solución:**
1. Verifica que MiKTeX esté instalado
2. Agrega MiKTeX al PATH del sistema:
   - Buscar la ubicación de `pdflatex.exe` (usualmente en `C:\Program Files\MiKTeX\miktex\bin\x64\`)
   - Agregar esa ruta a las variables de entorno PATH

### Error: "pypdf no está instalado"

**Solución:**
```powershell
pip install pypdf
```

### Los caracteres especiales (á, é, í, ñ) no se ven correctamente

El script incluye sanitización automática que convierte tildes a caracteres ASCII. Si deseas mantener los caracteres especiales, edita la función `sanitize_text()` en `generate_pdf.py`.

### El PDF se ve muy estrecho o muy ancho

Ajusta los márgenes en `generate_pdf.py` (ver sección [Personalización](#personalización)).

### Faltan paquetes LaTeX al compilar

Si MiKTeX no instaló automáticamente los paquetes necesarios:
1. Abre **MiKTeX Console**
2. Ve a **Packages**
3. Busca e instala: `minted`, `listings`, `xcolor`, `fancyhdr`, `lastpage`

### El cuadernillo tiene páginas en blanco al inicio

Esto es normal. Si tu documento tiene 33 páginas, se redondea a 36 (múltiplo de 4). Las páginas 34, 35 y 36 quedan en blanco y aparecen al inicio/final del cuadernillo por diseño.

## 📝 Notas Adicionales

- **Codificación:** Los archivos `.cpp` se procesan con UTF-8, CP1252 y Latin-1 como fallback
- **Formato de código:** El resaltado de sintaxis usa el paquete LaTeX `listings` con tema personalizado
- **Páginas en blanco:** El cuadernillo agrega automáticamente páginas en blanco para completar múltiplos de 4
- **Uso en competencias:** El PDF generado cumple con las reglas típicas de ACM ICPC que permiten material impreso

## 🏅 Equipo

**"Dónde estás CR7"** - Equipo ACM ICPC
