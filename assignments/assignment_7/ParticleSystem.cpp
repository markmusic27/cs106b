// This file defines the Particle class member functions (where the logic is)!
#include "DrawParticle.h"
#include "ParticleSystem.h"
#include "random.h"
using namespace std;

// Initializes all values before they're used
ParticleSystem::ParticleSystem() {
    // Initialize private variables
    particleCount = 0;
}

// Destroys all memory and resets values to initial state
ParticleSystem::~ParticleSystem() {
    particleCount = 0;
    cleanUp();
}

// NOTE TO TA: I implemented this function recursive and got errors (perhaps stack overflow!?)

// This is a helper function that has the logic for deallocating memory in the particles
void ParticleSystem::cleanUp() {
    // Nothing to clean up; yay!
    if (head == nullptr) {
        return;
    }

    while (head != nullptr) {
        ParticleCell* helper = head->next;
        delete head;
        head = helper;
    }

    // Ensure head and tail are set to nullptr after cleanup
    head = nullptr;
    tail = nullptr;
}


// State for numParticles is stored in private member particleCount. This function makes reading accessible
int ParticleSystem::numParticles() const {
    return particleCount;
}

// Checks if a particle should be in the canvas or not
bool ParticleSystem::isParticleValid(Particle data) {
    bool offscreenX = data.x >= SCENE_WIDTH || data.x < 0;
    bool offscreenY = data.y >= SCENE_HEIGHT || data.y < 0;
    bool dead = data.lifetime < 0;

    return !(offscreenX || offscreenY || dead);
}

// Removes a particle from the linked list
void ParticleSystem::remove(ParticleCell* p){
    // Edge case: empty list
    if (p == nullptr){
        return;
    }

    // Remove from list
    particleCount--;

    // Edge case: only one element
    if (p->next == nullptr && p->prev == nullptr){
        head = nullptr;
        tail = nullptr;
        delete p;
        return;
    }

    // Removing the head (Marie Antoinette typeshii)
    if (p == head) {
        head = p->next;
        head->prev = nullptr;
        delete p;
        return;
    }

    // Removing the tail
    if (p == tail) {
        tail = p->prev;
        tail->next = nullptr;
        delete p;
        return;
    }

    // Else you're just removing the middle element
    p->prev->next = p->next;
    p->next->prev = p->prev;
    delete p;

}

// Function responsible for adding a new particle to particle system and initializing it if necessary
void ParticleSystem::add(const Particle& data) {
    // Don't add particles dead or offscreen particles
    bool isValid = isParticleValid(data);

    if (!isValid) {
        return;
    }

    // Add to count
    particleCount++;

    // Initialize new cell
    ParticleCell* newCell = new ParticleCell;
    newCell->particle = data;
    newCell->next = nullptr;

    // Edge case: Empty list (add one item)
    if (head == nullptr){
        // Just set next to null, and head and tail to this new cell
        newCell->prev = nullptr;
        head = newCell;
        tail = newCell;

        return;
    }

    // Normal Case: append to end of the list
    ParticleCell* secondLast = tail;
    tail = newCell;
    tail->prev = secondLast;
    secondLast->next = tail;
}

// This function iterates through the particles and draws them
void ParticleSystem::drawParticles() const {

    // Edge case: empty list
    if (head == nullptr) {
        return;
    }

    ParticleCell* cell = head;

    // Draw the particles itteratively
    while (cell != nullptr){
        // Check if particle is valid
        Particle p = cell->particle;
        drawParticle(p.x, p.y, p.color);


        cell = cell->next;
    }
}

// Handles firwork explosions when it's lifetime is zero
void ParticleSystem::explode(Particle root){
    // Create new child template
    Particle p = Particle();
    p.x = root.x;
    p.y = root.y;
    p.type = ParticleType::STREAMER;
    p.color = Color::RANDOM();

    for (int i = 0; i < kExplosionChildren; i++){
        // Set new velocity, and lifetime values;
        p.dx = randomInteger(kExplosionRangeLow, kExplosionRangeHigh);
        p.dy = randomInteger(kExplosionRangeLow, kExplosionRangeHigh);
        p.lifetime = randomInteger(kLifetimeRangeLow, kLifetimeRangeHigh);

        add(p);
    }
}

// Itterate through all of the particles and move them by a little bit
void ParticleSystem::moveParticles() {
    // Edge case: empty list
    if (head == nullptr){
        return;
    }

    ParticleCell* cell = head;

    // Move the particles a little bit
    while (cell != nullptr){
        // Update the particle position
        Particle p = cell->particle;
        p.x+=p.dx;
        p.y+=p.dy;

        // Update velocity vector
        if (p.type != ParticleType::STREAMER){
            p.dy++;
        }

        // Update lifetime and set to cell value
        p.lifetime--;
        cell->particle=p;

        ParticleCell* helper = cell;
        cell=cell->next;

        // Check if the particle should still exist
        bool isValid = isParticleValid(p);

        // If firework lifetime is up then spawn firework children
        if (p.type == ParticleType::FIREWORK && p.lifetime < 0){
            explode(p);
        }

        if (!isValid){
            remove(helper);
        }
    }
}

