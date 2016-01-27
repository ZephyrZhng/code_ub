import java.util.*;

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

class Point{
	public int x;
	public int y;
	public Point(){}
	public Point(int _x, int _y){
		this.x = _x;
		this.y = _y;
	}
	public Point(Point _p){
		this.x = _p.x;
		this.y = _p.y;
	}
	public void display(){
		System.out.print("( " + this.x + ", " + this.y + " ) ");
	}
}

class Node{
	public Point p;
	public int f;
	public Node(){
		this.p = new Point();
		this.f = 0;
	}
	public Node(Point _p, int _f){
		this.p = new Point(_p);
		this.f = _f;
	}
	public Node(int _x, int _y, int _f){
		this.p = new Point(_x, _y);
		this.f = _f;
	}
	public Node(Node _n){
		this.p = new Point(_n.p);
		this.f = _n.f;
	}
	public void display(){
		this.p.display();
		System.out.print(this.f + "\n");
	}
}

class MapNode{
	public Point child;
	public Point parent;
	public MapNode(){
		this.child = new Point();
		this.parent = new Point();
	}
	public MapNode(Point _child, Point _parent){
		this.child = new Point(_child);
		this.parent = new Point(_parent);
	}
	public MapNode(int _child_x, int _child_y, int _parent_x, int _parent_y){
		this.child = new Point(_child_x, _child_y);
		this.parent = new Point(_parent_x, _parent_y);
	}
	public MapNode(MapNode _n){
		this.child = new Point(_n.child);
		this.parent = new Point(_n.parent);
	}
	public void display(){
		this.child.display();
		this.parent.display();
		System.out.println();
	}
}

class Map{
	private MapNode[] map;
	private int len;
	public Map(){
		this.map = new MapNode[72];
		this.len = 0;
	}
	public int getLen(){return this.len;}
	public void insert(Point child, Point parent){
		for(int i = 0; i < this.len; ++i)
			if(this.map[i].child.x == child.x &&
				this.map[i].child.y == child.y &&
				this.map[i].parent.x == parent.x &&
				this.map[i].parent.y == parent.y)
				return;
		this.map[this.len++] = new MapNode(child, parent);
	}
	public void display(){
		for(int i = 0; i < this.len; ++i)
			this.map[i].display();
	}
}

class Vec{
	public Node[] v;
	private int len;
	public Vec(){
		this.v = new Node[72];
		this.len = 0;
	}
	public int getLen(){return this.len;}
	public void append(Node n){
		this.v[this.len++] = new Node(n);
	}
	public int find(Node n){
		for(int i = 0; i < this.len; ++i)
			if(this.v[i].p.x == n.p.x &&
				this.v[i].p.y == n.p.y &&
				this.v[i].f == n.f)
				return i;
		return -1;
	}
	public int findMax(){
		int max = 0;
		int pos = 0;
		int i = 0;
		for(; i < this.len; ++i)
			if(this.v[i].f > max)
			{
				max = this.v[i].f;
				pos = i;
			}
		return pos;
	}
	public void delete(int _i){
		for(int i = _i + 1; i < this.len; ++i)
			this.v[i - 1] = new Node(this.v[i]);
		--this.len;
	}
	public boolean empty(){return (this.len == 0)? true: false;}
	public void display(){
		for(int i = 0; i < this.len; ++i)
			this.v[i].display();
		System.out.println();
	}
}

class Graph{
	private int g[][] = {
		{1, 1, 1, 1, 1, 1, 1, 1, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 1, 0, 0, 0, 1},
		{1, 0, 1, 0, 1, 0, 0, 0, 1},
		{1, 0, 0, 0, 0, 0, 0, 0, 1},
		{1, 1, 1, 1, 1, 1, 1, 1, 1}
	};
	private int column, row;
	private Point src, dst;
	public Graph(){
		this.src = new Point(4, 2);
		this.dst = new Point(5, 6);
		this.column = 8;
		this.row = 9;
	}
	public void display(){
		for(int i = 0; i < this.column; ++i)
		{
			for(int j = 0; j < this.row; ++j)
			{
				if(i == this.src.x && j == this.src.y)
					System.out.print("s ");
				else if(i == this.dst.x && j == this.dst.y)
					System.out.print("d ");
				else
					System.out.print(this.g[i][j] + " ");
			}
			System.out.println();
		}
	}
	public int getColumn(){return this.column;}
	public int getRow(){return this.row;}
	public Point getSrc(){return this.src;}
	public Point getDst(){return this.dst;}
	public boolean ok(Point p){
		return (this.g[p.x][p.y] == 1)? false: true;
	}
}

