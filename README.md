Dependencies:<br/>

SDL3      (https://github.com/libsdl-org/SDL)<br/>
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

   4. Buildnuta aplikacia bude v /build zlozke. 

Dodatocne info:<br/>
Projekt pouziva SDL3, lua ktore builduje zo source kodu.<br/>
https://github.com/libsdl-org/SDL/blob/main/docs/README-cmake.md<br/>

Linux Ubuntu:<br/>
Ak pri spusteni aplikacie vydiet ze aplikacia ide, ale nezobrazuje sa okno:<br/>
&emsp;echo $DISPLAY<br/>
&emsp;&emsp;ak sa nic nevypise nenaslo ziadny display, preto nezobrazuje okno<br/>
&emsp;echo $XDG_SESSION_TYPE<br/>
&emsp;&emsp;vypise momentalne pouzivany driver<br/>
&emsp;&emsp;wayland<br/>
&emsp;&emsp;x11<br/>
&emsp;skusit forsnut pouzitie ineho backendu<br/>
&emsp;&emsp;export SDL_VIDEODRIVER=wayland<br/>
&emsp;&emsp;export SDL_VIDEODRIVER=x11<br/>
&emsp;znovu pustit aplikaciu<br/>

