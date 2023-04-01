#include "graphics.h"
#include <SDL2/SDL.h>
#include <stdexcept>
#include <SDL_image.h>
#include <iostream>
#include <string>
#include <fstream>
#include "randomness.h"

Graphics::Graphics(const std::string &title, int window_width, int window_height)
    : width{window_width}, height{window_height}{
        
    int result = SDL_Init(SDL_INIT_VIDEO);
    if (result < 0){
        std::cout << SDL_GetError() << '\n';
    }
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height,0);
    if(!window){
        std::cout << SDL_GetError() << '\n';
    }
    renderer = SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    // initialize SDL, create a window and renderer
    // make sure to check all return values and throw exceptions when errors occur
    //SDL Image
    int img_flags = IMG_INIT_PNG;
    if (!(IMG_Init(img_flags) & img_flags)){
        throw std::runtime_error(IMG_GetError());
    }
}

Graphics::~Graphics() {
    IMG_Quit();
    // clean up: release SDL resources
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

Sprite Graphics::load_image(const std::string& filename) {
    int id = get_texture_id(filename);
    SDL_Texture* texture = textures.at(id);
    int width, height;
    SDL_QueryTexture(texture, nullptr, nullptr, &width, &height);
    Sprite sprite{id, {0, 0}, {width, height}};
    return sprite;
}

void Graphics::load_spritesheet(const std::string& filename){
    std::ifstream input{filename};
    if (!input){
        throw std::runtime_error("Cannot open asset file: " + filename);
    }
    std::string image_filename;
    input >> image_filename;
    // std::cout << image_filename << std::endl;
    auto i = filename.find('/');
    std::string parent_path{filename.substr(0, i+1)}; //assets/
    // std::cout << parent_path << std::endl;
    // image_filename = "../assets/" + image_filename; //assets/spritesheets.png
    image_filename = parent_path + image_filename;
    std::cout << image_filename << std::endl;

    int texture_id = get_texture_id(image_filename);
    // std::cout << texture_id << std::endl;


    // load sprites -> unordered map <strings, sprites>
    std::string name;
    // std::cout << name << std::endl;

    int x, y, width, height, scale;
    while(input >> name >> x >> y >> width >> height >> scale){
        Vec shift{-width/2, -height};
        Vec center{width/2,height/2};
        int number_of_frames;
        if (!(input >> number_of_frames)){//attempt to read optional value
            number_of_frames = 1;
            input.clear();
        }
        for (int i = 0; i < number_of_frames; ++i){
            Vec location{x + i * width, y};
            Vec size{width, height};
            Sprite sprite{
                texture_id, location, size, scale, shift, center
            };
            sprites[name].push_back(sprite);
        }
    }

}

Sprite Graphics::get_sprite(const std::string& name) const {
    auto i = sprites.find(name);
    if (i == sprites.end()) {
        throw std::runtime_error("Cannot find sprite: " + name);
    }
    std::cout << "Found sprite" << std::endl;
    return i->second.front();
}

AnimatedSprite Graphics::get_animated_sprite(const std::string& name, double dt_per_frame,bool random_start, bool shuffle_order) const {
    auto i = sprites.find(name);
    if (i == sprites.end()){
        throw std::runtime_error("cannot find animated sprite: " + name);
    }
    std::vector<Sprite> sprites = i->second;
    if (shuffle_order){
        shuffle(std::begin(sprites), std::end(sprites));
    }
    if (sprites.size() > 1 && random_start){
        int starting_frame = randint(0, sprites.size()-1);
        return AnimatedSprite{sprites,dt_per_frame,starting_frame};
    }
    else{
        return AnimatedSprite(sprites,dt_per_frame);
    }

}   

void Graphics::clear() {
    // clear the screen by painting it black
    SDL_SetRenderDrawColor(renderer,0,0,0,255);
    SDL_RenderClear(renderer);
}

void Graphics::draw(const SDL_Rect& rect, const Color& color, bool filled) {
    // draw a filled rectangle
    SDL_SetRenderDrawColor(renderer,color.red,color.green,color.blue,color.alpha);
    if (filled){
        SDL_RenderFillRect(renderer, &rect);
    }
    else{
        SDL_RenderDrawRect(renderer, &rect);
    }
}

void Graphics::update() {
    // show the current canvas on the screen
    SDL_RenderPresent(renderer);
}


int Graphics::get_texture_id(const std::string& image_filename){
    auto search = texture_ids.find(image_filename);
    if (search != texture_ids.end()){ //image already exists
        int texture_id = search->second;
        return texture_id;
    }
    else{ //new image file
        SDL_Texture* texture = IMG_LoadTexture(renderer, image_filename.c_str());
        if (!texture){
            throw std::runtime_error(IMG_GetError());
        }
        //register new texture
        int texture_id = textures.size();
        texture_ids[image_filename] = texture_id;

        // retain ownership of texture pointers
        textures.push_back(texture);
        return texture_id;
    }
}

void Graphics::draw_sprite(const Vec<int>& pixel, const Sprite& sprite) {
    if (sprite.texture_id < 0) {  // sprite with empty texture
        return;
    }
    // Calculate where sprite should appear on screen taking into account the scale factor (image size -> screen size)
    int x = pixel.x + sprite.shift.x * sprite.scale;
    int y = pixel.y + sprite.shift.y * sprite.scale;
    int w = sprite.size.x * sprite.scale;
    int h = sprite.size.y * sprite.scale;
    SDL_Rect screen_pixels{x, y, w, h};
    SDL_Rect image_pixels{sprite.location.x, sprite.location.y, sprite.size.x, sprite.size.y};
    // Calculate the center of the scaled up sprite
    SDL_Point center{sprite.center.x * sprite.scale, sprite.center.y * sprite.scale};
    
    // Get the sprite's SDL texture
    SDL_Texture* texture = textures.at(sprite.texture_id);
    SDL_RendererFlip flip = sprite.flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;

    // Draw the sprite on screen taking into account rotation (sprite.angle) about its center,
    // and whether to flip the sprite horizontally
    SDL_RenderCopyEx(renderer, texture, &image_pixels, &screen_pixels, sprite.angle, &center, flip);
}

