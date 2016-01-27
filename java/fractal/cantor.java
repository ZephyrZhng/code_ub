import java.awt.*;
import java.lang.*;
import javax.swing.*;

public class cantor extends JApplet
{
	public void init()
	{
		new cantor();
	}

	public void frameSet()
	{
		cantor shapes = new cantor();
		JFrame fra = new JFrame("cantor");
		fra.getContentPane().add(shapes, BorderLayout.CENTER);
		fra.setSize(new Dimension(900, 500));
		fra.setResizable(false);
		fra.setVisible(true);
	}

	public static void main(String args[])
	{
		new cantor().frameSet();
	}

	public void paint(Graphics g)
	{
		g.setColor(Color.blue);
		drawCantor(g, 100, 100, 800, 100);
	}

	public void drawCantor(Graphics g, double ax, double ay, double bx, double by)
	{
		double c = 1;

		if(bx - ax < c)
		{
			g.drawLine((int)ax, (int)ay, (int)bx, (int)by);
		}
		else
		{
			double cx = 0;
			double cy = 0;
			double dx = 0;
			double dy = 0;

			g.drawLine((int)ax, (int)ay, (int)bx, (int)by);
			cx = ax + (bx - ax) / 3;
			cy = ay + 50;
			dx = bx - (bx - ax) / 3;
			dy = by + 50;
			ay += 50;
			by += 50;

			drawCantor(g, ax, ay, cx, cy);
			drawCantor(g, dx, dy, bx, by);
		}

	}
}