/******************************************************************************

    N'gine Lib for C++
    *** Version 0.1.0-alpha ***
    Camara virtual en 2D

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2017 by Cesar Rincon "NightFox"
    http://www.nightfoxandco.com
    contact@nightfoxandco.com


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



#ifndef NGN_CAMERA_H_INCLUDED
#define NGN_CAMERA_H_INCLUDED


/*** Includes ***/

// C++
#include <vector>

// n'gine
#include "ngn_defines.h"
#include "ngn_sprite.h"
#include "ngn_tiledbg.h"



/******************************************************************************

    Clase NGN_Camera

******************************************************************************/

/*** Declaracion de la clase de la Camara virtual en 2D ***/
class NGN_Camera {

    // Private [Declaracion de los prototipos de datos]
    private:

        // Estructura de capas de dibujado
        struct layer_data{
            std::vector<NGN_Texture*> texture;  // Fondos de textura en esta capa
            std::vector<NGN_TiledBg*> bg;       // Fondos de tiles en esta capa
            std::vector<NGN_Sprite*> spr;       // Sprites en esta capa
            Size2I64 sprite_layer;                // Tama�o para la capa de sprites
            bool visible;                       // Visibilidad de la capa
            bool in_use;                        // Hay datos en la capa
        };

        // Parametros internos de la camara para su funcionamiento
        NGN_Sprite* target;         // Sprite al que seguira la camara
        Size2I64 scroll;               // Tama�o total del scroll en el mundo



    // Public
    public:

        // Contructor de la clase
        NGN_Camera();

        // Destructor de la clase
        ~NGN_Camera();

        // Vector de memoria con las capas
        std::vector<layer_data> layer;

        // Tama�o del mundo
        Size2I64 world;

        // Posicion de la camara en el mundo
        Vector2I64 position;

        // Define en numero de capas a crear (elimina las existentes)
        void CreateLayers(uint32_t layers);

        // Define el tama�o la capa de sprites (por defecto, todas las capas tienen el tama�o del mundo)
        void SizeOfLayer(uint32_t layer_number, float width, float height);

        // Inicializa la camara
        void Setup(uint32_t world_width, uint32_t world_height, NGN_Sprite* target_sprite = NULL);

        // A�ade un fondo de textura o de tiles a la capa y devuelve su indice en la lista. En caso de error, devuelve -1
        int32_t PushBackground(uint32_t layer_number, NGN_Texture* texture);
        int32_t PushBackground(uint32_t layer_number, NGN_TiledBg* background);

        // A�ade un fondo de textura o de tiles en modo tama�o virtual a la capa y devuelve su indice en la lista. En caso de error, devuelve -1
        int32_t PushVirtualBg(uint32_t layer_number, NGN_Texture* texture, uint32_t bg_width, uint32_t bg_height, uint32_t loop_x, uint32_t loop_y, float auto_x = 0.0f, float auto_y = 0.0f);
        int32_t PushVirtualBg(uint32_t layer_number, NGN_TiledBg* background, uint32_t bg_width, uint32_t bg_height, uint32_t loop_x, uint32_t loop_y, float auto_x = 0.0f, float auto_y = 0.0f);

        // A�ade un sprite a la capa  y devuelve su indice en la lista. En caso de error, devuelve -1
        int32_t PushSprite(uint32_t layer_number, NGN_Sprite* sprite);

        // Selecciona donde mira la camara
        void LookAt(NGN_Sprite* target_sprite);                     // Sigue a un sprite
        void LookAt(uint32_t position_x, uint32_t position_y);      // Colocala en la coordenada indicada

        // Actualiza la vista de la camara
        void Update();

        // Quita un fondo de la camara
        int32_t RemoveBackground(NGN_Texture* texture);
        int32_t RemoveBackground(NGN_TiledBg* background);

        // Quita un Sprite de la camara
        int32_t RemoveSprite(NGN_Sprite* sprite);

        // Cambia un elemento de capa
        int32_t ChangeLayer(NGN_Sprite* sprite, uint32_t layer_number);

        // Reset de la camara
        void Reset();

};



#endif // NGN_CAMERA_H_INCLUDED