# plane_simulator
Shows the simulation of motion of balls.

Balls attracted to each other with a force F(r) = 1/r + 1/r^2, where r - the distance between the balls. 

Balls can be:
- added when hit with left mouse button by the empty place;
- dragged with the mouse when holding left mouse button;
- removed when hit with right mouse button by the ball;

Written on C++11 using Qt5.

# Usage
Create a scene::Scene object and use next interfaces:
- scene::interfaces::Playable for manipulating with scene playback;
- scene::interface::Controllable for manipulating with selected object;
- scene::interface::Container for adding and removing objects;
- scene::interface::WithGravity for setting the gravity type;
- interchange::Broadcaster for creating a interchange::Subscription for alerting of changes on the scene;

Create QSimulatorWidget for viewing and and interacting with the scene.
