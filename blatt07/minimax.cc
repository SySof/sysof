#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <time.h>

#include <deque>
#include <queue>
#include <map>

#define buflen 128
#define CITY_COUNT 11094

#define _USE_MATH_DEFINES
#include <math.h>
#ifndef M_PI
    #define M_PI acos(-1)
#endif

double max(double a, double b) {
    return a>b ? a : b;
}

struct city {
    char name[buflen];
    unsigned int population;
    double latitude, longitude;
};

double to_rad(double degree) {
    return degree*(M_PI/180);
}

double dist_vincenty2(double lat1, double lon1, double lat2, double lon2) {
    static const double a = 6378137;
    static const double f = 1 / 298.257223563;

    double lat1_rad = to_rad(lat1);
    double lon1_rad = to_rad(lon1);
    double lat2_rad = to_rad(lat2);
    double lon2_rad = to_rad(lon2);

    double F = (lat1_rad + lat2_rad) / 2.0;
    double G = (lat1_rad - lat2_rad) / 2.0;
    double l = (lon1_rad - lon2_rad) / 2.0;

    double S = sin(G)*sin(G)*cos(l)*cos(l) + cos(F)*cos(F)*sin(l)*sin(l);
    double C = cos(G)*cos(G)*cos(l)*cos(l) + sin(F)*sin(F)*sin(l)*sin(l);
    double w = atan(sqrt(S/C));
    double D = 2*w*a;

    double T = sqrt(S*C)/w;
    double H1 = (3*T - 1)/(2*C);
    double H2 = (3*T + 1)/(2*S);

    double s = D*(1 + f*H1*sin(F)*sin(F)*cos(G)*cos(G) - f*H2*cos(F)*cos(F)*sin(G)*sin(G));
    return s/1000.0;
}

double dist_vincenty(struct city *city1, struct city *city2) {
    return dist_vincenty2(city1->latitude, city1->longitude, city2->latitude, city2->longitude);
}

void strcpy_encode(unsigned char *dst, unsigned char *src) {
    /* encode from ISO-8859-1 to UTF-8 */
    while(*src) {
        if(*src < 128) {
            *dst++ = *src++;
        } else {
            *dst++ = 0xc2 + (*src > 0xbf);
            *dst++ = (*src++ & 0x3f) + 0x80;
        }
    }
    *dst = '\0';
}

unsigned int city_list_read(const char *filename, struct city **cities) {
    char buffer[buflen];
    struct city cur_city;
    unsigned int i = 0;
    FILE* file = fopen(filename, "r");
    if(file == NULL) {
        return 1;
    }
    while(fgets(buffer, buflen, file)) {
        if(sscanf(buffer, "%63[^:]:%lf:%lf", cur_city.name, &cur_city.latitude, &cur_city.longitude) == 3) {
            cities[i] = new struct city();
            if(!cities[i]) {
                printf("Malloc failed.\n");
                exit(1);
            }
            strcpy_encode((unsigned char*)cities[i]->name, (unsigned char*)cur_city.name);
            cities[i]->latitude = cur_city.latitude;
            cities[i]->longitude = cur_city.longitude;
            i++;
            if(i >= CITY_COUNT) break;
        }
    }
    fclose(file);
    if(i != CITY_COUNT) {
        printf("Read failed: %d|%d.\n", i, CITY_COUNT);
        exit(1);
    }
    return 0;
}

unsigned int read_city(struct city **cities, FILE *file) {
    char name[buflen];
    unsigned int i;
    while(1) {
        printf("Please enter a city name:\n");
        if(!fgets(name, buflen, file)) {
            printf("EOF while reading.\n");
            exit(1);
        }
        name[strlen(name) - 1] = '\0'; /* delete newline */
        for(i = 0; i < CITY_COUNT; i++) {
            if(strcmp(name, cities[i]->name) == 0) {
                return i;
            }
        }
        printf("City not found.\n");
    }
}

