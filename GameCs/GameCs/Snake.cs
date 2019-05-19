using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace GameCs
{
    abstract class Snake                                                             //lop con ran 
    {
        public int fx { get; set; }
        public int fy { get; set; }
        public int lx { get; set; }
        public int ly { get; set; }

        protected int dir;
        protected int timeToGrow;
        protected ConsoleColor color;
        protected ConsoleColor hcolor;
        protected char figure;
        public const short UP = 0;
        public const short DOWN = 2;
        public const short RIGHT = 1;
        public const short LEFT = 3;
        protected List<int[]> part;



        public Snake(int size)
        {
            hcolor = ConsoleColor.Yellow;
            color = Game.snake_color;
            figure = Game.snake_fig;
            timeToGrow = 0;
            part = new List<int[]>();
            for (int i = 0; i < size; i++)
            {
                int[] pa = new int[2];
                part.Add(pa);
            }
        }

        public Snake(ConsoleColor color, ConsoleColor hcolor,char figure, int size)
        {
            this.color = color;
            this.hcolor = hcolor;
            this.figure = figure;
            timeToGrow = 0;
            part = new List<int[]>();
            for (int i = 0; i < size; i++)
            {
                int[] pa = new int[2];
                part.Add(pa);
            }
        }
        protected void move()     
        {
            int size = part.Count - 1;
            switch (dir)
            {
                case DOWN:

                    if (fy + 1 > 24)
                        fy = 0;
                    else
                        fy++;

                    break;
                case UP:

                    if (fy - 1 < 0)
                        fy = Game.HEIGHT - 1;
                    else
                        fy--;

                    break;
                case RIGHT:

                    if (fx + 1 > 79)
                        fx = 0;
                    else
                        fx++;
                    break;
                case LEFT:

                    if (fx - 1 < 0)
                        fx = Game.WIDTH - 1;
                    else
                        fx--;
                    break;
            }
            for (int i = size; i > 0; i--)
            {
                part[i][0] = part[i - 1][0];
                part[i][1] = part[i - 1][1];
            }

            part[0][0] = fx;
            part[0][1] = fy;
            lx = part[part.Count - 1][0];
            ly = part[part.Count - 1][1];

        }


        public void grow(int size)
        {
            timeToGrow = size;
            int[] last = part[part.Count - 1];
            for (int i = 0; i < size; i++)
            {
                int[] pa = { last[0], last[1] };
                part.Add(pa);
            }

        }
        public void showEntireSnake()
        {


            int size = part.Count();
            Console.ForegroundColor = color;
            for (int i = 1; i < size; i++)
            {
                Console.SetCursorPosition(part[i][0], part[i][1]);
                Console.Write(figure);
            }
            Console.ForegroundColor = hcolor;
            Console.SetCursorPosition(part[0][0], part[0][1]);
            Console.Write(figure);
        }
        public void work()
        {
            int last = part.Count - 1;
            int cx = part[last][0];
            int cy = part[last][1];
            move();
            if (timeToGrow == 0)
            {
                Console.SetCursorPosition(cx, cy);
                Console.Write(' ');
            }
            else
            {
                timeToGrow--;
            }

            Console.ForegroundColor = color;
            Console.SetCursorPosition(part[1][0], part[1][1]);
            Console.Write(figure);

            Console.ForegroundColor = hcolor;
            Console.SetCursorPosition(part[0][0], part[0][1]);
            Console.Write(figure);
        }


        public void setDir(int dir)                    //kiem tra phim di chuyen co hop le hay khong
        {
            this.dir = dir;
        }

        public List<int[]> getPart()
        {
            return part;
        }

        public int getDir
        {
            get
            {
                return dir;
            }
        }
        public ConsoleColor getColor
        {
            get
            {
                return color;
            }
        }

        public char getFigure
        {
            get
            {
                return figure;
            }
        }
    }
}
