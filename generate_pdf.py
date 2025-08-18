#!/usr/bin/env python3
"""
Generate PDF from C++ snippets using an external preamble (preamble.tex)
and \cppfile macro for including listings.
"""

import os
import sys
import subprocess
import datetime
import logging
import unicodedata
import re
from dataclasses import dataclass, field
from pathlib import Path
from typing import Dict, List, Optional


@dataclass
class PDFConfig:
    title: str = "El Bicho"
    author: str = "DondeEstasCR7"
    date_format: str = "%d/%m/%Y"


@dataclass
class ProjectPaths:
    project_dir: Path = field(default_factory=lambda: Path(__file__).parent)
    snippets_dir: Path = field(init=False)
    output_tex: Path = field(init=False)
    output_pdf: Path = field(init=False)
    preamble_tex: Path = field(init=False)
    build_dir: Path = field(init=False)
    sanitized_dir: Path = field(init=False)

    def __post_init__(self):
        self.snippets_dir = self.project_dir / "Snippets"
        self.output_tex = self.project_dir / "Snippets.tex"
        self.output_pdf = self.project_dir / "Snippets.pdf"
        self.preamble_tex = self.project_dir / "preamble.tex"
        self.build_dir = self.project_dir / "build"
        self.sanitized_dir = self.build_dir / "sanitized"


class SnippetCollector:
    def __init__(self, snippets_dir: Path):
        self.snippets_dir = snippets_dir

    def collect(self) -> Dict[str, List[Path]]:
        sections: Dict[str, List[Path]] = {}
        if not self.snippets_dir.exists():
            return sections
        for folder in sorted(self.snippets_dir.iterdir()):
            if not folder.is_dir():
                continue
            files = [p for p in sorted(folder.iterdir()) if p.suffix == ".cpp"]
            if files:
                sections[folder.name] = files
        return sections


class TemplateManager:
    def __init__(self, snippets_dir: Path):
        self.snippets_dir = snippets_dir

    def read_all(self) -> Dict[str, Dict[str, str]]:
        info: Dict[str, Dict[str, str]] = {}
        for folder in sorted(self.snippets_dir.iterdir()):
            if not folder.is_dir():
                continue
            folder_info: Dict[str, str] = {}
            folder_template = folder / "folder.template"
            if folder_template.exists():
                try:
                    folder_info["description"] = folder_template.read_text(encoding="utf-8", errors="replace").strip()
                except Exception:
                    folder_info["description"] = ""
            for fp in folder.iterdir():
                if fp.suffix == ".cpp":
                    t = fp.with_suffix(".template")
                    if t.exists():
                        try:
                            folder_info[fp.stem] = t.read_text(encoding="utf-8", errors="replace").strip()
                        except Exception:
                            folder_info[fp.stem] = ""
            if folder_info:
                info[folder.name] = folder_info
        return info

def sanitize_text(text: str) -> str:
    replacements = {
        "\ufeff": "",  # BOM
        # common accents
        "á": "a", "é": "e", "í": "i", "ó": "o", "ú": "u",
        "ñ": "n", "ü": "u", "à": "a", "è": "e", "ì": "i",
        "ò": "o", "ù": "u", "Á": "A", "É": "E", "Í": "I",
        "Ó": "O", "Ú": "U", "Ñ": "N", "Ü": "U",
        # quotes/dashes/ellipsis
        "’": "'", "‘": "'", "“": '"', "”": '"',
        "–": "-", "—": "-", "…": "...",
    }
    for s, r in replacements.items():
        text = text.replace(s, r)
    # strip control chars except \n, \t, \r
    text = "".join(ch for ch in text if ch in "\n\t\r" or ord(ch) >= 32)
    # remove any remaining diacritics by NFKD
    norm = unicodedata.normalize('NFKD', text)
    text = ''.join(c for c in norm if not unicodedata.combining(c))
    return text


