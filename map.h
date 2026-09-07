#include <vector>
#include <algorithm>
#include <random>
using namespace std;
const int MAP_WIDTH = 64;
const int MAP_HEIGHT = 36;
const int TILE_SIZE = 20;
enum class TileType{Wall,Floor};
struct Tile
{
	TileType type = TileType::Wall;
};
struct Room
{
	int x = 0;
	int y = 0;
	int h = 0;
	int w = 0;
	int centreX() const
	{
		return (x + w / 2);
	}
	int centreY()const
	{
		return (y + h / 2);
	}
};
class Map
{
public:
	Tile tiles[MAP_HEIGHT][MAP_WIDTH];
	vector<Room> rooms;
	void generate(unsigned int seed = 0);
private:
	std::mt19937 m_rng;
	struct BSPNode
	{
		int x = 0;
		int y = 0;
		int w = 0;
		int h = 0;
		std::unique_ptr<BSPNode> left;
		std::unique_ptr<BSPNode> right;
		Room room;
		bool hasroom = false;
	};
	void split(BSPNode& node, int depth);
	void buildRooms(BSPNode& node);
	void connectRooms(BSPNode& node);
	void carveHLine(int x1, int x2, int y);
	void carveVLine(int y1, int y2, int x);
	void carveTile(int x, int y, TileType type);
	bool coinFlip()
	{
		return(m_rng() &1) == 0;
	}
};