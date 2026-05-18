#include "Engine.h"

void Engine::input() {
	
	Event event;
	while (m_Window.pollEvent(event))
	{
		if (event.type == Event::KeyPressed)
		{
			// Handle the player quitting
			if (event.key.code == Keyboard::Escape)
			{
				m_Window.close();
			}

		}

		if (event.type == Event::Closed)
		{
			m_Window.close();
		}

		if (event.type == Event::MouseButtonPressed)
		{
			if (event.mouseButton.button == Mouse::Left)
			{
				std::cout << "the left button was pressed" << std::endl;
				std::cout << "mouse x(pixel): " << event.mouseButton.x << std::endl;
				std::cout << "mouse y(pixel): " << event.mouseButton.y << std::endl;
				//Particle p(m_Window, 4, { (int)event.mouseButton.x, (int)event.mouseButton.y });
				for (int i = 0; i < 5; i++)
				{
					Particle part(m_Window, rand() % 64 + 5  , { (int)event.mouseButton.x, (int)event.mouseButton.y});
					//Particle part(m_Window, 4, { (int)event.mouseButton.x, (int)event.mouseButton.y });
					
					m_particles.push_back(part);
				}

			}
		}
	}

}


void Engine::update(float dtAsSeconds) {
	for (Particle& particle : m_particles)
	{
		particle.update(dtAsSeconds);
	}
	for (int i = 0; i < m_particles.size(); i++) {
		if (m_particles[i].getTTL() <= 0.0) {
			m_particles.erase(m_particles.begin() + i);
			cout << "Particle removed, remain particles are:" << m_particles.size() << endl;
		}
	}
	
}


void Engine::draw() {

	m_Window.clear(sf::Color::Black);  // clear the window

	// Loop through each Particle
	for (Particle& particle : m_particles) {
		m_Window.draw(particle);  // polymorphism: calls Particle::draw() The RenderWindow m_Window has draw() fuction which dont do anythig, but instead call the draw from Drawable, which is the parent class of Particle, and then it calls the draw function from Particle.  This is how polymorphism works in C++.  The draw function from Particle is called because the type of the object is Particle, even though the type of the pointer is Drawable.
	}
	m_Window.display();  // show everything on screen
}


Engine::Engine() {
	int pixelWidth = VideoMode::getDesktopMode().width/2;
	int pixelHeight = VideoMode::getDesktopMode().height/2;
	VideoMode vm(pixelWidth, pixelHeight);
	m_Window.create(vm, "Particle Test", Style::Default);

}

// Run will call all the private functions
void Engine::run() {
	
	cout << "Starting Particle unit tests..." << endl;
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
	p.unitTests();
	cout << "Unit tests complete.  Starting engine..." << endl;

	Clock clock;

	// Start the game loop
	while (m_Window.isOpen())
	{
		Time time = clock.restart();
		float dt = time.asSeconds();
		//cout << "show td: " << dt << endl;
		
		// Process events
		input();
		
		// Update the state of the engine
		update(dt);
		
		// Draw everything
		draw();
	}
}


