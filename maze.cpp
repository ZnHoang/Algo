/*
** PROBLEMS:
** a.if the size is more than 800, the program may crash.
** b.aStar() is wrong, the queue must sort by the distance between the current point and the exit but not between the current point and the enter
*/
#include <ctime>
#include <queue>
#include <map>
#include "maze.h"
using std::priority_queue, std::map;


maze::maze(const int size)
{
    mazeSize = size >= 10 ? size : DEFAULTSIZE;
    createMaze();
}

maze::~maze()
{
}

void maze::_createMaze(vector<vector<cellStatu>>& maze, const int& row, const int& col)
{
    static int rank = 1;
    maze[row][col].first = MAZETYPE::ROAD;  maze[row][col].second = 1;
    auto direction = this->direction;
	for (int i = 0; i < 4; i++) {
		int r = rand() % 4;
		int temp = direction[0][0];
		direction[0][0] = direction[r][0];
		direction[r][0] = temp;
		temp = direction[0][1];
		direction[0][1] = direction[r][1];
		direction[r][1] = temp;
	}
    for (int i = 0; i < 4; i++) {
		int dx = row;
		int dy = col;
 
		int range = 1 + (rank == 0 ? 0 : rand() % rank);
		while (range>0) {
			dx += direction[i][0];
			dy += direction[i][1];
 
			if (maze[dx][dy].first == MAZETYPE::ROAD) {
				break;
			}
 
			int count = 0;
			for (int j = dx - 1; j < dx + 2; j++) {
				for (int k = dy - 1; k < dy + 2; k++) {
					if (abs(j - dx) + abs(k - dy) == 1 && maze[j][k].first == MAZETYPE::ROAD) {
						count++;
					}
				}
			}
			if (count > 1) {
				break;
			}
			--range;
			maze[dx][dy].first = MAZETYPE::ROAD;    maze[dx][dy].second = 1;
		}
		if (range <= 0) {
			_createMaze(maze, dx, dy);
		}
	}
}

void maze::createMaze()
{
    srand((uint)time(0));
    vector<vector<cellStatu>> tMaze(mazeSize + 2, vector<cellStatu>(mazeSize + 2, make_pair(MAZETYPE::WALL, 0)));
    for(int i = 1; i < mazeSize + 1; i++)
    {
        for(int j = 1; j < mazeSize + 1; j++)
        {
            tMaze[i][j].second = getWallWeight();
        }
    }
    // auto begin = NOW();

    for(int i = 0; i < tMaze.size(); i++)
    {
        tMaze[i][0].first = MAZETYPE::ROAD;                 tMaze[i][0].second = getWayWeight();
        tMaze[0][i].first = MAZETYPE::ROAD;                 tMaze[0][i].second = getWayWeight();
        tMaze[i][tMaze.size() - 1].first = MAZETYPE::ROAD;  tMaze[i][tMaze.size() - 1].second = getWayWeight();
        tMaze[tMaze.size() - 1][i].first = MAZETYPE::ROAD;  tMaze[tMaze.size() - 1][i].second = getWayWeight();
    }
    _createMaze(tMaze, 2, 2);
    tMaze[enter.first + 1][enter.second + 1].first = MAZETYPE::ROAD;   tMaze[enter.first + 1][enter.second + 1].second = getWayWeight();
    for (int i = tMaze.size() - 3; i >= 0; i--) 
    {
		if (tMaze[i][tMaze.size() - 3].first == MAZETYPE::ROAD) 
        {
			tMaze[i][tMaze.size() - 2].first = MAZETYPE::ROAD;  tMaze[i][tMaze.size() - 2].second = getWayWeight();
            exit = make_pair(i - 1, tMaze.size() - 3);
			break;
		}
    }
    for(int row = 1; row < tMaze.size() - 1; row++)
    {
        originalMaze.emplace_back(vector<cellStatu>(tMaze[row].begin() + 1, tMaze[row].end() - 1));
    }

    // auto used = USED(NOW(), begin);
    // printf("\033[33m%s\t%.8fs\t%d\n\033[0m", __FUNCTION__, used.count() * 1e-9, mazeSize);
}

void maze::showMaze(vector<vector<cellStatu>>& maze)
{
    for(uint i = 0; i < maze.size(); i++)
    {
        for(uint j = 0; j < maze.size(); j++)
        {
            if(maze[i][j].first == MAZETYPE::ROAD || maze[i][j].first == MAZETYPE::GONE)
            {
                printf("  ");
            }
            else if(maze[i][j].first == MAZETYPE::WALL)
            {
                printf("国");
            }
            else if(maze[i][j].first == MAZETYPE::REALWAY)
            {
                printf("××");
            }
        }
        printf("\n");
    }
}

void maze::setRealWay(vector<vector<cellStatu>>& maze, vector<cell>& way)
{
    for(const auto& p : way)
    {
        maze[p.first][p.second].first = MAZETYPE::REALWAY;
    }
}

bool maze::isRoadNearEdge(const uint& row, const uint& col)
{
    return row == 1 || row == mazeSize - 2 || col == 1 || col == mazeSize - 2;
}

