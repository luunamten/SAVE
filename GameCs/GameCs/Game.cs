using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace GameCs
{

    //Chua cac thong so cai dat, nut, cac trang thai game
    abstract class Game
    {
        public const char UP_KEY = 'w';
        public const char DOWN_KEY = 's';
        public const char RIGHT_KEY = 'd';
        public const char LEFT_KEY = 'a';
        public const char BACK_KEY = '1';
        public const char ZERO_KEY = '0';
        public const char REFRESH_KEY = '2';
        public const int WIDTH=80;
        public const int HEIGHT = 25;
        public const int W_WIDTH = 120;
        public const int W_HEIGHT = 26;

        public const int G_OVER =-1;
        public const int PAUSE = 0;
        public const int PLAY = 1;
        public const int OUG = -2;

        

        public const string G_OVER_DESCRIPTION = "Game over";
        public const string PAUSE_DESCRIPTION = "Pause";
        public const string PLAY_DESCRIPTION = "Play";
        public const string OUG_DESCRIPTION = "Out game";
        public const string AUTHOR = "Luu Phuoc Phuong Nam 14-12-2016";

        
        public const ConsoleColor W_COLOR = ConsoleColor.Black;
        public const ConsoleColor SELECT_COLOR = ConsoleColor.Magenta;
        public const ConsoleColor ITEM_SET_COLOR = ConsoleColor.DarkGray;

        public static ConsoleColor snake_color { get; set; }
        public static ConsoleColor mapColor { get; set; }
        public static char snake_fig { get; set; }
        public static int colorIndex { get; set; }
        public static int figureIndex { get; set; }
        
        public static int mapColorIndex { get; set; }
        
        
    }
}
