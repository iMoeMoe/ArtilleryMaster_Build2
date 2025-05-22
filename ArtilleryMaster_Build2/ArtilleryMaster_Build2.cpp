#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>
#include <vector>
#include <cmath>
#include <string>

// --- ArtilleryMath: static calculation helpers ---
struct ArtilleryMath {
    static float distance(const sf::Vector2f& a, const sf::Vector2f& b) {
        return std::hypot(b.x - a.x, b.y - a.y);
    }
    static float azimuth(const sf::Vector2f& a, const sf::Vector2f& b) {
        return std::atan2(b.x - a.x, b.y - a.y) * 180.0f / 3.14159265f;
    }
    static float elevation(float dist, float velocity, float gravity = 9.81f, bool highArc = false) {
        float v2 = velocity * velocity;
        float g = gravity;
        float root = v2 * v2 - g * (g * dist * dist);
        if (root < 0) return 0;
        float angle = std::atan((v2 + (highArc ? -1 : 1) * std::sqrt(root)) / (g * dist));
        return angle * 180.0f / 3.14159265f;
    }
    static float timeOfFlight(float dist, float velocity, float angle, float gravity = 9.81f) {
        float rad = angle * 3.14159265f / 180.0f;
        return dist / (velocity * std::cos(rad));
    }
};

// --- TRP (Target Registration Point) ---
struct TRP {
    sf::Vector2f user, target;
    std::string name;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1200, 700), "Artillery Calculator");
    window.setFramerateLimit(60);
    ImGui::SFML::Init(window);

    // Map state
    sf::View mapView(sf::FloatRect(0, 0, 600, 600));
    sf::Vector2f userPos, targetPos;
    bool userSet = false, targetSet = false;
    float zoom = 1.0f;
    bool dragging = false;
    sf::Vector2i lastDrag;
    std::vector<TRP> trps;

    // Ballistics
    float velocity = 300.0f; // m/s, example value

    sf::Clock deltaClock;
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            ImGui::SFML::ProcessEvent(event);

            // Map interaction
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::MouseButtonPressed) {
                auto mouse = sf::Mouse::getPosition(window);
                auto world = window.mapPixelToCoords(mouse, mapView);
                if (mouse.x < 600 && mouse.y < 600) { // Only in map area
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        userPos = world;
                        userSet = true;
                    }
                    else if (event.mouseButton.button == sf::Mouse::Right) {
                        targetPos = world;
                        targetSet = true;
                    }
                    else if (event.mouseButton.button == sf::Mouse::Middle) {
                        dragging = true;
                        lastDrag = mouse;
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Middle) {
                    dragging = false;
                }
            }
            if (event.type == sf::Event::MouseMoved && dragging) {
                auto mouse = sf::Mouse::getPosition(window);
                sf::Vector2f delta = window.mapPixelToCoords(lastDrag, mapView) - window.mapPixelToCoords(mouse, mapView);
                mapView.move(delta);
                lastDrag = mouse;
            }
            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.x < 600 && event.mouseWheelScroll.y < 600) {
                    float factor = (event.mouseWheelScroll.delta > 0) ? 0.9f : 1.1f;
                    mapView.zoom(factor);
                }
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        // --- ImGui UI ---
        ImGui::SetNextWindowPos(ImVec2(620, 20), ImGuiCond_Once);
        ImGui::SetNextWindowSize(ImVec2(350, 400), ImGuiCond_Once);
        ImGui::Begin("Artillery Data");

        float dist = 0, az = 0, elevLow = 0, elevHigh = 0, tofLow = 0, tofHigh = 0;
        if (userSet && targetSet) {
            dist = ArtilleryMath::distance(userPos, targetPos);
            az = ArtilleryMath::azimuth(userPos, targetPos);
            elevLow = ArtilleryMath::elevation(dist, velocity, 9.81f, false);
            elevHigh = ArtilleryMath::elevation(dist, velocity, 9.81f, true);
            tofLow = ArtilleryMath::timeOfFlight(dist, velocity, elevLow, 9.81f);
            tofHigh = ArtilleryMath::timeOfFlight(dist, velocity, elevHigh, 9.81f);
        }
        ImGui::Text("Elevation: %.2f (low) / %.2f (high)", elevLow, elevHigh);
        ImGui::Text("Azimuth: %.2f", az);
        ImGui::Text("Distance: %.2f m", dist);
        ImGui::Text("Time of Flight: %.2f (low) / %.2f (high)", tofLow, tofHigh);
        ImGui::InputFloat("Projectile Velocity (m/s)", &velocity);

        // TRP management
        static char trpName[64] = "";
        if (ImGui::InputText("TRP Name", trpName, 64) && ImGui::IsItemDeactivatedAfterEdit()) {}
        if (ImGui::Button("Save TRP") && userSet && targetSet && strlen(trpName) > 0) {
            trps.push_back({ userPos, targetPos, trpName });
            trpName[0] = 0;
        }
        ImGui::Separator();
        ImGui::Text("TRPs:");
        for (size_t i = 0; i < trps.size(); ++i) {
            ImGui::PushID((int)i);
            if (ImGui::Button(trps[i].name.c_str())) {
                userPos = trps[i].user;
                targetPos = trps[i].target;
                userSet = targetSet = true;
            }
            ImGui::PopID();
        }
        ImGui::End();

        // --- Draw Map ---
        window.clear(sf::Color(30, 30, 30));
        window.setView(mapView);

        // Draw grid
        sf::VertexArray grid(sf::Lines);
        for (int i = 0; i <= 600; i += 60) {
            grid.append(sf::Vertex(sf::Vector2f(i, 0), sf::Color(100, 100, 100)));
            grid.append(sf::Vertex(sf::Vector2f(i, 600), sf::Color(100, 100, 100)));
            grid.append(sf::Vertex(sf::Vector2f(0, i), sf::Color(100, 100, 100)));
            grid.append(sf::Vertex(sf::Vector2f(600, i), sf::Color(100, 100, 100)));
        }
        window.draw(grid);

        // Draw user/target
        if (userSet) {
            sf::CircleShape user(8); user.setFillColor(sf::Color::Blue);
            user.setOrigin(8, 8); user.setPosition(userPos);
            window.draw(user);
        }
        if (targetSet) {
            sf::CircleShape target(8); target.setFillColor(sf::Color::Red);
            target.setOrigin(8, 8); target.setPosition(targetPos);
            window.draw(target);
        }

        window.setView(window.getDefaultView());
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}