void bfs(std::map<unsigned int, double> *links, bool *visits, int *parents, unsigned int src) {
    // Initialize visits and parents vectors.
    for(unsigned int i = 0; i < CITY_COUNT; i++) {
        visits[i] = false;
        parents[i] = -1;
    }
    visits[src] = true;
    // Start iterating over vertices.
    std::deque<unsigned int> queue;
    queue.push_back(src);
    while(!queue.empty()) {
        unsigned int curr = queue.front();
        queue.pop_front();
        for(std::map<unsigned int, double>::iterator next = links[curr].begin(); next != links[curr].end(); next++)
            if(!visits[next->first]) {
                visits[next->first] = true;
                parents[next->first] = curr;
                queue.push_back(next->first);
            }
    }
}

unsigned int bfs2(std::map<unsigned int, double> *links, bool *visits, int *parents, unsigned int src, unsigned int dst) {
    // Initialize visits and parents vectors.
    for(unsigned int i = 0; i < CITY_COUNT; i++) {
        visits[i] = false;
        parents[i] = -1;
    }
    visits[src] = true;
    // Start iterating over vertices.
    std::deque<unsigned int> queue;
    queue.push_back(src);
    while(!queue.empty()) {
        unsigned int curr = queue.front();
        queue.pop_front();
        for(std::map<unsigned int, double>::iterator next = links[curr].begin(); next != links[curr].end(); next++) {
            if(!visits[next->first]) {
                visits[next->first] = true;
                parents[next->first] = curr;
                queue.push_back(next->first);
            }
            if(next->first == dst) {
                return 1;
            }
        }
    }
    return 0;
}

std::deque<unsigned int> extractPath(int *parents, int dst) {
    if(parents[dst] == -1) {
        printf("Error.\n");
        exit(1);
    }
    std::deque<unsigned int> path;
    path.push_front(dst);
    for(int parent = parents[dst]; parent != -1; parent = parents[parent]) {
        path.push_front(parent);
    }
    return path;
}

struct link {
    unsigned int src;
    unsigned int dst;
    double wgt;
};

bool operator<(const struct link &link1, const struct link &link2) {
    return link1.wgt < link2.wgt;
}

