#pragma once

#include <glm/vec2.hpp>
class ScreenBounds
{
	glm::dvec2 centerPos_;
	glm::dvec2 size_;
public:
	ScreenBounds();
	ScreenBounds(glm::dvec2 centerPos, glm::dvec2 size);
	
	void setCenter(double x, double y);
	void setCenter(glm::dvec2 centerPos);

	void setSize(double x, double y);
	void setSize(glm::dvec2 size);

	void zoom(glm::dvec2 zoomPos, double zoomFactor);
	void translate(glm::dvec2 translationVec);

	glm::dvec2 getCenter() const;
	glm::dvec2 getSize() const;
	glm::dvec2 getLLcorner() const; // Gets location of lower-left corner in world space

	glm::dvec2 screenPointToWorld(glm::dvec2 screenCoords) const; // Transforms a point in screen coords to world coords ()
	glm::dvec2 screenVecToWorld(glm::dvec2 screenCoords) const; // Transforms a vector in screen coords to world coords ()
};

