using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace GameCs
{
    class BigFood : Food
    {

        public const int EXIST = 0;
        public const int NO_LEFT = -2;
        public const int NO_RIGHT = -1;
        public const int SCRORE = 10;
        public const int WORTH = 4;

        public BigFood(int x, int y) : base(x, y)
        {

            figure = "▓▓";
        }

        //lam viec
        public override void work()
        {
            show();
            changeTime++;
        }


        //bi an chua ?
        public int isEated(int snx, int sny)
        {
            if ((snx == x && sny == y))
            {
                return NO_LEFT;
            }
            else if (snx == x + 1 && sny == y)
            {
                return NO_RIGHT;
            }
            return EXIST;

        }


        //hien hinh ra
        public override void show()
        {
            if (changeTime % 5 == 0)
            {
                Console.ForegroundColor = ConsoleColor.Red;
                changeTime = 0;
            }
            else
            {
                Console.ForegroundColor = ConsoleColor.White;
            }
            Console.SetCursorPosition(x, y);
            Console.Write(figure);
        }
    }
}
