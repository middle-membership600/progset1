#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <vector>
#include <queue>
#include <iostream>
#include <thread>
#include <tuple>
#include <utility>
#include <cstring>
#include <cmath>
#include <thread>
using namespace std;

float calculateSD(float data[])
{
  float sum = 0.0, mean, standardDeviation = 0.0;
  int i;

  for (i = 0; i < 10; ++i)
  {
    sum += data[i];
  }

  mean = sum / 10;

  for (i = 0; i < 10; ++i)
  {
    standardDeviation += pow(data[i] - mean, 2);
  }

  return sqrt(standardDeviation / 10);
}

struct Node
{
  float weight;
  int node_num;
};

struct MST_info
{
  float total_weight;
  bool isMST;
};

vector<vector<float> > gengraph(int numpoints, int dimension)
{
  if (dimension == 0)
  {
    dimension += 1;
  }
  srand((unsigned)time(NULL));
  vector<vector<float> > vect; // contains the euclidean coordinates of each node
  for (int i = 0; i < numpoints; i++)
  {
    vector<float> v;
    for (int i = 0; i < dimension; i++)
    {
      v.push_back((float)rand() / RAND_MAX);
    }
    vect.push_back(v); // add each node to the list of nodes
  }                    // printing out all nodes and their coordinates

  return vect;
}
float euclidean_distance(vector<float> v1, vector<float> v2)
{
  // given two vectors with points in R^n, calculate the distance between them,
  if (v1.size() != v2.size())
  {
    throw std::invalid_argument("Vectors must have the same size");
  }
  float sum_of_squares = 0.0;
  for (size_t i = 0; i < v1.size(); i++)
  {
    float diff = v1[i] - v2[i];
    sum_of_squares += diff * diff;
  }
  return std::sqrt(sum_of_squares);
}

vector<vector<Node> > adjacency_list(vector<vector<float> > g)
{

  vector<vector<Node> > a;
  for (int i = 0; i < g.size(); i++)
  {
    vector<Node> k;

    for (int j = 0; j < g.size(); j++)
    {
      if (i == j)
        continue;
      Node h;
      h.node_num = j;
      float w;
      if (g[i].size() == 1)
      {
        // float k = (float)g[i][j];
        h.weight = (float)rand() / RAND_MAX;
        w = 1.2;
        // cout << h.weight << "\n";
      }
      else
      {
        h.weight = euclidean_distance(g[i], g[j]);
      }

      if (g[i].size() == 2)
      {
        w = (float)1 * pow((0.91105165234), log2(g.size()) - 7);
      }
      if (g[i].size() == 3)
      {
        w = (float)1 * pow((0.83266771883), log2(g.size()) - 7);
      }
      if (g[i].size() == 4)
      {
        w = (float)1 * pow((0.79943993619), log2(g.size()) - 7);
      }
      // prune based off of w.
      //where w is based off of the ratio between the average length of the minimum spanning trees at the given dimension.

      if (h.weight >= w)
        continue;

      k.push_back(h);
    }
    a.push_back(k);
  }

  return a;
}

void heapify(vector<Node> &heap, int i)
{
  int size = heap.size();
  int minimum = i;
  int left_parent = i * 2 + 1;
  int right_parent = i * 2 + 2;

  // Check both parents if we can move element at index i
  if (left_parent < size && heap[minimum].weight > heap[left_parent].weight)
  {
    minimum = left_parent;
  }

  if (right_parent < size && heap[minimum].weight > heap[right_parent].weight)
  {
    minimum = right_parent;
  }

  if (minimum != i)
  {
    swap(heap[i], heap[minimum]);
    heapify(heap, minimum);
  }
};

void build_heap(vector<Node> &heap)
{
  int mid = (heap.size() / 2) - 1;
  for (int i = mid; i >= 0; i--)
  {
    heapify(heap, i);
  }
};

void insert_into_heap(vector<Node> &heap, int node, float weight)
{
  Node new_vertex = {weight, node};
  heap.push_back(new_vertex);
  if (heap.size() != 0)
  {
    int mid = (heap.size() / 2) - 1;
    for (int i = mid; i >= 0; i--)
    {
      heapify(heap, i);
    }
  }
};

struct Node extract_min(vector<Node> &heap)
{
  int last_index = heap.size() - 1;
  swap(heap[0], heap[last_index]);
  float min_weight = heap[last_index].weight;
  int min_vertex = heap[last_index].node_num;
  heap.pop_back();
  heapify(heap, 0);
  Node minimum_node = {min_weight, min_vertex};

  return minimum_node;
};

void print_heap(vector<Node> &heap)
{
  for (int i = 0; i < heap.size(); i++)
  {
    cout << heap[i].node_num << ": " << heap[i].weight << endl;
  }
};

struct MST_info prims_MST(int num_vertices, vector<vector<Node> > graph)
{
  float total_weight = 0;
  bool is_MST = true;
  vector<Node> heap;
  Node k;
  k.weight = 0;
  k.node_num = 0;
  heap.push_back(k);
  bool in_MST[graph.size()];
  memset(in_MST, false, sizeof(bool) * graph.size());

  while (heap.size())
  {
    Node k;
    k = extract_min(heap);
    if (in_MST[k.node_num])
    {
      continue;
    }

    in_MST[k.node_num] = true;
    total_weight += k.weight;
    for (int i = 0; i < graph[k.node_num].size(); i++)
    {
      int neighbor = graph[k.node_num][i].node_num;
      float neighbor_weight = graph[k.node_num][i].weight;
      if (in_MST[neighbor])
      {
        continue;
      }
      insert_into_heap(heap, neighbor, neighbor_weight);
    }
  };

  for (int i = 0; i < graph.size(); i++)
  {
    if (!in_MST[i])
    {
      is_MST = false;
      break;
    }
  }
  MST_info a = {.total_weight = total_weight, .isMST = is_MST};
  return a;
};

int main(int argc, char *argv[])
{
  if (argc < 4)
  {
    printf("Not enough arguments\n");
    exit(1);
  } // set numpoints, numtrials, dimension to the CLI arguments
  int numpoints = atoi(argv[1]);
  int numtrials = atoi(argv[2]);
  int dimension = atoi(argv[3]);
  float average;
  float data[numtrials];
  time_t start = time(0);
  for (int i = 0; i < numtrials; i++)
  {
    vector<vector<Node> > k = adjacency_list(gengraph(numpoints, dimension)); // create an adjacency list from randomly generated euclidean graph
    MST_info result = prims_MST(numpoints, k);
    // cout << result.isMST << ": " << result.total_weight << endl;
    data[i] = result.total_weight;
    average += result.total_weight;
    assert(result.isMST);
  }
  float devi = calculateSD(data);
  average = average / numtrials;
  time_t end = time(0);
  cout << "number of points: " << numpoints << " "
       << "dimensions: " << dimension << "\naverage weight: " << average << "\naverage deviation" << devi << "\n";
  double time = difftime(end, start) * 1000.0 / numtrials;
  cout << "time: " << time << "\n";

  return 0;
}