def normalize_ascii_filename(name: str) -> str:
    # remove accents
    norm = unicodedata.normalize('NFKD', name)
    norm = ''.join(c for c in norm if not unicodedata.combining(c))
    # replace spaces with underscores
    norm = norm.replace(' ', '_')
    # allow only safe chars
    norm = re.sub(r'[^A-Za-z0-9._\-/]', '_', norm)
    return norm


class PDFCompiler:
    def __init__(self, latex_cmd: str = "pdflatex"):
        self.latex_cmd = latex_cmd
        self._latex_path = self._find_latex()

    def _find_latex(self) -> str:
        try:
            result = subprocess.run([self.latex_cmd, "--version"], capture_output=True, text=True, timeout=5)
            if result.returncode == 0:
                return self.latex_cmd
        except (subprocess.TimeoutExpired, FileNotFoundError):
            pass
        candidates = [
            r"C:\\Users\\zlaro\\AppData\\Local\\Programs\\MiKTeX\\miktex\\bin\\x64\\pdflatex.exe",
            r"C:\\Program Files\\MiKTeX\\miktex\\bin\\x64\\pdflatex.exe",
            r"C:\\Program Files (x86)\\MiKTeX\\miktex\\bin\\x64\\pdflatex.exe",
            r"C:\\Program Files\\MiKTeX\\miktex\\bin\\win32\\pdflatex.exe",
            r"C:\\Program Files (x86)\\MiKTeX\\miktex\\bin\\win32\\pdflatex.exe",
        ]
        for p in candidates:
            if Path(p).exists():
                logging.info(f"✅ Encontrado pdflatex en: {p}")
                return p
        logging.warning(f"⚠️ No se encontró pdflatex en ubicaciones conocidas, usando: {self.latex_cmd}")
        return self.latex_cmd

    def compile(self, tex_file: Path) -> bool:
        try:
            cmd = [self._latex_path, "-interaction=nonstopmode", "-shell-escape", str(tex_file)]
            subprocess.run(cmd, check=True, capture_output=True, text=True)
            return True
        except subprocess.CalledProcessError as e:
            logging.error("Error compilando PDF")
            logging.error(e.stdout)
            logging.error(e.stderr)
            return False
        except FileNotFoundError:
            logging.error(f"Comando no encontrado: {self._latex_path}")
            return False


