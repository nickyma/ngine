/******************************************************************************

    N'gine Lib for C++
    Ejemplo: Texto

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2017 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com

    Requiere SDL2 (2.0.5 or higher)
    http://www.libsdl.org/download-2.0.php

    Requiere SFML (2.4.1 or higher)
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

        // Update
        bool Update();


    private:

        // Textura de fondo
        NGN_TextureData* bg_data;
        NGN_Texture* bg;

        // Textura para la caja de texto
        NGN_TextureData* textbox_data;

        // Tipografias
        NGN_TextFont* fnt_banner;       // Fuente para el banner de texto
        NGN_TextFont* fnt_box_aa;       // Fuente para la caja de texto con anti-alias
        NGN_TextFont* fnt_box_px;       // Fuente para la caja de texto sin anti-alias
        NGN_TextFont* fnt_cursor;       // Fuente para el cursor en movimiento

        // Capas de texto
        NGN_TextLayer* textbox;
        NGN_TextLayer* banner;
        NGN_TextLayer* cursor;

        // Carga los archivos necesarios
        bool Load();

        // Crea la escena
        void CreateStage();

        // Crea la caja de texto
        void CreateTextBox();

        // Crea el banner superior
        void CreateBanner();

        // Crea el cursor
        void CreateCursor();
        // Actualiza el cursor
        void UpdateCursor();

        // Renderiza la escena
        void Render();


};


#endif // DEMO_H_INCLUDED