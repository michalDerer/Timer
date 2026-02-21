Dependencies:<br/>

SDL3      (https://github.com/libsdl-org/SDL)<br/>
SDL_image<br/>
lua<br/>

Ako buildnut projekt:<br/>

Linux:<br/>
   1. Nainstalovat vsetky potrebne packages.<br/>
	&emsp;www.github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies<br/>
	&emsp;apt (Debian/Ubuntu...): install_dependencies.sh

   2. Vytvorit build system. Vytvori sa /build zlozka v root adresari.<br/>
	&emsp;make_build.sh

   3. Buildnut projekt.<br/>
	&emsp;build_build.sh

   4. Buildnuta aplikacia bude v /build/Debug alebo /build/Release zlozke. 

Dodatocne info:<br/>
Projekt pouziva SDL3, SDL_image, lua - ktore builduje zo source kodu.<br/>
https://github.com/libsdl-org/SDL/blob/main/docs/README-cmake.md<br/>

Poznamky-Linux(Ubuntu):<br/>
Ak pri spusteni aplikacie vydiet ze aplikacia ide, ale nezobrazuje sa okno:<br/>
&emsp;echo $DISPLAY<br/>
&emsp;&emsp;ak sa nic nevypise nenaslo ziadny display, preto nezobrazuje okno<br/>
&emsp;echo $XDG_SESSION_TYPE<br/>
&emsp;&emsp;vypise momentalne pouzivany driver<br/>
&emsp;&emsp;wayland<br/>
&emsp;&emsp;x11<br/>
&emsp;skusit forsnut pouzitie ineho backendu, v konzole prikazom:<br/>
&emsp;&emsp;export SDL_VIDEODRIVER=wayland<br/>
&emsp;&emsp;export SDL_VIDEODRIVER=x11<br/>
&emsp;znovu pustit aplikaciu<br/>
Ak sa zobrazi prazdne okno bez dekoracii (Title, close button ..):<br/>
&emsp;Pravdepodobne je videodriver wayland a nieje nainstalovana podpora client side dekoratorov libdecor.<br/>
&emsp;&emsp;v konzole sudo apt install libdecor-0-0 libdecor-0-dev<br/>
&emsp;&emsp;znova vygenerovat build system a buildnut<br/>
&emsp;Alebo v kode pridat SDL_SetHint(SDL_HINT_VIDEO_DRIVER, "x11"); pred SDL_Init(SDL_INIT_VIDEO) a tym forsnut pouzitie x11<br/>