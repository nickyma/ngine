/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Fondos de tiles

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere N'gine 0.6.0-a o superior

    Requiere GCC 7.3.0 MinGW (SEH) - 64-bits
    http://downloads.sourceforge.net/project/mingw-w64/

    Requiere SDL2 (2.0.8) - 64-bits
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.5.0) - 64-bits
    http://www.sfml-dev.org/

    Requiere LodePNG
    (c) 2005 - 2016 by Lode Vandevenne
    http://lodev.org/lodepng/


    N'gine se distribuye bajo la licencia CREATIVE COMMONS
    "Attribution-NonCommercial 4.0 International"
    https://creativecommons.org/licenses/by-nc/4.0/

    You are free to:

        - Share
        copy and redistribute the material in any medium or format.
        - Adapt
        remix, transform, and build upon the material.

        The licensor cannot revoke these freedoms as long as you follow
        the license terms.

    Under the following terms:

        - Attribution
        You must give appropriate credit, provide a link to the license,
        and indicate if changes were made. You may do so in any reasonable
        manner, but not in any way that suggests the licensor endorses you
        or your use.

        - NonCommercial
        You may not use the material for commercial purposes.

        - No additional restrictions
        You may not apply legal terms or technological measures that
        legally restrict others from doing anything the license permits.

******************************************************************************/



#ifndef DEMO_H_INCLUDED
#define DEMO_H_INCLUDED



/*** Includes ***/
// Includes de la libreria
#include <ngn.h>



/*** Declaracion de la clase ***/
class Demo {

    public:

        // Constructor
        Demo();

        // Destructor
        ~Demo();

        // Awake
        bool Awake();

        // Start
        bool Start();

        // Run
        void Run();


    private:

        // Variables para almacenar los datos de los fondos
        NGN_TiledBgData* bg_main_data;
        NGN_TiledBgData* bg_water_data;
        NGN_TiledBgData* bg_sky_data;

        // Variables para crear los objetos de los fondos
        NGN_TiledBg* bg_main;
        NGN_TiledBg* bg_water;
        NGN_TiledBg* bg_sky;

        // Datos de control
        int32_t left, right;        // Limites del movimiento del scroll
        Vector2 position;        // Posicion del fondo

        // Carga los archivos necesarios
        bool Load();

        // Crea los fondos
        void CreateStage();

        // Update
        void Update();

        // Mueve el fondo con los cursores
        void Move();

        // Renderiza el escenario
        void Render();

};


#endif // DEMO_H_INCLUDED
