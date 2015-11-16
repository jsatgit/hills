#include <SFML/Graphics/CircleShape.hpp>

#include "monster.h"
#include "gameWindow.h"
#include "positioner.h"
#include "textCreator.h"
#include <cmath>

#include <iostream>
#include <string>

using namespace sf;
using namespace std;

Monster::Monster(int size) :
    m_hasCollision(false),
    m_size(size),
    m_gold(0),
    m_health(100),
    m_goal(Positioner::instance().generateGoal())
{
}

void Monster::setPosition(const Vector2f& position)
{
    m_position = position;
}

const sf::Vector2f& Monster::getPosition() const
{
    return m_position;
}

void Monster::setSpeed(int speed)
{
    int framerate = GameWindow::instance().getFramerate();
    m_speed = speed  > framerate ? framerate : speed;
    m_moveCounter = TurnCounter(framerate / m_speed);
}

void Monster::move()
{
    if (m_moveCounter.update()) {
        m_position = Positioner::instance().nextRandGoal(m_position, m_goal);
    }
}

void Monster::render()
{
    CircleShape circle(m_size);
    if (m_hasCollision) {
        circle.setFillColor(Color::Red);
    } else {
        circle.setFillColor(Color::Blue);
    }
    circle.setPosition(m_position);
    GameWindow::instance().render(circle);

    Text text = TextCreator::instance().createDialog(to_string(m_gold));
    text.setPosition(m_position);
    text.setCharacterSize(m_size * 8);
    GameWindow::instance().render(text);
}

bool Monster::collidesWith(const Monster& monster)
{
    const Vector2f& otherPosition = monster.getPosition();
    Vector2f diff = otherPosition - m_position;
    int combinedRadius = m_size + monster.getSize();
    int combinedRadiusSquared = combinedRadius * combinedRadius;
    int squaredDistance = diff.x * diff.x + diff.y * diff.y;
    return squaredDistance <= combinedRadiusSquared;
}

void Monster::addCollider(Monster& monster)
{
    auto res = m_colliders.insert(&monster);
    if (res.second) {
        onBeginCollisionWith(monster);
    }
}

int Monster::askGold() const
{
    return m_gold;
}

int Monster::getSize() const
{
    return m_size;
}

void Monster::clear()
{
}

void Monster::setGold(int gold)
{
    m_gold = gold;
}

void Monster::giveGold(int gold)
{
    m_gold += gold;
}

void Monster::removeCollider(Monster& monster)
{
    auto erased = m_colliders.erase(&monster);
    if (erased) {
        onEndCollisionWith(monster);
    }
}

void Monster::onBeginCollisionWith(Monster& monster) {
    m_gold++;
}

void Monster::onEndCollisionWith(Monster& monster) {
}

void Monster::interact() {
    m_hasCollision = m_colliders.empty() ? false : true;
    m_health--;
}