/* * * * * Test Cases Below This Point * * * * */

STUDENT_TEST("Ensures that particles are added to the back") {
    ParticleSystem system;

    // Create particles and change x to tell them apart
    Particle a;
    Particle b;
    a.x = 1;
    b.x = 2;

    /* Add each to the particle system. The order should be one, then two. */
    system.add(a);
    system.add(b);

    // Make sure both are there
    EXPECT_EQUAL(system.numParticles(), 2);

    // Ensures that particle a is in the back (denoted by tail)
    EXPECT_EQUAL(system.tail->particle.x, 2);
}

STUDENT_TEST("Remove function works adequately") {
    ParticleSystem system;

    // Create particles and change x to tell them apart
    Particle a;
    Particle b;
    a.x = 1;
    b.x = 2;

    // Add each to the particle system
    system.add(a);
    system.add(b);

    // Make sure both are there
    EXPECT_EQUAL(system.numParticles(), 2);

    system.remove(system.head);

    EXPECT_EQUAL(system.numParticles(), 1);
}

STUDENT_TEST("Test parabolic projection with velocity") {
    ParticleSystem system;

    // Create particles and change x to tell them apart
    Particle a;
    a.x = SCENE_WIDTH / 2;
    a.y = SCENE_HEIGHT / 2;
    a.dx = 1;
    a.dy = 1;
    a.type = ParticleType::BALLISTIC;

    // Add particle to the system
    system.add(a);

    // Run 20 cycles
    for (int i = 0; i < 20; i++){
        system.moveParticles();
    }

    // Check velocity vector
    EXPECT_EQUAL(system.head->particle.dy, 21);
}

/* * * * * Provided Tests Below This Point * * * * */

PROVIDED_TEST("Milestone 1: Constructor creates an empty particle system.") {
    ParticleSystem system;
    EXPECT_EQUAL(system.head, nullptr);
}

PROVIDED_TEST("Milestone 1: Empty system has no particles.") {
    ParticleSystem system;
    EXPECT_EQUAL(system.numParticles(), 0);
}

PROVIDED_TEST("Milestone 1: Can add a single particle.") {
    ParticleSystem system;

    Particle particle;
    particle.x = 13.7;
    particle.y = 4.2;
    particle.color = Color::CYAN();

    system.add(particle);

    /* Should have one particle. */
    EXPECT_EQUAL(system.numParticles(), 1);

    /* Invasively check to make sure the head pointer isn't null,
     * since it needs to hold our particle.
     */
    EXPECT_NOT_EQUAL(system.head, nullptr);

    /* Make sure the particle's x, y, and color are copied over. */
    EXPECT_EQUAL(system.head->particle.x, particle.x);
    EXPECT_EQUAL(system.head->particle.y, particle.y);
    EXPECT_EQUAL(system.head->particle.color, particle.color);

    /* Make sure the list is wired correctly. */
    EXPECT_EQUAL(system.head->prev, nullptr);
    EXPECT_EQUAL(system.head->next, nullptr);
}

PROVIDED_TEST("Milestone 1: Can add two particles.") {
    ParticleSystem system;

    /* Make two particles. Use the x and y coordinates to tell them apart. */
    Particle one, two;
    one.x = 1;
    two.x = 2;

    /* Add each to the particle system. The order should be one, then two. */
    system.add(one);
    system.add(two);

    /* Make sure we see two particles. */
    EXPECT_EQUAL(system.numParticles(), 2);

    /* Make sure the list has two items in it. */
    EXPECT_NOT_EQUAL(system.head, nullptr);
    EXPECT_NOT_EQUAL(system.head->next, nullptr);
    EXPECT_EQUAL(system.head->next->next, nullptr);

    /* Make sure previous pointers work correctly. */
    EXPECT_EQUAL(system.head->prev, nullptr);
    EXPECT_EQUAL(system.head->next->prev, system.head);

    /* Make sure the particles are in the right order. */
    EXPECT_EQUAL(system.head->particle.x, 1);
    EXPECT_EQUAL(system.head->next->particle.x, 2);
}

PROVIDED_TEST("Milestone 1: Can add multiple particles.") {
    ParticleSystem system;

    /* Add some particles. */
    const int kNumParticles = 5;
    for (int i = 0; i < kNumParticles; i++) {
        Particle particle;
        particle.x = i;
        system.add(particle);
    }

    EXPECT_EQUAL(system.numParticles(), kNumParticles);

    /* Confirm they're there and in the right order. */
    int numSeen = 0;
    ParticleSystem::ParticleCell* prev = nullptr;
    ParticleSystem::ParticleCell* curr = system.head;

    /* Walk the list checking particles. */
    while (curr != nullptr) {
        /* x coordinate tracks which particle this is, so this is a way of
         * checking whether we've got the particles in the right order.
         */
        EXPECT_EQUAL(curr->particle.x, numSeen);

        /* Check the list wiring - we should point back to the cell before us,
         * or to nullptr if there are no cells here.
         */
        EXPECT_EQUAL(curr->prev, prev);

        /* Advance forward. */
        prev = curr;
        curr = curr->next;
        numSeen++;
    }

    /* Confirm we saw the right number. */
    EXPECT_EQUAL(numSeen, kNumParticles);
    EXPECT_EQUAL(system.numParticles(), kNumParticles);
}

