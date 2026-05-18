#include "Particle.h"



Particle::Particle(RenderTarget& target, int numPoints, Vector2i mouseClickPosition) : 
    m_numPoints(numPoints), m_A(2, numPoints), m_ttl(TTL) {

    //m_centerCoordinate(mouseClickPosition.x, mouseClickPosition.y);
    


    m_cartesianPlane.setCenter(0, 0);
    //int pixelWidth = VideoMode::getDesktopMode().width;
    //int pixelHeight = VideoMode::getDesktopMode().height;
    m_cartesianPlane.setSize(target.getSize().x, (-1.0) * target.getSize().y);
    
    m_centerCoordinate = target.mapPixelToCoords(mouseClickPosition, m_cartesianPlane); // it converts the display coordinates to cartesian plane coordinate, and save it to m_centerCoordinate to represent the position of particle on the m_cartesianPlane
    cout << "converted coordinates x:" << m_centerCoordinate.x << "y: " << m_centerCoordinate.y << endl;


	m_color1 = Color::White;
    m_color2 = Color::Blue;
    m_vx = (rand() % 501 + 100) * ((rand() % 2) ? -1.0 : 1.0); // initial velocity
	m_vy = (rand() % 501 + 100) * ((rand() % 2) ? -1.0 : 1.0); // 
	m_radiansPerSec = (rand() / (float)(RAND_MAX) * 2 - 1) * M_PI; // 180 degrees per second both sides. to make it negative or positive. (rand() / (float)(RAND_MAX) * 2)  give  0.0 to 2.0 a float. so if -1 the we get  -1.0 to 1.0 random persentage
	cout << "Initial radians per second: " << m_radiansPerSec << endl;

    float theta = M_PI / 4;
    float dTheta = (2 * M_PI) / (m_numPoints - 1);

    for (size_t j = 0; j < m_numPoints - 1; j++) { // dont assign the last point, because it assigned with the first itration, so radius is same, and so points of first and last of Matrix coinsides
        size_t  r = rand() % 101 + 20;
        int  dx = r * cos(theta + j * dTheta);
        int  dy = r * sin(theta + j * dTheta);
        m_A(0, j) = m_centerCoordinate.x + dx;
        m_A(1, j) = m_centerCoordinate.y + dy;
        if (j == 0) {  // this is to assign the first poit with the last  poit, so the points coincide
            m_A(0, m_numPoints - 1) = m_centerCoordinate.x + dx;
            m_A(1, m_numPoints - 1) = m_centerCoordinate.y + dy;
        }
    }
}



///rotate Particle by theta radians counter-clockwise
///construct a RotationMatrix R, left mulitply it to m_A
void Particle::rotate(double theta) {
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y); // translate the particle to origin
    RotationMatrix R(theta);
    m_A = R * m_A; // the R here represent the rotate matrix 2x2, which used to rotate the vertices
    translate(temp.x, temp.y); // translate the particle back

    //RotationMatrix R(theta);  // local rotation matrix
    //m_A = R * m_A;             // apply rotation
}

///Scale the size of the Particle by factor c
///construct a ScalingMatrix S, left multiply it to m_A
void Particle::scale(double c) {
    Vector2f temp = m_centerCoordinate;
    translate(-m_centerCoordinate.x, -m_centerCoordinate.y);
    ScalingMatrix S(c);
    m_A = S * m_A;
    translate(temp.x, temp.y);
}

///shift the Particle by (xShift, yShift) coordinates
///construct a TranslationMatrix T, add it to m_A
void Particle::translate(double xShift, double yShift) {
    TranslationMatrix T(xShift, yShift, m_numPoints);
    m_A = T + m_A;  // add the the xShift and yShift in every Matrix m_A.x and m_A.y
    m_centerCoordinate.x += xShift;
    m_centerCoordinate.y += yShift;
}


/*
m_Window.draw(particle)
->RenderTarget::draw(const Drawable& drawable)
->drawable.draw(*this, states)  // virtual call
-> Particle::draw(RenderTarget& target, RenderStates states)
*/


void Particle::draw(RenderTarget& target, RenderStates states) const {
	sf::VertexArray lines(sf::TriangleFan, m_numPoints + 1); // convert part to a VertexArray for drawing/ + 1 because plus the center 
    
    Vector2f center = (Vector2f)target.mapCoordsToPixel(m_centerCoordinate, m_cartesianPlane);
    lines[0].position = center;
    lines[0].color = m_color1;

    for (size_t j = 1; j <= m_numPoints; j++) {
        lines[j].position = (Vector2f)target.mapCoordsToPixel( Vector2f (m_A(0, j-1), m_A(1, j-1)), m_cartesianPlane);
        //lines[j].position = Vector2f(m_A(0, j - 1), m_A(1, j - 1));
        lines[j].color = m_color2;
    }
    target.draw(lines, states);  // draw vertex array
}



void Particle::update(float dt) {
    m_ttl -= dt;
    rotate(dt * m_radiansPerSec);
    scale(SCALE);
    float dx = m_vx * dt; // determiane the distance per frame
    m_vy -= G * dt;
    float dy = m_vy * dt; // accelerating the down speed and shift farther to the y down
    translate(dx, dy);
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