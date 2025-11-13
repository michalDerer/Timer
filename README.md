
Ako buildnut projekt:

1. Nainstalovat vsetky potrebne packages: www.github.com/libsdl-org/SDL/blob/main/docs/README-linux.md#build-dependencies
   Ubuntu/Debian:
        install_dependencies.sh

2. Vytvorit build system. Vytvori sa build zlozka v projekte.
        make_build.sh
   
3. Buildnut projekt.
        build_build.sh
   
4. Buildnuta aplikacia bude v build zlozke. 
   
   
Dodatocne info:
Projekt pouziva kniznice SDL3, SDL3_image, ktore builduje zo source kodu.
https://github.com/libsdl-org/SDL/blob/main/docs/README-cmake.md

Ubuntu:
Ak pri spusteni aplikacie vydiet ze aplikacia ide, ale nezobrazuje sa okno:
    echo $DISPLAY
        ak sa nic nevypise nenaslo ziadny display, preto nezobrazuje okno
    echo $XDG_SESSION_TYPE
        vypise momentalne pouzivany driver
        wayland
        x11
    skusit forsnut pouzitie ineho backendu 
        export SDL_VIDEODRIVER=wayland
        export SDL_VIDEODRIVER=x11
        znovu pustit aplikaciu

