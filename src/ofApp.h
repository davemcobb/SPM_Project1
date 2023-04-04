#pragma once

#include <vector>
#include "ofMain.h"
#include "cFactory.h"
#include "cCellMatrix.h"
#include "cLife.h"

class ofApp : public ofBaseApp {

public:
	// custom constructor to allow for easier initialisation
	ofApp::ofApp();

	void setup();
	void update();
	void draw();
	void exit();


	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

private:
	enum Patterns {
		Spot = 0,
		Blinker,
		Toad,
		Beacon,
		Glider,
		Peak,
		_First = Blinker,
		_Last = Peak,
	};

	void    createNewGeneration();

	const int       CELL_SIZE{ 16 };
	const int		INITIAL_RESET_COUNT { 2 };
	const int		RESET_FRAME_COUNT { 10 };
	const int		FRAME_DELAY_MS{ 1200 };
		
	// the Factory object handles creating the Life objects as required
	cFactory		m_factory;

	// the Cell Matrix own the 2d array/matrix of Life objects, and interactions with them
	cCellMatrix		m_cellMatrix{ CELL_SIZE, ofGetWidth(), ofGetHeight() };

	// prevent sim from running until cells are displayed once
	bool			m_runSim{ true };		
	// a countdown to create a new generation if no Life exists
	int				m_resetCountdown{ INITIAL_RESET_COUNT };

};
