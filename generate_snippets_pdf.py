#!/usr/bin/env python3
"""
PDF Snippet Generator for Competitive Programming
================================================

A scalable and maintainable tool to generate PDF documents from C++ code snippets
organized in folders. Supports templates for descriptions and custom configurations.

Author: DóndeEstásCR7
Version: 2.0.0
"""

import os
import subprocess
import sys
import shutil
import datetime
import logging
from pathlib import Path
from typing import Dict, List, Optional, Any
from dataclasses import dataclass, field
from enum import Enum

# ===============================
# CONFIGURACIÓN Y CONSTANTES
# ===============================

class FontSize(Enum):
    """Tamaños de fuente disponibles para el código."""
    TINY = "\\tiny"
    SCRIPT = "\\scriptsize"
    FOOTNOTE = "\\footnotesize"
    SMALL = "\\small"
    NORMAL = "\\normalsize"

class PageOrientation(Enum):
    """Orientaciones de página disponibles."""
    PORTRAIT = "portrait"
    LANDSCAPE = "landscape"

@dataclass
class PDFConfig:
    """Configuración para la generación del PDF."""
    title: str = "Snippets 2025"
    author: str = "DóndeEstásCR7"
    date_format: str = "%d/%m/%Y"
    font_size: FontSize = FontSize.SCRIPT
    orientation: PageOrientation = PageOrientation.LANDSCAPE
    margins: str = "0.75in"
    columns: int = 2
    column_separation: str = "1cm"
    line_numbers: bool = True
    syntax_highlighting: bool = True
    frame_thickness: str = "0.3pt"
    code_spacing: Dict[str, str] = field(default_factory=lambda: {
        "section_title": "0.3cm",
        "subsection_title": "0.1cm",
        "after_code": "0.4cm",
        "between_sections": "0.6cm"
    })

@dataclass
class ProjectPaths:
    """Rutas del proyecto."""
    project_dir: Path = field(default_factory=lambda: Path(__file__).parent)
    snippets_dir: Path = field(init=False)
    output_tex: Path = field(init=False)
    output_pdf: Path = field(init=False)
    ignore_file: Path = field(init=False)
    
    def __post_init__(self):
        self.snippets_dir = self.project_dir / "Snippets"
        self.output_tex = self.project_dir / "Snippets.tex"
        self.output_pdf = self.project_dir / "Snippets.pdf"
        self.ignore_file = self.project_dir / ".snipignore"

# ===============================
# CLASES PRINCIPALES
# ===============================

class TemplateManager:
    """Gestiona la lectura y procesamiento de archivos de template."""
    
    def __init__(self, snippets_dir: Path):
        self.snippets_dir = snippets_dir
        self._template_cache: Dict[str, Dict[str, str]] = {}
    
    def read_all_templates(self) -> Dict[str, Dict[str, str]]:
        """Lee todos los archivos de template del proyecto."""
        if self._template_cache:
            return self._template_cache
            
        template_info = {}
        
        for folder in sorted(self.snippets_dir.iterdir()):
            if not folder.is_dir():
                continue
                
            folder_info = self._read_folder_templates(folder)
            if folder_info:
                template_info[folder.name] = folder_info
        
        self._template_cache = template_info
        return template_info
    
    def _read_folder_templates(self, folder: Path) -> Dict[str, str]:
        """Lee los templates de una carpeta específica."""
        folder_info = {}
        
        # Leer descripción de la carpeta
        folder_template = folder / "folder.template"
        if folder_template.exists():
            try:
                with open(folder_template, encoding="utf-8") as f:
                    folder_info["description"] = f.read().strip()
            except Exception as e:
                logging.warning(f"Error leyendo {folder_template}: {e}")
        
        # Leer descripciones de archivos individuales
        for file_path in folder.iterdir():
            if file_path.suffix == ".cpp":
                template_file = file_path.with_suffix(".template")
                if template_file.exists():
                    try:
                        with open(template_file, encoding="utf-8") as f:
                            folder_info[file_path.stem] = f.read().strip()
                    except Exception as e:
                        logging.warning(f"Error leyendo {template_file}: {e}")
        
        return folder_info

