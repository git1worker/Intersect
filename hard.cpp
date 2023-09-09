#include <iostream>

struct Vector2f {
    Vector2f(float x, float y) : x(x), y(y) {}
    float x;
    float y;
};

struct Ray {
    Ray(Vector2f pos, Vector2f direction) : pos(pos), direction(direction) {}
    Vector2f pos;
    Vector2f direction;
};

struct Section {
    Section(float x1, float y1, float x2, float y2) : x1(x1), y1(y1), x2(x2), y2(y2) {}
    float x1, y1;
    float x2, y2;
};

bool isPointOnTheRay(Vector2f point, Ray ray) {
    // Нужна проверка, если линия вертикальная, то есть если ray.direction.x == 0
    if (ray.direction.x <= 0.1 && ray.direction.x >= -0.1) {
        // Если точка принадлежит прямой
        if (point.x + 0.1 >= ray.pos.x && point.x - 0.1 <= ray.pos.x) {
            // Луч направлен в вверх относительно y
            if (ray.direction.y > 0) {
                if (point.y > ray.pos.y)
                    return true;

            }
            // Луч направлен в вниз относительно y
            else {
                if (point.y < ray.pos.y)
                    return true;
            }
        }
    } else {
        float k = ray.direction.y / ray.direction.x;
        float b = ray.pos.y - k * ray.pos.x;
        float assumedY = k * point.x + b;
        float assumedX = (point.y - b) / k;
        // Если координаты У совпадают у точки и луча
        if (point.y + 0.1 >= assumedY && point.y - 0.1 <= assumedY) {
            // График стремится вниз относительно y
            if (ray.direction.y < -0.1) {
                if (assumedY > ray.pos.y)
                    return false;
                else
                    return true;
            }
            // График стремится вверх относительно у
            if (ray.direction.y > 0.1) {
                if (assumedY < ray.pos.y)
                    return false;
                else
                    return true;
            }
            // График постоянен
            if (ray.direction.y < 0.1 && ray.direction.y > -0.1) {
                // График направлен в сторону возрастания относительно х
                if (ray.direction.x > 0) {
                    if (assumedX < ray.pos.x)
                        return false;
                    else
                        return true;
                }
                // График направлен в сторону убывания относительно х
                else {
                    if (assumedX > ray.pos.x)
                        return false;
                    else
                        return true;
                }
            }
        }
    }

    return false;
}



bool intersectionRayAndSection(const Ray& ray, const Section& sect){
    // Если и луч и отрезок вертикальные
    if (ray.direction.x <= 0.1 && ray.direction.x >= -0.1 && std::abs(sect.x1 - sect.x2) <= 0.1 && std::abs(sect.x1 - sect.x2) >= -0.1)
        return true;
    // Если луч вертикальный
	if (ray.direction.x <= 0.1 && ray.direction.x >= -0.1){
	    float kS = (sect.y1 - sect.y2) / (sect.x1 - sect.x2);
    	float bS = sect.y1 - kS * sect.x1;
    	float intersectY = kS * ray.pos.x + bS;
    	Vector2f p{ray.pos.x, intersectY};
    	if (isPointOnTheSection(p, sect) && isPointOnTheRay(p, ray))
    	    return true;
	}
	// Если отрезок вертикальный
	if (std::abs(sect.x1 - sect.x2) <= 0.1 && std::abs(sect.x1 - sect.x2) >= -0.1)
	{
	    float kR = ray.direction.y / ray.direction.x;
    	float bR = ray.pos.y - kR * ray.pos.x;
    	float intersectY = kR * sect.x1 + bR;
    	Vector2f p{sect.x1, intersectY};
    	if (isPointOnTheSection(p, sect) && isPointOnTheRay(p, ray))
    	    return true;
	}
	else{
	    
	    float kS = (sect.y1 - sect.y2) / (sect.x1 - sect.x2);
    	float bS = sect.y1 - kS * sect.x1;
    	float kR = ray.direction.y / ray.direction.x;
    	float bR = ray.pos.y - kR * ray.pos.x;
        // 	std::cout << kR << ' ' << bR << ' '
        // 	<< kS << ' ' << bS << std::endl;
    	if (std::abs(kS - kR) >= 0.1 || std::abs(kS - kR) <= -0.1){
    	    float intersectX = (bR - bS) / (kS - kR);
        	float intersectY = kS * intersectX + bS;
        	Vector2f p{intersectX, intersectY};
        	if (isPointOnTheSection(p, sect) && isPointOnTheRay(p, ray))
        	    return true;
    	}
	}
	return false;
}

