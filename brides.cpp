#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <iomanip>
#include <set>

using namespace std;
using ll = long long;
const ll INF = 100000000000000000;

struct Edge {
    ll from, to, cost, flow, capacity, back_position, id;
};

Edge& get_inv_edge(vector<vector<Edge>>& graph, Edge& edge) {
    return graph[edge.to][edge.back_position];
}

ll get_potential_cost(vector<ll>& potentials, Edge& e) {
    return e.cost + potentials[e.from] - potentials[e.to];
}

vector<ll> calc_distances(vector<vector<Edge>>& graph, ll start, vector<Edge*>& previous, vector<ll>& potentials) {
    vector<ll> distances(graph.size(), INF);
    previous.resize(graph.size());
    set<pair<ll, pair<ll, Edge*>>> heap;
    heap.insert({0, {start, 0}});

    while (heap.size() > 0) {
        ll dist = heap.begin()->first;
        Edge* edge = heap.begin()->second.second;
        ll vertex = heap.begin()->second.first;
        heap.erase(heap.begin());

        if (distances[vertex] > dist) {
            distances[vertex] = dist;
            previous[vertex] = edge;
            // cerr << dist << " " << vertex << " " << p << "\n";
            for (Edge& e : graph[vertex]) {
                if (e.capacity > 0) {
                    heap.insert({dist + get_potential_cost(potentials, e), {e.to, &e}});
                    // cerr << vertex << " to " << e.to << " with cost " << e.cost << "\n";
                }
            }
        }
    }
    return distances;
}

ll find_mincost_maxflow(vector<vector<Edge>>& graph, ll start, ll finish, ll flow) {
    vector<Edge*> previous;
    vector<ll> potentials(graph.size(), 0);
    potentials = calc_distances(graph, start, previous, potentials);
    ll cur_flow = 0;
    while (cur_flow < flow) {
        ll vertex = finish;
        ll found_flow = INF;
        while (previous[vertex] != 0) {
            found_flow = min(found_flow, previous[vertex]->capacity);

            vertex = previous[vertex]->from;
        }
        // cerr << "finded flow " << found_flow << "\n";
        cur_flow += found_flow;
        if (found_flow == 0)
            break;
        vertex = finish;
        while (previous[vertex] != 0) {
            previous[vertex]->capacity -= found_flow;
            previous[vertex]->flow += found_flow;
            get_inv_edge(graph, *previous[vertex]).capacity += found_flow;
            get_inv_edge(graph, *previous[vertex]).flow -= found_flow;
            vertex = previous[vertex]->from;
        }

        potentials = calc_distances(graph, start, previous, potentials);
        // cerr << "updated potentials cur flow is " << cur_flow << "\n";
    }
    return cur_flow;
}

ll decompose_one_path(vector<vector<Edge>>& graph, vector<ll>& path, ll cur, ll finish) {
    if (cur == finish)
        return 0;
    // cerr << cur << "\n";
    for (Edge& e : graph[cur]) {
        if (e.flow == 1) {
            e.flow = 0;
            path.push_back(e.id);
            return decompose_one_path(graph, path, e.to, finish) + e.cost;
        }
    }
    return -1000;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(0);
    cout.tie(0);

    ll n, m, k;

    cin >> n >> m >> k;
    vector<vector<Edge>> graph(n);
    for (int i = 0; i < m; ++i) {
        ll a, b, c;
        cin >> a >> b >> c;
        --a; --b;
        graph[a].push_back({a, b, c, 0, 1, static_cast<ll>(graph[b].size()), i});
        graph[b].push_back({b, a, -c, 0, 0, static_cast<ll>(graph[a].size() - 1), i});

        graph[a].push_back({a, b, -c, 0, 0, static_cast<ll>(graph[b].size()), i});
        graph[b].push_back({b, a, c, 0, 1, static_cast<ll>(graph[a].size() - 1), i});
    }
    ll flow =  find_mincost_maxflow(graph, 0, n - 1, k);
    if (flow < k) {
        cout << "-1\n";
        return 0;
    }
    // cerr << "starting of decomposition\n";
    vector<vector<ll>> answers(k);
    ll sum_of_dists = 0;
    for (int i = 0; i < k; ++i) {
        sum_of_dists += decompose_one_path(graph, answers[i], 0, n - 1);
    }
    cout << fixed << setprecision(6) << static_cast<double>(sum_of_dists) / k << "\n";
    for (int i = 0; i < k; ++i) {
        cout << answers[i].size() << " ";
        for (ll a : answers[i])
            cout << a + 1 << " ";
        cout << "\n";
    }
    return 0;
}
