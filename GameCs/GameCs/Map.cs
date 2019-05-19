using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;


namespace GameCs
{

    class Map                                                           
    {
        int x;
        int y;
        int direction;
        int targetFood;
        int[,] matrix;
        int timeToSnakeGrow;
        int count;
        int bigTimeLimit;
        int bigTime;
        bool GameOver;

        readonly string name;
        readonly string data;
        public const int SNAKE_DIE = -1;
        public const int NORMAL = 0;
        public const int NO_BAIT = 1;
        public const int SMALL_NO_EXIST = 2;
        public const int SMALL_EXIST = 3;
        public const int SMALL_IS_EATED = 4;
        public const int BIG_NO_EXIST = 5;
        public const int BIG_EXIST = 6;
        public const int BIG_IS_EATED_LEFT = 7;
        public const int BIG_IS_EATED_RIGHT = 8;

        ConsoleColor color;
        UserSnake sn;
        BigFood big;
        SmallFood small;
        List<int[]> blank;
        List<int[]> bigBlank;
        

        public Map(string source)
        {
            //du lieu ban do
            matrix = new int[Game.HEIGHT, Game.WIDTH];
            FileStream mapdata = null;
            StreamReader tmapdata = null;
            bool isException = false;
            try
            {
                mapdata = new FileStream(source, FileMode.Open, FileAccess.Read);
                tmapdata = new StreamReader(mapdata, true);

                string[] tempstr = Regex.Split(tmapdata.ReadToEnd().Trim(), "/\\*\\*\\*\\*\\*/");
                name = tempstr[0].Trim();
                string[] iniCoords = Regex.Split(tempstr[1].Trim(), "\\s+");
                bigTimeLimit = int.Parse(iniCoords[0]);
                targetFood = int.Parse(iniCoords[1]);
                x = int.Parse(iniCoords[2]);
                y = int.Parse(iniCoords[3]);
                direction = int.Parse(iniCoords[4]);


                data = tempstr[2].Replace("#", "▓");
                string[] sarray = Regex.Split(tempstr[2], "[\\r\\n]+");
                for (int i = 0; i < Game.HEIGHT; i++)
                {
                    char[] c = sarray[i].ToCharArray();
                    for (int j = 0; j < Game.WIDTH; j++)
                    {
                        if (c[j] == '#') matrix[i, j] = 1;
                        else matrix[i, j] = 0;
                    }
                }


                blank = new List<int[]>();
                string[] bar = Regex.Split(tempstr[3].Trim(), "\\s+");
                int bsize = bar.GetLength(0) - 1;

                for (int i = 0; i < bsize; i += 2)
                {
                    int[] blm = { int.Parse(bar[i]), int.Parse(bar[i + 1]) };
                    blank.Add(blm);
                }


                bigBlank = new List<int[]>();
                int blankSize = blank.Count;
                for (int i = 0; i < blankSize; i++)
                {
                    int x = blank[i][0];
                    int y = blank[i][1];
                    if (x < Game.WIDTH - 1)
                    {
                        int nextX = matrix[y, x + 1];
                        if (nextX == 0)
                        {
                            int[] blm = { x, y };
                            bigBlank.Add(blm);
                        }
                    }
                }
                count = 0;
                bigTime = 0;
                timeToSnakeGrow = 0;
                color = Game.mapColor;

            }
            catch (Exception e)
            {
                Environment.Exit(1);
            }
            finally
            {
                if (mapdata != null)
                {
                    mapdata.Close();
                }
                if (tmapdata != null)
                {
                    tmapdata.Close();
                }
                if (isException)
                    Environment.Exit(-1);
            }
        }

  

        private bool isSnakeDie()                             
        {
            int flag = matrix[sn.fy, sn.fx];
            if (flag==1 || flag==2)
                return true;
            return false;
        }
        private void genBigBait(int res)
        {
            int blankSize = bigBlank.Count;
            int bx = bigBlank[res][0];
            int by = bigBlank[res][1];
            if (matrix[by, bx] == 2)
            {
                int loop = 0;
                foreach (int[] coord in bigBlank)
                {
                    loop++;
                    if (matrix[coord[1], coord[0]] == 0 && matrix[coord[1], coord[0]+1] == 0)
                    {
                        bx = coord[0];
                        by = coord[1];
                        break;
                    }
                }
                if (loop == blankSize) return;
            }
            matrix[by, bx] = 3;
            matrix[by, bx+1] = 3;
            big=new BigFood(bx, by);
        }

        private void genSmallBait(int res)
        {
            int blankSize = blank.Count;
            int sx = blank[res][0];
            int sy = blank[res][1];
            if (matrix[sy, sx] == 2 || matrix[sy,sx]==3)
            {
                int loop = 0;
                foreach (int[] coord in blank)
                {
                    loop++;
                    if (matrix[coord[1], coord[0]] == 0)
                    {
                        sx = coord[0];
                        sy = coord[1];
                        break;
                    }
                }
                if (loop == blankSize) return;

            }
            small=new SmallFood(sx, sy);
        }


