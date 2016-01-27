#include <algorithm>
#include <cassert>
#include <cmath>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <vector>

/*
function A*(start,goal)
     closedset := the empty set                 //已经被估算的节点集合    
     openset := set containing the initial node //将要被估算的节点集合
     came_from := empty map
     g_score[start] := 0                        //g(n)
     h_score[start] := heuristic_estimate_of_distance(start, goal)    //h(n)
     f_score[start] := h_score[start]            //f(n)=h(n)+g(n)，由于g(n)=0，所以……
     while openset is not empty                 //当将被估算的节点存在时，执行
         x := the node in openset having the lowest f_score[] value   //取x为将被估算的节点中f(x)最小的
         if x = goal            //若x为终点，执行
             return reconstruct_path(came_from,goal)   //返回到x的最佳路径
         remove x from openset      //将x节点从将被估算的节点中删除
         add x to closedset      //将x节点插入已经被估算的节点
         foreach y in neighbor_nodes(x)  //对于节点x附近的任意节点y，执行
             if y in closedset           //若y已被估值，跳过
                 continue
             tentative_g_score := g_score[x] + dist_between(x,y)    //从起点到节点y的距离
 
             if y not in openset          //若y不是将被估算的节点
                 add y to openset         //将y插入将被估算的节点中
                 tentative_is_better := true     
             elseif tentative_g_score < g_score[y]         //如果y的估值小于y的实际距离
                 tentative_is_better := true         //暂时判断为更好
             else
                 tentative_is_better := false           //否则判断为更差
             if tentative_is_better = true            //如果判断为更好
                 came_from[y] := x                  //将y设为x的子节点
                 g_score[y] := tentative_g_score
                 h_score[y] := heuristic_estimate_of_distance(y, goal)
                 f_score[y] := g_score[y] + h_score[y]
     return failure
 
 function reconstruct_path(came_from,current_node)
     if came_from[current_node] is set
         p = reconstruct_path(came_from,came_from[current_node])
         return (p + current_node)
     else
         return current_node
*/

int G(const std::pair<int, int>& src, const std::pair<int, int>& cur)
{
	return abs(cur.first - src.first) + abs(cur.second - src.second);
}

int H(const std::pair<int, int>& cur, const std::pair<int, int>& dst)
{
	return abs(dst.first - cur.first) + abs(dst.second - cur.second);
}

int f(const std::pair<int, int>& src, const std::pair<int, int>& cur, const std::pair<int, int>& dst)
{
	return G(src, cur) + H(cur, dst);
}

class graph
{
public:
	graph()
	{
		std::ifstream f("/home/zephyr/code/cpp/A-Star/graph.txt");
		assert(f);
		f >> column >> row;
		g.resize(column);
		for(int i = 0; i < column; ++i)
		{
			g[i].resize(row);
			for(int j = 0; j < row; ++j)
			{
				f >> g[i][j];
			}
		}
		f >> src.first >> src.second >> dst.first >> dst.second;
		f.close();
	}
	void display()
	{
		for(int i = 0; i < column; ++i)
		{
			for(int j = 0; j < row; ++j)
			{
				if(i == src.first && j == src.second) std::cout << "s" << " ";
				else if(i == dst.first && j == dst.second) std::cout << "d" << " ";
				else std::cout << g[i][j] << " ";
			}
			std::cout << std::endl;
		}
	}
	int getColumn(){return column;}
	int getRow(){return row;}
	std::pair<int, int> getSrc(){return src;}
	std::pair<int, int> getDst(){return dst;}
	bool ok(std::pair<int, int> p){return !g[p.first][p.second];}
private:
	std::vector< std::vector<int> > g;
	int column, row;
	std::pair<int, int> src, dst;
};

void examine(
	graph g, 
	std::pair< std::pair<int, int>, int > n, 
	std::pair< std::pair<int, int>, int > x,
	std::vector< std::pair< std::pair<int, int>, int > >& open,
	std::vector< std::pair< std::pair<int, int>, int > > close,
	std::map< std::pair<int, int>, std::pair<int, int> >& parent
	)
{
	if(g.ok(x.first))
	{
		if(find(close.begin(), close.end(), x) != close.end()) return;
		bool better;
		if(find(open.begin(), open.end(), x) == open.end())
		{
			open.push_back(x);
			better = true;
		}
		else if(G(g.getSrc(), n.first) + 1 < G(g.getSrc(), x.first))
			better = true;
		else
			better = false;
		if(better)
		{
			parent[x.first] = n.first;
		}
	}
}

std::pair< std::pair<int, int>, int > right(graph g, std::pair< std::pair<int, int>, int > n)
{
	return (std::pair< std::pair<int, int>, int >)
	{
		std::pair<int, int>{n.first.first, n.first.second + 1},
		f(
			g.getSrc(),
			std::pair<int, int>{n.first.first, n.first.second + 1}, 
			g.getDst()
			)
	};
}

