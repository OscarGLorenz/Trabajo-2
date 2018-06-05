===================================================================================
		   PARA COMPILAR USANDO EL MAKEFILE (LINUX/MACOS)
===================================================================================

make
	-> Compila el programa principal, el main debe encontrarse en la carpeta src. 
	   EL EJECUTABLE APARECERÁ EN LA CARPETA BIN

make clean  
	-> Borra el directorio build y ele ejecutable
===================================================================================
		           	VISUAL STUDIO (WINDOWS)
===================================================================================

Debe compilarse como Release de x86, los elementos gráficos estan solo en ese
lugar, si se quiere hacer en otro hay una carpeta llamada graphics que debe 
encontrarse en el mismo directorio que el ejecutable

===================================================================================
				ESTRUCTURA DE CARPETAS
===================================================================================
bin: Los ejecutables saldrán en esta carpeta

build: Esta carpeta contiene los archivos objeto, al hacer make clean se borra

doc: Documentación y configuraciones.

include: Todas las cabeceras van en esta carpeta (.hpp .h)

src: Archivos fuente del programa. (.cpp .c)


