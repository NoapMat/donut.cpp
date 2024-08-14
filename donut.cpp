#include <iostream>
#include <cmath>
#include <thread>
#include <chrono>

using namespace std;

const int WIDTH = 80;
const int HEIGHT = 22;
const int BUFFER_SIZE = WIDTH * HEIGHT;

// draw donut or called "torus" in geometry
void drawTorus(double& angleA, double& angleB) {
    char buffer[BUFFER_SIZE];
    double depthBuffer[BUFFER_SIZE];

    // initialize buffer as well as Z-buffer, we could've use memset here (much more cleaner to look at. but ok
    for (int i = 0; i < BUFFER_SIZE; i++) {
        buffer[i] = ' ';
        depthBuffer[i] = 0;
    }

    // rotation angles increments, (rotation around axis)
    angleA += 0.10;
    angleB += 0.06;

    double cosA = cos(angleA), sinA = sin(angleA);
    double cosB = cos(angleB), sinB = sin(angleB);

    // 3d space to project 2d screen
    // uses M_PI constant (in cmath library) which equates to 3.14, we need radian so we multiply it by 2
    for (double theta = 0; theta < 2 * M_PI; theta += 0.10) {
        double cosTheta = cos(theta), sinTheta = sin(theta);
        for (double phi = 0; phi < 2 * M_PI; phi += 0.05) {
          	// assign cosphi, and senpai, oh my god
            double cosPhi = cos(phi), sinPhi = sin(phi);

            double circleX = cosTheta + 2;  // circ x coords
            double circleY = sinTheta;      // circ y coords
            // ooz...
            double oneOverZ = 1 / (sinPhi * circleX * sinA + circleY * cosA + 5);
            // intermediate value used to calculate 3D coordinates
            // factors x' and y'
            double t = sinPhi * circleX * cosA - circleY * sinA;
            
			      // the x and y projections
            int x = int(WIDTH / 2 + 30 * oneOverZ * (cosPhi * circleX * cosB - t * sinB));
            int y = int(HEIGHT / 2 + 15 * oneOverZ * (cosPhi * circleX * sinB + t * cosB));
            // precompute index here for cleanliness
            int index = x + WIDTH * y;
            
            // luminance index
            int luminanceIndex = int(8 * ((sinTheta * sinA - sinPhi * cosTheta * cosA) * cosB - sinPhi * cosTheta * sinA - sinTheta * cosA - cosPhi * cosTheta * sinB));

		       	// this is much cleaner version, instead of using nested ifs
	      		// just used ternary operator to compare if luminance>0.
            if (y >= 0 && y < HEIGHT && x >= 0 && x < WIDTH && oneOverZ > depthBuffer[index]) {
                depthBuffer[index] = oneOverZ;
	    	    		// shamelessly copied the shades light to dark ascii from stack overflow [https://stackoverflow.com/a/67780964]
                buffer[index] = ".'`^\",:;Il!i><~+_-?][}{1)(|/tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$"[luminanceIndex > 0 ? luminanceIndex : 0];
            }
        }
    }

    // print shits on screen
    cout << "\x1b[H"; // ansi esc to move the cursor to the top left, instead of clearing the whole screen which causes flickers
    for (int i = 0; i < BUFFER_SIZE; i++) {
        putchar(i % WIDTH ? buffer[i] : '\n');
    }
}

int main() {
    double angleA = 1, angleB = 1;
    while (true) {
        drawTorus(angleA, angleB);
        this_thread::sleep_for(chrono::milliseconds(35));
    }
    return 0;
}
