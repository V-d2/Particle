#include "Particle.h"



Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : 
    m_numPoints(numPoints), m_A(2, numPoints), m_centerCoordinate(mouseClickPosition.x, mouseClickPosition.y), 
    m_ttl(TTL) {



    m_cartesianPlane.setCenter(0, 0); // view object
    //int pixelWidth = VideoMode::getDesktopMode().width;
    //int pixelHeight = VideoMode::getDesktopMode().height;
    m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y); // viw object

	m_color1 = Color::Red;
    m_color2 = Color::Blue;
	m_vx = 0;
	m_vy = 0;
	m_radiansPerSec = (rand() / (float)(RAND_MAX) * 2 - 1) * M_PI; // 180 degrees per second both sides. to make it negative or positive. (rand() / (float)(RAND_MAX) * 2)  give  0.0 to 2.0 a float. so if -1 the we get  -1.0 to 1.0 random persentage
	cout << "Initial radians per second: " << m_radiansPerSec << endl;


    // Minimal: assign points in a line (unit test just checks 2xN matrix exists)
    for (int i = 0; i < m_numPoints; ++i) {
        float angle = ( 2 * M_PI * i) / m_numPoints;
		
        //int radius = rand() % 100;  // radius of
        // 
        // 
        // the circle
        int radius = 100;  // radius of the circle

        m_A(0, i) = radius * cos(angle);  // x      m_A(0, x) this is for all x   m_A(0, 0) is the center for x
        m_A(1, i) = radius * sin(angle);  // y      m_A(1, y) this is for all y   m_A(0, 1) is the center for y
    }
}



///rotate Particle by theta radians counter-clockwise
///construct a RotationMatrix R, left mulitply it to m_A
void Particle::rotate(double theta) {
    RotationMatrix R(theta);  // local rotation matrix
    m_A = R * m_A;             // apply rotation
}

///Scale the size of the Particle by factor c
///construct a ScalingMatrix S, left multiply it to m_A
void Particle::scale(double c) {
    ScalingMatrix S(c);
    m_A = S * m_A;
}

///shift the Particle by (xShift, yShift) coordinates
///construct a TranslationMatrix T, add it to m_A
void Particle::translate(double xShift, double yShift) {
    TranslationMatrix T(xShift, yShift, m_numPoints);
    m_A = T + m_A;
}


/*
m_Window.draw(particle)
->RenderTarget::draw(const Drawable& drawable)
->drawable.draw(*this, states)  // virtual call
-> Particle::draw(RenderTarget& target, RenderStates states)
*/

void Particle::draw(RenderTarget& target, RenderStates states) const {
	sf::VertexArray Vertex_Array(sf::TriangleFan, m_numPoints + 1); // convert part to a VertexArray for drawing

    // Center vertex
    Vertex_Array[0].position = m_centerCoordinate; // because we put the first vertex as snter, later do 
    Vertex_Array[0].color = sf::Color::White;
    // Vertices from m_A matrix
    for (int i = 0; i < m_numPoints; ++i) {
        Vertex_Array[i + 1].position = sf::Vector2f( m_centerCoordinate.x + m_A(0, i), m_centerCoordinate.y + m_A(1, i));
        Vertex_Array[i + 1].color = sf::Color::Blue;
    }

    target.draw(Vertex_Array, states);  // draw vertex array
}





bool Particle::almostEqual(double a, double b, double eps)
{
	return fabs(a - b) < eps;
}

void Particle::unitTests()
{
    int score = 0;

    cout << "Testing RotationMatrix constructor...";
    double theta = M_PI / 4.0;
    RotationMatrix r(M_PI / 4);
    if (r.getRows() == 2 && r.getCols() == 2 && almostEqual(r(0, 0), cos(theta))
        && almostEqual(r(0, 1), -sin(theta))
        && almostEqual(r(1, 0), sin(theta))
        && almostEqual(r(1, 1), cos(theta)))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing ScalingMatrix constructor...";
    ScalingMatrix s(1.5);
    if (s.getRows() == 2 && s.getCols() == 2
        && almostEqual(s(0, 0), 1.5)
        && almostEqual(s(0, 1), 0)
        && almostEqual(s(1, 0), 0)
        && almostEqual(s(1, 1), 1.5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Testing TranslationMatrix constructor...";
    TranslationMatrix t(5, -5, 3);
    if (t.getRows() == 2 && t.getCols() == 3
        && almostEqual(t(0, 0), 5)
        && almostEqual(t(1, 0), -5)
        && almostEqual(t(0, 1), 5)
        && almostEqual(t(1, 1), -5)
        && almostEqual(t(0, 2), 5)
        && almostEqual(t(1, 2), -5))
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    
    cout << "Testing Particles..." << endl;
    cout << "Testing Particle mapping to Cartesian origin..." << endl;
    if (m_centerCoordinate.x != 0 || m_centerCoordinate.y != 0)
    {
        cout << "Failed.  Expected (0,0).  Received: (" << m_centerCoordinate.x << "," << m_centerCoordinate.y << ")" << endl;
    }
    else
    {
        cout << "Passed.  +1" << endl;
        score++;
    }

    cout << "Applying one rotation of 90 degrees about the origin..." << endl;
    Matrix initialCoords = m_A;
    rotate(M_PI / 2.0);
    bool rotationPassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), -initialCoords(1, j)) || !almostEqual(m_A(1, j), initialCoords(0, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            rotationPassed = false;
        }
    }
    if (rotationPassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a scale of 0.5..." << endl;
    initialCoords = m_A;
    scale(0.5);
    bool scalePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 0.5 * initialCoords(0,j)) || !almostEqual(m_A(1, j), 0.5 * initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            scalePassed = false;
        }
    }
    if (scalePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Applying a translation of (10, 5)..." << endl;
    initialCoords = m_A;
    translate(10, 5);
    bool translatePassed = true;
    for (int j = 0; j < initialCoords.getCols(); j++)
    {
        if (!almostEqual(m_A(0, j), 10 + initialCoords(0, j)) || !almostEqual(m_A(1, j), 5 + initialCoords(1, j)))
        {
            cout << "Failed mapping: ";
            cout << "(" << initialCoords(0, j) << ", " << initialCoords(1, j) << ") ==> (" << m_A(0, j) << ", " << m_A(1, j) << ")" << endl;
            translatePassed = false;
        }
    }
    if (translatePassed)
    {
        cout << "Passed.  +1" << endl;
        score++;
    }
    else
    {
        cout << "Failed." << endl;
    }

    cout << "Score: " << score << " / 7" << endl;
}