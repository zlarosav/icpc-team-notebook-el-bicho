import os
import subprocess
import sys
import shutil
import datetime
from pathlib import Path

# ===============================
# CONFIGURACIÓN
# ===============================
PROJECT_DIR = Path(__file__).parent
SNIPPETS_DIR = PROJECT_DIR / "Snippets"
OUTPUT_TEX = PROJECT_DIR / "Snippets.tex"
OUTPUT_PDF = PROJECT_DIR / "Snippets.pdf"
IGNOREFILE = PROJECT_DIR / ".snipignore"
TITLE = "DóndeEstásCR7"
AUTHOR = "DóndeEstásCR7"
DATE = datetime.datetime.now().strftime("%d/%m/%Y")
LATEX_CMD = "pdflatex"

# ===============================
# FUNCIONES DE UTILIDAD
# ===============================

def check_pygments():
    """Verifica si Pygments está instalado y si pygmentize está en PATH."""
    try:
        import pygments
    except ImportError:
        print("❌ No tienes Pygments instalado. Ejecuta:")
        print("   pip install Pygments")
        sys.exit(1)
    
    if not shutil.which("pygmentize"):
        print("❌ El comando 'pygmentize' no está en PATH.")
        print("   Si usas un .venv, actívalo antes de compilar:")
        print("   .venv\\Scripts\\activate")
        sys.exit(1)
    print("✅ Pygments y pygmentize listos.")

def read_ignore_patterns():
    """Lee patrones de exclusión desde .snipignore."""
    patterns = []
    if IGNOREFILE.exists():
        with open(IGNOREFILE, encoding="utf-8") as f:
            patterns = [line.strip() for line in f if line.strip() and not line.startswith("#")]
    return patterns

def collect_snippets():
    """Obtiene los snippets organizados por carpeta en orden alfabético."""
    ignore_patterns = read_ignore_patterns()
    snippets = {}

    for folder in sorted(SNIPPETS_DIR.iterdir()):
        if folder.is_dir():
            files = sorted([f for f in folder.iterdir() if f.suffix == ".cpp"])
            files = [f for f in files if not any(f.match(p) for p in ignore_patterns)]
            if files:
                snippets[folder.name] = files
    return snippets

def to_title_case(name):
    """Convierte nombre de archivo/carpeta a Título con espacios."""
    return name.replace("_", " ").replace(".cpp", "").title()

def generate_tex(snippets):
    """Genera el contenido del archivo .tex."""
    preamble = r"""
\documentclass[11pt,twoside]{article}
\usepackage[utf8]{inputenc}
\usepackage[T1]{fontenc}
\usepackage[spanish]{babel}
\usepackage{minted}
\renewcommand{\MintedPygmentize}{pygmentize}
\usepackage{hyperref}
\usepackage{titlesec}
\usepackage{geometry}
\geometry{margin=1in}
\hypersetup{colorlinks=true,linkcolor=blue}

\titleformat{\section}{\normalfont\Large\bfseries}{\thesection}{1em}{}
\titleformat{\subsection}{\normalfont\large\bfseries}{\thesubsection}{1em}{}

\title{%s}
\author{%s}
\date{%s}

\begin{document}
\maketitle
\tableofcontents
\newpage
""" % (TITLE, AUTHOR, DATE)

    body = ""
    section_num = 1
    for folder_name, files in snippets.items():
        body += f"\\section*{{{to_title_case(folder_name)}}}\n"
        body += f"\\addcontentsline{{toc}}{{section}}{{{to_title_case(folder_name)}}}\n"
        sub_num = 1
        for file_path in files:
            body += f"\\subsection*{{{section_num}.{sub_num} {to_title_case(file_path.stem)}}}\n"
            body += f"\\addcontentsline{{toc}}{{subsection}}{{{section_num}.{sub_num} {to_title_case(file_path.stem)}}}\n"
            body += "\\begin{minted}[fontsize=\\footnotesize,breaklines,tabsize=2,style=monokai]{cpp}\n"
            with open(file_path, encoding="utf-8") as f:
                body += f.read()
            body += "\n\\end{minted}\n\n"
            sub_num += 1
        section_num += 1

    ending = r"\end{document}"

    return preamble + body + ending

def compile_pdf():
    """Compila el .tex a PDF con pdflatex y -shell-escape."""
    cmd = [
        LATEX_CMD,
        "-shell-escape",
        str(OUTPUT_TEX)
    ]
    subprocess.run(cmd, check=True)

# ===============================
# MAIN
# ===============================

def main():
    check_pygments()
    snippets = collect_snippets()
    tex_content = generate_tex(snippets)

    with open(OUTPUT_TEX, "w", encoding="utf-8") as f:
        f.write(tex_content)

    compile_pdf()
    compile_pdf()  # Compilación doble para actualizar TOC
    print(f"✅ PDF generado en: {OUTPUT_PDF}")

if __name__ == "__main__":
    main()
