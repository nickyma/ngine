/******************************************************************************

    N'gine Lib for C++
    *** Version 0.6.1-alpha ***
    Camara virtual en 2D

    Proyecto iniciado el 1 de Febrero del 2016
    (cc) 2016 - 2018 by Cesar Rincon "NightFox"
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



/*** Includes ***/

// C++
#include <cstdio>
#include <iostream>

// n'gine
#include "ngn.h"

// Class
#include "ngn_camera.h"



/******************************************************************************

    Clase NGN_Camera

******************************************************************************/

/*** Contructor de la clase NGN_Camera ***/
NGN_Camera::NGN_Camera() {

    // Limpia los vectores
    layer.clear();
    // Parametros iniciales
    target = NULL;
    //world.width = ngn->graphics->native_w;     // Se autoconfigura en ngn_graphics.cpp
    //world.height = ngn->graphics->native_h;
    position.x = position.y = 0.0f;
    scroll.width = scroll.height = 0.0f;
    animation_pause = false;

}



/*** Destructor de la clase NGN_Camera ***/
NGN_Camera::~NGN_Camera() {

    // Elimina el contenido del gestor de capas
    Reset();

}



/*** Crea las capas que usara la camara ***/
void NGN_Camera::CreateLayers(uint32_t layers) {

    // Borra el contenido actual
    Reset();

    // Asigna el espacio solicitado
    layer.resize(layers);

    // Inicializa los datos
    for (uint32_t i = 0; i < layer.capacity(); i ++) {
        layer[i].sprite_layer.width = NGN_DEFAULT_VALUE;
        layer[i].sprite_layer.height = NGN_DEFAULT_VALUE;
        layer[i].in_use = false;
        layer[i].visible = true;
        layer[i].texture.clear();
        layer[i].bg.clear();
        layer[i].spr.clear();
    }

}



/*** Especifica el tama�o de la capa para los sprites ***/
void NGN_Camera::SizeOfLayer(uint32_t layer_number, float width, float height) {

    if (layer_number < layer.size()) {
        if (width > 0.0f) layer[layer_number].sprite_layer.width = width;
        if (height > 0.0f) layer[layer_number].sprite_layer.height = height;
    }

}



/*** Inicializa la camara con los parametros por defecto ***/
void NGN_Camera::Setup(uint32_t world_width, uint32_t world_height, NGN_Sprite* target_sprite) {

    // Registra las medidas del mundo
    world.width = world_width;
    world.height = world_height;

    // El mundo NO puede ser menor del tama�o de la pantalla
    if (world.width < ngn->graphics->native_w) world.width = ngn->graphics->native_w;
    if (world.height < ngn->graphics->native_h) world.height = ngn->graphics->native_h;

    // Tama�o del scroll principal
    scroll.width = (world.width - ngn->graphics->native_w);
    scroll.height = (world.height - ngn->graphics->native_h);

    // Registra el target
    target = target_sprite;

}



/*** A�ade un fondo de textura a la capa especificada ***/
int32_t NGN_Camera::PushBackground(uint32_t layer_number, NGN_Texture* texture) {

    // Si la capa es valida
    if (layer_number < layer.capacity()) {

        // Indica al sprite en que capa se ha a�adido
        texture->camera_layer = layer_number;

        // A�ade el fondo a la lista
        layer[layer_number].texture.push_back(texture);
        // Y registra que esta en uso la capa
        layer[layer_number].in_use = true;

        // Si es necesario, registra el tama�o del fondo como el de la capa de Sprites
        if ((layer[layer_number].sprite_layer.width == NGN_DEFAULT_VALUE) || (layer[layer_number].sprite_layer.height == NGN_DEFAULT_VALUE)) {
            SizeOfLayer(layer_number, texture->width, texture->height);
        }

    } else {

        return -1;

    }

    // Devuelve el indice
    return (layer[layer_number].bg.size() - 1);

}



