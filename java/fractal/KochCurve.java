import java.awt.*;
import java.lang.*;
import javax.swing.*;
import java.lang.Math.*;

public class KochCurve extends JApplet
{
	public static double PI = 3.1415926;
	public void init()
	{
		new KochCurve();
	}

	public void frameSet()
	{
		KochCurve shapes = new KochCurve();
		JFrame fra = new JFrame("koch curve");
		fra.getContentPane().add(shapes, BorderLayout.CENTER);
		fra.setSize(new Dimension(900, 500));
		fra.setResizable(false);
		fra.setVisible(true);
	}

	public static void main(String args[])
	{
		new KochCurve().frameSet();
	}

	public void paint(Graphics g)
	{
		g.setColor(Color.blue);
		drawKoch(g, 100, 100, 800, 100);
	}

	public double distance(double ax, double ay, double bx, double by)
	{
		return Math.sqrt((ax - bx) * (ax - bx) + (ay - by) * (ay - by));
	}

	public void drawKoch(Graphics g, double ax, double ay, double bx, double by)
	{
		double c = 3;
		double cx = 0;
		double cy = 0;
		double dx = 0;
		double dy = 0;
		double ex = 0;
		double ey = 0;
		double l = 0;
		double theta = 0;

		if(distance(ax, ay, bx, by) < c)
		{
			g.drawLine((int)ax, 500 - (int)ay, (int)bx, 500 - (int)by);
		}
		else
		{
			cx = ax + (bx - ax) / 3;
			cy = ay + (by - ay) / 3;
			ex = bx - (bx - ax) / 3;
			ey = by - (by - ay) / 3;
			l = distance(cx, cy, ex, ey);
			theta = Math.atan((ey - cy) / (ex - cx));

			if(ex - cx < 0)
			{
				theta += PI;
			}
			
			dx = cx + Math.cos(theta + PI / 3) * l;
			dy = cy + Math.sin(theta + PI / 3) * l;

			drawKoch(g, ax, ay, cx, cy);
			drawKoch(g, ex, ey, bx, by);
			drawKoch(g, cx, cy, dx, dy);
			drawKoch(g, dx, dy, ex, ey);
		}
	}
}