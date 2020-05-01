#include <SFML/Graphics.hpp>
#include <bits/stdc++.h>
#include "RoundendedLine.h"
#include "RoundendedLine.cpp"

using namespace std;
using namespace sf;

class point
{
	public:
		double x;
		double y;
		point(double _x, double _y)
		{
			x = _x;
			y = _y;
		};
		point()
		{
			x = 0;
			y = 0;
		}
		point operator-(const point &other)
		{
			return point(this->x - other.x, this->y - other.y);
		}
		point operator+(const point &other)
		{
			return point(this->x + other.x, this->y + other.y);
		}
		point operator*(const double &c)
		{
			return point(this->x * c, this->y * c);
		}
		void operator*=(const double &c)
		{
			*this = *this * c;
		}
		double operator * (const point&other)
		{
			return this->x * other.x + this->y * other.y;
		}
		void operator = (const point &other)
		{
			this->x = other.x, this->y = other.y;
		}
		double lengthsq()
		{
			return *this * *this;
		}
		double length()
		{
			return sqrt(lengthsq());
		}
		void print()
		{
			printf("(%.5f, %.5f)\n", this->x, this->y);
		}
			
};

Color hsv2rgb(float &H, float &X, float &C, float &m)
{
	float R, G, B;
	if (H < 60)
	{
		R = C, G = X, B = 0;
	}
	else if (H < 120)
	{
		R = X, G = C, B = 0;
	}
	else if (H < 180)
	{
		R = 0, G = C, B = X;
	}
	else if (H < 240)
	{
		R = 0; G = X, B = C;
	}
	else if (H < 300)
	{
		R = X, G = 0, B = C;
	}
	else if (H < 360)
	{
		R = C, G = 0, B = X;
	}
	return Color((R + m) * 255, (G + m) * 255, (B + m) * 255);
}


point bezier(vector<point> &points, float &time, RenderWindow &target, int depth = 0)
{
	unsigned int n = points.size();
	if (n == 0) return point();
	if (n == 1) return points[0];
	vector<point> next;
	for (unsigned i = 0; i < n-1; i++)
	{
		point a = points[i+1], b = points[i];
		point p = b + (a-b) * time;
		if (i > 0)
		{
			int dec = min((depth+1)*40, 230);
			Color c = Color(dec, dec, dec);
			Vertex line[] =
			{
				Vertex(Vector2f(next[i-1].x, next[i-1].y), c),
				Vertex(Vector2f(p.x, p.y), c)
			};
			target.draw(line, 2, Lines);
		}
		next.push_back(p);
	}
	return bezier(next, time, target, depth+1);
}


int main()
{
    RenderWindow window(VideoMode(1920, 1080), "Bezier Curve");
    bool limitedFPS = true;
    int FPS = 60;
    if (limitedFPS)
		window.setFramerateLimit(FPS);
	vector<point> points;
	vector<point> curve;
	bool done = false;
	bool finished = false;
	int f = 0;
	int T = 0;
	int duration = 3;
	if (limitedFPS) T = FPS * duration;
	point prev;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::MouseButtonPressed)
            {
				if (Mouse::isButtonPressed(Mouse::Left))
				{
					if (!done)
					{
						Vector2i pos = Mouse::getPosition(window);
						double x = pos.x;
						double y = pos.y;
						points.push_back(point(x,y));
					}
					else
					{
						done = false;
						points.clear();
						curve.clear();
						f = 0;
						finished = false;
					}
				}
				if (Mouse::isButtonPressed(Mouse::Right))
				{
					if (points.size() > 0) prev = points[0];
					done = true;
					if (!limitedFPS) T = points.size() * 50;
				}
			}
        }
		window.clear(Color(255,255,255));
        
        if (!finished)
			{
				for (unsigned int i = 1; i < points.size(); i++)
				{
					point a = points[i], b = points[i-1];
					Vertex line[] =
					{
						Vertex(Vector2f(b.x, b.y), Color(0, 0, 0)),
						Vertex(Vector2f(a.x, a.y), Color(0, 0, 0))
					};
					window.draw(line, 2, Lines);
				}
			}
        
		if (done)
		{
			float t = (float)f/(float)(T);
			finished = f > T ? true : false;
			// calculate points
			point curr;
			if (!finished)
			{
				curr = bezier(points, t, window);
				curve.push_back(curr);
			}
			
			if (curve.size() > 1)
			{
				for (unsigned int i = 1; i < curve.size(); i++)
				{
					point a = curve[i], b = curve[i-1];
					CRoundendedLine line;
					line.setPosition(b.x, b.y);
					line.setEndPoint(Vector2f(a.x, a.y));
					line.setWidth(4);
					line.setFillColor(Color(Color::Red));
					window.draw(line);
				}
			}
			f++;	
			prev = curr;
		}
		if (!finished)
		{
			for (point &p : points)
			{
				RectangleShape c(Vector2f(6,6));
				c.setFillColor(Color(0,0,0));
				c.setOrigin(3, 3);
				c.setPosition(p.x, p.y);
				window.draw(c);
			}
		}
		
		
        
        
        window.display();
    }

    return 0;
}