/*** A�ade un fondo de tiles a la capa especificada ***/
int32_t NGN_Camera::PushBackground(uint32_t layer_number, NGN_TiledBg* background) {

    // Si la capa es valida
    if (layer_number < layer.capacity()) {

        // Indica al sprite en que capa se ha a�adido
        background->camera_layer = layer_number;

        // A�ade el fondo a la lista
        layer[layer_number].bg.push_back(background);
        // Y registra que esta en uso la capa
        layer[layer_number].in_use = true;

        // Si es necesario, registra el tama�o del fondo como el de la capa de Sprites
        if ((layer[layer_number].sprite_layer.width == NGN_DEFAULT_VALUE) || (layer[layer_number].sprite_layer.height == NGN_DEFAULT_VALUE)) {
            SizeOfLayer(layer_number, background->width, background->height);
        }

    } else {

        return -1;

    }

    // Devuelve el indice
    return (layer[layer_number].bg.size() - 1);

}



/*** A�ade un fondo de textura a la camara en modo virtual ***/
int32_t NGN_Camera::PushVirtualBg(uint32_t layer_number, NGN_Texture* texture, uint32_t bg_width, uint32_t bg_height, uint32_t loop_x, uint32_t loop_y, float auto_x, float auto_y) {

    // A�ade el fondo a la camara
    int32_t id = PushBackground(layer_number, texture);

    // Registra los parametros virtuales
    if (id >= 0) {
        layer[layer_number].texture[id]->virtual_texture.enabled = true;
        layer[layer_number].texture[id]->virtual_texture.texture_size.width = bg_width;
        layer[layer_number].texture[id]->virtual_texture.texture_size.height = bg_height;
        layer[layer_number].texture[id]->virtual_texture.loop.x = loop_x;
        layer[layer_number].texture[id]->virtual_texture.loop.y = loop_y;
        layer[layer_number].texture[id]->virtual_texture.scroll.x = -auto_x;
        layer[layer_number].texture[id]->virtual_texture.scroll.y = -auto_y;
        layer[layer_number].sprite_layer.width = bg_width;
        layer[layer_number].sprite_layer.height = bg_height;
    }

    // Devuelve el indice
    return id;

}



/*** A�ade un fondo de tiles a la camara en modo virtual ***/
int32_t NGN_Camera::PushVirtualBg(uint32_t layer_number, NGN_TiledBg* background, uint32_t bg_width, uint32_t bg_height, uint32_t loop_x, uint32_t loop_y, float auto_x, float auto_y) {

    // A�ade el fondo a la camara
    int32_t id = PushBackground(layer_number, background);

    // Registra los parametros virtuales
    if (id >= 0) {
        layer[layer_number].bg[id]->virtual_bg.enabled = true;
        layer[layer_number].bg[id]->virtual_bg.bg_size.width = bg_width;
        layer[layer_number].bg[id]->virtual_bg.bg_size.height = bg_height;
        layer[layer_number].bg[id]->virtual_bg.loop.x = loop_x;
        layer[layer_number].bg[id]->virtual_bg.loop.y = loop_y;
        layer[layer_number].bg[id]->virtual_bg.scroll.x = -auto_x;
        layer[layer_number].bg[id]->virtual_bg.scroll.y = -auto_y;
        layer[layer_number].sprite_layer.width = bg_width;
        layer[layer_number].sprite_layer.height = bg_height;
    }

    // Devuelve el indice
    return id;

}



/*** A�ade un sprite a la capa especificada ***/
int32_t NGN_Camera::PushSprite(uint32_t layer_number, NGN_Sprite* sprite) {

    // Si la capa es valida
    if (layer_number < layer.capacity()) {

        // Indica al sprite en que capa se ha a�adido
        sprite->camera_layer = layer_number;
        // A�ade el fondo a la lista
        layer[layer_number].spr.push_back(sprite);
        // Y registra que esta en uso la capa
        layer[layer_number].in_use = true;

    } else {

        return -1;

    }

    // Devuelve el indice
    return (layer[layer_number].spr.size() - 1);

}



/*** Haz que la camara siga a un sprite concreto [Sobrecarga 1] ***/
void NGN_Camera::LookAt(NGN_Sprite* target_sprite) {

    // Si el target es valido, asignalo
    if (target_sprite != NULL) target = target_sprite;

}