bool isPointOnTheSection(Vector2f point, Section sect){
    // Нужна проверка, если линия вертикальная, то есть если ray.direction.x == 0
    if (std::abs(sect.x1 - sect.x2) <= 0.1 && std::abs(sect.x1 - sect.x2) >= -0.1){
        // Если точка принадлежит прямой
        if (point.x + 0.1 >= sect.x1 && point.x - 0.1 <= sect.x1){
            // Если точка принадлежит отрезку
            if ((point.y >= sect.y1 && point.y <= sect.y2) || (point.y >= sect.y2 && point.y <= sect.y1)){
                return true;
            }
        }
    }
    else {
        float k = (sect.y1 - sect.y2) / (sect.x1 - sect.x2);
    	float b = sect.y1 - k * sect.x1;
    	float assumedY = k * point.x + b;
    	float assumedX = (point.y - b) / k;
    	// Если координаты У совпадают у точки и прямой
    	if (point.y + 0.1 >= assumedY && point.y - 0.1 <= assumedY)
    	{   
	        if ((point.x >= sect.x1 && point.x <= sect.x2) || (point.x >= sect.x2 && point.x <= sect.x1))
	            return true;
    	}
    	
    }
	
	return false;
}
bool intersectionSectionAndSection(const Section sect1, const Section sect2){
    // Если отрезок1 и отрезок2 вертикальные
    if (std::abs(sect1.x1 - sect1.x2) <= 0.1 && std::abs(sect1.x1 - sect1.x2) >= -0.1 && 
    std::abs(sect2.x1 - sect2.x2) <= 0.1 && std::abs(sect2.x1 - sect2.x2) >= -0.1)
        return false;
    // Если отрезок1 вертикальный
	else if (std::abs(sect1.x1 - sect1.x2) <= 0.1 && std::abs(sect1.x1 - sect1.x2) >= -0.1){
	    float kS = (sect2.y1 - sect2.y2) / (sect2.x1 - sect2.x2);
    	float bS = sect2.y1 - kS * sect2.x1;
    	float intersectY = kS * sect1.x1 + bS;
    	Vector2f p{sect1.x1, intersectY};
    	if (isPointOnTheSection(p, sect1) && isPointOnTheSection(p, sect2))
    	    return true;
	}
	// Если отрезок2 вертикальный
	else if (std::abs(sect2.x1 - sect2.x2) <= 0.1 && std::abs(sect2.x1 - sect2.x2) >= -0.1)
	{
	    float kS = (sect1.y1 - sect1.y2) / (sect1.x1 - sect1.x2);
    	float bS = sect1.y1 - kS * sect1.x1;
    	float intersectY = kS * sect2.x1 + bS;
    	Vector2f p{sect2.x1, intersectY};
    	if (isPointOnTheSection(p, sect1) && isPointOnTheSection(p, sect2))
    	    return true;
	}
	else{
	    float kS1 = (sect1.y1 - sect1.y2) / (sect1.x1 - sect1.x2);
    	float bS1 = sect1.y1 - kS1 * sect1.x1;
    	float kS2 = (sect2.y1 - sect2.y2) / (sect2.x1 - sect2.x2);
    	float bS2 = sect2.y1 - kS2 * sect2.x1;
        // 	std::cout << kR << ' ' << bR << ' '
        // 	<< kS << ' ' << bS << std::endl;
    	if (std::abs(kS1 - kS2) >= 0.1 || std::abs(kS1 - kS2) <= -0.1){
    	    float intersectX = (bS2 - bS1) / (kS1 - kS2);
        	float intersectY = kS1 * intersectX + bS1;
        	Vector2f p{intersectX, intersectY};
        	if (isPointOnTheSection(p, sect1) && isPointOnTheSection(p, sect2))
        	    return true;
    	}
	}
	return false;
}

int main()
{   
	Section s1{1408, 192, 1440, 224};
	Section s2{1424, 142, 1419, 513.7897};
	
	cout<<intersectionSectionAndSection(s1, s2)<< std::endl;
    
	return 0;
	
}
