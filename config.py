#!/usr/bin/env python3
"""
Configuration Examples for PDF Snippet Generator
===============================================

This file contains various configuration examples to customize your PDF generation.
You can import and use these configurations in your own scripts.

Author: DóndeEstásCR7
"""

from generate_snippets_pdf import PDFConfig, FontSize, PageOrientation, SnippetPDFGenerator

# ===============================
# CONFIGURACIONES PREDEFINIDAS
# ===============================

def get_default_config():
    """Configuración por defecto (landscape, 2 columnas, fuente pequeña)."""
    return PDFConfig()

def get_portrait_config():
    """Configuración para orientación vertical (portrait)."""
    return PDFConfig(
        title="Mis Snippets de Programación Competitiva",
        author="Tu Nombre",
        orientation=PageOrientation.PORTRAIT,
        columns=1,
        margins="1in",
        font_size=FontSize.FOOTNOTE
    )

def get_large_font_config():
    """Configuración con fuente grande para mejor legibilidad."""
    return PDFConfig(
        title="Snippets con Fuente Grande",
        author="Tu Nombre",
        font_size=FontSize.NORMAL,
        margins="1.5in",
        code_spacing={
            "section_title": "0.5cm",
            "subsection_title": "0.3cm",
            "after_code": "0.6cm",
            "between_sections": "1cm"
        }
    )

def get_compact_config():
    """Configuración compacta para máximo contenido."""
    return PDFConfig(
        title="Snippets Compactos",
        author="Tu Nombre",
        font_size=FontSize.TINY,
        margins="0.5in",
        columns=3,
        column_separation="0.5cm",
        code_spacing={
            "section_title": "0.2cm",
            "subsection_title": "0.1cm",
            "after_code": "0.2cm",
            "between_sections": "0.4cm"
        }
    )

def get_no_line_numbers_config():
    """Configuración sin números de línea."""
    return PDFConfig(
        title="Snippets sin Números de Línea",
        author="Tu Nombre",
        line_numbers=False,
        font_size=FontSize.FOOTNOTE,
        margins="1in"
    )

def get_custom_spacing_config():
    """Configuración con espaciado personalizado."""
    return PDFConfig(
        title="Snippets con Espaciado Personalizado",
        author="Tu Nombre",
        font_size=FontSize.SCRIPT,
        code_spacing={
            "section_title": "0.8cm",      # Más espacio después del título de sección
            "subsection_title": "0.4cm",   # Más espacio después del título de subsección
            "after_code": "0.8cm",         # Más espacio después del código
            "between_sections": "1.2cm"    # Más espacio entre secciones
        }
    )

# ===============================
# FUNCIONES DE UTILIDAD
# ===============================

def generate_with_config(config: PDFConfig, output_name: str = None):
    """
    Genera un PDF con una configuración específica.
    
    Args:
        config: Configuración del PDF
        output_name: Nombre personalizado para el archivo (opcional)
    """
    try:
        # Crear generador con configuración personalizada
        generator = SnippetPDFGenerator(config=config)
        
        # Si se especifica un nombre personalizado, modificar las rutas
        if output_name:
            generator.paths.output_tex = generator.paths.project_dir / f"{output_name}.tex"
            generator.paths.output_pdf = generator.paths.project_dir / f"{output_name}.pdf"
        
        # Generar PDF
        success = generator.generate_pdf()
        
        if success:
            print(f"✅ PDF generado exitosamente con configuración personalizada")
            if output_name:
                print(f"📄 Archivo: {output_name}.pdf")
            return True
        else:
            print("❌ Error generando PDF")
            return False
            
    except Exception as e:
        print(f"❌ Error: {e}")
        return False

# ===============================
# EJEMPLOS DE USO
# ===============================

def example_basic_usage():
    """Ejemplo básico de uso."""
    print("=== Ejemplo Básico ===")
    config = get_default_config()
    generate_with_config(config, "snippets_default")

def example_portrait_usage():
    """Ejemplo con orientación vertical."""
    print("=== Ejemplo Portrait ===")
    config = get_portrait_config()
    generate_with_config(config, "snippets_portrait")

def example_large_font_usage():
    """Ejemplo con fuente grande."""
    print("=== Ejemplo Fuente Grande ===")
    config = get_large_font_config()
    generate_with_config(config, "snippets_large_font")

def example_compact_usage():
    """Ejemplo compacto."""
    print("=== Ejemplo Compacto ===")
    config = get_compact_config()
    generate_with_config(config, "snippets_compact")

def example_custom_spacing_usage():
    """Ejemplo con espaciado personalizado."""
    print("=== Ejemplo Espaciado Personalizado ===")
    config = get_custom_spacing_config()
    generate_with_config(config, "snippets_custom_spacing")

# ===============================
# CONFIGURACIONES ESPECÍFICAS
# ===============================

def get_competition_config():
    """Configuración optimizada para competencias (compacta pero legible)."""
    return PDFConfig(
        title="Competitive Programming Snippets",
        author="Competitor",
        font_size=FontSize.SCRIPT,
        margins="0.75in",
        columns=2,
        code_spacing={
            "section_title": "0.3cm",
            "subsection_title": "0.1cm",
            "after_code": "0.3cm",
            "between_sections": "0.5cm"
        }
    )

def get_study_config():
    """Configuración para estudio (legible y bien espaciado)."""
    return PDFConfig(
        title="Study Material - Competitive Programming",
        author="Student",
        orientation=PageOrientation.PORTRAIT,
        columns=1,
        font_size=FontSize.FOOTNOTE,
        margins="1in",
        code_spacing={
            "section_title": "0.5cm",
            "subsection_title": "0.2cm",
            "after_code": "0.5cm",
            "between_sections": "0.8cm"
        }
    )

def get_presentation_config():
    """Configuración para presentaciones (fuente grande, sin números de línea)."""
    return PDFConfig(
        title="Presentation - Code Snippets",
        author="Presenter",
        font_size=FontSize.NORMAL,
        line_numbers=False,
        margins="1.5in",
        columns=1,
        code_spacing={
            "section_title": "0.8cm",
            "subsection_title": "0.4cm",
            "after_code": "0.8cm",
            "between_sections": "1.2cm"
        }
    )

# ===============================
# FUNCIÓN PRINCIPAL DE EJEMPLOS
# ===============================

def run_examples():
    """Ejecuta todos los ejemplos de configuración."""
    print("🚀 Ejecutando ejemplos de configuración...\n")
    
    examples = [
        ("Básico", get_default_config, "snippets_default"),
        ("Portrait", get_portrait_config, "snippets_portrait"),
        ("Fuente Grande", get_large_font_config, "snippets_large_font"),
        ("Compacto", get_compact_config, "snippets_compact"),
        ("Espaciado Personalizado", get_custom_spacing_config, "snippets_custom_spacing"),
        ("Competencia", get_competition_config, "snippets_competition"),
        ("Estudio", get_study_config, "snippets_study"),
        ("Presentación", get_presentation_config, "snippets_presentation")
    ]
    
    for name, config_func, output_name in examples:
        print(f"📄 Generando {name}...")
        config = config_func()
        success = generate_with_config(config, output_name)
        if success:
            print(f"✅ {name} completado\n")
        else:
            print(f"❌ {name} falló\n")
    
    print("🎉 Todos los ejemplos completados!")

if __name__ == "__main__":
    # Ejecutar ejemplos si se ejecuta directamente
    run_examples()