public class A_Star{
	public static int G(Point src, Point cur){
		return Math.abs(cur.x - src.x) + Math.abs(cur.y - src.y);
	}
	public static int H(Point cur, Point dst){
		return Math.abs(dst.x - cur.x) + Math.abs(dst.y - cur.y);
	}
	public static int F(Point src, Point cur, Point dst){
		return G(src, cur) + H(cur, dst);
	}
	public static void examine(Graph g,	Node n, Node x, Vec open, Vec close, Map parent){
		if(g.ok(x.p)){
			if(close.find(x) >= 0) return;
			boolean better = false;
			if(open.find(x) < 0){
				open.append(x);
				better = true;
			}else if(G(g.getSrc(), n.p) + 1 < G(g.getSrc(), x.p)){
				better = true;
			}
			if(better) parent.insert(x.p, n.p);
		}
	}
	public static Node right(Graph g, Node n){
		return new Node(
			new Point(n.p.x, n.p.y + 1),
			F(
				g.getSrc(),
				new Point(n.p.x, n.p.y + 1),
				g.getDst()
				)
			);
	}
	public static Node down(Graph g, Node n){
		return new Node(
			new Point(n.p.x + 1, n.p.y),
			F(
				g.getSrc(),
				new Point(n.p.x + 1, n.p.y),
				g.getDst()
				)
			);
	}
	public static Node left(Graph g, Node n){
		return new Node(
			new Point(n.p.x, n.p.y - 1),
			F(
				g.getSrc(),
				new Point(n.p.x, n.p.y - 1),
				g.getDst()
				)
			);
	}
	public static Node up(Graph g, Node n){
		return new Node(
			new Point(n.p.x - 1, n.p.y),
			F(
				g.getSrc(),
				new Point(n.p.x - 1, n.p.y),
				g.getDst()
				)
			);
	}
	public static void main(String args[]){
		// Map m = new Map();
		// m.insert(new Point(1, 1), new Point(1, 1));
		// m.insert(new Point(1, 1), new Point(2, 2));
		// m.insert(new Point(2, 2), new Point(1, 1));
		// m.insert(new Point(1, 1), new Point(1, 1));
		// m.insert(new Point(2, 2), new Point(2, 2));
		// m.display();

		// Vec v = new Vec();
		// v.append(new Node(3, 3, 3));
		// v.append(new Node(2, 2, 2));
		// v.append(new Node(1, 1, 1));
		// v.append(new Node(3, 3, 3));
		// v.display();
		// System.out.println(v.getLen());

		// v.v[v.find(new Node(1, 1, 1))].display();
		// v.v[v.findMax()].display();
		// v.delete(2);
		// System.out.println(v.getLen());
		// v.display();

		Graph g = new Graph();
		g.display();

		// Node n = new Node(2, 3, F(g.getSrc(), new Point(2, 3), g.getDst()));
		// n.display();
		// new Node(right(g, n)).display();
		// new Node(down(g, n)).display();
		// new Node(left(g, n)).display();
		// new Node(up(g, n)).display();

		for(int i = 0; i < g.getColumn(); ++i)
		{
			for(int j = 0; j < g.getRow(); ++j)
				System.out.print(g.ok(new Point(i, j)) + " ");
			System.out.println();
		}
		g.getSrc().display();
		g.getDst().display();
		System.out.println();

		Vec open = new Vec();
		Vec close = new Vec();
		Map parent = new Map();
		open.append(new Node(g.getSrc(), F(g.getSrc(), g.getSrc(), g.getDst())));
		Node n = new Node();

		while(!open.empty()){
			n = open.v[open.findMax()];
			if(n.p.x == g.getDst().x && n.p.y == g.getDst().y) break;
			open.delete(open.findMax());
			close.append(n);

			examine(g, n, down(g, n), open, close, parent);
			examine(g, n, right(g, n), open, close, parent);
			examine(g, n, left(g, n), open, close, parent);
			examine(g, n, up(g, n), open, close, parent);

			System.out.println("open:");
			open.display();
			System.out.println("close:");
			close.display();
		}
		System.out.println("parent:");
		parent.display();
	}
}