PROVIDED_TEST("Milestone 1: Stress Test: Efficiently adds and counts particles.") {
    ParticleSystem system;

    /* Add a bunch of particles. If the add operation takes time O(n), this
     * will run very, very slowly. If the add operation takes time O(1),
     * this will run very quickly.
     */
    const int kNumParticles = 100000;
    EXPECT_COMPLETES_IN(5.0, {
        for (int i = 0; i < kNumParticles; i++) {
            Particle particle;
            particle.x = i / (kNumParticles * 1.0);
            system.add(particle);
        }
    });

    /* Count how many particles there are. If this takes time O(n), this
     * will run very, very slowly. If the numParticles operation takes
     * time O(1), this will run almost instantaneously.
     */
    EXPECT_COMPLETES_IN(1.0, {
        const int kNumTimes = 100000;
        for (int i = 0; i < kNumTimes; i++) {
            EXPECT_EQUAL(system.numParticles(), kNumParticles);
        }
    });
}

#include "Demos/ParticleCatcher.h"

PROVIDED_TEST("Milestone 2: Single particle gets drawn.") {
    ParticleSystem system;

    /* This ParticleCatcher will write down all the calls to the global
     * drawParticle function so that we can see what's drawn.
     */
    ParticleCatcher catcher;

    /* Draw all the particles. This should do nothing. */
    system.drawParticles();

    /* Make sure nothing was drawn. */
    EXPECT_EQUAL(catcher.numDrawn(), 0);

    /* Now add a particle. */
    Particle particle;
    particle.x = 13.7;
    particle.y = 4.2;
    particle.color = Color::RANDOM();
    system.add(particle);

    /* Draw everything again. We should see this particle. */
    system.drawParticles();
    EXPECT_EQUAL(catcher.numDrawn(), 1);
    EXPECT_EQUAL(catcher[0].x,     particle.x);
    EXPECT_EQUAL(catcher[0].y,     particle.y);
    EXPECT_EQUAL(catcher[0].color, particle.color);
}

PROVIDED_TEST("Milestone 2: Multiple particles are drawn in order.") {
    ParticleSystem system;

    /* Add three particles in a specific order. */
    Particle one, two, three;
    one.color   = Color::CYAN();
    two.color   = Color::YELLOW();
    three.color = Color::MAGENTA();

    system.add(one);
    system.add(two);
    system.add(three);

    /* Draw the particles and confirm they come back in the right order. */
    ParticleCatcher catcher;
    system.drawParticles();

    EXPECT_EQUAL(catcher.numDrawn(), 3);
    EXPECT_EQUAL(catcher[0].color, Color::CYAN());
    EXPECT_EQUAL(catcher[1].color, Color::YELLOW());
    EXPECT_EQUAL(catcher[2].color, Color::MAGENTA());
}

PROVIDED_TEST("Milestone 3: Can move a single particle.") {
    ParticleSystem system;

    /* The particle we'll move around. */
    Particle particle;
    particle.x = 100;
    particle.y = 100;
    particle.dx = 3;
    particle.dy = -4;
    system.add(particle);
    EXPECT_EQUAL(system.numParticles(), 1);

    /* Confirm we have all the right information stored. */
    EXPECT_NOT_EQUAL(system.head, nullptr);
    EXPECT_EQUAL(system.head->particle.x,     particle.x);
    EXPECT_EQUAL(system.head->particle.y,     particle.y);
    EXPECT_EQUAL(system.head->particle.color, particle.color);
    EXPECT_EQUAL(system.head->particle.dx,    particle.dx);
    EXPECT_EQUAL(system.head->particle.dy,    particle.dy);

    /* Move the particle. */
    system.moveParticles();

    /* The particle should be in a new spot with the same initial velocity. */
    EXPECT_EQUAL(system.head->particle.x,     particle.x + particle.dx);
    EXPECT_EQUAL(system.head->particle.y,     particle.y + particle.dy);
    EXPECT_EQUAL(system.head->particle.color, particle.color);
    EXPECT_EQUAL(system.head->particle.dx,    particle.dx);
    EXPECT_EQUAL(system.head->particle.dy,    particle.dy);

    /* Move the particle again. */
    system.moveParticles();

    /* The particle should be in a new spot with the same initial velocity. */
    EXPECT_EQUAL(system.head->particle.x,     particle.x + 2 * particle.dx);
    EXPECT_EQUAL(system.head->particle.y,     particle.y + 2 * particle.dy);
    EXPECT_EQUAL(system.head->particle.color, particle.color);
    EXPECT_EQUAL(system.head->particle.dx,    particle.dx);
    EXPECT_EQUAL(system.head->particle.dy,    particle.dy);
}