class SnippetCollector:
    """Recopila y organiza los snippets de código."""
    
    def __init__(self, snippets_dir: Path, ignore_file: Path):
        self.snippets_dir = snippets_dir
        self.ignore_file = ignore_file
    
    def collect_snippets(self) -> Dict[str, List[Path]]:
        """Obtiene los snippets organizados por carpeta en orden alfabético."""
        ignore_patterns = self._read_ignore_patterns()
        snippets = {}
        
        for folder in sorted(self.snippets_dir.iterdir()):
            if not folder.is_dir():
                continue
                
            files = self._get_cpp_files(folder, ignore_patterns)
            if files:
                snippets[folder.name] = files
        
        return snippets
    
    def _read_ignore_patterns(self) -> List[str]:
        """Lee patrones de exclusión desde .snipignore."""
        patterns = []
        if self.ignore_file.exists():
            try:
                with open(self.ignore_file, encoding="utf-8") as f:
                    patterns = [
                        line.strip() for line in f 
                        if line.strip() and not line.startswith("#")
                    ]
            except Exception as e:
                logging.warning(f"Error leyendo {self.ignore_file}: {e}")
        return patterns
    
    def _get_cpp_files(self, folder: Path, ignore_patterns: List[str]) -> List[Path]:
        """Obtiene archivos .cpp de una carpeta, aplicando patrones de exclusión."""
        files = [f for f in folder.iterdir() if f.suffix == ".cpp"]
        files = sorted(files)
        
        # Aplicar patrones de exclusión
        filtered_files = []
        for file in files:
            if not any(file.match(p) for p in ignore_patterns):
                filtered_files.append(file)
        
        return filtered_files

class CodeProcessor:
    """Procesa y limpia el código para LaTeX."""
    
    @staticmethod
    def clean_code_for_latex(code_content: str) -> str:
        """Limpia el código para compatibilidad con LaTeX."""
        # Reemplazar caracteres especiales con versiones ASCII simples
        replacements = {
            "á": "a", "é": "e", "í": "i", "ó": "o", "ú": "u",
            "ñ": "n", "ü": "u", "à": "a", "è": "e", "ì": "i",
            "ò": "o", "ù": "u", "Á": "A", "É": "E", "Í": "I",
            "Ó": "O", "Ú": "U", "Ñ": "N", "Ü": "U"
        }
        
        for special, simple in replacements.items():
            code_content = code_content.replace(special, simple)
        
        return code_content
    
    @staticmethod
    def read_code_file(file_path: Path) -> str:
        """Lee un archivo de código y lo limpia."""
        try:
            with open(file_path, encoding="utf-8") as f:
                content = f.read()
            return CodeProcessor.clean_code_for_latex(content)
        except Exception as e:
            logging.error(f"Error leyendo {file_path}: {e}")
            return f"// Error reading file: {e}"

