using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace GameCs
{
    class ChoseMapMenu : Activity
    {
        
        const int X = 28;
        const int Y = 4;
        int index;
        List<string> source;
        List<string> item;
        Poster poster;

        public ChoseMapMenu(Poster poster, CentraProccessing cpu)
        {
            
            st = Game.OUG;
            label = "Map Menu";
            index = 0;
            source = new List<string>();
            item = new List<string>();
            this.poster = poster;
            this.cpu = cpu;
            try
            {
                string[] file = Directory.GetFiles("map").Where(n => n.EndsWith(".mp")).ToArray();
                if (file.Length==0) throw new Exception();
                int i = 1;
                foreach (var s in file)
                {
                    source.Add(s);
                    item.Add(i + "." + Path.GetFileName(s).Split('.')[0]);
                    i++;
                }
                
            }
            catch (Exception e)
            {
                Environment.Exit(1);
            }
            
        }
        public override void getKey(char key)
        {
            switch (key)
            {
                case Game.UP_KEY:
                    itemUp();
                    break;
                case Game.DOWN_KEY:
                    itemDown();
                    break;
                case Game.ZERO_KEY:
                    cpu.createMapSourceTD("Tu do",source[index]);
                    Poster p = new Poster(cpu.blankPoster);
                    SpeedMenu spmenu = new SpeedMenu(p, cpu);
                    cpu.pushStack(spmenu);
                    cpu.topOfStackWork();
                    break;
                case Game.BACK_KEY:
                    cpu.popStack();
                    cpu.drawTopOfStack();
                    break;
                case Game.REFRESH_KEY:
                    cpu.drawInfoFrame();
                    drawAll();
                    break;
            }
        }

        public override void drawAll()
        {
            poster.showPoster();
            cpu.addInfomation(InfoTable.TYPE.LEVEL, label, ConsoleColor.Yellow);
            cpu.addInfomation(InfoTable.TYPE.STATE, Game.OUG_DESCRIPTION, ConsoleColor.Red);
            int size = item.Count;
            for (int i = 0; i < size; i++)
            {
                Console.ForegroundColor = ConsoleColor.White;
                Console.SetCursorPosition(X, Y + i);
                Console.WriteLine(item[i]);
            }
            Console.ForegroundColor = Game.SELECT_COLOR;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
        }

        public override void workFirst()
        {
            drawAll();
        }

        public override void work()
        {
            drawAll();
        }

        private void itemUp()
        {
            Console.ForegroundColor = ConsoleColor.White;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
            if (index > 0) index--;
            else if (index == 0) index = item.Count - 1;
            Console.ForegroundColor = Game.SELECT_COLOR;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
        }

        private void itemDown()
        {
            Console.ForegroundColor = ConsoleColor.White;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
            if (index < item.Count - 1) index++;
            else if (index == item.Count - 1) index = 0;
            Console.ForegroundColor = Game.SELECT_COLOR;
            Console.SetCursorPosition(X, Y + index);
            Console.WriteLine(item[index]);
        }
    }
}
