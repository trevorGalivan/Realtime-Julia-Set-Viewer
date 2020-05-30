#include "screenBounds.h"

ScreenBounds::ScreenBounds() : centerPos_(0., 0.), size_(1., 1.) {}

ScreenBounds::ScreenBounds(glm::dvec2 centerPos, glm::dvec2 size) : centerPos_(centerPos), size_(size) {}

void ScreenBounds::setCenter(double x, double y) 
{
	setCenter(glm::dvec2(x, y));
}
void ScreenBounds::setCenter(glm::dvec2 centerPos)
{
	centerPos_ = centerPos;
}

void ScreenBounds::setSize(double x, double y)
{
	setSize(glm::dvec2(x, y));
}

void ScreenBounds::setSize(glm::dvec2 size)
{
	size_ = size;
}

void ScreenBounds::zoom(glm::dvec2 zoomPos, double zoomFactor)
{
	size_ *= zoomFactor;
	centerPos_ = zoomPos - (zoomFactor * (zoomPos - centerPos_)); // Adjusts position of window so that zoomPos stays at same position on screen
}

void ScreenBounds::translate(glm::dvec2 translationVec)
{
	centerPos_ += translationVec;
}

glm::dvec2 ScreenBounds::getCenter() const
{
	return centerPos_;
}

glm::dvec2 ScreenBounds::getSize() const
{
	return size_;
}

glm::dvec2 ScreenBounds::getLLcorner() const
{
	return centerPos_ - 0.5 * size_;
}

glm::dvec2 ScreenBounds::screenPointToWorld(glm::dvec2 screenCoords) const
{
	return glm::dvec2(screenCoords * size_ * 0.5 + centerPos_);
}

glm::dvec2 ScreenBounds::screenVecToWorld(glm::dvec2 screenCoords) const
{
	return glm::dvec2(screenCoords * size_ * 0.5);
}