PROVIDED_TEST("Milestone 3: Can move multiple particles with different velocities.") {
    ParticleSystem system;

    /* Set up the particles. */
    Particle left, down, diag;
    left.dx = 1;
    left.dy = 0;

    down.dx = 0;
    down.dy = 1;

    diag.dx = 1;
    diag.dy = 1;

    system.add(left);
    system.add(down);
    system.add(diag);

    /* Move the particles multiple times and confirm they're in the right
     * places.
     */
    const int kNumSteps = 50;
    for (int i = 0; i < kNumSteps; i++) {
        /* Draw the particles to see where they are. */
        ParticleCatcher catcher;
        system.drawParticles();

        /* Make sure we see all three particles and that they're in the right
         * places.
         */
        EXPECT_EQUAL(catcher.numDrawn(), 3);

        /* Left */
        EXPECT_EQUAL(catcher[0].x, i);
        EXPECT_EQUAL(catcher[0].y, 0);

        /* Down */
        EXPECT_EQUAL(catcher[1].x, 0);
        EXPECT_EQUAL(catcher[1].y, i);

        /* Diag */
        EXPECT_EQUAL(catcher[2].x, i);
        EXPECT_EQUAL(catcher[2].y, i);

        /* Move the particles for the next iteration. */
        system.moveParticles();
    }
}

PROVIDED_TEST("Milestone 4: Don't add invalid particles to the list.") {
    ParticleSystem system;

    /* Add an unobjectionable particle. */
    Particle good;
    good.x = 3;
    good.y = 5;
    good.color = Color::BLUE();
    good.lifetime = 137;

    /* Add the particle; confirm it's there. */
    system.add(good);
    EXPECT_NOT_EQUAL(system.head, nullptr);
    EXPECT_EQUAL(system.head->particle.x,        good.x);
    EXPECT_EQUAL(system.head->particle.y,        good.y);
    EXPECT_EQUAL(system.head->particle.color,    good.color);
    EXPECT_EQUAL(system.head->particle.lifetime, good.lifetime);
    EXPECT_EQUAL(system.numParticles(), 1);

    /* Now make a mix of bad particles that are out of bounds. */
    Particle bad;

    bad = good;
    bad.x = -0.001; // x too low
    system.add(bad);
    EXPECT_EQUAL(system.numParticles(), 1);

    bad = good;
    bad.y = -0.001; // y too low
    system.add(bad);
    EXPECT_EQUAL(system.numParticles(), 1);

    bad = good;
    bad.x = SCENE_WIDTH; // x too high
    system.add(bad);
    EXPECT_EQUAL(system.numParticles(), 1);

    bad = good;
    bad.y = SCENE_HEIGHT; // y too high
    system.add(bad);
    EXPECT_EQUAL(system.numParticles(), 1);

    bad = good;
    bad.lifetime = -1; // Negative lifetime
    system.add(bad);
    EXPECT_EQUAL(system.numParticles(), 1);

    /* Make sure the first particle is still there and unchanged. */
    EXPECT_EQUAL(system.head->particle.x,        good.x);
    EXPECT_EQUAL(system.head->particle.y,        good.y);
    EXPECT_EQUAL(system.head->particle.color,    good.color);
    EXPECT_EQUAL(system.head->particle.lifetime, good.lifetime);

    /* Make sure the list is still valid. */
    EXPECT_EQUAL(system.head->next, nullptr);
    EXPECT_EQUAL(system.head->prev, nullptr);
}

PROVIDED_TEST("Milestone 4: Particle removed when it leaves the screen.") {
    ParticleSystem system;

    /* A particle moving out of the world. */
    Particle escapee;
    escapee.x = 1.5;
    escapee.y = 1.5;
    escapee.dx = -1;
    escapee.dy = -1;
    escapee.lifetime = 1000;
    system.add(escapee);

    /* Use a ParticleCatcher to see which particles are drawn. */
    ParticleCatcher catcher;

    /* Initially, we should find our particle where we created it. */
    system.drawParticles();

    EXPECT_EQUAL(catcher.numDrawn(), 1);
    EXPECT_EQUAL(catcher[0].x, 1.5);
    EXPECT_EQUAL(catcher[0].y, 1.5);
    catcher.reset(); // Forget we saw the particle

    /* Moving the particle one step should leave the particle alive. */
    system.moveParticles();
    system.drawParticles();

    EXPECT_EQUAL(catcher.numDrawn(), 1);
    EXPECT_EQUAL(catcher[0].x, 0.5);
    EXPECT_EQUAL(catcher[0].y, 0.5);
    catcher.reset(); // Forget we saw the particle

    /* Moving the particle once more should remove the particle because
     * it'll be at position (-0.5, -0.5).
     */
    system.moveParticles();
    system.drawParticles();

    EXPECT_EQUAL(catcher.numDrawn(), 0);
    EXPECT_EQUAL(system.numParticles(), 0);
}

