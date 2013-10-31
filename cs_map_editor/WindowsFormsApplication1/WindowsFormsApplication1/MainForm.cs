using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Windows.Forms;

namespace WindowsFormsApplication1
{
    enum TILETYPE { FLOOR=0, WALL,BBOX, GOAL, CHARA};
    public partial class MainForm : Form
    {
        public Bitmap bmFloor;
        public Bitmap bmBbox;
        public Bitmap bmGoal;
        public Bitmap bmWall;
        public Bitmap bmChara;
        const int TSIZE = 30; //tile size
        const int MAXW = 16;
        const int MAXH = 10;
        TILETYPE tileType = TILETYPE.FLOOR;
        TILETYPE[,] map;
        public MainForm()
        {
            InitializeComponent();
        }

        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {

        }

        private void button3_Click(object sender, EventArgs e)
        {

        }

        private void pictureBox1_MouseUp(object sender, MouseEventArgs e)
        {

        }

        private void button4_Click(object sender, EventArgs e)
        {
            int x = int.Parse(textBoxW.Text);
            int y = int.Parse(textBoxH.Text);
            mapDisplay.Width = x * 30;
            mapDisplay.Height = y * 30;
        }

        private void MainForm_Load(object sender, EventArgs e)
        {
            bmFloor = new Bitmap(WindowsFormsApplication1.Properties.Resources.floor);
            bmBbox = new Bitmap(WindowsFormsApplication1.Properties.Resources.bbox);
            bmGoal = new Bitmap(WindowsFormsApplication1.Properties.Resources.goal);
            bmWall = new Bitmap(WindowsFormsApplication1.Properties.Resources.wall);
            bmChara = new Bitmap(WindowsFormsApplication1.Properties.Resources._char);

            map = new TILETYPE[MAXH, MAXW];
            for (int y = 0; y < MAXH; y++)
                for (int x = 0; x < MAXW; x++)
                    map[y, x] = TILETYPE.FLOOR;
        }

        private void mapDisplay_MouseDown(object sender, MouseEventArgs e)
        {
            label5.Text = e.X + " : " + e.Y;
            int w =  e.X / TSIZE;
            int h = e.Y / TSIZE;
            map[h, w] = tileType;

            Graphics g = mapDisplay.CreateGraphics();
            for (int y = 0; y < MAXH; y++)
                for (int x = 0; x < MAXW; x++)
                {
                    int X = x * TSIZE;
                    int Y = y * TSIZE;
                    switch (map[y, x])
                    {
                        case TILETYPE.FLOOR:
                            g.DrawImage(bmFloor, new Point(X, Y));
                            break;
                        case TILETYPE.WALL:
                            g.DrawImage(bmWall, new Point(X, Y));
                            break;
                        case TILETYPE.BBOX:
                            g.DrawImage(bmBbox, new Point(X, Y));
                            break;
                        case TILETYPE.GOAL:
                            g.DrawImage(bmGoal, new Point(X, Y));
                            break;
                        case TILETYPE.CHARA:
                            g.DrawImage(bmChara, new Point(X, Y));
                            break;
                    }
                }
            g.Dispose(); 
        }

        private void radioButton1_CheckedChanged_1(object sender, EventArgs e)
        {
            tileType = TILETYPE.WALL;
        }

        private void radioButton2_CheckedChanged(object sender, EventArgs e)
        {
            tileType = TILETYPE.GOAL;
        }

        private void radioButton5_CheckedChanged(object sender, EventArgs e)
        {
            tileType = TILETYPE.FLOOR;
        }

        private void radioButton3_CheckedChanged(object sender, EventArgs e)
        {
            tileType = TILETYPE.BBOX;
        }

        private void radioButton4_CheckedChanged(object sender, EventArgs e)
        {
            tileType = TILETYPE.CHARA;
        }
    
    }
}
