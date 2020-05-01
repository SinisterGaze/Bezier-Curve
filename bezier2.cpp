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
		next.push_back(p);
	}
	return bezier(next, time, target, depth+1);
}
template <class T>
float distSq(T const &a, T const &b)
{
	return (a.x-b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y);
}


int main()
{
    RenderWindow window(VideoMode(800, 600), "Bezier Curve");
    window.setKeyRepeatEnabled(false);
	vector<point> points;
	vector<point> curve;
	bool done = false;
	bool finished = false;
	bool selected = false;
	bool hide = false;
	int precision = 15;
	unsigned selected_idx;
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
					if (selected)
					{
						selected = false;
						selected_idx = -1;
	
					}
					else if (!done)
					{
						Vector2i pos = Mouse::getPosition(window);
						double x = pos.x;
						double y = pos.y;
						curve.clear();
						points.push_back(point(x,y));
						finished = false;
					}
					else
					{
						Vector2i mousePos = Mouse::getPosition(window);
						float best = 1e9;
						for (unsigned int i = 0; i < points.size(); i++)
						{
							point p = points[i];
							float d = distSq(Vector2i(p.x, p.y), mousePos);
							if (d < best)
							{
								best = d;
								selected_idx = i;
							}
						}
						if (best <= 50*50)
						{
							selected = true;
						}
					}
				}
				if (Mouse::isButtonPressed(Mouse::Right))
				{
					done = !done;
					if (done)
					{
						curve.clear();
						finished = false;
					}
				}
			}
			if (event.type == Event::KeyPressed)
			{
				if (Keyboard::isKeyPressed(Keyboard::H))
				{
					hide = !hide;
				}
				if (Keyboard::isKeyPressed(Keyboard::C))
				{
					selected = false;
					points.clear();
					curve.clear();
					done = false;
					finished = false;
				}
				if (selected && Keyboard::isKeyPressed(Keyboard::D))
				{
					points.erase(points.begin() + selected_idx);
					curve.clear();
					selected = false;
					finished = false;
				}
			}
			
			
        }
        if (selected)
        {
			Vector2i mousePos = Mouse::getPosition(window);
			points[selected_idx].x = mousePos.x;
			points[selected_idx].y = mousePos.y;
			curve.clear();
			finished = false;
		}
		window.clear(Color(255,255,255));
		if (!finished)//(done && !finished)
		{
			for (unsigned it = 0; it <= points.size() * precision; it++)
			{
				float t = (float)it/(float)(points.size() * precision);
				// calculate points
				point curr;

				curr = bezier(points, t, window);
				curve.push_back(curr);
			}
			finished = true;
		}
		if (1)
		{
			
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
			
		}
		if (!hide)
		{
			for (point &p : points)
			{
				RectangleShape c(Vector2f(6,6));
				c.setFillColor(Color(0,0,0));
				c.setOrigin(3, 3);
				c.setPosition(p.x, p.y);
				window.draw(c);
			}
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
		
		
		
        
        
        window.display();
    }

    return 0;
}
