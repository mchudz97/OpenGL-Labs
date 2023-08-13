#ifndef __CGROUND
#define __CGROUND

#define __ALTITUDE_ERROR	99999
#include <algorithm>
#include <functional>

// -------------------------------
// Klasa reprezentujaca trojkat
class CTriangle
{
public:

	// wspolrzedne punktow trojkata
	glm::vec3 p[3];

	// rownanie plaszczyzny Ax + By + Cz + D = 0
	float A, B, C, D;

	// konstruktory
	CTriangle() { }
	CTriangle(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
	{
		p[0] = p1;
		p[1] = p2;
		p[2] = p3;

		calculateEquation();
	}

	// obliczanie wspolczynnikow rownania
	void calculateEquation(void)
	{
		glm::vec3 v1, v2;

		v1 = p[1] - p[0];
		v2 = p[2] - p[0];

		A = v1.y * v2.z - v1.z * v2.y;
		B = v1.z * v2.x - v1.x * v2.z;
		C = v1.x * v2.y - v1.y * v2.x;

		D = -(A * p[0].x + B * p[0].y + C * p[0].z);

	}

	// czy punkt p jest po lewej stronie odcinka (A, B)
	// obliczanie wyznacznika
	inline float det(glm::vec3 p, glm::vec3 A, glm::vec3 B)
	{
		return (p.x - B.x) * (A.z - B.z) - (A.x - B.x) * (p.z - B.z);
	}

	// czy punkt jest wewnatrz trojkata rzutowanego na plaszczyzne xy
	inline bool isInside(glm::vec3 point)
	{

		float d1, d2, d3;
		bool isNeg, isPos;

		d1 = det(point, p[0], p[1]);
		d2 = det(point, p[1], p[2]);
		d3 = det(point, p[2], p[0]);

		// czy jeden z wyznacznikow ma znak ujemny
		isNeg = (d1 < 0) || (d2 < 0) || (d3 < 0);
		// czy jeden z wyznacznikow ma znak dodatni
		isPos = (d1 > 0) || (d2 > 0) || (d3 > 0);

		// czy sa dodatnie i ujemne wartosci wyznacznikow
		// jesli tak, wtedy punkt nie lezy wewnatrz trojkata
		if (isNeg && isPos)
			return false;
		else
			return true;
	}

	// obliczamy wysokosc punktu w trojkacie z rownania plaszczyzny
	inline GLfloat calulateY(glm::vec3 point)
	{
		if (B) return -(A * point.x + C * point.z + D) / B;
		else {
			return __ALTITUDE_ERROR;
		}
	}

};



// ---------------------------------------
// Klasa reprezentujaca podloze
class CGround
{
public:

	std::vector<CTriangle> triangles;
	GLfloat treshold = .3f;
	void setTreshold(GLfloat tresh) 
	{
		this->treshold = tresh;
	}
	// stworzenie struktury z listy trojkatow
	// na przyklad przy okazji ladowania z pliku OBJ
	void attach(std::vector<glm::vec3> verts)
	{	
		glm::vec3 p1, p2, p3;
		std::vector<glm::vec3>::iterator it = verts.begin();
		while (it != verts.end())
		{
			p1 = *it++;
			p2 = *it++;
			p3 = *it++;

			// utworzenie nowego obiektu CTriangle
			triangles.push_back(CTriangle(p1, p2, p3));

		}
	}

	void checkMove(glm::vec3 &posToChange, glm::vec3 newPos) 
	{
		
		std::vector<CTriangle> collTriangles;
		for (auto it = triangles.begin(); it != triangles.end(); it++)
		{
			if ((*it).isInside(newPos))
			{
				collTriangles.push_back(*it);
			}
		}
		// jesli zostal znaleziony
		if (!collTriangles.empty())
		{
			std::vector<GLfloat> heights;
			for (CTriangle &c : collTriangles)
				heights.push_back(c.calulateY(newPos));
			std::sort(heights.begin(), heights.end(), [](GLfloat x, GLfloat y) {return x < y;});
			for (GLfloat f : heights)
			{
				if (f < newPos.y + treshold)
					posToChange = glm::vec3(newPos.x, f, newPos.z);
			}
		}
	}
};

#endif