bool maze::_dfs(vector<vector<cellStatu>>& maze, const int& row, const int& col, vector<cell>& way)
{
    if(row < 0 || row >= mazeSize || col < 0 || col > mazeSize || (col != mazeSize && maze[row][col].first != MAZETYPE::ROAD))
    {
        return false;
    }
    way.emplace_back(make_pair(row, col));
    if(row == exit.first && col == exit.second)
    {
        return true;
    }

    maze[row][col].first = MAZETYPE::GONE;
    for(auto& d : direction)
    {
        if(_dfs(maze, row + d[0], col + d[1], way))
        {
            return true;
        }
    }
    way.erase(way.end() - 1);
    return false;
}

void maze::dfs()
{
    auto vMaze = originalMaze;
    // auto begin = NOW();

    vector<cell> way;
    bool res = _dfs(vMaze, enter.first, enter.second, way);

    // auto used = USED(NOW(), begin);
    // PRINT(__FUNCTION__, used, res, way.size());
    // setRealWay(vMaze, way);
    // showMaze(vMaze);
}

bool maze::_bfs(vector<vector<cellStatu>>& maze, vector<cell>& way)
{
    std::queue<vector<cell>> q;
    q.push(vector<cell>({enter}));
    maze[enter.first][enter.second].first = MAZETYPE::GONE;
    while(!q.empty())
    {
        auto tWay = q.front();
        q.pop();
        auto last = tWay.back();
        for(auto& d : direction)
        {
            auto newRow = last.first + d[0], newCol = last.second + d[1];
            if(newRow >= 0 && newRow < maze.size() && newCol >= 0 && newCol < maze[newRow].size() && maze[newRow][newCol].first == MAZETYPE::ROAD)
            {
                maze[newRow][newCol].first = MAZETYPE::GONE;
                auto newTWay = tWay;
                newTWay.emplace_back(make_pair(newRow, newCol));
                if(newRow == exit.first && newCol == exit.second)
                {
                    way = newTWay;
                    return true;
                }
                q.push(newTWay);
            }
        }
    }
    return false; 
}

void maze::bfs()
{
    auto tMaze = originalMaze;
    // auto begin = NOW();

    vector<cell> way;
    bool res = _bfs(tMaze, way);

    // auto used = USED(NOW(), begin);
    // PRINT(__FUNCTION__, used, res, way.size());
    // setRealWay(tMaze, way);
    // showMaze(tMaze);
}

void maze::dijkstra()
{
    // auto begin = NOW();

    // auto used = USED(NOW(), begin);
    // // PRINT(__FUNCTION__, used, )
}

weight maze::_aStar(vector<vector<cellStatu>>& maze, vector<cell>& way, bool climb = true)
{
    /*if the climb is true, it could pass the wall*/
    auto cmp = [](const pair<cell, pair<weight, cell>>& a, const pair<cell, pair<weight, cell>>& b) {return a.second.first >= b.second.first;};
    map<cell, cell> hashCellCameFrom;
    map<cell, weight> hashCell2Weight;
    priority_queue<pair<cell, pair<weight,cell>>, vector<pair<cell, pair<weight, cell>>>, decltype(cmp)> q(cmp);

    cell tCell = make_pair(UINT32_MAX, UINT32_MAX);
    q.push(make_pair(enter, make_pair(0, tCell)));

    while(!q.empty())
    {
        auto p = q.top();
        q.pop();
        cell point = p.first;
        weight w = p.second.first;
        cell lastPoint = p.second.second;
        if(hashCell2Weight.find(point) != hashCell2Weight.end())
        {
            continue;
        }
        hashCellCameFrom[point] = lastPoint;
        hashCell2Weight[point] = w;
        weight curWeight = maze[point.first][point.second].second;
        if(point == exit)
        {
            while(point != tCell)
            {
                way.emplace_back(point);
                point = hashCellCameFrom[point];
            }
            std::reverse(way.begin(), way.end());
            return w;
        }
        for(auto& d : direction)
        {
            auto nextRow = point.first + d[0], nextCol = point.second + d[1];
            if(nextRow < 0 || nextRow >= maze.size() || nextCol < 0 || nextCol >= maze[nextRow].size() || (!climb && maze[nextRow][nextCol].first == MAZETYPE::WALL))
            {
                continue;
            }
            cell nextPoint = make_pair(nextRow, nextCol);
            weight nextWeight = maze[nextRow][nextCol].second;
            q.push(make_pair(nextPoint, make_pair(w + getDiff(curWeight, nextWeight), point)));
        }
    }
}

void maze::aStar()
{
    auto tMaze = originalMaze;
    // auto begin = NOW();

    vector<cell> way;
    weight minWeight = _aStar(tMaze, way);

    // auto used = USED(NOW(), begin);
    // PRINT(__FUNCTION__, used, minWeight, way.size());
    setRealWay(tMaze, way);
    showMaze(tMaze);
}

void maze::find()
{
    printf("\033[4;31m%-20s%-20s%-20s%-20s\n\033[0m", "FUNCTION", "USED_TIME(s)", "ESCAPE", "DISTANCE(cell)");
    dfs();
    bfs();
    printf("\033[4;31m%-20s%-20s%-20s%-20s\n\033[0m", "FUNCTION", "USED_TIME(s)", "WEIGHT", "DISTANCE(cell)");
    dijkstra();
    aStar();
}

