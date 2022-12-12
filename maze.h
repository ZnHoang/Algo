#ifndef MAZE
#define MAZE

#include <iostream>
#include <vector>
#include <chrono>
using std::vector, std::pair;
using std::make_pair;

enum class MAZETYPE
{
    ROAD, WALL, GONE, REALWAY
};
using weight = int;
using cell = pair<uint, uint>;
using cellStatu = pair<MAZETYPE, weight>;
// using timepoint = std::chrono::_V2::steady_clock::time_point;

class maze
{

private:
    void _createMaze(vector<vector<cellStatu>>&, const int&, const int&);
    void createMaze();
    void showMaze(vector<vector<cellStatu>>&);
    bool _dfs(vector<vector<cellStatu>>&, const int&, const int&, vector<cell>&);
    bool _bfs(vector<vector<cellStatu>>&, vector<cell>&);
    int _aStar(vector<vector<cellStatu>>&, vector<cell>&, bool);
    void setRealWay(vector<vector<cellStatu>>&, vector<cell>&);
    bool isRoadNearEdge(const uint&, const uint&);
    inline weight getDiff(const weight& begin, const weight& end) {return begin - end;}
public:
    maze(const int size);
    ~maze();
    void dfs();
    void bfs();
    void dijkstra();
    void aStar();
    void find();
private:
    vector<vector<cellStatu>> originalMaze;
    cell enter = std::make_pair(1, 0);
    cell exit;
    weight direction[4][2]{-1, 0, 0, 1, 1, 0, 0, -1};
    uint mazeSize = DEFAULTSIZE;
    vector<cell> roadNrEdge;
private:
    const weight MINWAYWEIGHT = 1;
    const weight MINWALLWEIGHT = 20;
    const weight WEIGHTGAP = 19;
    const uint DEFAULTSIZE = 50;
    inline weight getWayWeight() {return rand() % WEIGHTGAP + MINWAYWEIGHT;}
    inline weight getWallWeight() {return rand() % WEIGHTGAP + MINWALLWEIGHT;}
    // inline timepoint NOW() {return std::chrono::steady_clock::now();}
    // inline std::chrono::nanoseconds USED(timepoint end, timepoint begin) {return std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);}
    // inline void PRINT(const char* func, const std::chrono::nanoseconds time, bool res, const int len) {printf("\033[33m%-20s%-20.8f%-20s%-20d\n\033[0m", (func), time.count() * 1e-9, res?"WAY_OUT":"NO_WAY_OUT", len);}
    // inline void PRINT(const char* func, const std::chrono::nanoseconds time, const weight w, const int len) {printf("\033[33m%-20s%-20.8f%-20d%-20d\n\033[0m", func, time.count() * 1e-9, w, len);}

};

#endif