class LaTeXGenerator:
    """Genera el contenido LaTeX para el PDF."""
    
    def __init__(self, config: PDFConfig):
        self.config = config
    
    def generate_document(self, snippets: Dict[str, List[Path]], 
                         template_info: Dict[str, Dict[str, str]]) -> str:
        """Genera el documento LaTeX completo."""
        preamble = self._generate_preamble()
        body = self._generate_body(snippets, template_info)
        ending = self._generate_ending()
        
        return preamble + body + ending
    
    def _generate_preamble(self) -> str:
        """Genera el preámbulo del documento LaTeX."""
        orientation = self.config.orientation.value
        font_size = self.config.font_size.value
        
        return f"""
\\documentclass[11pt,twoside]{{article}}
\\usepackage[utf8]{{inputenc}}
\\usepackage[T1]{{fontenc}}
\\usepackage[spanish]{{babel}}
\\usepackage{{listings}}
\\usepackage{{hyperref}}
\\usepackage{{titlesec}}
\\usepackage{{geometry}}
\\usepackage{{xcolor}}
\\usepackage{{multicol}}
\\usepackage{{parskip}}

\\geometry{{{orientation},margin={self.config.margins}}}
\\hypersetup{{colorlinks=true,linkcolor=blue}}

\\titleformat{{\\section}}{{\\normalfont\\Large\\bfseries}}{{\\thesection}}{{1em}}{{}}
\\titleformat{{\\subsection}}{{\\normalfont\\large\\bfseries}}{{\\thesubsection}}{{1em}}{{}}

% Configuración para listings
\\lstset{{
    language=C++,
    basicstyle={font_size}\\ttfamily,
    keywordstyle=\\color{{blue}}\\bfseries,
    commentstyle=\\color{{green}},
    stringstyle=\\color{{red}},
    numbers={'left' if self.config.line_numbers else 'none'},
    numberstyle=\\tiny,
    numbersep=5pt,
    breaklines=true,
    breakatwhitespace=true,
    showstringspaces=false,
    tabsize=2,
    frame=single,
    framerule={self.config.frame_thickness},
    rulecolor=\\color{{black}},
    escapechar=@,
    xleftmargin=15pt,
    xrightmargin=5pt
}}

\\title{{{self.config.title}}}
\\author{{{self.config.author}}}
\\date{{{datetime.datetime.now().strftime(self.config.date_format)}}}

\\begin{{document}}
\\maketitle
\\tableofcontents
\\newpage
\\setlength{{\\columnsep}}{{{self.config.column_separation}}}
\\begin{{multicols}}{{{self.config.columns}}}
"""
    
    def _generate_body(self, snippets: Dict[str, List[Path]], 
                      template_info: Dict[str, Dict[str, str]]) -> str:
        """Genera el cuerpo del documento."""
        body = ""
        section_num = 1
        total_sections = len(snippets)
        
        for folder_name, files in snippets.items():
            body += self._generate_section(folder_name, files, section_num, 
                                         template_info.get(folder_name, {}))
            
            # Si es la última sección, cerrar multicols
            if section_num == total_sections:
                body += f"\\vspace{{{self.config.code_spacing['between_sections']}}}\n"
                body += "\\end{multicols}\n"
            else:
                body += f"\\vspace{{{self.config.code_spacing['between_sections']}}}\n"
            
            section_num += 1
        
        return body
    
    def _generate_section(self, folder_name: str, files: List[Path], 
                         section_num: int, folder_templates: Dict[str, str]) -> str:
        """Genera una sección del documento."""
        section = ""
        section += f"\\section*{{{self._to_title_case(folder_name)}}}\n"
        section += f"\\addcontentsline{{toc}}{{section}}{{{self._to_title_case(folder_name)}}}\n"
        
        # Agregar descripción de la carpeta si existe
        if "description" in folder_templates:
            section += f"\\textit{{{folder_templates['description']}}}\n"
        
        section += f"\\vspace{{{self.config.code_spacing['section_title']}}}\n"
        
        # Generar subsections para cada archivo
        sub_num = 1
        for file_path in files:
            section += self._generate_subsection(file_path, section_num, sub_num, 
                                               folder_templates)
            sub_num += 1
        
        return section
    
    def _generate_subsection(self, file_path: Path, section_num: int, 
                           sub_num: int, folder_templates: Dict[str, str]) -> str:
        """Genera una subsección para un archivo de código."""
        subsection = ""
        subsection += f"\\subsection*{{{section_num}.{sub_num} {self._to_title_case(file_path.stem)}}}\n"
        subsection += f"\\addcontentsline{{toc}}{{subsection}}{{{section_num}.{sub_num} {self._to_title_case(file_path.stem)}}}\n"
        
        # Agregar descripción del archivo si existe
        if file_path.stem in folder_templates:
            subsection += f"\\textit{{{folder_templates[file_path.stem]}}}\n"
        
        subsection += f"\\vspace{{{self.config.code_spacing['subsection_title']}}}\n"
        subsection += "\\begin{lstlisting}\n"
        
        # Leer y procesar el código
        code_content = CodeProcessor.read_code_file(file_path)
        subsection += code_content
        
        subsection += "\n\\end{lstlisting}\n"
        subsection += f"\\vspace{{{self.config.code_spacing['after_code']}}}\n"
        
        return subsection
    
    def _generate_ending(self) -> str:
        """Genera el final del documento."""
        return r"\end{document}"
    
    @staticmethod
    def _to_title_case(name: str) -> str:
        """Convierte nombre de archivo/carpeta a Título con espacios."""
        return name.replace("_", " ").replace(".cpp", "").title()

class PDFCompiler:
    """Compila el archivo LaTeX a PDF."""
    
    def __init__(self, latex_cmd: str = "pdflatex"):
        self.latex_cmd = latex_cmd
    
    def compile_pdf(self, tex_file: Path) -> bool:
        """Compila el archivo .tex a PDF."""
        try:
            cmd = [self.latex_cmd, "-shell-escape", str(tex_file)]
            subprocess.run(cmd, check=True, capture_output=True, text=True)
            return True
        except subprocess.CalledProcessError as e:
            logging.error(f"Error compilando PDF: {e}")
            logging.error(f"Salida: {e.stdout}")
            logging.error(f"Errores: {e.stderr}")
            return False
        except FileNotFoundError:
            logging.error(f"Comando {self.latex_cmd} no encontrado. Asegúrate de tener LaTeX instalado.")
            return False

