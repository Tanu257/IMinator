#define _USE_MATH_DEFINES

#include "imgui.h"
#include "imgui-SFML.h"
#include <vector>
#include "SFML/Graphics.hpp"
#include <iostream>
#include <string>
#include <math.h>
#include <cmath>

void OnAddButtonClick(const char* toCreateShape, std::vector<sf::CircleShape> &spawnedObjects_Circles, std::vector<sf::RectangleShape> &spawnedObjects_Rects,sf::Vector2i &spawnedObject_Indexes,sf::Vector2f pivote) {
    if (toCreateShape == "Circle")
    {
        sf::CircleShape shape(100,100);
        shape.setOrigin(100,100);
        shape.setPosition(pivote);
       
        spawnedObjects_Circles.push_back(shape);

        spawnedObject_Indexes.x += 1;
        

    }
    if (toCreateShape == "Rect")
    {
        sf::RectangleShape rect(sf::Vector2f(100,100));

        rect.setPosition(pivote);
        rect.setOrigin(50, 50);
        spawnedObjects_Rects.push_back(rect);

        spawnedObject_Indexes.y += 1;

    }
}
    

int main()
{
    sf::RenderWindow window(sf::VideoMode(1600, 800), "IMinator");
    ImGui::SFML::Init(window);

    sf::Clock deltaClock;

    std::vector<sf::CircleShape> spawnedObjects_Circles;
    std::vector<sf::RectangleShape> spawnedObjects_Rects;

    sf::Vector2i MaxspawnedObject_Indexes;
    MaxspawnedObject_Indexes.x = 0;
    MaxspawnedObject_Indexes.y = 0;

    sf::Vector2i SelectedspawnedObject_Indexes;
    SelectedspawnedObject_Indexes.x = 0;
    SelectedspawnedObject_Indexes.y = 0;

    sf::Vector2f pivote;
    pivote.x = 500;
    pivote.y = 500;

    const char* creatorShapes[] = {"Circle","Rect"};
    static const char* toCreateShape = creatorShapes[0];
    static const char* shapePropertySelected = creatorShapes[0];

    sf::Font font;

    font.loadFromFile("F:\\Tirth\\download\\Aller\\Aller_Bd.ttf");

    sf::Text text;
    text.setFont(font); 
    text.setOrigin(sf::Vector2f(6,7));
    text.setCharacterSize(16);
    text.setFillColor(sf::Color::Red);

    // For Test

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            if (event.type == sf::Event::Closed)
                window.close();
        }
        window.clear(sf::Color::Black);

        ImGui::SFML::Update(window, deltaClock.restart());

        // SFML
        {
            for (int i = 0; i < spawnedObjects_Circles.size(); i++)
            {
                window.draw(spawnedObjects_Circles[i]);
                text.setString(std::to_string(i));
                text.setPosition(spawnedObjects_Circles[i].getPosition());
                window.draw(text);
            }
            for (int i = 0; i < spawnedObjects_Rects.size(); i++)
            {
                window.draw(spawnedObjects_Rects[i]);
                text.setString(std::to_string(i));
                text.setPosition(spawnedObjects_Rects[i].getPosition());
                window.draw(text);
            }
            window.draw(text);

        }
        // IMGUI
        {
            // Herianchery
            {
                ImGui::Begin("Herianchery");
                
                if (ImGui::BeginCombo("##combo", toCreateShape)) // The second parameter is the label previewed before opening the combo.
                {
                    for (int n = 0; n < IM_ARRAYSIZE(creatorShapes); n++)
                    {
                        bool is_selected = (toCreateShape == creatorShapes[n]); // You can store your selection however you want, outside or inside your objects
                        if (ImGui::Selectable(creatorShapes[n], is_selected)) {
                            toCreateShape = creatorShapes[n];
                            
                            if (is_selected){

                                ImGui::SetItemDefaultFocus();
                            }  // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                        }
                    }
                    ImGui::EndCombo();
                }

                if (ImGui::Button("Add"))
                {
                    OnAddButtonClick(toCreateShape,spawnedObjects_Circles,spawnedObjects_Rects,MaxspawnedObject_Indexes,pivote);
                }
                ImGui::End();
            }

            // Properties
            {
                ImGui::Begin("Properties");

                // Selector
                {
                    ImGui::Text("Circle Selector");
                    ImGui::SliderInt("##Circle", &SelectedspawnedObject_Indexes.x, 0, MaxspawnedObject_Indexes.x-1);

                    ImGui::Text("Rect Selector");
                    ImGui::SliderInt("##Rect", &SelectedspawnedObject_Indexes.y, 0, MaxspawnedObject_Indexes.y-1);

                    if (ImGui::BeginCombo("##combo", shapePropertySelected)) // The second parameter is the label previewed before opening the combo.
                    {
                        for (int n = 0; n < IM_ARRAYSIZE(creatorShapes); n++)
                        {
                            bool is_selected = (shapePropertySelected == creatorShapes[n]); // You can store your selection however you want, outside or inside your objects
                            if (ImGui::Selectable(creatorShapes[n], is_selected)) {
                                shapePropertySelected = creatorShapes[n];

                                if (is_selected) {

                                    ImGui::SetItemDefaultFocus();
                                }  // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                            }
                        }
                        ImGui::EndCombo();
                    }
                }
                // Inner transforms
                {
                        // Circle
                        if (shapePropertySelected == "Circle" && spawnedObjects_Circles.size() > 0)
                        {
                            ImGui::Text("Color");
                            sf::CircleShape *circle = &spawnedObjects_Circles[SelectedspawnedObject_Indexes.x];
                            float ContextColor[3] = {
                                (float)circle->getFillColor().r / 255,
                                (float)circle->getFillColor().g / 255,
                                (float)circle->getFillColor().b / 255,
                            };
                            ImGui::ColorPicker3("##c12", ContextColor, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

                            circle->setFillColor(sf::Color(
                                ContextColor[0] * 255,
                                ContextColor[1] * 255,
                                ContextColor[2] * 255

                            ));
                            {
                                float x = circle->getPosition().x;
                                float y = circle->getPosition().y;

                                ImGui::Text("Transform");

                                ImGui::DragFloat("x", &x);
                                ImGui::DragFloat("y", &y);
                                circle->setPosition(sf::Vector2f(x, y));
                            }
                            {
                                ImGui::Text("Dimension");

                                float radius = circle->getRadius();

                                ImGui::SliderFloat("Radius", &radius,0.1,500);

                                circle->setOrigin(sf::Vector2f(radius, radius));
                                circle->setRadius(radius);

                            }

                        }

                        // Rect
                        else if (shapePropertySelected == "Rect" && spawnedObjects_Rects.size() > 0)
                        {
                            ImGui::Text("Color");
                            sf::RectangleShape* rectShape = &spawnedObjects_Rects[SelectedspawnedObject_Indexes.y];
                            float ContextColor[3] = {
                                (float)rectShape->getFillColor().r / 255,
                                (float)rectShape->getFillColor().g / 255,
                                (float)rectShape->getFillColor().b / 255,
                            };
                            ImGui::ColorPicker3("##c12", ContextColor, ImGuiColorEditFlags_NoSidePreview | ImGuiColorEditFlags_NoSmallPreview);

                            rectShape->setFillColor(sf::Color(
                                ContextColor[0] * 255,
                                ContextColor[1] * 255,
                                ContextColor[2] * 255

                            ));

                            {
                                float x = rectShape->getPosition().x;
                                float y = rectShape->getPosition().y;

                                ImGui::Text("Transform");

                                ImGui::DragFloat("x", &x);
                                ImGui::DragFloat("y", &y);
                                rectShape->setPosition(sf::Vector2f(x, y));
                            }
                            {
                                ImGui::Text("Dimension");

                                sf::Vector2f rect_size = rectShape->getSize();

                                ImGui::SliderFloat("Width", &rect_size.x, 0.1, 500);
                                ImGui::SliderFloat("Height", &rect_size.y, 0.1, 500);

                                rectShape->setOrigin(rect_size / 2.f);
                                rectShape->setSize(rect_size);
                            }
                            {
                                ImGui::Text("Rotation");

                                float rotation = rectShape->getRotation() * M_PI/180;

                                ImGui::SliderAngle("Angle",&rotation);

                                rectShape->setRotation(rotation * 180 / M_PI);
                            }

                        }
                }
                
                ImGui::End();
            }
        }

        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}