/*** Haz que la camara se mueva a una coordenada concreta [Sobrecarga 2] ***/
void NGN_Camera::LookAt(uint32_t position_x, uint32_t position_y) {

    // Anula el target actual si este existe
    target = NULL;

    // Posiciona la camara en estas coordenadas
    position.x = position_x;
    position.y = position_y;

}



/*** Haz que la camara se mueva a una coordenada concreta [Sobrecarga 3] ***/
void NGN_Camera::LookAt(Vector2I32 pos) {

    // Anula el target actual si este existe
    target = NULL;

    // Posiciona la camara en estas coordenadas
    position.x = (pos.x >= 0) ? (uint32_t)pos.x : 0;
    position.y = (pos.y >= 0) ? (uint32_t)pos.y : 0;

}



/*** Actualiza la vista de la camara ***/
void NGN_Camera::Update() {

    // Si hay capas definidas
    if (layer.size() > 0) {

        // Control de la camara
        Vector2I64 origin;     // Esquina superior-izquierda del mundo en pantalla
        origin.x = origin.y = 0;
        Vector2I32 screen;     // Posicion del objeto en la pantalla
        screen.x = screen.y = 0;
        Vector2I64 sprite;     // Posicion del sprite en la camara
        sprite.x = sprite.y = 0;
        Vector2I64 temp;       // Vector2 de uso general
        temp.x = temp.y = 0;

        // Calcula la posicion REAL de la camara en el mundo segun su target
        Vector2 campos;
        if (target != NULL) {
            campos.x = target->position.x;
            campos.y = target->position.y;
        } else {
            campos.x = position.x;
            campos.y = position.y;
        }
        // Ajusta su posicion para que quede dentro del mundo
        if (campos.x < (ngn->graphics->native_w / 2)) campos.x = (ngn->graphics->native_w / 2);
        if (campos.x > (world.width - (ngn->graphics->native_w / 2))) campos.x = (world.width - (ngn->graphics->native_w / 2));
        if (campos.y < (ngn->graphics->native_h / 2)) campos.y = (ngn->graphics->native_h / 2);
        if (campos.y > (world.height - (ngn->graphics->native_h / 2))) campos.y = (world.height - (ngn->graphics->native_h / 2));
        // Calcula la coordenada de origen del dibujado
        origin.x = (campos.x - (ngn->graphics->native_w / 2));
        origin.y = (campos.y - (ngn->graphics->native_h / 2));

        //std::cout << scroll.width << " " << scroll.height << std::endl;

        // Dibuja las capas una a una (la ultima se dibuja en frente)
        for (uint32_t l = 0; l < layer.size(); l ++) {

            // Si la capa tiene contenido y es visible
            if (layer[l].in_use && layer[l].visible) {

                // Primero, dibuja los fondos de textura (si existen)
                if (layer[l].texture.size() > 0) {
                    for (uint32_t b = 0; b < layer[l].texture.size(); b ++) {
                        // Calcula el rango de desplazamiento de este fondo
                        if (layer[l].texture[b]->virtual_texture.enabled) {
                            temp.x = (layer[l].texture[b]->virtual_texture.texture_size.width - ngn->graphics->native_w);
                            temp.y = (layer[l].texture[b]->virtual_texture.texture_size.height - ngn->graphics->native_h);
                        } else {
                            temp.x = (layer[l].texture[b]->width - ngn->graphics->native_w);
                            temp.y = (layer[l].texture[b]->height - ngn->graphics->native_h);
                        }
                        // Calcula la posicion relativa en X (efecto parallax)
                        if (scroll.width > 0) {
                            screen.x = ((temp.x * origin.x) / scroll.width);
                        } else {
                            screen.x = 0;
                        }
                        // Auto scroll en X
                        if (layer[l].texture[b]->virtual_texture.scroll.x != 0.0f) {
                            layer[l].texture[b]->virtual_texture.offset.x += layer[l].texture[b]->virtual_texture.scroll.x;
                            if (layer[l].texture[b]->virtual_texture.scroll.x > 0.0f) {
                                if (layer[l].texture[b]->virtual_texture.offset.x > layer[l].texture[b]->virtual_texture.loop.x) layer[l].texture[b]->virtual_texture.offset.x -= layer[l].texture[b]->virtual_texture.loop.x;
                            } else {
                                if (layer[l].texture[b]->virtual_texture.offset.x < 0) layer[l].texture[b]->virtual_texture.offset.x += layer[l].texture[b]->virtual_texture.loop.x;
                            }
                            screen.x += (int32_t)layer[l].texture[b]->virtual_texture.offset.x;
                        }
                        // Punto de loop
                        if (layer[l].texture[b]->virtual_texture.enabled && (layer[l].texture[b]->virtual_texture.loop.x > 0)) screen.x %= layer[l].texture[b]->virtual_texture.loop.x;
                        // Calcula la posicion relativa en Y (efecto parallax)
                        if (scroll.height > 0) {
                            screen.y = ((temp.y * origin.y) / scroll.height);
                        } else {
                            screen.y = 0;
                        }
                        // Auto scroll en Y
                        if (layer[l].texture[b]->virtual_texture.scroll.y != 0.0f) {
                            layer[l].texture[b]->virtual_texture.offset.y += layer[l].texture[b]->virtual_texture.scroll.y;
                            if (layer[l].texture[b]->virtual_texture.scroll.y > 0.0f) {
                                if (layer[l].texture[b]->virtual_texture.offset.y > layer[l].texture[b]->virtual_texture.loop.y) layer[l].texture[b]->virtual_texture.offset.y -= layer[l].texture[b]->virtual_texture.loop.y;
                            } else {
                                if (layer[l].texture[b]->virtual_texture.offset.y < 0) layer[l].texture[b]->virtual_texture.offset.y += layer[l].texture[b]->virtual_texture.loop.y;
                            }
                            screen.y += (int32_t)layer[l].texture[b]->virtual_texture.offset.y;
                        }
                        // Punto de loop Y
                        if (layer[l].texture[b]->virtual_texture.enabled && (layer[l].texture[b]->virtual_texture.loop.y > 0)) screen.y %= layer[l].texture[b]->virtual_texture.loop.y;
                        // Y dibujalo en el renderer
                        ngn->render->Texture(layer[l].texture[b], -screen.x, -screen.y);
                    }
                }

                // Luego, dibuja los fondos tileados (si existen)
                if (layer[l].bg.size() > 0) {
                    for (uint32_t b = 0; b < layer[l].bg.size(); b ++) {
                        // Calcula el rango de desplazamiento de este fondo, sea real o virtual
                        if (layer[l].bg[b]->virtual_bg.enabled) {
                            temp.x = (layer[l].bg[b]->virtual_bg.bg_size.width - ngn->graphics->native_w);
                            temp.y = (layer[l].bg[b]->virtual_bg.bg_size.height - ngn->graphics->native_h);
                        } else {
                            temp.x = (layer[l].bg[b]->width - ngn->graphics->native_w);
                            temp.y = (layer[l].bg[b]->height - ngn->graphics->native_h);
                        }
                        //if (b == 0) std::cout << layer[l].bg[b]->width << " " << layer[l].bg[b]->height << std::endl;
                        // Calcula la posicion relativa en X (efecto parallax)
                        if (scroll.width > 0) {
                            screen.x = ((temp.x * origin.x) / scroll.width);
                        } else {
                            screen.x = 0;
                        }
                        // Auto scroll en X
                        if (layer[l].bg[b]->virtual_bg.scroll.x != 0.0f) {
                            layer[l].bg[b]->virtual_bg.offset.x += layer[l].bg[b]->virtual_bg.scroll.x;
                            if (layer[l].bg[b]->virtual_bg.scroll.x > 0.0f) {
                                if (layer[l].bg[b]->virtual_bg.offset.x > layer[l].bg[b]->virtual_bg.loop.x) layer[l].bg[b]->virtual_bg.offset.x -= layer[l].bg[b]->virtual_bg.loop.x;
                            } else {
                                if (layer[l].bg[b]->virtual_bg.offset.x < 0) layer[l].bg[b]->virtual_bg.offset.x += layer[l].bg[b]->virtual_bg.loop.x;
                            }
                            screen.x += (int32_t)layer[l].bg[b]->virtual_bg.offset.x;
                        }
                        // Punto de loop en X
                        if (layer[l].bg[b]->virtual_bg.enabled && (layer[l].bg[b]->virtual_bg.loop.x > 0)) screen.x %= layer[l].bg[b]->virtual_bg.loop.x;
                        // Calcula la posicion relativa en Y (efecto parallax)
                        if (scroll.height > 0) {
                            screen.y = ((temp.y * origin.y) / scroll.height);
                        } else {
                            screen.y = 0;
                        }
                        // Auto scroll en Y
                        if (layer[l].bg[b]->virtual_bg.scroll.y != 0.0f) {
                            layer[l].bg[b]->virtual_bg.offset.y += layer[l].bg[b]->virtual_bg.scroll.y;
                            if (layer[l].bg[b]->virtual_bg.scroll.y > 0.0f) {
                                if (layer[l].bg[b]->virtual_bg.offset.y > layer[l].bg[b]->virtual_bg.loop.y) layer[l].bg[b]->virtual_bg.offset.y -= layer[l].bg[b]->virtual_bg.loop.y;
                            } else {
                                if (layer[l].bg[b]->virtual_bg.offset.y < 0) layer[l].bg[b]->virtual_bg.offset.y += layer[l].bg[b]->virtual_bg.loop.y;
                            }
                            screen.y += (int32_t)layer[l].bg[b]->virtual_bg.offset.y;
                        }
                        // Punto de loop en Y
                        if (layer[l].bg[b]->virtual_bg.enabled && (layer[l].bg[b]->virtual_bg.loop.y > 0)) screen.y %= layer[l].bg[b]->virtual_bg.loop.y;
                        // Posiciona el fondo
                        layer[l].bg[b]->Position(screen.x, screen.y);
                        //std::cout << "L: " << l << " BG:" << b << " POS:" << layer[l].bg[b]->position.x << "x" << layer[l].bg[b]->position.y << std::endl;
                        // Y dibujalo en el renderer
                        ngn->render->TiledBg(layer[l].bg[b]);
                    }
                }

                // Dibuja los sprites encima de estos fondos (si existen)
                if (layer[l].spr.size() > 0) {
                    // Calcula el rango de desplazamiento de este fondo
                    temp.x = (layer[l].sprite_layer.width - ngn->graphics->native_w);
                    temp.y = (layer[l].sprite_layer.height - ngn->graphics->native_h);
                    for (uint32_t s = 0; s < layer[l].spr.size(); s ++) {
                        // Calcula la posicion relativa segun la capa que esta
                        if (scroll.width > 0) {
                            sprite.x = ((temp.x * origin.x) / scroll.width);
                        } else {
                            sprite.x = 0.0f;
                        }
                        if (scroll.height > 0) {
                            sprite.y = ((temp.y * origin.y) / scroll.height);
                        } else {
                            sprite.y = 0.0f;
                        }
                        // Calcula la posicion del sprite en pantalla segun el origen de dibujado
                        screen.x = layer[l].spr[s]->position.x - sprite.x;
                        screen.y = layer[l].spr[s]->position.y - sprite.y;
                        // Si existe una animacion y no hay pausa, aplicala
                        if (!animation_pause) layer[l].spr[s]->PlayAnimation();
                        // Si esta dentro de la pantalla, dibujalo en el renderer e indicalo
                        if (
                            (screen.x > -(layer[l].spr[s]->width / 2.0f))
                            &&
                            (screen.x < (ngn->graphics->native_w + (layer[l].spr[s]->width / 2.0f)))
                            &&
                            (screen.y > -(layer[l].spr[s]->height / 2.0f))
                            &&
                            (screen.y < (ngn->graphics->native_h + (layer[l].spr[s]->height / 2.0f)))
                        ) {
                            // Indica que esta en pantalla
                            layer[l].spr[s]->on_screen = true;
                            // Y dibujalo
                            ngn->render->Sprite(layer[l].spr[s], screen.x, screen.y);
                        } else {
                            // Indica que no esta en pantalla
                            layer[l].spr[s]->on_screen = false;
                        }
                    }
                }

            }
        }

    }

}