std::pair< std::pair<int, int>, int > down(graph g, std::pair< std::pair<int, int>, int > n)
{
	return (std::pair< std::pair<int, int>, int >)
	{
		(std::pair<int, int>){n.first.first + 1, n.first.second}, 
		f(
			g.getSrc(), 
			(std::pair<int, int>){n.first.first + 1, n.first.second}, 
			g.getDst()
			)
	};
}
std::pair< std::pair<int, int>, int > left(graph g, std::pair< std::pair<int, int>, int > n)
{
	return (std::pair< std::pair<int, int>, int >)
	{
		(std::pair<int, int>){n.first.first, n.first.second - 1}, 
		f(
			g.getSrc(), 
			(std::pair<int, int>){n.first.first, n.first.second - 1}, 
			g.getDst()
			)
	};
}
std::pair< std::pair<int, int>, int > up(graph g, std::pair< std::pair<int, int>, int > n)
{
	return (std::pair< std::pair<int, int>, int >)
	{
		(std::pair<int, int>){n.first.first - 1, n.first.second},
		f(
			g.getSrc(), 
			(std::pair<int, int>){n.first.first - 1, n.first.second}, 
			g.getDst()
			)
	};
}

void display(std::vector< std::pair< std::pair<int, int>, int > > v)
{
	for(auto& p: v)
		std::cout << "( " << p.first.first << ", " << p.first.second << " ) " << p.second << std::endl;
}

void reconstructPath(
	std::map< std::pair<int, int>, 
	std::pair<int, int> > parent, 
	std::pair<int, int> currentNode, 
	std::vector< std::pair<int, int> >& path
	)
{
	path.insert(path.begin(), currentNode);
	if(parent.find(currentNode) != parent.end())
	{
		reconstructPath(parent, parent[currentNode], path);
	}
}

int main()
{
	graph g;
	g.display();
	std::cout << std::endl;
	for(int i = 0; i < g.getColumn(); ++i)
	{
		for(int j = 0; j < g.getRow(); ++j)
			std::cout << g.ok(std::pair<int, int>{i, j}) << " ";
		std::cout << std::endl;
	}
	std::cout << std::endl;

	std::cout << "src: ( " << g.getSrc().first << ", " << g.getSrc().second << " )" << std::endl;
	std::cout << "dst: ( " << g.getDst().first << ", " << g.getDst().second << " )" << std::endl;

	std::pair< std::pair<int, int>, int > testn = {
		std::pair<int, int>{2, 3}, 
		f(g.getSrc(), std::pair<int, int>{2, 3}, g.getDst())
	};
	std::cout << "right: ( " << right(g, testn).first.first << ", " << right(g, testn).first.second << " ) " << right(g, testn).second << std::endl;
	std::cout << "down: ( " << down(g, testn).first.first << ", " << down(g, testn).first.second << " ) " << down(g, testn).second << std::endl;
	std::cout << "left: ( " << left(g, testn).first.first << ", " << left(g, testn).first.second << " ) " << left(g, testn).second << std::endl;
	std::cout << "up: ( " << up(g, testn).first.first << ", " << up(g, testn).first.second << " ) " << up(g, testn).second << std::endl;
	std::cout << std::endl;

	std::vector< std::pair< std::pair<int, int>, int > > open;
	std::vector< std::pair< std::pair<int, int>, int > > close;
	std::map< std::pair<int, int>, std::pair<int, int> > parent;
	std::vector< std::pair<int, int> > path;
	open.push_back(
		(std::pair< std::pair<int, int>, int >)
		{
			g.getSrc(), 
			f(
				g.getSrc(), 
				g.getSrc(), 
				g.getDst()
				)
		}
	);
	std::pair< std::pair<int, int>, int > n, x;

	while(!open.empty())
	{
		n = (
			*std::max_element(
				open.begin(), 
				open.end(), 
				[](std::pair< std::pair<int, int>, int > a, 
					std::pair< std::pair<int, int>, int > b){
					return (a.second > b.second);
				}
				)
			);
		if(n.first.first == g.getDst().first && n.first.second == g.getDst().second)
		{
			reconstructPath(parent, g.getDst(), path);
			break;
		}
		open.erase(std::find(open.begin(), open.end(), n));
		close.push_back(n);

		examine(g, n, right(g, n), open, close, parent);
		examine(g, n, down(g, n), open, close, parent);
		examine(g, n, left(g, n), open, close, parent);
		examine(g, n, up(g, n), open, close, parent);

		std::cout << "open:" << std::endl;
		display(open);
		std::cout << "close:" << std::endl;
		display(close);
	}

	std::cout << "parent:" << std::endl;
	for(auto& p: parent)
	{
		std::cout << "( " << p.first.first << ", " << p.first.second << " ) ";
		std::cout << "( " << p.second.first << ", " << p.second.second << " )" << std::endl;
	}

	std::cout << "path:" << std::endl;
	for(auto& p: path)
	{
		std::cout << "( " << p.first << ", " << p.second << ")" << std::endl;
	}

	return 0;
}
