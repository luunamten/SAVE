using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameCs
{

    //Activity chua cac tuy chon khi dang choi game
    class Option : Activity
    {
        public const string OUG = "Out game";
        int index;
        const int X = 28;
        const int Y = 4;
        char key;
        Poster poster;
        string[] item= { "1.Choi tiep", "2.Tro ve menu truoc" };
        
        public Option(Poster poster, CentraProccessing cpu, string label)
        {
            st = Game.OUG;
            this.label = label;
            this.poster = poster;
            this.cpu = cpu;
            key = '\0';
            index = 0;
        }
        //lay du lieu phim nhap vao
        public override void getKey(char key)
        {
            this.key = key;
        }

        public override void work()
        {
            if (reactKey()) return;
           
        }

        //lam gi khi co phim nhap vao
        public bool reactKey()
        {
            if (key == '\0') return false;
            switch (key)
            {
                case Game.UP_KEY:
                    itemUp();
                    break;
                case Game.DOWN_KEY:
                    itemDown();
                    break;
                case Game.REFRESH_KEY:
                    cpu.showAllInfo();
                    drawAll();
                    break;
                case Game.ZERO_KEY:
                    switch (index)
                    {
                        case 0:
                            cpu.popStack();
                            cpu.drawTopOfStack();
                            break;
                        case 1:
                            if (cpu.getPlayType() == CentraProccessing.TypePlay.CHIENDICH)
                            {
                                cpu.emptySourceMap();
                            }
                            cpu.popStack();
                            cpu.popStack();
                            cpu.killThread();
                            cpu.clearInfo();
                            cpu.drawTopOfStack();
                            break;
                    }
                    return true;
            }
            key = '\0';
            return false;
        }


        //lam viec lai
        public override void workFirst()
        {

            drawAll();
        }


        //ve toan bo
        public override void drawAll()
        {
            cpu.addInfomation(InfoTable.TYPE.LEVEL, label, ConsoleColor.Yellow);
            cpu.addInfomation(InfoTable.TYPE.STATE, Game.OUG_DESCRIPTION, ConsoleColor.Red);
            poster.showPoster();
            int size = item.Length;
            for (int i = 0; i < size; i++)
            {
                Console.SetCursorPosition(X, i + Y);
                Console.ForegroundColor = ConsoleColor.White;
                Console.WriteLine(item[i]);
            }
            Console.SetCursorPosition(X, Y + index);
            Console.ForegroundColor = Game.SELECT_COLOR;
            Console.WriteLine(item[index]);
        }

        //di chuyen len
        private void itemUp()
        {
            Console.ForegroundColor = ConsoleColor.White;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
            if (index > 0) index--;
            else if (index == 0) index = item.Length - 1;
            Console.ForegroundColor = Game.SELECT_COLOR;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
        }

        //di chuyen xuong
        private void itemDown()
        {
            Console.ForegroundColor = ConsoleColor.White;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
            if (index < item.Length - 1) index++;
            else if (index == item.Length - 1) index = 0;
            Console.ForegroundColor = Game.SELECT_COLOR;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
        }
    }
}
