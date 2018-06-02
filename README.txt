===================================================================================
					COMPILAR
===================================================================================

make
	-> Compila el programa principal, el main debe encontrarse en la carpeta src. 
	   El ejecutable irá a la carpeta bin.

make TEST=prueba test  
	-> Compila el programa prueba.cpp de la carpeta test

make clean  
	-> Borra el directorio build

===================================================================================
				ESTRUCTURA DE CARPETAS
===================================================================================
bin: Los ejecutables saldrán en esta carpeta

build: Esta carpeta contiene los archivos objeto, al hacer make clean se borra

doc: Documentación y configuraciones.

include: Todas las cabeceras van en esta carpeta (.hpp .h)

src: Archivos fuente del programa. (.cpp .c)

test: Archivos para hacer test (.cpp .c)

