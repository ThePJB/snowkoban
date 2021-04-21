#include "scene.hpp"
#include "fireworks.hpp"
#include "util.hpp" 

#define min(A,B) (A < B ? A : B)

fireworks_particle fireworks_particle::spawn_child(fireworks_particle_state child_state) {
    auto c = *this;
    nchildren++;
    c.age = 0;
    c.t_child1 = 0;
    c.t_child2 = 0;
    c.seed = hash_intn(seed + nchildren * 34234, 0, 999999);
    c.state = child_state;
    return c;
}

void fireworks::update(double dt) {
    for (int i = 0; i < particles.length; i++) {
        const auto p = &particles.items[i];
        if (p->state == FPS_DEAD) continue;

        p->age += dt;
        p->t_child1 += dt;
        p->t_child2 += dt;
        p->x += p->vx * dt;
        p->y += p->vy * dt;

        switch(p->state) {
        case FPS_DEAD:
        {}
        case FPS_ASCENDING:
        {
            const auto asc_age = 1.4 * hash_floatn(p->seed, 0.6, 1.4);
            if (p->age > asc_age) {
                kill(i);
                const auto n_children = hash_intn(p->seed, 6, 12);
                for (int j = 0; j < n_children; j++) {
                    const auto angle = (2*M_PI / n_children) * j;
                    auto new_child = p->spawn_child(FPS_EXPLODED);
                    const auto new_velocity_magnitude = 0.1;
                    new_child.vx = new_velocity_magnitude * cos(angle);
                    new_child.vy = new_velocity_magnitude * sin(angle);
                    insert_particle(new_child);
                }
            }
            const auto spawn_trail = 0.05;
            const auto spawn_spark = 0.09;
            if (p->t_child1 > spawn_trail) {
                p->t_child1 -= spawn_trail;
                auto new_child = p->spawn_child(FPS_ASCENDING_TRAIL);
                new_child.vx *= 0.1;
                new_child.vy *= 0.1;
                const auto random_magnitude = 0.05;
                new_child.vx += hash_floatn(new_child.seed + 234234, -random_magnitude, random_magnitude); 
                new_child.vy += hash_floatn(new_child.seed + 897575, -random_magnitude, random_magnitude);
                insert_particle(new_child);
            }
            if (p->t_child2 > spawn_spark) {
                p->t_child2 -= spawn_spark;
                auto new_child = p->spawn_child(FPS_SPARK);
                new_child.vx *= 0.05;
                new_child.vy *= 0.05;
                const auto random_magnitude = 0.15;
                new_child.vx += hash_floatn(new_child.seed + 234234, -random_magnitude, random_magnitude); 
                new_child.vy += hash_floatn(new_child.seed + 897575, -random_magnitude, random_magnitude); 
                insert_particle(new_child);
            }
            break;
        }
        case FPS_ASCENDING_TRAIL:
        {
            // just time and kill
            const auto trail_age = 0.4;
            if (p->age > trail_age) {
                kill(i);
            }
            break;
        }
        case FPS_SPARK:
        {
            // also just time and kill
            const auto spark_age = 0.3;
            if (p->age > spark_age) {
                kill(i);
            }
            break;
        }
        case FPS_EXPLODED:
        {
            const auto max_age = 1 * hash_floatn(p->seed, 1, 1.5);
            if (p->age > max_age) {
                kill(i);
            }
            
            const auto spawn_trail = 0.05;
            if (p->t_child1 > spawn_trail) {
                p->t_child1 -= spawn_trail;
                auto new_child = p->spawn_child(FPS_ASCENDING_TRAIL);
                new_child.vx *= 0.1;
                new_child.vy *= 0.1;
                const auto random_magnitude = 0.05;
                new_child.vx += hash_floatn(new_child.seed + 234234, -random_magnitude, random_magnitude); 
                new_child.vy += hash_floatn(new_child.seed + 897575, -random_magnitude, random_magnitude);
                insert_particle(new_child);
            }
            // 
            break;
        }
        case FPS_EXPLODED_TRAIL:
        {
        // fuk it lol just using asc trail for now
        break;
        }
        }
    }
}

// ah it gets stretched so aspect ratio lol
// imma just get it drawing anything first
void fireworks::draw(shared_data *app_d) {
    const auto px_size = 8;
    for (int i = 0; i < particles.length; i++) {
        const auto p = &particles.items[i];
        if (p->state == FPS_DEAD) continue;

        const auto x = p->x * app_d->gc.xres;
        const auto y = p->y * app_d->gc.yres;

        const auto r = rect::centered(x, y, px_size, px_size);

        switch (p->state) {
        case FPS_DEAD:
            continue;
        case FPS_ASCENDING:
            gef_draw_rect(&app_d->gc, p->primary, r);
            break;
        case FPS_ASCENDING_TRAIL:
            gef_draw_rect(&app_d->gc, gef_rgb(200, 200, 200), r);
            break;
        case FPS_SPARK:
            gef_draw_rect(&app_d->gc, gef_rgb(255, 0, 0), r);
            break;
        case FPS_EXPLODED:
            gef_draw_rect(&app_d->gc, p->primary, r);
            break;
        case FPS_EXPLODED_TRAIL:
            continue;
        }
    }
}

void fireworks::kill(int i) {
    first_vacancy = min(first_vacancy, i);
    particles.items[i].state = FPS_DEAD;
}

void fireworks::insert_particle(fireworks_particle p) {
    for (; first_vacancy < particles.length; first_vacancy++) {
        if (particles.items[first_vacancy].state == FPS_DEAD) {
            particles.items[first_vacancy] = p;
            return;
        }
    }
    particles.push(p); // maybe inc first vacancy idk
}

void fireworks::spawn_primary() {
    static int firework_seed = 0;
    firework_seed = hash_intn(firework_seed, 0, 999999);
    // todo hsv and random high sat or whatever
    const colour colours[] = {
        gef_rgb(255,0,0),
        gef_rgb(0,255,0),
        gef_rgb(0,0,255),
        gef_rgb(0,255,255),
        gef_rgb(255,255,255),
        gef_rgb(255,0,255),
        gef_rgb(255,255,0),
    };

    auto f = fireworks_particle();
    f.y = 1;
    f.x = 0.5;
    f.seed = firework_seed;
    f.primary = colours[hash_intn(f.seed + 324234, 0, 7)];
    const auto angle = hash_floatn(f.seed + 123123321, 0, M_PI);

    const auto vmag = 0.4;
    f.vx = vmag * cos(angle);
    f.vy = -vmag * sin(angle);
    f.state = FPS_ASCENDING;
    insert_particle(f);
}