class Generator:
    def __init__(self, config: Optional[PDFConfig] = None, paths: Optional[ProjectPaths] = None):
        self.config = config or PDFConfig()
        self.paths = paths or ProjectPaths()
        self.collector = SnippetCollector(self.paths.snippets_dir)
        self.compiler = PDFCompiler()

    def ensure_preamble(self):
        content = r"""\documentclass[10pt,landscape,twocolumn,a4paper,notitlepage]{article}
\usepackage{hyperref}
\usepackage[english, activeacute]{babel}
\usepackage[utf8]{inputenc}
\usepackage[T1]{fontenc}
\usepackage{lmodern}
\usepackage{fancyhdr}
\usepackage{lastpage}
\usepackage{listings}
\usepackage{listingsutf8}
\usepackage{listingsutf8}
\usepackage{amssymb}
\usepackage[usenames,dvipsnames]{color}
\usepackage{graphicx}
\usepackage{wrapfig}
\usepackage{amsmath}
\usepackage{makeidx}

%%% Margenes
\setlength{\columnsep}{0.25in}
\setlength{\columnseprule}{0.5pt}

\addtolength{\textheight}{2.35in}
\addtolength{\topmargin}{-0.9in}

\addtolength{\textwidth}{1.1in}
\addtolength{\oddsidemargin}{-0.55in}

\setlength{\headsep}{0.08in}
\setlength{\parskip}{0in}
\setlength{\headheight}{15pt}
\setlength{\parindent}{0mm}

%%% Encabezado y pie de pagina
\pagestyle{fancy}
\fancyhead[LO]{\textbf{\title}}
\fancyhead[C]{\leftmark\ -\ \rightmark}
\fancyhead[RO]{Page \thepage\ of \pageref{LastPage}}
\renewcommand{\headrulewidth}{0.4pt}
\fancyfoot{}
\definecolor{darkblue}{rgb}{0,0,0.4}
%%% Configuracion de Listings
\lstloadlanguages{C++}
\lstnewenvironment{code}
    {\csname lst@SetFirstLabel\endcsname}
    {\csname lst@SaveFirstLabel\endcsname}
\lstset{% general command to set parameter(s)
    language=C++, basicstyle=\small\ttfamily, keywordstyle=\slshape,
    emph=[1]{tipo,usa}, emphstyle={[1]\sffamily\bfseries},
    morekeywords={tint,forn,forsn,fore},
    basewidth={0.47em,0.40em},
    columns=fixed, fontadjust, resetmargins, xrightmargin=5pt, xleftmargin=15pt,
    flexiblecolumns=false, tabsize=2, breaklines, breakatwhitespace=false, extendedchars=true,
    inputencoding=utf8,
    numbers=left, numberstyle=\tiny, stepnumber=1, numbersep=9pt,
    frame=l, framesep=3pt,
    basicstyle=\ttfamily,
    keywordstyle=\color{darkblue}\ttfamily,
    stringstyle=\color{magenta}\ttfamily,
    commentstyle=\color{RedOrange}\ttfamily,
    morecomment=[l][\color{OliveGreen}]{\#},
    literate={á}{{\'a}}1 {é}{{\'e}}1 {í}{{\'\i}}1 {ó}{{\'o}}1 {ú}{{\'u}}1
             {Á}{{\'A}}1 {É}{{\'E}}1 {Í}{{\'I}}1 {Ó}{{\'O}}1 {Ú}{{\'U}}1
             {ñ}{{\~n}}1 {Ñ}{{\~N}}1 {ü}{{\"u}}1 {Ü}{{\"U}}1
}

\lstdefinestyle{C++}{
    language=C++, basicstyle=\small\ttfamily, keywordstyle=\slshape,
    emph=[1]{tipo,usa,tipo2}, emphstyle={[1]\sffamily\bfseries},
    morekeywords={tint,forn,forsn,fore},
    basewidth={0.47em,0.40em},
    columns=fixed, fontadjust, resetmargins, xrightmargin=5pt, xleftmargin=15pt,
    flexiblecolumns=false, tabsize=2, breaklines, breakatwhitespace=false, extendedchars=true,
    numbers=left, numberstyle=\tiny, stepnumber=1, numbersep=9pt,
    frame=l, framesep=3pt,
    basicstyle=\ttfamily,
    keywordstyle=\color{darkblue}\ttfamily,
    stringstyle=\color{magenta}\ttfamily,
    commentstyle=\color{RedOrange}\ttfamily,
    morecomment=[l][\color{OliveGreen}]{\#}
}

%%% Macros
\def\nbtitle#1{\begin{Large}\begin{center}\textbf{#1}\end{center}\end{Large}}
\def\nbsection#1{\section{#1}}
\def\nbsubsection#1{\subsection{#1}}
\def\nbcoment#1{\begin{small}\textbf{#1}\end{small}}
\newcommand{\comb}[2]{\left( \begin{array}{c} #1 \\ #2 \end{array}\right)}
\def\complexity#1{\texorpdfstring{$\mathcal{O}(#1)$}{O(#1)}}
\newcommand\cppfile[2][]{
    \lstinputlisting[style=C++,#1]{\detokenize{#2}}
}
"""
        self.paths.preamble_tex.write_text(content, encoding="utf-8")

    def _to_title_case(self, name: str) -> str:
        return name.replace("_", " ").replace(".cpp", "").title()

    def _original_path(self, src: Path) -> Path:
        return src

    def build_tex(self) -> str:
        self.ensure_preamble()
        sections = self.collector.collect()
        templates = TemplateManager(self.paths.snippets_dir).read_all()
        today = datetime.datetime.now().strftime(self.config.date_format)
        lines: List[str] = []
        lines.append("% Generated by generate_pdf.py\n")
        lines.append("\\input{preamble.tex}\n")
        lines.append("\\begin{document}\n")
        lines.append(f"\\def\\title{{{self.config.title}}}\n")
        # Portada: Título, Autor, Imagen, Fecha (en ese orden) y en una sola página
        lines.append("\\vspace{0.6cm}\n")
        lines.append(f"\\centering{{\\LARGE\\textbf{{{self.config.title}}}}}\\\\[0.5cm]\n")
        lines.append(f"\\centering{{{self.config.author}}}\\\\[0.5cm]\n")
        lines.append("\\centering{\\includegraphics[width=5.5cm]{img/cr7.jpg}}\\\\[0.5cm]\n")
        lines.append(f"\\centering{{{today}}}\\\\[0.2cm]\n")
        # Índice inmediatamente después de la portada (misma página)
        lines.append("\\tableofcontents\n")
        lines.append("\\newpage\n\n")

        for folder, files in sections.items():
            lines.append(f"\\section{{{self._to_title_case(folder)}}}\n")
            # descripción de carpeta
            folder_desc = templates.get(folder, {}).get("description", "").strip()
            if folder_desc:
                lines.append(f"\\textit{{{folder_desc}}}\\\n")
            for f in files:
                title = self._to_title_case(f.stem)
                lines.append(f"\\subsection{{{title}}}\n")
                # descripción de archivo
                file_desc = templates.get(folder, {}).get(f.stem, "").strip()
                if file_desc:
                    lines.append(f"\\textit{{{file_desc}}}\\\n")
                # Siempre crear copia saneada sin tildes/ñ
                tmp_dir = (self.paths.build_dir / "sanitized_include")
                tmp_dir.mkdir(parents=True, exist_ok=True)
                out_path = tmp_dir / f.name
                try:
                    raw = f.read_bytes()
                    try:
                        code = raw.decode('utf-8')
                    except UnicodeDecodeError:
                        try:
                            code = raw.decode('cp1252')
                        except UnicodeDecodeError:
                            code = raw.decode('latin-1', errors='replace')
                    code = sanitize_text(code)
                    out_path.write_text(code, encoding='utf-8', newline='\n')
                except Exception as e:
                    # Fallback: referencia original si algo falla
                    out_path = f
                rel_path = out_path.relative_to(self.paths.project_dir).as_posix()
                lines.append(f"\\cppfile{{{rel_path}}}\n")
            lines.append("\n")

        lines.append("\\end{document}\n")
        return "".join(lines)

    def write_tex(self, content: str):
        self.paths.output_tex.write_text(content, encoding="utf-8")

    def generate(self) -> bool:
        try:
            logging.basicConfig(level=logging.INFO, format='%(asctime)s - %(levelname)s - %(message)s')
            if not self.paths.snippets_dir.exists():
                logging.error(f"Directorio de snippets no encontrado: {self.paths.snippets_dir}")
                return False
            tex = self.build_tex()
            self.write_tex(tex)
            ok = self.compiler.compile(self.paths.output_tex)
            if not ok:
                return False
            # second pass for TOC
            ok = self.compiler.compile(self.paths.output_tex)
            return ok and self.paths.output_pdf.exists()
        except Exception as e:
            logging.error(f"Error inesperado: {e}")
            return False


def main():
    gen = Generator()
    if gen.generate():
        try:
            print("\nPDF generado exitosamente!")
        except Exception:
            sys.stdout.write("\nPDF generado exitosamente!\n")
        sys.exit(0)
    try:
        print("\nError generando PDF. Revisa los logs.")
    except Exception:
        sys.stdout.write("\nError generando PDF. Revisa los logs.\n")
    sys.exit(1)


if __name__ == "__main__":
    main()