PROVIDED_TEST("Milestone 4: Particle removed when its lifetime ends.") {
    ParticleSystem system;

    /* A particle moving out of the world. */
    Particle timeout;
    timeout.x = 1.5;
    timeout.y = 1.5;
    timeout.dx = 1;
    timeout.dy = 1;
    timeout.lifetime = 1;
    system.add(timeout);

    /* Use a ParticleCatcher to see which particles are drawn. */
    ParticleCatcher catcher;

    /* Initially, we should find our particle where we created it. */
    system.drawParticles();

    EXPECT_EQUAL(catcher.numDrawn(), 1);
    EXPECT_EQUAL(catcher[0].x, 1.5);
    EXPECT_EQUAL(catcher[0].y, 1.5);
    catcher.reset(); // Forget we saw the particle

    /* Moving the particle one step should leave the particle alive but with
     * a lifetime of 0.
     */
    system.moveParticles();
    system.drawParticles();

    EXPECT_EQUAL(catcher.numDrawn(), 1);
    EXPECT_EQUAL(catcher[0].x, 2.5);
    EXPECT_EQUAL(catcher[0].y, 2.5);
    catcher.reset(); // Forget we saw the particle

    /* Moving the particle once more should remove the particle because its
     * lifetime becomes negative.
     */
    system.moveParticles();
    system.drawParticles();

    EXPECT_EQUAL(catcher.numDrawn(), 0);
    EXPECT_EQUAL(system.numParticles(), 0);
}

PROVIDED_TEST("Milestone 4: All particles move even if first needs to be removed.") {
    /* Create a list of five particles at positions (0, 0), (1, 0), ... (4, 0).
     * All particles move downward. All particles have a long lifetime except
     * for the first, which has a lifetime of 0 and thus disappears as soon as
     * it moves. We should see all the others shifted down by one spot.
     */
    ParticleSystem system;

    /* Used to identify particles. */
    const Vector<Color> colors = {
        Color::BLACK(), Color::WHITE(), Color::RED(), Color::GREEN(), Color::YELLOW()
    };

    /* Initialize the particles. */
    Vector<Particle> particles;
    for (int i = 0; i < 5; i++) {
        Particle particle;
        particle.x = i;
        particle.dx = 0;
        particle.dy = 1;
        particle.lifetime = 100;
        particle.color = colors[i]; // So we can flag it later
        particles += particle;
    }
    particles[0].lifetime = 0;

    /* Add the particles. */
    for (Particle particle: particles) {
        system.add(particle);
    }
    EXPECT_EQUAL(system.numParticles(), 5);

    /* Move them all. */
    system.moveParticles();
    EXPECT_EQUAL(system.numParticles(), 4); // One was removed

    /* See where they are. */
    ParticleCatcher catcher;
    system.drawParticles();

    /* Confirm we have particles 1, 2, 3, and 4, in that order. */
    EXPECT_EQUAL(catcher.numDrawn(), 4);
    EXPECT_EQUAL(catcher[0], { 1, 1, colors[1] });
    EXPECT_EQUAL(catcher[1], { 2, 1, colors[2] });
    EXPECT_EQUAL(catcher[2], { 3, 1, colors[3] });
    EXPECT_EQUAL(catcher[3], { 4, 1, colors[4] });

    /* Check the linked list to make sure the wiring is right. */
    ParticleSystem::ParticleCell* curr = system.head;
    ParticleSystem::ParticleCell* prev = nullptr;
    for (int i = 0; i < 4; i++) {
        EXPECT_NOT_EQUAL(curr, nullptr);
        EXPECT_EQUAL(curr->prev, prev);

        prev = curr;
        curr = curr->next;
    }
    EXPECT_EQUAL(curr, nullptr);
}

PROVIDED_TEST("Milestone 4: All particles move even if second needs to be removed.") {
    /* Create a list of five particles at positions (0, 0), (1, 0), ... (4, 0).
     * All particles move downward. All particles have a long lifetime except
     * for the second, which has a lifetime of 0 and thus disappears as soon as
     * it moves. We should see all the others shifted down by one spot.
     */
    ParticleSystem system;

    /* Used to identify particles. */
    const Vector<Color> colors = {
        Color::BLACK(), Color::WHITE(), Color::RED(), Color::GREEN(), Color::YELLOW()
    };

    /* Initialize the particles. */
    Vector<Particle> particles;
    for (int i = 0; i < 5; i++) {
        Particle particle;
        particle.x = i;
        particle.dx = 0;
        particle.dy = 1;
        particle.lifetime = 100;
        particle.color = colors[i]; // So we can flag it later
        particles += particle;
    }
    particles[1].lifetime = 0;

    /* Add the particles. */
    for (Particle particle: particles) {
        system.add(particle);
    }
    EXPECT_EQUAL(system.numParticles(), 5);

    /* Move them all. */
    system.moveParticles();
    EXPECT_EQUAL(system.numParticles(), 4); // One was removed

    /* See where they are. */
    ParticleCatcher catcher;
    system.drawParticles();

    /* Confirm we have particles 0, 2, 3, and 4, in that order. */
    EXPECT_EQUAL(catcher.numDrawn(), 4);
    EXPECT_EQUAL(catcher[0], { 0, 1, colors[0] });
    EXPECT_EQUAL(catcher[1], { 2, 1, colors[2] });
    EXPECT_EQUAL(catcher[2], { 3, 1, colors[3] });
    EXPECT_EQUAL(catcher[3], { 4, 1, colors[4] });

    /* Check the linked list to make sure the wiring is right. */
    ParticleSystem::ParticleCell* curr = system.head;
    ParticleSystem::ParticleCell* prev = nullptr;
    for (int i = 0; i < 4; i++) {
        EXPECT_NOT_EQUAL(curr, nullptr);
        EXPECT_EQUAL(curr->prev, prev);

        prev = curr;
        curr = curr->next;
    }
    EXPECT_EQUAL(curr, nullptr);
}

