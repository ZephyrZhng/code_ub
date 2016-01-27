import java.applet.*;
import java.awt.*;
import java.awt.event.*;
import java.lang.*;
import java.lang.Math.*;
import javax.swing.*;

public class KochSnowFlake extends JApplet
{
	int depth;

	public void init()
	{
		setBackground(Color.white);
		depth = 1;
	}

	void drawKochSnowFlake(double x1, double y1, double x2, double y2, int depth, Graphics g)
	{
		if(depth <= 1)
		{
			g.drawLine((int)x1, (int)y1, (int)x2, (int)y2);
		}
		else
		{
			double x4 = x1 * 2 / 3 + x2 * 1 / 3;
			double y4 = y1 * 2 / 3 + y2 * 1 / 3;
			double x5 = x1 * 1 / 3 + x2 * 2 / 3;
			double y5 = y1 * 1 / 3 + y2 * 2 / 3;
			double x6 = (x4 + x5) / 2 + (y4 - y5) * Math.sqrt(3) / 2;
			double y6 = (y4 + y5) / 2 + (x5 - x4) * Math.sqrt(3) / 2;

			drawKochSnowFlake(x1, y1, x4, y4, depth - 1, g);
			drawKochSnowFlake(x4, y4, x6, y6, depth - 1, g);
			drawKochSnowFlake(x6, y6, x5, y5, depth - 1, g);
			drawKochSnowFlake(x5, y5, x2, y2, depth - 1, g);
		}
	}

	public void paint(Graphics g)
	{
		g.setColor(Color.blue);
		int myDepth = depth + 10;
		drawKochSnowFlake(180.0,10.0,64.5,210.0,myDepth,g);
		drawKochSnowFlake(64.5,210.0,295.5,210.0,myDepth,g);
		drawKochSnowFlake(295.5,210.0,180.0,10.0,myDepth,g);
	}

	public void frameSet()
	{
		KochSnowFlake shapes = new KochSnowFlake();
		JFrame fra = new JFrame("koch snow flake");
		fra.getContentPane().add(shapes, BorderLayout.CENTER);
		fra.setSize(new Dimension(350, 300));
		fra.setResizable(false);
		fra.setVisible(true);
	}

	public static void main(String args[])
	{
		new KochSnowFlake().frameSet();
	}
}