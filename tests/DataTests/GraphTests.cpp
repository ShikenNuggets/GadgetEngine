#include "Data/Graph.h"

#include "_Catch2/catch_amalgamated.hpp"

using namespace Gadget;

//------------------------------------------------------------//
//---------------------- Graph BFS ---------------------------//
//------------------------------------------------------------//
TEST_CASE("Graph - BFS" "[graph_bfs]"){
	Graph<int> graph;
	auto node1 = graph.AddNode(10);
	auto node2 = graph.AddNode(20, node1);	auto node2_1 = graph.AddNode(21, node1);
	auto node3 = graph.AddNode(30, node2);	auto node3_1 = graph.AddNode(31, node2_1);
	auto node4 = graph.AddNode(40, node3);	auto node4_1 = graph.AddNode(41, node3);
	auto node5 = graph.AddNode(50, node4);	auto node5_1 = graph.AddNode(51, node4_1);
	auto node6 = graph.AddNode(60, node5);	auto node6_1 = graph.AddNode(61);

	REQUIRE(graph.HasRoute(graph.GetRoot(), node1));
	REQUIRE(graph.HasRoute(graph.GetRoot(), node2));
	REQUIRE(graph.HasRoute(graph.GetRoot(), node2_1));
	REQUIRE(graph.HasRoute(graph.GetRoot(), node3));
	REQUIRE(graph.HasRoute(graph.GetRoot(), node3_1));
	REQUIRE(graph.HasRoute(graph.GetRoot(), node4));
	REQUIRE(graph.HasRoute(graph.GetRoot(), node4_1));
	REQUIRE(graph.HasRoute(graph.GetRoot(), node5));
	REQUIRE(graph.HasRoute(graph.GetRoot(), node5_1));
	REQUIRE(graph.HasRoute(graph.GetRoot(), node6));
	REQUIRE(!graph.HasRoute(graph.GetRoot(), node6_1));
}