#!/usr/bin/env python3
"""
Example Usage of PDF Snippet Generator
=====================================

This file demonstrates how to use the optimized PDF generator with different configurations.

Author: DóndeEstásCR7
"""

from generate_snippets_pdf import SnippetPDFGenerator, PDFConfig, FontSize, PageOrientation

def example_1_basic_usage():
    """Ejemplo 1: Uso básico con configuración por defecto."""
    print("=== Ejemplo 1: Uso Básico ===")
    
    # Usar configuración por defecto
    generator = SnippetPDFGenerator()
    success = generator.generate_pdf()
    
    if success:
        print("✅ PDF generado con configuración por defecto")
    else:
        print("❌ Error generando PDF")

def example_2_portrait_layout():
    """Ejemplo 2: Layout vertical (portrait) con una columna."""
    print("\n=== Ejemplo 2: Layout Portrait ===")
    
    # Configuración personalizada para orientación vertical
    config = PDFConfig(
        title="Mis Snippets - Layout Vertical",
        author="Tu Nombre",
        orientation=PageOrientation.PORTRAIT,
        columns=1,
        margins="1in",
        font_size=FontSize.FOOTNOTE
    )
    
    generator = SnippetPDFGenerator(config=config)
    success = generator.generate_pdf()
    
    if success:
        print("✅ PDF generado con layout portrait")
    else:
        print("❌ Error generando PDF")

def example_3_large_font():
    """Ejemplo 3: Fuente grande para mejor legibilidad."""
    print("\n=== Ejemplo 3: Fuente Grande ===")
    
    config = PDFConfig(
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
    
    generator = SnippetPDFGenerator(config=config)
    success = generator.generate_pdf()
    
    if success:
        print("✅ PDF generado con fuente grande")
    else:
        print("❌ Error generando PDF")

def example_4_compact_layout():
    """Ejemplo 4: Layout compacto para máximo contenido."""
    print("\n=== Ejemplo 4: Layout Compacto ===")
    
    config = PDFConfig(
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
    
    generator = SnippetPDFGenerator(config=config)
    success = generator.generate_pdf()
    
    if success:
        print("✅ PDF generado con layout compacto")
    else:
        print("❌ Error generando PDF")

def example_5_no_line_numbers():
    """Ejemplo 5: Sin números de línea."""
    print("\n=== Ejemplo 5: Sin Números de Línea ===")
    
    config = PDFConfig(
        title="Snippets sin Números de Línea",
        author="Tu Nombre",
        line_numbers=False,
        font_size=FontSize.FOOTNOTE,
        margins="1in"
    )
    
    generator = SnippetPDFGenerator(config=config)
    success = generator.generate_pdf()
    
    if success:
        print("✅ PDF generado sin números de línea")
    else:
        print("❌ Error generando PDF")

def example_6_competition_optimized():
    """Ejemplo 6: Optimizado para competencias."""
    print("\n=== Ejemplo 6: Optimizado para Competencias ===")
    
    config = PDFConfig(
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
    
    generator = SnippetPDFGenerator(config=config)
    success = generator.generate_pdf()
    
    if success:
        print("✅ PDF optimizado para competencias generado")
    else:
        print("❌ Error generando PDF")

def example_7_study_material():
    """Ejemplo 7: Configuración para material de estudio."""
    print("\n=== Ejemplo 7: Material de Estudio ===")
    
    config = PDFConfig(
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
    
    generator = SnippetPDFGenerator(config=config)
    success = generator.generate_pdf()
    
    if success:
        print("✅ PDF para material de estudio generado")
    else:
        print("❌ Error generando PDF")

def run_all_examples():
    """Ejecuta todos los ejemplos."""
    print("🚀 Ejecutando ejemplos de uso del PDF Generator...\n")
    
    examples = [
        example_1_basic_usage,
        example_2_portrait_layout,
        example_3_large_font,
        example_4_compact_layout,
        example_5_no_line_numbers,
        example_6_competition_optimized,
        example_7_study_material
    ]
    
    for i, example_func in enumerate(examples, 1):
        try:
            example_func()
            print(f"✅ Ejemplo {i} completado\n")
        except Exception as e:
            print(f"❌ Ejemplo {i} falló: {e}\n")
    
    print("🎉 Todos los ejemplos completados!")
    print("\n📋 Resumen de archivos generados:")
    print("   - Snippets.pdf (configuración por defecto)")
    print("   - Revisa el directorio para ver todos los archivos generados")

if __name__ == "__main__":
    # Ejecutar todos los ejemplos
    run_all_examples()