class SnippetPDFGenerator:
    """Clase principal que coordina todo el proceso de generación."""
    
    def __init__(self, config: Optional[PDFConfig] = None, 
                 paths: Optional[ProjectPaths] = None):
        self.config = config or PDFConfig()
        self.paths = paths or ProjectPaths()
        self.template_manager = TemplateManager(self.paths.snippets_dir)
        self.snippet_collector = SnippetCollector(self.paths.snippets_dir, 
                                                 self.paths.ignore_file)
        self.latex_generator = LaTeXGenerator(self.config)
        self.pdf_compiler = PDFCompiler()
        
        # Configurar logging
        self._setup_logging()
    
    def _setup_logging(self):
        """Configura el sistema de logging."""
        # Configurar logging para evitar problemas con emojis en Windows
        logging.basicConfig(
            level=logging.INFO,
            format='%(asctime)s - %(levelname)s - %(message)s',
            handlers=[
                logging.StreamHandler(sys.stdout),
                logging.FileHandler('snippet_generator.log', encoding='utf-8')
            ]
        )
    
    def generate_pdf(self) -> bool:
        """Genera el PDF completo."""
        try:
            logging.info("🚀 Iniciando generación de PDF...")
            
            # Verificar dependencias
            if not self._check_dependencies():
                return False
            
            # Recopilar snippets
            logging.info("📁 Recopilando snippets...")
            snippets = self.snippet_collector.collect_snippets()
            if not snippets:
                logging.warning("⚠️ No se encontraron snippets para procesar.")
                return False
            
            logging.info(f"✅ Encontrados {len(snippets)} carpetas con snippets")
            
            # Leer templates
            logging.info("📝 Leyendo templates...")
            template_info = self.template_manager.read_all_templates()
            
            # Generar LaTeX
            logging.info("📄 Generando contenido LaTeX...")
            tex_content = self.latex_generator.generate_document(snippets, template_info)
            
            # Escribir archivo .tex
            logging.info("💾 Escribiendo archivo .tex...")
            with open(self.paths.output_tex, "w", encoding="utf-8") as f:
                f.write(tex_content)
            
            # Compilar PDF
            logging.info("🔨 Compilando PDF...")
            if not self.pdf_compiler.compile_pdf(self.paths.output_tex):
                return False
            
            # Segunda compilación para actualizar TOC
            logging.info("🔄 Segunda compilación para actualizar índice...")
            if not self.pdf_compiler.compile_pdf(self.paths.output_tex):
                return False
            
            # Verificar que el PDF se generó
            if self.paths.output_pdf.exists():
                size_kb = self.paths.output_pdf.stat().st_size / 1024
                logging.info(f"✅ PDF generado exitosamente: {self.paths.output_pdf}")
                logging.info(f"📊 Tamaño: {size_kb:.1f} KB")
                self._print_usage_tips()
                return True
            else:
                logging.error("❌ El PDF no se generó correctamente.")
                return False
                
        except Exception as e:
            logging.error(f"❌ Error inesperado: {e}")
            return False
    
    def _check_dependencies(self) -> bool:
        """Verifica que las dependencias estén disponibles."""
        try:
            # Verificar que pdflatex esté disponible
            result = subprocess.run([self.pdf_compiler.latex_cmd, "--version"], 
                                  capture_output=True, text=True)
            if result.returncode != 0:
                logging.error(f"❌ {self.pdf_compiler.latex_cmd} no está disponible")
                return False
            
            # Verificar que el directorio de snippets existe
            if not self.paths.snippets_dir.exists():
                logging.error(f"❌ Directorio de snippets no encontrado: {self.paths.snippets_dir}")
                return False
            
            logging.info("✅ Dependencias verificadas correctamente")
            return True
            
        except FileNotFoundError:
            logging.error(f"❌ {self.pdf_compiler.latex_cmd} no encontrado en PATH")
            return False
        except Exception as e:
            logging.error(f"❌ Error verificando dependencias: {e}")
            return False
    
    def _print_usage_tips(self):
        """Imprime consejos de uso."""
        print("\n💡 Consejos de uso:")
        print("   📁 folder.template: descripción de la carpeta")
        print("   📄 archivo.template: descripción del snippet")
        print("   🚫 .snipignore: patrones para excluir archivos")
        print("   ⚙️  Modifica PDFConfig para personalizar el formato")

# ===============================
# FUNCIÓN PRINCIPAL
# ===============================

def main():
    """Función principal del programa."""
    try:
        # Crear instancia del generador
        generator = SnippetPDFGenerator()
        
        # Generar PDF
        success = generator.generate_pdf()
        
        if success:
            print("\n🎉 ¡PDF generado exitosamente!")
            sys.exit(0)
        else:
            print("\n❌ Error generando PDF. Revisa los logs.")
            sys.exit(1)
            
    except KeyboardInterrupt:
        print("\n⚠️ Proceso cancelado por el usuario.")
        sys.exit(1)
    except Exception as e:
        print(f"\n❌ Error inesperado: {e}")
        sys.exit(1)

if __name__ == "__main__":
    main()