/*** Quita un fondo de textura de la camara ***/
int32_t NGN_Camera::RemoveBackground(NGN_Texture* texture) {

    int32_t r = -1;

    // Capas
    for (uint32_t l = 0; l < layer.size(); l ++) {
        // Fondos
        if (layer[l].texture.size() > 0) {
            for (uint32_t b = 0; b < layer[l].texture.size(); b ++) {
                // Si el fondo es el solicitado...
                if (layer[l].texture[b] == texture) {
                    // Borra el elemento
                    layer[l].texture.erase((layer[l].texture.begin() + b));
                    r = 0;
                    break;
                }
            }
        }
        if (r == 0) break;
    }

    return r;

}



/*** Quita un fondo de tiles de la camara ***/
int32_t NGN_Camera::RemoveBackground(NGN_TiledBg* background) {

    int32_t r = -1;

    // Capas
    for (uint32_t l = 0; l < layer.size(); l ++) {
        // Fondos
        if (layer[l].bg.size() > 0) {
            for (uint32_t b = 0; b < layer[l].bg.size(); b ++) {
                // Si el fondo es el solicitado...
                if (layer[l].bg[b] == background) {
                    // Borra el elemento
                    layer[l].bg.erase((layer[l].bg.begin() + b));
                    r = 0;
                    break;
                }
            }
        }
        if (r == 0) break;
    }

    return r;

}



