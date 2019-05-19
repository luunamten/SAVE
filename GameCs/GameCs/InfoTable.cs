using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading.Tasks;
namespace GameCs
{
    class InfoTable
    {
        //cac toa do cac o tren bang thong tin
        const int SCORE_X = 86;
        const int SCORE_Y = 2;
        const int LEVEL_X = 97;
        const int LEVEL_Y = 2;
        const int DIR_X = 86;
        const int DIR_Y = 2;
        const int NAME_X = 97;
        const int NAME_Y = 5;
        const int STATE_X = 108;
        const int STATE_Y = 2;
        const int TIME_X = 86;
        const int TIME_Y = 3;
        const int FOOD_X =86;
        const int FOOD_Y = 1;
        const int LIM_X = 97;
        const int LIM_Y = 3;

        class Cell

        {
            int center;
            int x;
            int y;
            string data;
            ConsoleColor color;

            public Cell(int x, int y)
            {
                data = "";
                center = x;
                this.y = y;
            }
            public void setData(string data)
            {
                this.data = data;
                int size = data.Length;
                x =center -size / 2;
            }
            public void setColor(ConsoleColor color)
            {
                this.color = color;
            }
            public void empty()
            {
                if (data == "") return;
                data = Regex.Replace(data, ".", " ");
                Console.SetCursorPosition(x, y);
                Console.Write(data);
                data = "";
            }
            public void show()
            {
                Console.SetCursorPosition(x,y);
                Console.ForegroundColor =color;
                Console.Write(data);
            }
        
        }

        string[] data;
        List<Cell> cell;
        public enum TYPE
        {
            SCORE=0, LEVEL=1, STATE=2, DIRECTION=3, NAME=4, TIME=5, FOOD=6,LIM=7
        }
        public InfoTable(string source)
        {
            cell = new List<Cell>();
            cell.Add(new Cell(SCORE_X,SCORE_Y));
            cell.Add(new Cell(LEVEL_X,LEVEL_Y));
            cell.Add(new Cell(STATE_X,STATE_Y));
            cell.Add(new Cell(DIR_X,DIR_Y));
            cell.Add(new Cell(NAME_X,NAME_Y));
            cell.Add(new Cell(TIME_X, TIME_Y));
            cell.Add(new Cell(FOOD_X,FOOD_Y));
            cell.Add(new Cell(LIM_X, LIM_Y));
            FileStream file = null;
            StreamReader tmapdata = null;
            bool isException = false;
            try
            {

                file = new FileStream(source, FileMode.Open, FileAccess.Read);
                tmapdata = new StreamReader(file);
                string temp = tmapdata.ReadToEnd();
                temp = temp.Trim().Replace("#", "▓");
                data = Regex.Split(temp, "\n");
            }
            catch (Exception e)
            {
                isException = true;

            }
            finally
            {
                if (file != null)
                {
                    file.Close();
                }
                if (tmapdata != null)
                {
                    tmapdata.Close();
                }
                if (isException)
                {
                    Environment.Exit(-1);
                }
            }


            file.Close();
        }
        //ve bang
        public void showFrame()
        {
            int i = 0;
            Console.ForegroundColor = ConsoleColor.Cyan;
            foreach (string x in data)
            {
                Console.SetCursorPosition(80, i);
                Console.Write(x);
                i++;
            }
        }
        //them du lieu vao bang
        public void addData(TYPE t, string data, ConsoleColor cl)
        {
            int index = (int)t;
            cell[index].setColor(cl);
            cell[index].empty();
            cell[index].setData(data);
            cell[index].show();
        }

        public void clear()
        {
            foreach(Cell x in cell)
            {
                x.empty();
            }
        }
        public void showAll()
        {
            showFrame();
            foreach(Cell x in cell)
            {
                x.show();
            }
        }
    }

}