std::deque<unsigned int> find_minimax_path(struct city **cities, unsigned int src, unsigned int dst) {
    // Create storage for BFS.
    bool visits[CITY_COUNT];
    int parents[CITY_COUNT];

    std::map<unsigned int, double> links[CITY_COUNT];
    double max_dist = 0;
    unsigned int c1 = 0, c2 = 0;
    double max_distance = 50;
    for(unsigned int i = 0; i < CITY_COUNT; i++) {
        if(i % (int)(CITY_COUNT/100.0 + 1) == 0) {
            printf("\r%d %%", (i*100) / CITY_COUNT + 1);
            fflush(stdout);
        }
        for(unsigned int j = 0; j < CITY_COUNT; j++) {
            if(j > i) {
                double dist = dist_vincenty(cities[i], cities[j]);
                if(dist <= max_distance) {
                    links[i][j] = dist;
                }
                if(dist > max_dist) {
                    max_dist = dist;
                    c1 = i; c2 = j;
                }
            } else if(j < i) {
                if(links[j].count(i) > 0) {
                    links[i][j] = links[j][i];
                }
            }
        }
    }
    printf("\n");
    printf("Maximum distance is between %s and %s: %.1f km\n", cities[c1]->name, cities[c2]->name, max_dist);
    fflush(stdin);

    // Check if the source and destination nodes are connected at all.
    bfs(links, visits, parents, src);
    if(parents[dst] == -1) {
        printf("Cities not connected.\n");
        exit(1);
    }
    std::deque<unsigned int> path = extractPath(parents, dst);
    // Enqueue links in decreasing cost order.
    // Carefull!! Decreasing order!!
    std::priority_queue<struct link> pq;
    for(unsigned int i = 0; i < CITY_COUNT; i++) {
        for(std::map<unsigned int, double>::iterator next = links[i].begin(); next != links[i].end(); next++) {
            struct link cur_link = {i, next->first, next->second};
            pq.push(cur_link);
        }
    }
    // Copy initial links to a transient link storage.
    //~ std::map<unsigned int, double> transLinks[CITY_COUNT];
    unsigned long int total_num_links = 0;
    for (int i = 0; i < CITY_COUNT; i++) {
        for (int j = 0; j < CITY_COUNT; j++) {
            if(links[i].count(j) > 0) {
                //~ transLinks[i][j] = links[i][j];
                total_num_links++;
            }
        }
    }
    // Remove links incrementally in decreasing weight order until
    // we disconnect all nodes from the source node.
    std::deque<struct link> deleted_queue;
    struct link cur_link;
    int last_time = time(NULL);
    unsigned long int cur_num_links = total_num_links;
    for(;;) {
        deleted_queue.clear();
        for(cur_link = pq.top(); cur_link.wgt > max_distance; pq.pop(), cur_link = pq.top()) {
            links[cur_link.src].erase(cur_link.dst);
            deleted_queue.push_back(cur_link);
            cur_num_links--;
            if(pq.empty() || pq.top().wgt != cur_link.wgt) break;
        }
        // Consume the next costly links in the queue.
        for(;;) {
            cur_link = pq.top();
            pq.pop();
            links[cur_link.src].erase(cur_link.dst);
            deleted_queue.push_back(cur_link);
            cur_num_links--;
            if(pq.empty() || pq.top().wgt != cur_link.wgt) break;
        }
        // Check connectivity.
        //~ bfs(links, visits, parents, src);
        //~ if(parents[dst] != -1) path = extractPath(parents, dst);
        //~ else break;

        //~ if(!bfs2(links, visits, parents, src, dst)) break;

        if(!bfs2(links, visits, parents, src, dst)) {
            break;
        }
        path = extractPath(parents, dst);
        max_distance = 0;
        for(auto node = path.begin(); node != path.end() && (node+1) != path.end(); ++node) {
            max_distance = max(max_distance, links[*node][*(node+1)]);
        }
        if(time(NULL) >= last_time + 2) {
            printf("%.1f %%. Current maximum distance: %.1f km\nPath: ",
                100.0-(cur_num_links * 100.0 / total_num_links), max_distance);
            for(auto node = path.begin(); node != path.end(); ++node) {
                if(node == path.begin()) {
                    printf("%s", cities[*node]->name);
                } else {
                    printf(", %s", cities[*node]->name);
                }
            }
            printf("\n\n");
            fflush(stdout);
            last_time = time(NULL);
        }
    }
    while(!deleted_queue.empty()) {
        struct link cur_link = deleted_queue.front();
        deleted_queue.pop_front();
        links[cur_link.src][cur_link.dst] = cur_link.wgt;
    }
    bfs(links, visits, parents, src);
    path = extractPath(parents, dst);
    // Return the last found shortest path.
    return path;
}

int main() {
    struct city *cities[CITY_COUNT];
    unsigned int start_city, dest_city;
    const char *rating[7] = {"Impossible!", "Fantastic!", "Great!",
        "Very good!", "Good.", "Not bad.", "You can do better."};
    double max_distance;
    unsigned int i;

    /* init */
    if(city_list_read("gemeinden.txt", cities)) {
        printf("Error reading file!\n");
        return 1;
    }

    /* game start */
    printf("Welcome to the 'Long chain of short trips' game.\n");
    printf("To begin, please enter a city where you would like to start.\n");
    start_city = read_city(cities, stdin);
    printf("Next enter a city as your destination.\n");
    dest_city = read_city(cities, stdin);
    while(dest_city == start_city) {
        printf("You have to select a different city.\n");
        dest_city = read_city(cities, stdin);
    }
    printf("\nYour aim is to travel from %s to %s using steps as small as possible.\n",
        cities[start_city]->name, cities[dest_city]->name);

    /* game loop */
    max_distance = 1;
    std::deque<unsigned int> path = find_minimax_path(cities, start_city, dest_city);
    /* game end */
    printf("Congratulations, you reached the destination! Selected route:\n");
    for(std::deque<unsigned int>::iterator node = path.begin(); node != path.end(); ++node) {
        printf("%s\n", cities[*node]->name);
        if(node+1 != path.end()) {
            max_distance = max(max_distance, dist_vincenty(cities[*node], cities[*(node+1)]));
        }
    }
    printf("Your maximum distance is %.1f km. %s\n", max_distance, rating[(unsigned int)log(max_distance)]);

    /* clear up */
    for(i = 0; i < CITY_COUNT; i++) {
        delete cities[i];
    }
    return 0;
}

