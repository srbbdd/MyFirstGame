#include "map.h"
void Map::generate(unsigned int seed = 0)
{
	for (auto& row : tiles)
	{
		for (auto& tile : row)
		{
			tile = Tile{};
		}
	}
	rooms.clear();
	if (seed == 0)
	{
		std::random_device rd;
		seed = rd();
	}
	m_rng.seed(seed);
	BSPNode root;
	root.x = 0;
	root.y = 0;
	root.h = MAP_HEIGHT;
	root.w = MAP_WIDTH;
	split(root, 0);
	buildRooms(root);
	connectRooms(root);
}
static constexpr const int MAX_DEPTH = 4;
static constexpr const int MIN_LEAF_W = 12;
static constexpr const int MIN_LEAF_H = 9;
static constexpr const int ROOM_PADDING = 1;
static constexpr const int ROOM_MIN = 4;
void Map::split(BSPNode& node, int depth)
{
	if (depth >= MAX_DEPTH)
	{
		return;
	}
	bool canSplitH = (node.h >= MIN_LEAF_H * 2);
	bool canSplitV = (node.w >= MIN_LEAF_W * 2);
	if (canSplitH && canSplitV)
	{
		return;
	}
	bool horizontal;
	if (!canSplitV)
	{
		horizontal = true;
	}
	else if (!canSplitH)
	{
		horizontal = false;
	}
	else
	{
		horizontal = (node.h > node.w) || (node.h == node.w && coinFlip());
	}
	node.left = std::make_unique<BSPNode>();
	node.right = std::make_unique<BSPNode>();
	if (horizontal)
	{
		int splity = std::uniform_int_distribution<int>(node.y + MIN_LEAF_H, node.y + node.h - MIN_LEAF_H)(m_rng);
		node.left->x = node.x;
		node.left->y = node.y;
		node.left->w = node.w;
		node.left->h = splity - node.y;
		node.right->x = node.x;
		node.right->y = node.y;
		node.right->w = node.w;
		node.right->h = (node.h + node.y) - splity;
	}
	else
	{
		int splitx = std::uniform_int_distribution<int>(node.x + MIN_LEAF_W, node.x + node.w - MIN_LEAF_W)(m_rng);
		node.left->x = node.x;
		node.left->y = node.y;
		node.left->w = splitx - node.x;
		node.left->h = node.h;
		node.right->x = splitx;
		node.right->y = node.y;
		node.right->w = (node.x + node.w)-splitx;
		node.right->h = node.h;
	}
	split(*node.left, depth + 1);
	split(*node.right, depth + 1);
}
void Map::buildRooms(BSPNode& node)
{
	if (node.left)
	{
		buildRooms(*node.left);
		buildRooms(*node.right);
		return;
	}
	int maxW = node.w - ROOM_PADDING * 2;
	int maxH = node.h - ROOM_PADDING * 2;
	if (maxW < ROOM_MIN || maxH < ROOM_MIN)
	{
		return;
	}
	int rw = std::uniform_int_distribution<int>(ROOM_MIN,maxW)(m_rng);
	int rh = std::uniform_int_distribution<int>(ROOM_MIN, maxH)(m_rng);
	int rx = node.x + ROOM_PADDING + std::uniform_int_distribution<int>(0, maxW - rw)(m_rng);
	int ry = node.y + ROOM_PADDING + std::uniform_int_distribution<int>(0, maxH - rh)(m_rng);
	for (int y = ry; y < ry + rh; y++)
	{
		for (int x = rx; x < rx + rw; x++)
		{
			carveTile(x, y, TileType::Floor);
		}
	}
	node.room = { rx,ry,rw,rh };
	node.hasroom = true;
	rooms.push_back(node.room);
}
void Map::connectRooms(BSPNode& node)
{

}