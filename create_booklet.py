#!/usr/bin/env python3
"""
Convierte un PDF normal en formato de cuadernillo para impresión.
Imprime 2 páginas por cara (4 por hoja), ordenadas para crear un libro
al apilar y doblar las hojas.
"""

import sys
from pathlib import Path

# Intentar importar desde pypdf primero (versión moderna)
try:
    from pypdf import PdfReader, PdfWriter, PageObject, Transformation
except ImportError:
    # Si falla, intentar PyPDF2 (versión antigua)
    try:
        from PyPDF2 import PdfReader, PdfWriter, PageObject
        from PyPDF2.generic import Transformation
    except ImportError:
        print("Error: pypdf o PyPDF2 no están instalados.")
        print("Instala con uno de estos comandos:")
        print("  pip install pypdf")
        print("  pip install PyPDF2")
        sys.exit(1)


def create_booklet(input_pdf: Path, output_pdf: Path):
    """
    Crea un PDF en formato cuadernillo desde un PDF normal.
    
    Args:
        input_pdf: Ruta al PDF de entrada
        output_pdf: Ruta al PDF de salida en formato cuadernillo
    """
    reader = PdfReader(str(input_pdf))
    writer = PdfWriter()
    
    num_pages = len(reader.pages)
    
    # Redondear al múltiplo de 4 más cercano (necesario para cuadernillos)
    pages_needed = ((num_pages + 3) // 4) * 4
    
    # Obtener dimensiones de la página original
    page_width = float(reader.pages[0].mediabox.width)
    page_height = float(reader.pages[0].mediabox.height)
    
    # Dimensiones de la hoja final (2 páginas lado a lado)
    sheet_width = page_width * 2
    sheet_height = page_height
    
    # Calcular el orden de páginas para cuadernillo
    num_sheets = pages_needed // 4  # Número de hojas físicas
    
    for sheet in range(num_sheets):
        # Cara frontal (anverso): páginas exteriores
        front_left_idx = pages_needed - 1 - (sheet * 2)
        front_right_idx = sheet * 2
        
        # Cara trasera (reverso): páginas interiores
        back_left_idx = sheet * 2 + 1
        back_right_idx = pages_needed - 2 - (sheet * 2)
        
        print(f"Hoja {sheet + 1}: Anverso [{front_left_idx + 1}, {front_right_idx + 1}], Reverso [{back_left_idx + 1}, {back_right_idx + 1}]")
        
        # ==== CARA FRONTAL (ANVERSO) ====
        front_page = PageObject.create_blank_page(width=sheet_width, height=sheet_height)
        
        # Página izquierda del anverso
        if front_left_idx < num_pages:
            left_content = reader.pages[front_left_idx]
            # Posicionar en la izquierda (x=0)
            front_page.merge_translated_page(left_content, tx=0, ty=0)
        
        # Página derecha del anverso
        if front_right_idx < num_pages:
            right_content = reader.pages[front_right_idx]
            # Posicionar en la derecha (x=page_width)
            front_page.merge_translated_page(right_content, tx=page_width, ty=0)
        
        writer.add_page(front_page)
        
        # ==== CARA TRASERA (REVERSO) ====
        back_page = PageObject.create_blank_page(width=sheet_width, height=sheet_height)
        
        # Página izquierda del reverso
        if back_left_idx < num_pages:
            left_content_back = reader.pages[back_left_idx]
            # Posicionar en la izquierda (x=0)
            back_page.merge_translated_page(left_content_back, tx=0, ty=0)
        
        # Página derecha del reverso
        if back_right_idx < num_pages:
            right_content_back = reader.pages[back_right_idx]
            # Posicionar en la derecha (x=page_width)
            back_page.merge_translated_page(right_content_back, tx=page_width, ty=0)
        
        writer.add_page(back_page)
    
    # Guardar el PDF resultante
    with open(output_pdf, 'wb') as f:
        writer.write(f)
    
    print(f"✅ Cuadernillo creado exitosamente!")
    print(f"📄 Páginas originales: {num_pages}")
    print(f"📄 Páginas en cuadernillo: {pages_needed} (con {pages_needed - num_pages} páginas en blanco)")
    print(f"📋 Hojas a imprimir: {num_sheets} (imprimir ambas caras)")
    print(f"💾 Guardado en: {output_pdf}")
    print(f"\n📖 Instrucciones de impresión:")
    print(f"   1. Imprime en modo 'doble cara' (o imprime impares, luego pares)")
    print(f"   2. Imprime sin escalar ('tamaño real' o '100%')")
    print(f"   3. Apila todas las hojas en orden")
    print(f"   4. Dobla por la mitad y grapa en el centro")


def main():
    # Archivos de entrada y salida
    input_file = Path("Snippets.pdf")
    output_file = Path("Snippets_Booklet.pdf")
    
    if not input_file.exists():
        print(f"❌ Error: No se encontró el archivo {input_file}")
        print(f"   Asegúrate de haber generado el PDF primero con: python generate_pdf.py")
        sys.exit(1)
    
    try:
        create_booklet(input_file, output_file)
    except Exception as e:
        print(f"❌ Error creando el cuadernillo: {e}")
        import traceback
        traceback.print_exc()
        sys.exit(1)


if __name__ == "__main__":
    main()
