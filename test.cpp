//>>>Disclaimer Written by ai for now
#include <SFML/Graphics.hpp>
#include <cmath>
#include <optional>
#include <sstream>
#include <iomanip>

struct PendulumState {
    double x = 0.0, x_d = 0.0, x_dd = 0.0;
    double theta = 3.141519, theta_d = 0.0, theta_dd = 0.0;
    double F = 0.0;
    bool balanced = false;
};

class PendulumSim {
public:
    //parameters
    const double M = 10.0;  
    const double m = 1.0;    
    const double l = 1.0;    
    const double g = 9.81;
    const double dt = 0.01;

    // LQR gains
    const double k1 = -2.23606798;
    const double k2 = -8.43909272;
    const double k3 = -262.37833267;
    const double k4 = -80.58232035;

    PendulumState s;

    void reset(double theta0) {
        s = PendulumState{};
        s.theta = theta0;
    }

    void step() {
        if (s.balanced) return;

        if (std::abs(s.theta) < 0.2 && std::abs(s.theta_d) < 1.0) {
            // LQR balance control near upright
            s.F = -1.0 * (k1 * s.x + k2 * s.x_d + k3 * s.theta + k4 * s.theta_d);
        } else {
            // energy-shaping swing-up
            s.F = 15.0 * ((0.5 * m * l * l * s.theta_d * s.theta_d)
                        + m * g * l * (1 + std::cos(s.theta)) - 2 * m * g * l)
                        * s.theta_d * std::cos(s.theta)
                  - 20.0*s.x - 10.0* s.x_d;
        }

        double denom = M + m - m * std::cos(s.theta) * std::cos(s.theta);
        s.x_dd = (s.F + (m * l * s.theta_d * s.theta_d * std::sin(s.theta))
                 - m * g * std::sin(s.theta) * std::cos(s.theta)) / denom;
        s.theta_dd = (g * std::sin(s.theta) - s.x_dd * std::cos(s.theta)) / l;

        s.x_d += s.x_dd * dt;
        s.x   += s.x_d * dt;
        s.theta_d += s.theta_dd * dt;
        s.theta   += s.theta_d * dt;

        while (s.theta > M_PI)  s.theta -= 2 * M_PI;
        while (s.theta < -M_PI) s.theta += 2 * M_PI;

        if (std::abs(s.theta) < 0.001 && std::abs(s.theta_d) < 0.001 && std::abs(s.x_d) < 0.001) {
            s.balanced = true;
        }
    }
};

int main() {
    const unsigned WIN_W = 1000, WIN_H = 600;
    sf::RenderWindow window(sf::VideoMode({WIN_W, WIN_H}), "Inverted Pendulum — SFML 3");
    window.setFramerateLimit(120);

    sf::Font font;
    const std::string FONT_PATH = "arial.ttf"; // put a .ttf next to the exe, or give an absolute path
    bool haveFont = font.openFromFile(FONT_PATH);

    PendulumSim sim;
    sim.reset(M_PI / 2.0); // start horizontal, same as your original code

    const float scale = 150.f;               // px per meter
    const float groundY = WIN_H * 0.65f;
    const float centerX = WIN_W * 0.5f;

    sf::RectangleShape track({(float)WIN_W, 2.f});
    track.setPosition({0.f, groundY});
    track.setFillColor(sf::Color(90, 90, 90));

    sf::RectangleShape cart({80.f, 40.f});
    cart.setOrigin({40.f, 20.f});
    cart.setFillColor(sf::Color(60, 140, 220));

    sf::CircleShape bob(18.f);
    bob.setOrigin({18.f, 18.f});
    bob.setFillColor(sf::Color(230, 90, 60));

    sf::VertexArray rod(sf::PrimitiveType::Lines, 2);
    rod[0].color = sf::Color::White;
    rod[1].color = sf::Color::White;

    std::optional<sf::Text> hud;
    if (haveFont) hud.emplace(font, "", 16);
    if (hud) hud->setFillColor(sf::Color::White);

    bool paused = false;
    sf::Clock clock;
    double accumulator = 0.0;

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            } else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->code == sf::Keyboard::Key::Escape) window.close();
                else if (key->code == sf::Keyboard::Key::Space) paused = !paused;
                else if (key->code == sf::Keyboard::Key::R) sim.reset(M_PI / 2.0);
            }
        }

        double frameTime = clock.restart().asSeconds();
        if (frameTime > 0.05) frameTime = 0.05; // clamp on hitches
        if (!paused) {
            accumulator += frameTime;
            while (accumulator >= sim.dt) {
                sim.step();
                accumulator -= sim.dt;
            }
        }

        // world -> screen
        float cartSx = centerX + (float)sim.s.x * scale;
        float bobSx  = cartSx + (float)(sim.l * std::sin(sim.s.theta)) * scale;
        float bobSy  = groundY - (float)(sim.l * std::cos(sim.s.theta)) * scale;

        cart.setPosition({cartSx, groundY});
        bob.setPosition({bobSx, bobSy});
        rod[0].position = {cartSx, groundY};
        rod[1].position = {bobSx, bobSy};

        window.clear(sf::Color(25, 25, 30));
        window.draw(track);
        window.draw(rod);
        window.draw(cart);
        window.draw(bob);

        if (hud) {
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(3);
            ss << (sim.s.balanced ? "BALANCED\n" : (std::abs(sim.s.theta) < 0.2 ? "mode: LQR balance\n" : "mode: swing-up\n"))
               << "theta   = " << sim.s.theta   << "\n"
               << "theta_d = " << sim.s.theta_d << "\n"
               << "x       = " << sim.s.x       << "\n"
               << "x_d     = " << sim.s.x_d     << "\n"
               << "F       = " << sim.s.F       << "\n"
               << (paused ? "\n[SPACE] resume  [R] reset  [ESC] quit"
                          : "\n[SPACE] pause  [R] reset  [ESC] quit");
            hud->setString(ss.str());
            hud->setPosition({15.f, 15.f});
            window.draw(*hud);
        }

        window.display();
    }
    return 0;
}