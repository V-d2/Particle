#include "Engine.h"

void Engine::input() {

}


void Engine::update(float dtAsSeconds) {

}


void Engine::draw() {

}


Engine::Engine() {
	int pixelWidth = VideoMode::getDesktopMode().width/2;
	int pixelHeight = VideoMode::getDesktopMode().height/2;
	VideoMode vm(pixelWidth, pixelHeight);
	m_Window.create(vm, "Particle Test", Style::Default);
}

// Run will call all the private functions
void Engine::run() {

	Clock time;
	
	cout << "Starting Particle unit tests..." << endl;
	Particle p(m_Window, 4, { (int)m_Window.getSize().x / 2, (int)m_Window.getSize().y / 2 });
	p.unitTests();
	cout << "Unit tests complete.  Starting engine..." << endl;


	// Start the game loop
	while (m_Window.isOpen())
	{
		// Process events
		input();
		// Update the state of the engine
		update(1.0f / 60.0f);
		// Draw everything
		draw();
	}
}