/*** Quita un Sprite de la camara ***/
int32_t NGN_Camera::RemoveSprite(NGN_Sprite* sprite) {

    int32_t r = -1;                     // Resultado de la operacion
    int32_t l = sprite->camera_layer;   // Capa donde esta el sprite alojado

    // Si el sprite no esta asignado a la camara, sal
    if (l < 0) return r;

    // Sprites
    if (layer[l].spr.size() > 0) {
        for (uint32_t s = 0; s < layer[l].spr.size(); s ++) {
            // Si el sprite es el solicitado...
            if (layer[l].spr[s] == sprite) {
                // Indica al sprite que ya no pertenece a ninguna capa
                sprite->camera_layer = -1;
                // Borra el elemento
                layer[l].spr.erase((layer[l].spr.begin() + s));
                r = 0;
                break;
            }
        }
    }

    //for (uint32_t l = 0; l < layer.size(); l ++) std::cout << "Sprites in layer " << l << ": " << layer[l].spr.size() << std::endl;

    // Devuelve el resultado de la operacion
    return r;

}



/*** Cambia un sprite de capa ***/
int32_t NGN_Camera::ChangeLayer(NGN_Sprite* sprite, uint32_t layer_number) {

    uint32_t s = 0;
    uint32_t l = 0;
    int32_t r = -1;

    // Verifica que la capa sea valida y el sprite no sea nulo
    if ((layer_number < layer.capacity()) && (sprite != NULL)) {
        // Capa actual
        l = sprite->camera_layer;
        // Si la capa de origen y destino son identicas, sal y devuelve error
        if (l == layer_number) return r;
        // Busca el sprite en su capa asignada
        for (s = 0; s < layer[l].spr.size(); s ++) {
            if (layer[l].spr[s] == sprite) {
                // Borra el elemento de la capa actual
                layer[l].spr.erase((layer[l].spr.begin() + s));
                // Y a�adelo a su nueva capa de destino
                r = PushSprite(layer_number, sprite);
                // Fuerza la salida del bucle
                break;
            }
        }
        // En caso de exito, devuelve el ID en la capa de este sprite
        return r;
    } else {
        // Devuelve error
        return r;
    }

}



/*** Reinicia la camara ***/
void NGN_Camera::Reset() {

    for (uint32_t i = 0; i < layer.capacity(); i ++) {
        layer[i].texture.clear();
        layer[i].bg.clear();
        layer[i].spr.clear();
    }
    layer.clear();

    // Quita la pausa de la animacion
    animation_pause = false;

}
