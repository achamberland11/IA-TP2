//
// Created by Antoine on 2026-03-09.
//

#pragma once
#include "Component.h"
#include "SFML/Graphics/RectangleShape.hpp"
#include "SFML/Graphics/RenderWindow.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"
#include "SFML/Window/Context.hpp"


class GRendererComponent : public GComponent
{
public:
    GENERATE_COMPONENT(false, true, false)
    GRendererComponent(GEntity* owner) : GComponent(owner)
    {
        Name = "Renderer";
        Color = sf::Color::White;
        Shape = sf::RectangleShape({16.f, 16.f});
        Shape.setFillColor(Color);
        Shape.setPosition(sf::Vector2f(0, 0));
        Shape.setScale(sf::Vector2f(1, 1));
    }
    ~GRendererComponent() override {
        // delete Sprite;
    }

    void Start() override;
    void Update(float deltaSeconds) override;

    void Render(sf::RenderWindow& window);

    sf::RectangleShape& GetShape() { return Shape; }
    void SetColor(const sf::Color& color) { Color = color; }
    void SetSprite(const sf::Sprite& sprite, std::string name) { Sprite = std::make_unique<sf::Sprite>(sprite); SpriteName = name; }

    std::string GetSpriteName() { return SpriteName; }
    sf::Sprite* GetSprite() { return Sprite.get(); }

private:
    /*const std::unique_ptr<::sf::Texture> Texture;
    sf::Sprite* Sprite;*/
    sf::RectangleShape Shape;
    sf::Color Color;
    std::unique_ptr<sf::Sprite> Sprite;
    std::string SpriteName;
};