PROVIDED_TEST("Milestone 4: All particles move even if last needs to be removed.") {
    /* Create a list of five particles at positions (0, 0), (1, 0), ... (4, 0).
     * All particles move downward. All particles have a long lifetime except
     * for the last, which has a lifetime of 0 and thus disappears as soon as
     * it moves. We should see all the others shifted down by one spot.
     */
    ParticleSystem system;

    /* Used to identify particles. */
    const Vector<Color> colors = {
        Color::BLACK(), Color::WHITE(), Color::RED(), Color::GREEN(), Color::YELLOW()
    };

    /* Initialize the particles. */
    Vector<Particle> particles;
    for (int i = 0; i < 5; i++) {
        Particle particle;
        particle.x = i;
        particle.dx = 0;
        particle.dy = 1;
        particle.lifetime = 100;
        particle.color = colors[i]; // So we can flag it later
        particles += particle;
    }
    particles[4].lifetime = 0;

    /* Add the particles. */
    for (Particle particle: particles) {
        system.add(particle);
    }
    EXPECT_EQUAL(system.numParticles(), 5);

    /* Move them all. */
    system.moveParticles();
    EXPECT_EQUAL(system.numParticles(), 4); // One was removed

    /* See where they are. */
    ParticleCatcher catcher;
    system.drawParticles();

    /* Confirm we have particles 0, 1, 2, and 3, in that order. */
    EXPECT_EQUAL(catcher.numDrawn(), 4);
    EXPECT_EQUAL(catcher[0], { 0, 1, colors[0] });
    EXPECT_EQUAL(catcher[1], { 1, 1, colors[1] });
    EXPECT_EQUAL(catcher[2], { 2, 1, colors[2] });
    EXPECT_EQUAL(catcher[3], { 3, 1, colors[3] });

    /* Check the linked list to make sure the wiring is right. */
    ParticleSystem::ParticleCell* curr = system.head;
    ParticleSystem::ParticleCell* prev = nullptr;
    for (int i = 0; i < 4; i++) {
        EXPECT_NOT_EQUAL(curr, nullptr);
        EXPECT_EQUAL(curr->prev, prev);

        prev = curr;
        curr = curr->next;
    }
    EXPECT_EQUAL(curr, nullptr);
}

PROVIDED_TEST("Milestone 4: All particles move even if second-to-last needs to be removed.") {
    /* Create a list of five particles at positions (0, 0), (1, 0), ... (4, 0).
     * All particles move downward. All particles have a long lifetime except
     * for the penultimate, which has a lifetime of 0 and thus disappears as soon as
     * it moves. We should see all the others shifted down by one spot.
     */
    ParticleSystem system;

    /* Used to identify particles. */
    const Vector<Color> colors = {
        Color::BLACK(), Color::WHITE(), Color::RED(), Color::GREEN(), Color::YELLOW()
    };

    /* Initialize the particles. */
    Vector<Particle> particles;
    for (int i = 0; i < 5; i++) {
        Particle particle;
        particle.x = i;
        particle.dx = 0;
        particle.dy = 1;
        particle.lifetime = 100;
        particle.color = colors[i]; // So we can flag it later
        particles += particle;
    }
    particles[3].lifetime = 0;

    /* Add the particles. */
    for (Particle particle: particles) {
        system.add(particle);
    }
    EXPECT_EQUAL(system.numParticles(), 5);

    /* Move them all. */
    system.moveParticles();
    EXPECT_EQUAL(system.numParticles(), 4); // One was removed

    /* See where they are. */
    ParticleCatcher catcher;
    system.drawParticles();

    /* Confirm we have particles 0, 1, 2, and 4, in that order. */
    EXPECT_EQUAL(catcher.numDrawn(), 4);
    EXPECT_EQUAL(catcher[0], { 0, 1, colors[0] });
    EXPECT_EQUAL(catcher[1], { 1, 1, colors[1] });
    EXPECT_EQUAL(catcher[2], { 2, 1, colors[2] });
    EXPECT_EQUAL(catcher[3], { 4, 1, colors[4] });

    /* Check the linked list to make sure the wiring is right. */
    ParticleSystem::ParticleCell* curr = system.head;
    ParticleSystem::ParticleCell* prev = nullptr;
    for (int i = 0; i < 4; i++) {
        EXPECT_NOT_EQUAL(curr, nullptr);
        EXPECT_EQUAL(curr->prev, prev);

        prev = curr;
        curr = curr->next;
    }
    EXPECT_EQUAL(curr, nullptr);
}

