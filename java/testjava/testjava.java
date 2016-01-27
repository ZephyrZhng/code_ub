/**
* title: testjavadoc<br>
* description: show how javadoc works by this testjava class<br>
* copyright:(c) 2014 zephyr<br>
* company: neu</br>
* @author zephyr
* @version 1.00
*/

class myClass{
	private String s;
	public myClass(String _s){this.s = _s;}
	public boolean equals(Object myObject){
		myClass c1 = this;
		boolean flag = true;
		if(myObject instanceof myClass){
			myClass c2 = (myClass)myObject;
			if(!c1.s.equals(c2.s)) flag = false;
		}else{
			flag = false;
		}
		return flag;
	}
	public String toString(){
		return s;
	}
}

class Tag{
	Tag(int i){
		System.out.println("Tag" + i);
	}
}

class Card{
	Tag t1 = new Tag(1);
	Card(){
		System.out.println("Card()");
		t3 = new Tag(33);
	}
	Card(int i){
		this();
		int j = 1;
	}
	Tag t2 = new Tag(2);
	void f(){
		System.out.println("f()");
	}
	Tag t3 = new Tag(3);
}

public class testjava{
	private static int test = 233;

	/**
	* constructor of testjava class without parameters
	*/
	public testjava(){}

	/**
	* constructor of testjava class with parameters
	* @param test
	*/
	public testjava(int my_test){
		this.test = my_test;
	}

	/**
	* method to set value of test
	* @param test
	*/
	public void setTest(int my_test){
		this.test = my_test;
	}

	/**
	* method to get value of test
	* @return int
	*/
	public int getTest(){
		return test;
	}

	public static void main(String args[]){
		// System.out.println("test: " + test);
		// myClass c1 = new myClass("fuck you");
		// myClass c2 = new myClass("fuck you");
		// myClass c3 = new myClass("fuck your mother");
		// System.out.println("c1 and c2 are " + (c1.equals(c2)? "same": "different"));
		// System.out.println("c1 and c3 are " + (c1.equals(c3)? "same": "different"));
		// System.out.println(c1);
		Card c = new Card(1);
		c.f();

		int i = 2, j = 2;
		int p = (i++)+(++j);
		System.out.println(p);

		int a = 2;
		int b = (a++)+3*a;
		System.out.println(b);
	}
}	
