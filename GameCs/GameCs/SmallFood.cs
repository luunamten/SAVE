using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameCs
{
    class SmallFood :Food                                                     //lop con Bait
    {

        public const int WORTH = 1;
        public SmallFood(int x, int y):base(x,y)
        {
            figure = "o";
        }

        public override void work()
        {
            show();
            changeTime++;
        }

        //bi an chua ?
        public bool isEated(int snx, int sny)
        {
            if (snx != x || sny != y)
            {
                return false;
            }
            else
            {
                return true;
            }
        }

        //hien con moi
        public override void show()
        {
            if (changeTime == 10)
            {
                figure = "o";
                changeTime = 0;
            }
            else if (changeTime == 5) figure = "C";
            Console.ForegroundColor = ConsoleColor.Green;
            Console.SetCursorPosition(x, y);
            Console.Write(figure);
        }
    }
}