PROVIDED_TEST("Milestone 4: All particles move even if first needs to be removed.") {
    /* Create a list of five particles at positions (0, 0), (1, 0), ... (4, 0).
     * All particles move downward. All particles have a long lifetime except
     * for the first, which has a lifetime of 0 and thus disappears as soon as
     * it moves. We should see all the others shifted down by one spot.
     */
    ParticleSystem system;

    /* Used to identify particles. */
    const Vector<Color> colors = {
        Color::BLACK(), Color::WHITE(), Color::RED(), Color::GREEN(), Color::YELLOW()
    };

    /* Initialize the particles. */
    Vector<Particle> particles;
    for (int i = 0; i < 5; i++) {
        Particle particle;
        particle.x = i;
        particle.dx = 0;
        particle.dy = 1;
        particle.lifetime = 100;
        particle.color = colors[i]; // So we can flag it later
        particles += particle;
    }
    particles[2].lifetime = 0;

    /* Add the particles. */
    for (Particle particle: particles) {
        system.add(particle);
    }
    EXPECT_EQUAL(system.numParticles(), 5);

    /* Move them all. */
    system.moveParticles();
    EXPECT_EQUAL(system.numParticles(), 4); // One was removed

    /* See where they are. */
    ParticleCatcher catcher;
    system.drawParticles();

    /* Confirm we have particles 0, 1, 3, and 4, in that order. */
    EXPECT_EQUAL(catcher.numDrawn(), 4);
    EXPECT_EQUAL(catcher[0], { 0, 1, colors[0] });
    EXPECT_EQUAL(catcher[1], { 1, 1, colors[1] });
    EXPECT_EQUAL(catcher[2], { 3, 1, colors[3] });
    EXPECT_EQUAL(catcher[3], { 4, 1, colors[4] });

    /* Check the linked list to make sure the wiring is right. */
    ParticleSystem::ParticleCell* curr = system.head;
    ParticleSystem::ParticleCell* prev = nullptr;
    for (int i = 0; i < 4; i++) {
        EXPECT_NOT_EQUAL(curr, nullptr);
        EXPECT_EQUAL(curr->prev, prev);

        prev = curr;
        curr = curr->next;
    }
    EXPECT_EQUAL(curr, nullptr);
}

PROVIDED_TEST("Milestone 4: All particles move even if many need to be removed.") {
    /* Create a list of five particles at positions (0, 0), (1, 0), ... (4, 0).
     * All particles move downward. Particles 0, 2, and 4 have lifetime 0 and
     * thus will disappear after the first step. The other two will live for
     * multiple time steps.
     */
    ParticleSystem system;

    /* Used to identify particles. */
    const Vector<Color> colors = {
        Color::BLACK(), Color::WHITE(), Color::RED(), Color::GREEN(), Color::YELLOW()
    };

    /* Initialize the particles. */
    Vector<Particle> particles;
    for (int i = 0; i < 5; i++) {
        Particle particle;
        particle.x = i;
        particle.dx = 0;
        particle.dy = 1;
        particle.lifetime = 100;
        particle.color = colors[i]; // So we can flag it later
        particles += particle;
    }
    particles[0].lifetime = 0;
    particles[2].lifetime = 0;
    particles[4].lifetime = 0;

    /* Add the particles. */
    for (Particle particle: particles) {
        system.add(particle);
    }
    EXPECT_EQUAL(system.numParticles(), 5);

    /* Move them all. */
    system.moveParticles();
    EXPECT_EQUAL(system.numParticles(), 2); // Three were removed

    /* See where they are. */
    ParticleCatcher catcher;
    system.drawParticles();

    /* Confirm we have particles 1 and 3, in that order. */
    EXPECT_EQUAL(catcher.numDrawn(), 2);
    EXPECT_EQUAL(catcher[0], { 1, 1, colors[1] });
    EXPECT_EQUAL(catcher[1], { 3, 1, colors[3] });

    /* Check the linked list to make sure the wiring is right. */
    ParticleSystem::ParticleCell* curr = system.head;
    ParticleSystem::ParticleCell* prev = nullptr;
    for (int i = 0; i < 2; i++) {
        EXPECT_NOT_EQUAL(curr, nullptr);
        EXPECT_EQUAL(curr->prev, prev);

        prev = curr;
        curr = curr->next;
    }
    EXPECT_EQUAL(curr, nullptr);
}