        private void genBaits()
        {
            count++;
            Random r = new Random();
            if(count==7)
            {
                count = 0;
                bigTime = bigTimeLimit;
                if(big !=null)
                {
                    removeBigBait(big.X,big.Y);
                    big = null;
                }
                genBigBait(r.Next(0,bigBlank.Count()));
            }
            genSmallBait(r.Next(0,blank.Count()));            
        }

        private void baitsWork()
        {
            if (big != null)
            {
                if(bigTime==0)
                {
                    removeBigBait(big.X,big.Y);
                    big = null;
                } else
                    big.work();
                bigTime--;
            }
            if (small != null) small.work();
        }
        private void showBaits()
        {
            if (big != null) big.show();
            if (small != null) small.show();
        }

        private int smallBaitState(int x, int y)
        {
            if (small == null)
                return SMALL_NO_EXIST;
            if (small.isEated(x, y))
                return SMALL_IS_EATED;
            else
                return SMALL_EXIST;
        }

        private int bigBaitState(int x, int y)
        {
            if (big == null)
                return BIG_NO_EXIST;
            int flag = big.isEated(x, y);
            if (flag == BigFood.NO_LEFT)
                return BIG_IS_EATED_LEFT;
            else if (flag == BigFood.NO_RIGHT)
                return BIG_IS_EATED_RIGHT;
            else
                return BIG_EXIST;
        }



        private void removePartOfBigBait(int x1, int y1, int x2, int y2)
        {
            matrix[y1, x1] = 0;
            matrix[y2, x2] = 0;
            Console.SetCursorPosition(x1, y1);
            Console.Write(' ');
        }
        private void removeBigBait(int bx, int by)
        {
            matrix[by, bx] = 0;
            matrix[by, bx+1] = 0;
            Console.SetCursorPosition(bx, by);
            Console.Write("  ");
        }

        public void workFirst()
        {
            genBaits();
            show();
        }

        

        public int work()
        {
            baitsWork();
            if (timeToSnakeGrow == 0)
                matrix[sn.ly, sn.lx] = 0;
            else
                timeToSnakeGrow--;
            
            sn.work();          
            if (isSnakeDie())
            {
                GameOver = true;
                return SNAKE_DIE;
            }
            matrix[sn.fy, sn.fx] = 2;         
               
            int flag1 = smallBaitState(sn.fx, sn.fy);
            if(flag1==SMALL_IS_EATED)
            {
                sn.grow(SmallFood.WORTH);              
                small = null;
                genBaits();
                return SmallFood.WORTH;
            }
            int flag2 = bigBaitState(sn.fx,sn.fy);
            if(flag2==BIG_IS_EATED_LEFT)
            {
                removePartOfBigBait(big.X+1,big.Y,big.X,big.Y);
                sn.grow(BigFood.WORTH);
                bigTime = 0;               
                big = null;
                return BigFood.SCRORE;
            }
            if (flag2 == BIG_IS_EATED_RIGHT)
            {
                removePartOfBigBait(big.X, big.Y, big.X+1, big.Y);
                sn.grow(BigFood.WORTH);
                bigTime = 0;
                big = null;
                return BigFood.SCRORE;
            }
            if (flag1 == SMALL_NO_EXIST && flag2 == BIG_NO_EXIST)
                return NO_BAIT;
            else
                return NORMAL;
        }

        public void show()
        {
            Console.ForegroundColor = color;
            Console.SetCursorPosition(0, 0);
            Console.Write(data);
            sn.showEntireSnake();
            showBaits();
        }

        public bool checkScore(int score)
        {
            if (score >= targetFood) return true;
            return false;
        }

        public void addSnake(UserSnake n)
        {
            n.fx = x;
            n.fy = y;
            n.setDir(direction);
            List<int[]> part = n.getPart();
            int size = part.Count;
            int a, b;
            switch (direction)
            {
                case Snake.DOWN:
                    a = 0; b = -1;
                    break;
                case Snake.UP:
                    a = 0; b = 1;
                    break;
                case Snake.RIGHT:
                    a = -1; b = 0;
                    break;
                default:
                    a = 1; b = 0;
                    break;
            }

            for (int i = 0; i < size; i++)
            {
                part[i][0] = x + a * i;
                part[i][1] = y + b * i;
                matrix[part[i][1], part[i][0]] = 2;
            }
            n.lx = part[size - 1][0];
            n.ly = part[size - 1][1];
            sn=n;
        }
        public UserSnake getSnake
        {
            get
            {
                return sn;
            }
        }
        public int getMaxFood {
            get
            {
                return targetFood;
            }
        }
        public void reset()
        {
            
            List<int[]> part = sn.getPart();
            int size = part.Count;
            for(int i=1;i<size;i++)
            {
                matrix[part[i][1], part[i][0]] = 0;
            }
            sn = null;
            small= null;
            big = null;
            timeToSnakeGrow = 0;
            count = 0;
            bigTime = 0;
            GameOver = false;
        }

        public string getName
        {
            get
            {
                return name;
            }
        }

        public bool isGameOver
        {
            get
            {
                return GameOver;
            }
        }
        public int getBigBaitTime
        {
            get
            {
                return bigTime;
            }
        }
        public int getBigBaitTimeLimt
        {
            get
            {
                return bigTimeLimit;
            }
        }
    }
}
