#include <SFML/Graphics.hpp>
#include "bob.h"
#include <math.h>
using namespace sf;

int main(){
    RenderWindow window(VideoMode({1200,600}),"Window");
    window.setVerticalSyncEnabled(true);

    const float Scale = 40.f;
    const float CartScale = 40.f;
    const float PendulumScale = 100.f; 

    RectangleShape cart({80.f, 30.f});
    cart.setOrigin({40.f, 15.f});
    cart.setFillColor(Color(60, 140, 220));

    RectangleShape bob({20.f, 20.f});
    bob.setOrigin({10.f, 10.f});
    bob.setFillColor(Color(230, 90, 60));

    VertexArray rod(PrimitiveType::Lines, 2);
    rod[0].color = Color::White;
    rod[1].color = Color::White;

    RectangleShape track({1200.f, 2.f});
    track.setPosition({0.f,300.f});
    track.setFillColor(Color(90, 90, 90));

    const float CENTER_X = 600.f;
    const float CENTER_Y = 300.f;

    float cartScreenX = CENTER_X;
    float cartScreenY = CENTER_Y;

    float cartX = CENTER_X + Scale * state.cartX;
    float cartY = 300.f;

    float relX = state.bobX - state.cartX;
    float relY = state.bobY;

    float bobX = cartX + Scale * relX;
    float bobY = cartY - Scale * relY;

    float CART_Y = CENTER_Y;

    cart.setPosition({cartScreenX, cartScreenY});
    bob.setPosition({bobX, bobY});

    const double fixedDt = 0.01;
    Clock clock;
    double accumulator = 0.0;



    while(window.isOpen()){

        while(std::optional event = window.pollEvent()){
        if(event->is<Event::Closed>()){
            window.close();
        }
        if(event->is<Event::KeyPressed>() && event->getIf<Event::KeyPressed>()->code == Keyboard::Key::Escape){
            window.close();
        }
    }

    double frameTime = clock.restart().asSeconds();
    
    if (frameTime > 0.05)
        frameTime = 0.05;

        accumulator += frameTime;

        while (accumulator >= fixedDt){
        updatePhysics(fixedDt);
        accumulator -= fixedDt;
        }

    cartX = CENTER_X + CartScale * state.cartX;
    cartY = CART_Y;

    relX = state.bobX - state.cartX;
    relY = state.bobY;                 

    bobX = cartX + PendulumScale * relX;
    bobY = cartY - PendulumScale * relY;

    rod[0].position = {cartX, CART_Y};
    rod[1].position = {bobX, bobY};

    cart.setPosition({cartX, cartY});
    bob.setPosition({bobX, bobY});

    window.clear(Color(0, 0, 0));

    window.draw(track);
    window.draw(rod);
    window.draw(cart);
    window.draw(bob);

    window.display();
    }
    return 0;
}