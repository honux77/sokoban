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
    enum TILETYPE { FLOOR = 0, WALL, BBOX, GOAL, CHARA, GRASS };
    public partial class MainForm : Form
    {
        public Bitmap bmFloor, bmBbox, bmGoal, bmWall, bmChara, bmGrass;
        public Bitmap mapImage;
        const int TSIZE = 32; //tile size
        const int IW = 15;
        const int IH = 10;
        int mapw = IW;
        int maph = IH;
        TILETYPE tileType = TILETYPE.FLOOR;
        TILETYPE[,] map;
        Boolean isMouseDown;
        public MainForm()
        {
            InitializeComponent();
            bmFloor = new Bitmap(WindowsFormsApplication1.Properties.Resources.floor);
            bmBbox = new Bitmap(WindowsFormsApplication1.Properties.Resources.bbox);
            bmGoal = new Bitmap(WindowsFormsApplication1.Properties.Resources.goal);
            bmWall = new Bitmap(WindowsFormsApplication1.Properties.Resources.wall);
            bmChara = new Bitmap(WindowsFormsApplication1.Properties.Resources._char);
            bmGrass = new Bitmap(WindowsFormsApplication1.Properties.Resources.grass);
            map = new TILETYPE[maph, mapw];
            for (int y = 0; y < maph; y++)
                for (int x = 0; x < mapw; x++)
                    map[y, x] = TILETYPE.FLOOR;
            mapImage = new Bitmap(mapw * TSIZE, maph * TSIZE);
            mapReset();
        }

        void mapReset()
        {
            Graphics g = Graphics.FromImage(mapImage);
            for (int y = 0; y < maph; y++)
                for (int x = 0; x < mapw; x++)
                {
                    int X = x * TSIZE;
                    int Y = y * TSIZE;
                    g.DrawImage(bmGrass, new Point(X, Y));
                }
            g.Dispose();
            mapDisplay.Image = mapImage;
        }
        private void button4_Click(object sender, EventArgs e)
        {
            mapw = int.Parse(textBoxW.Text);
            maph = int.Parse(textBoxH.Text);
            mapDisplay.Width = mapw * TSIZE;
            mapDisplay.Height = maph * TSIZE;
            map = new TILETYPE[maph, mapw];
            mapReset();
        }

        private void MainForm_Load(object sender, EventArgs e)
        {

        }

        private void mapDisplay_MouseDown(object sender, MouseEventArgs e)
        {
            isMouseDown = true;
            drawTileImage(e);
        }

        void drawTileImage(MouseEventArgs e) {
            label5.Text = e.X + " : " + e.Y;      

            int w = e.X / TSIZE;
            int h = e.Y / TSIZE;
            map[h, w] = tileType;
            Point p = new Point(w * TSIZE, h * TSIZE);
            Graphics g = Graphics.FromImage(mapImage);
            switch (tileType)
            {
                case TILETYPE.BBOX:
                    g.DrawImage(bmBbox, p);
                    break;
                case TILETYPE.CHARA:
                    g.DrawImage(bmChara, p);
                    break;

                case TILETYPE.FLOOR:
                    g.DrawImage(bmFloor, p);
                    break;
                case TILETYPE.GOAL:
                    g.DrawImage(bmGoal, p);
                    break;
                case TILETYPE.GRASS:
                    g.DrawImage(bmGrass, p);
                    break;
                case TILETYPE.WALL:
                    g.DrawImage(bmWall, p);
                    break;
            }
            g.Dispose();
            mapDisplay.Image = mapImage; 
        }


        private void radioButton1_CheckedChanged(object sender, EventArgs e)
        {
            tileType = TILETYPE.GRASS;
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

        private void radioButton6_CheckedChanged(object sender, EventArgs e)
        {
            tileType = TILETYPE.WALL;
        }

        private void mapDisplay_MouseMove(object sender, MouseEventArgs e)
        {
            if (e.X <= 0 || e.X >= mapDisplay.Width || e.Y <= 0 || e.Y >= mapDisplay.Height) return;
            if (isMouseDown)
            {
                drawTileImage(e);
            }
        }

        private void mapDisplay_MouseUp(object sender, MouseEventArgs e)
        {
            isMouseDown = false;
        }

    }
}
