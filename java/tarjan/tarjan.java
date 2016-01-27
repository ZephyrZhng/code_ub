import java.io.*;
import java.lang.Math;
import java.util.*;

/*
algorithm tarjan is
  input: 图 G = (V, E)
  output: 以所在的强连通分量划分的顶点集

  index := 0
  S := empty    // 置栈为空
  for each v in V do
    if (v.index is undefined)
      strongconnect(v)
    end if

  function strongconnect(v)
    // 将未使用的最小index值作为结点v的index
    v.index := index
    v.lowlink := index
    index := index + 1
    S.push(v)

    // 考虑v的后继结点
    for each (v, w) in E do
      if (w.index is undefined) then
        // 后继结点w未访问，递归调用
        strongconnect(w)
        v.lowlink := min(v.lowlink, w.lowlink)
      else if (w is in S) then
        // w已在栈S中，亦即在当前强连通分量中
        v.lowlink := min(v.lowlink, w.index)
      end if

    // 若v是根则出栈，并求得一个强连通分量
    if (v.lowlink = v.index) then
      start a new strongly connected component
      repeat
        w := S.pop()
        add w to current strongly connected component
      until (w = v)
      output the current strongly connected component
    end if
  end function
*/

class graph{
	public ArrayList<ArrayList> g = new ArrayList<ArrayList>();
	public Integer vexNum = null;

	public graph(){
		FileInputStream fis = null;
		try{
			fis = new FileInputStream("./graph.txt");
		}catch(FileNotFoundException fnfe){
			fnfe.printStackTrace();
		}
		InputStreamReader isr = new InputStreamReader(fis);
		BufferedReader br = new BufferedReader(isr);
		try{
			String line = br.readLine();
			this.vexNum = Integer.parseInt(line);
			for(int i = 0; i < this.vexNum; ++i){
				line = br.readLine();
				String[] str = line.split(" ");
				ArrayList gi = new ArrayList();
				for(int j = 0; j < this.vexNum; ++j){
					gi.add(Integer.parseInt(str[j]));
				}
				this.g.add(gi);
			}
		}catch(IOException ioe){
			ioe.printStackTrace();
		}
		try{
			isr.close();
		}catch(IOException ioe){
			ioe.printStackTrace();
		}
		System.out.println("vexNum: " + this.vexNum);
		this.display();
	}

	public void display(){
		System.out.println("graph:");
		Iterator it = this.g.iterator();
		while(it.hasNext()){
			System.out.println(it.next());
		}
	}
}

public class tarjan{
	private static ArrayList index = new ArrayList();
	private static ArrayList lowLink = new ArrayList();
	private static ArrayList s = new ArrayList();
	private static ArrayList<ArrayList> stronglyConnectedComponent = new ArrayList<ArrayList>();
	private static int idx = 0;

	public tarjan(graph g){
		for(int i = 0; i < g.vexNum; ++i){
			this.index.add(-1);
			this.lowLink.add(-1);
		}
	}

	// public static void display(ArrayList<ArrayList> al){
	// 	Iterator it = al.iterator();
	// 	while(it.hasNext()){
	// 		System.out.println(it.next());
	// 	}
	// }

	public void strongConnect(int v, graph g){
		this.index.set(v, this.idx);
		this.lowLink.set(v, this.idx);
		++idx;
		this.s.add(v);

		int w = 0;

		for(; w < g.vexNum; ++w){
			if(Integer.valueOf((Integer)g.g.get(v).get(w)) <= 0){
				continue;
			}

			if(Integer.valueOf((Integer)this.index.get(w)) <= 0){
				strongConnect(w, g);
				this.lowLink.set(v, Math.min(Integer.valueOf((Integer)this.lowLink.get(v)), 
											 Integer.valueOf((Integer)this.lowLink.get(w))));
			}else if(this.s.contains(w)){
				this.lowLink.set(v, Math.min(Integer.valueOf((Integer)this.lowLink.get(v)),
											 Integer.valueOf((Integer)this.index.get(w))));
			}
		}

		if(Integer.valueOf((Integer)this.lowLink.get(v)).equals(Integer.valueOf((Integer)this.index.get(v)))){
			ArrayList newStronglyConnectedComponent = new ArrayList();
			do{
				w = Integer.valueOf((Integer)this.s.get(this.s.size() - 1));
				this.s.remove(this.s.size() - 1);
				newStronglyConnectedComponent.add(w);
			}while(w != v);
			// System.out.println(newStronglyConnectedComponent);
			stronglyConnectedComponent.add(newStronglyConnectedComponent);
		}
	}

	public void _tarjan(graph g){
		for(int v = 0; v < g.vexNum; ++v){
			if(Integer.valueOf((Integer)this.index.get(v)) < 0){
				this.strongConnect(v, g);
			}
		}
	}
	
	public static void main(String args[]){
		graph g = new graph();
		tarjan t = new tarjan(g);

		t._tarjan(g);
		System.out.println(t.stronglyConnectedComponent);
	}
}