PROVIDED_TEST("Milestone 4: After all particles expire, can add new particles.") {
    ParticleSystem system;

    /* On entry to this loop, the particle system should be empty.
     * On exit, it should be empty.
     */
    for (int round = 0; round < 10; round++) {
        /* Shouldn't see anything. */
        ParticleCatcher catcher;
        system.drawParticles();
        EXPECT_EQUAL(catcher.numDrawn(), 0);

        /* Create a bunch of short-lived particles. */
        for (int i = 0; i < 5; i++) {
            Particle p;
            p.x = i;
            p.y = i;
            p.lifetime = 2;
            system.add(p);
        }
        EXPECT_EQUAL(system.numParticles(), 5);

        /* Should see five particles. */
        system.drawParticles();
        EXPECT_EQUAL(catcher.numDrawn(), 5);

        /* They should be in the right place. */
        for (int i = 0; i < 5; i++) {
            EXPECT_EQUAL(catcher[i].x, i);
            EXPECT_EQUAL(catcher[i].y, i);
        }

        /* Move the particles a bunch to clear everything out as their
         * lifetimes expire.
         */
        for (int i = 0; i < 10; i++) {
            system.moveParticles();
        }

        /* We should have no particles. */
        EXPECT_EQUAL(system.numParticles(), 0);
        catcher.reset();
        system.drawParticles();
        EXPECT_EQUAL(catcher.numDrawn(), 0);
    }
}

PROVIDED_TEST("Milestone 5: Ballistic particles accelerate downward.") {
    ParticleSystem system;

    Particle ballistic;
    ballistic.type = ParticleType::BALLISTIC;
    ballistic.x = 0;
    ballistic.y = 100;

    /* Fire upward and to the right. */
    ballistic.dx = 1;
    ballistic.dy = -10;

    system.add(ballistic);

    /* Expected positions. */
    Vector<GPoint> positions = {
        {  0, 100 },
        {  1,  90 }, // y += -10
        {  2,  81 }, // y += -9
        {  3,  73 }, // y += -8
        {  4,  66 },
        {  5,  60 },
        {  6,  55 },
        {  7,  51 },
        {  8,  48 },
        {  9,  46 },
        { 10,  45 },
        { 11,  45 }, // y += 0
        { 12,  46 },
        { 13,  48 },
        { 14,  51 },
        { 15,  55 },
        { 16,  60 },
        { 17,  66 },
        { 18,  73 },
        { 19,  81 }, // y += 8
        { 20,  90 }, // y += 9
        { 21, 100 }, // y += 10
    };

    /* Repeatedly check positions and move forward. */
    for (int i = 0; i < positions.size(); i++) {
        ParticleCatcher catcher;
        system.drawParticles();
        EXPECT_EQUAL(catcher.numDrawn(), 1);
        EXPECT_EQUAL(catcher[0].x, positions[i].x);
        EXPECT_EQUAL(catcher[0].y, positions[i].y);

        system.moveParticles();
    }
}

PROVIDED_TEST("Milestone 5: Fireworks accelerate downward.") {
    ParticleSystem system;

    Particle firework;
    firework.type = ParticleType::FIREWORK;
    firework.x = 0;
    firework.y = 100;

    /* Fire upward and to the right. */
    firework.dx = 1;
    firework.dy = -10;

    system.add(firework);

    /* Expected positions. */
    Vector<GPoint> positions = {
        {  0, 100 },
        {  1,  90 }, // y += -10
        {  2,  81 }, // y += -9
        {  3,  73 }, // y += -8
        {  4,  66 },
        {  5,  60 },
        {  6,  55 },
        {  7,  51 },
        {  8,  48 },
        {  9,  46 },
        { 10,  45 },
        { 11,  45 }, // y += 0
        { 12,  46 },
        { 13,  48 },
        { 14,  51 },
        { 15,  55 },
        { 16,  60 },
        { 17,  66 },
        { 18,  73 },
        { 19,  81 }, // y += 8
        { 20,  90 }, // y += 9
        { 21, 100 }, // y += 10
    };

    /* Repeatedly check positions and move forward. */
    for (int i = 0; i < positions.size(); i++) {
        ParticleCatcher catcher;
        system.drawParticles();
        EXPECT_EQUAL(catcher.numDrawn(), 1);
        EXPECT_EQUAL(catcher[0].x, positions[i].x);
        EXPECT_EQUAL(catcher[0].y, positions[i].y);

        system.moveParticles();
    }
}

PROVIDED_TEST("Milestone 5: Fireworks explode when timer expires.") {
    ParticleSystem system;

    /* Create and add a firework. */
    Particle firework;
    firework.type = ParticleType::FIREWORK;
    firework.lifetime = 0; // Explodes as soon as it moves
    firework.x = 100;
    firework.y = 100;
    system.add(firework);

    EXPECT_EQUAL(system.numParticles(), 1);

    /* Move the particles. This should trigger the explosion. */
    system.moveParticles();

    /* There should be 50 new particles, all of which are streamers, and
     * all of which are the same color.
     */
    EXPECT_EQUAL(system.numParticles(), 50);
    EXPECT_NOT_EQUAL(system.head, nullptr);

    /* Color of the first particle. */
    Color color = system.head->particle.color;
    for (auto* curr = system.head; curr != nullptr; curr = curr->next) {
        EXPECT_EQUAL(curr->particle.color, color);
    }
}
